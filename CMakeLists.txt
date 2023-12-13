cmake_minimum_required (VERSION 3.8)

set(PROJ "UselessEngine")
project (${PROJ})

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/3rd)
include_directories(${PROJECT_SOURCE_DIR}/core/include)

file(GLOB_RECURSE SRC_LIST
	"${PROJECT_SOURCE_DIR}/core/src/*.c" 
	"${PROJECT_SOURCE_DIR}/core/src/*.cpp")
	
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)
add_executable(${PROJ} core/main.cpp ${SRC_LIST})

link_directories(${PROJECT_SOURCE_DIR}/lib)

if (CMAKE_GENERATOR MATCHES "Visual Studio 17 2022")
	target_link_libraries(${PROJ} 
		${PROJECT_SOURCE_DIR}/lib/glfw-vc2022/glfw3.lib
		${PROJECT_SOURCE_DIR}/lib/vulkan/vulkan-1.lib)
else()
	target_link_libraries(${PROJ} 
		${PROJECT_SOURCE_DIR}/lib/glfw/glfw3.lib
		${PROJECT_SOURCE_DIR}/lib/vulkan/vulkan-1.lib)
endif()