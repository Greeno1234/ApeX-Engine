cmake_minimum_required(VERSION ${CMAKE_VERSION})

project(APEX)


include_directories(
  src
)



add_library(ApeX
	src/ApeX/ApeX.h
	src/ApeX/Core.cpp
	src/ApeX/Core.h	

	src/ApeX/Entity.h
	src/ApeX/Component.h
)

add_executable(demo
  src/demo/main.cpp
  src/demo/Employee.cpp src/demo/Employee.h
)
target_link_libraries(demo ApeX)