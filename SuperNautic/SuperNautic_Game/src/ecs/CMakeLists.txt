set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


add_library(ecs SHARED
  impl/ecs.cpp
  impl/Entity.cpp
  impl/TypeId.cpp
  impl/DefaultContext.cpp
  src/EntityManager.cpp
)

target_include_directories(ecs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)