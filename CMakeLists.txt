cmake_minimum_required (VERSION 3.8)

set(CMAKE_C_FLAGS "/utf-8")
set(CMAKE_CXX_FLAGS "/utf-8")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJ "UselessEngine")
project (${PROJ})

include_directories(${PROJECT_SOURCE_DIR}/3rd)
include_directories(${PROJECT_SOURCE_DIR}/core/include)

file(GLOB_RECURSE SRC_LIST
	"${PROJECT_SOURCE_DIR}/3rd/*.cpp" 
	"${PROJECT_SOURCE_DIR}/core/src/*.cpp")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/out)
add_executable(${PROJ} ${SRC_LIST} "core/main.cpp")

set_target_properties(${PROJ} PROPERTIES COMPILE_FLAGS "/EHsc")

link_directories(${PROJECT_SOURCE_DIR}/lib)

if (MSVC)
	target_link_libraries(${PROJ} 
		${PROJECT_SOURCE_DIR}/lib/glfw-vc2022/glfw3.lib
		${PROJECT_SOURCE_DIR}/lib/vulkan/vulkan-1.lib
		${PROJECT_SOURCE_DIR}/lib/assimp/assimp-vc143-mt.lib)

		target_link_options(${PROJ} PRIVATE "/NODEFAULTLIB:MSVCRT")
else()
	target_link_libraries(${PROJ} 
		${PROJECT_SOURCE_DIR}/lib/glfw/glfw3.lib
		${PROJECT_SOURCE_DIR}/lib/vulkan/vulkan-1.lib
		${PROJECT_SOURCE_DIR}/lib/assimp/assimp-vc143-mt.lib)
endif()