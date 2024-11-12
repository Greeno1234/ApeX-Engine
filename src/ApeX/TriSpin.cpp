#include "TriSpin.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace apex {


	void TriSpin::on_initialize()
	{
		window = SDL_CreateWindow("Triangle Spinner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

		if (!SDL_GL_CreateContext(window))
		{
			throw std::exception();
		}

		if (glewInit() != GLEW_OK)
		{
			throw std::exception();
		}

		quit = false;
		e = { 0 };
	}




	void TriSpin::on_render()
	{
		
		while (SDL_PollEvent(&e)) //should be in core later
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		//draw stuff
		glClearColor(0, 0, 0.5f, 1); //background colour
		glClear(GL_COLOR_BUFFER_BIT);



		//list of points making up the object
		const GLfloat positions[] = {
				0.0f, 0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,

				/*0.5f, -0.5f, 0.0f,
				0.5f, 0.5f, 0.0f,
				-0.5f, 0.5f, 0.0f,*/
		};

		const GLfloat colors[] = {
		0.5f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		};


		////////////////
		//SET THE VBO//  
		///////////////						  
		GLuint positionsVboId = 0;
		GLuint colorsVboId = 0;



		// Create a new VBO on the GPU and bind it
		glGenBuffers(1, &positionsVboId);


		if (!positionsVboId)
		{
			throw std::exception();
		}

		// Create a colors VBO on the GPU and bind it
		glGenBuffers(1, &colorsVboId);
		if (!colorsVboId)
		{
			throw std::exception();
		}


		glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

		// Upload a copy of the data from memory into the new VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);





		////////////////
		//SET THE VAO//  
		///////////////	
		GLuint vaoId = 0;

		// Create a new VAO on the GPU and bind it
		glGenVertexArrays(1, &vaoId);

		if (!vaoId)
		{
			throw std::exception();
		}

		glBindVertexArray(vaoId);

		// Bind the position VBO, assign it to position 0 on the bound VAO
		// and flag it to be used
		glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);//assign to position on array




		/////////////////
		//VERTEX SHADER//
		/////////////////
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			3 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(0);

		// Reset the state
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

		// Upload a copy of the data from memory into the new VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		// Bind the color VBO, assign it to position 1 on the bound VAO
		// and flag it to be used
		glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
			2 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);



		glBindVertexArray(0);

		const GLchar* vertexShaderSrc =
			"attribute vec3 a_Position;             " \
			"attribute vec2 a_TexCoord;             " \
			"                                       " \
			"uniform mat4 u_Projection;             " \
			"uniform mat4 u_Model;                  " \
			"                                       " \
			"varying vec2 v_TexCoord;               " \
			"                                       " \
			"void main()                            " \
			"{                                      " \
			" v_TexCoord = a_TexCoord;              " \
			" gl_Position = u_Projection * u_Model * vec4(a_Position, 1.0);            " \
			"}                                      " \
			"                                       ";



		//" gl_Position = vec4(a_Position, 1.0);  " \

		// Create a new vertex shader, attach source code, compile it and
		// check for errors.
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShaderId);
		GLint success = 0;
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}



		///////////////////
		//FRAGMENT SHADER//
		///////////////////
	/*const GLchar* fragmentShaderSrc =
		"uniform vec4 u_Color;    " \
		"                          " \
		"void main()               " \
		"{                         " \
		" gl_FragColor = u_Color; " \
		"}                         " \
		"                          ";*/


		const GLchar* fragmentShaderSrc =
			"varying vec2 v_TexCoord;    " \
			"uniform sampler2D u_Texture; " \
			"                          " \
			"void main()               " \
			"{                         " \
			" gl_FragColor = texture2D(u_Texture, v_TexCoord); " \
			"}                         " \
			"                          ";


		// Create a new fragment shader, attach source code, compile it and
		// check for errors.
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShaderId);
		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		//////////////////
		//SHADER PROGRAM//
		//////////////////

	// Create new shader program and attach our shader objects
		GLuint programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);

		// Ensure the VAO "position" attribute stream gets set as the first position
		// during the link.
		glBindAttribLocation(programId, 0, "a_Position");

		glBindAttribLocation(programId, 1, "a_TexCoord");

		// Perform the link and check for failure
		glLinkProgram(programId);
		glGetProgramiv(programId, GL_LINK_STATUS, &success);

		if (!success)
		{
			throw std::exception();
		}

		// Detach and destroy the shader objects. These are no longer needed
		// because we now have a complete shader program.
		glDetachShader(programId, vertexShaderId);
		glDeleteShader(vertexShaderId);
		glDetachShader(programId, fragmentShaderId);
		glDeleteShader(fragmentShaderId);


		//// Store location of the color uniform and check if successfully found
		//GLint colorUniformId = glGetUniformLocation(programId, "u_Color");

		//if (colorUniformId == -1)
		//{
		//	throw std::exception();
		//}


		//// Bind the shader to change the uniform, set the uniform and reset state
		//glUseProgram(programId);
		//glUniform4f(colorUniformId, 0, 1, 0, 1);
		//glUseProgram(0);





		/////////////
		//TEXTURING//
		/////////////


		int w = 0;
		int h = 0;

		unsigned char* data = stbi_load("image.png", &w, &h, NULL, 4);

		if (!data)
		{
			throw std::exception();
		}

		// Create and bind a texture.
		GLuint textureId = 0;
		glGenTextures(1, &textureId);

		if (!textureId)
		{
			throw std::exception();
		}

		glBindTexture(GL_TEXTURE_2D, textureId);

		// Upload the image data to the bound texture unit in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, data);

		// Free the loaded data because we now have a copy on the GPU
		free(data);

		// Generate Mipmap so the texture can be mapped correctly
		glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind the texture because we are done operating on it
		glBindTexture(GL_TEXTURE_2D, 0);




		float angle = 0;

		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			int width = 0;
			int height = 0;

			SDL_GetWindowSize(window, &width, &height);

			glViewport(0, 0, width, height);


			//draw stuff
			glClearColor(0, 1, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			////////////////////
			//PREPARE PIPELINE//
			////////////////////
			//glCullFace(GL_FRONT); //changes culled face to the forward facing triangle
			//glEnable(GL_CULL_FACE);
			// Drawing operation

			// Instruct OpenGL to use our shader program and our VAO
			glUseProgram(programId);
			glBindVertexArray(vaoId);
			glBindTexture(GL_TEXTURE_2D, textureId);

			// Find uniform locations
			GLint modelLoc = glGetUniformLocation(programId, "u_Model");
			GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");

			// Prepare the perspective projection matrix
			glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float)800 / (float)600, 0.1f, 100.f);

			// Prepare the model matrix
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(0, 0, -2.5f));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));

			// Increase the float angle so next frame the triangle rotates further
			angle += 2.5f;

			// Make sure the current program is bound

			// Upload the model matrix
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			// Upload the projection matrix
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
				glm::value_ptr(projection));

			// Draw shape as before


			// Draw 3 vertices (a triangle)
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisable(GL_CULL_FACE);
			//// Draw 6 vertices (2 triangles making a square)
			//glDrawArrays(GL_TRIANGLES, 0, 6);

			// Reset the state
			glBindVertexArray(0);
			glUseProgram(0);
			glBindTexture(GL_TEXTURE_2D, 0);

			SDL_GL_SwapWindow(window);
		}

	}


}