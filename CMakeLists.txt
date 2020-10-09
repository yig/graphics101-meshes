cmake_minimum_required(VERSION 3.1.0)

project(meshes)

# We want C++11
set (CMAKE_CXX_STANDARD 11)

add_executable(meshes
    src/main_meshes.cpp
    src/halfedge.cpp
    src/mesh.cpp
    src/shape.cpp
)

target_include_directories(meshes PUBLIC include)
target_link_libraries(meshes)
