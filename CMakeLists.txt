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
   	src/ApeX/TriSpin.h
    src/ApeX/TriSpin.cpp
	src/ApeX/Window.cpp
	src/ApeX/Window.h

)
target_link_libraries(
	ApeX SDL2 GLEW32 OpenGL32
)


add_executable(demo
  src/demo/main.cpp
  src/demo/Employee.cpp
  src/demo/Employee.h
)
target_link_libraries(demo ApeX)