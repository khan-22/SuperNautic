cmake_minimum_required(VERSION 3.9.0)

project(SuperNautic)

add_subdirectory(thirdparty/assimp)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/SFML)

add_subdirectory(ModelConverter)
add_subdirectory(SuperNautic_Game)