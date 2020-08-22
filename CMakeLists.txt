cmake_minimum_required(VERSION 3.1.0)

project(meshes)

# We want C++11
set (CMAKE_CXX_STANDARD 11)

add_executable(meshes
    main_meshes.cpp
    halfedge.cpp
    mesh.cpp
    shape.cpp
)

target_link_libraries(meshes)
