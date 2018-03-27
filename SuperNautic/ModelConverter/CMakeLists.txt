add_executable(ModelConverter
  src/fileOutputFuncs.cpp
  src/main.cpp
  src/mathConverterFuncs.cpp
  src/stringFuncs.cpp
)

target_link_libraries(ModelConverter 
assimp
glm_static
)


target_compile_features(ModelConverter
  PUBLIC
    cxx_std_11
)

target_include_directories(ModelConverter
  PUBLIC 
    ${CMAKE_SOURCE_DIR}/SuperNautic/thirdparty/assimp/include
)
