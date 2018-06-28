cmake_minimum_required(VERSION 3.5.0)

project(SuperNautic)

add_subdirectory(thirdparty/glew/build/cmake)
target_include_directories(glew PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glew/include>)
add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/SFML)

add_subdirectory(ModelConverter)
add_subdirectory(SuperNautic_Game)
