cmake_minimum_required(VERSION ${CMAKE_VERSION})

project(APEX)

add_definitions(-DSDL_MAIN_HANDLED)

include_directories(
  src
  contrib/include
  contrib/cl-windows-x86_64/include
)
link_directories(
	contrib/cl-windows-x86_64/lib
)


add_library(ApeX
	src/ApeX/ApeX.h
	src/ApeX/Core.cpp
	src/ApeX/Core.h	
	src/ApeX/Entity.cpp
	src/ApeX/Entity.h
	src/ApeX/Component.h
	src/ApeX/Component.cpp
	src/ApeX/TriangleRenderer.cpp
   	src/ApeX/TriangleRenderer.h
	src/ApeX/Window.cpp
	src/ApeX/Window.h
	src/ApeX/Resource.h
	src/ApeX/Resources.h
	src/ApeX/Texture.h
	src/ApeX/Texture.cpp
	src/ApeX/Model.h
	src/ApeX/Model.cpp
	src/ApeX/Transform.h
	src/ApeX/Transform.cpp
	src/ApeX/Renderer.cpp
   	src/ApeX/Renderer.h
	src/ApeX/Audio.h
	src/ApeX/Audio.cpp
	src/ApeX/Camera.h
	src/ApeX/Camera.cpp
	src/ApeX/AudioSource.h
	src/ApeX/AudioSource.cpp
	src/ApeX/Keyboard.h
	src/ApeX/Keyboard.cpp
	src/ApeX/BoxCollider.h
	src/ApeX/BoxCollider.cpp
	src/ApeX/RigidBody.h
	src/ApeX/RigidBody.cpp
)
target_link_libraries(
	ApeX rend
)

###########################################################################
# rend
###########################################################################

add_library(rend
  src/rend/Animation.cpp
  src/rend/Buffer.cpp
  src/rend/Debug.cpp
  src/rend/Font.cpp
  src/rend/Mesh.cpp
  src/rend/Model.cpp
  src/rend/ModelShader.cpp
  src/rend/RenderTexture.cpp
  src/rend/Shader.cpp
  src/rend/TextShader.cpp
  src/rend/Texture.cpp
  src/rend/rendmath.cpp
  src/rend/utils.cpp
)
target_link_libraries(rend SDL2 GLEW32 OpenGL32 openal32)


add_executable(demo
  src/demo/main.cpp
)
target_link_libraries(demo ApeX)


add_executable(demo2
  src/demo2/main.cpp
)
target_link_libraries(demo2 ApeX)