cmake_minimum_required(VERSION 3.14)

project(meshes LANGUAGES CXX C)

# We want C++11
set (CMAKE_CXX_STANDARD 11)

set(SRCS
    src/mesh.cpp
    src/types.h
    src/shape.h
    src/mesh.h
    src/halfedge.cpp
    src/halfedge.h
    src/debugging.h
    src/shape.cpp
)

include(cmake/CPM.cmake)
CPMAddPackage("https://github.com/g-truc/glm.git#0.9.9.8")

## Put all our code (except main_meshes.cpp) in a library so that testers can also use it without recompiling.
add_library(meshlib OBJECT ${SRCS})
target_include_directories(meshlib PUBLIC include)
target_link_libraries(meshlib glm::glm)

## The main meshes executable
add_executable(meshes src/main_meshes.cpp)
target_link_libraries(meshes meshlib)

## On Windows, the following line may help build a static binary that can be called from the autograder.
## Some platforms can't build entirely static binaries, so I'm leaving it off by default.
if(WIN32)
    target_link_libraries(meshes -static)
endif()

## Add tests
enable_testing()
add_subdirectory( test )

# A CMake target to create the desired outputs
# Create the output directory and remember its location.
set(OUTPUT "${PROJECT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY "${OUTPUT}")
set(EXAMPLES "${PROJECT_SOURCE_DIR}/examples")
add_custom_target(output
    COMMAND meshes cylinder 5 10 cylinder-5-10.obj
    COMMAND meshes cylinder 30 2 cylinder-30-2.obj
    COMMAND meshes cylinder 3 2 cylinder-3-2.obj
    
    COMMAND meshes sphere 5 30 sphere-5-30.obj
    COMMAND meshes sphere 30 30 sphere-30-30.obj
    COMMAND meshes sphere 3 1 sphere-3-1.obj
    
    COMMAND meshes normals unweighted "${EXAMPLES}/cube_binary.obj" cube_binary-unweighted.obj
    COMMAND meshes normals_halfedge unweighted "${EXAMPLES}/cube_binary.obj" cube_binary-unweighted-halfedge.obj
    
    COMMAND meshes normals unweighted "${EXAMPLES}/bunny.obj" bunny-unweighted.obj
    COMMAND meshes normals_halfedge unweighted "${EXAMPLES}/bunny.obj" bunny-unweighted-halfedge.obj
    
    COMMAND meshes normals unweighted "${EXAMPLES}/cylinder_irregular.obj" cylinder_irregular-unweighted.obj
    COMMAND meshes normals_halfedge unweighted "${EXAMPLES}/cylinder_irregular.obj" cylinder_irregular-unweighted-halfedge.obj
    
    COMMAND meshes normalize "${EXAMPLES}/bunny.obj" bunny-normalize.obj
    
    COMMAND meshes transform 0 0 1 0 0 1 0 0 -.1 0 0 0 0 0 0 1 "${EXAMPLES}/cylinder-30-2.obj" cylinder-30-2-transform-yzsquash.obj
    
    # the following are bonus
    COMMAND meshes cone 5 10 cone-5-10.obj
    COMMAND meshes cone 30 1 cone-30-1.obj
    COMMAND meshes cone 3 1 cone-3-1.obj
    
    COMMAND meshes torus 5 10 .25 torus-5-10-.25.obj
    COMMAND meshes torus 20 3 .1 torus-20-3-.1.obj
    COMMAND meshes torus 3 3 .25 torus-3-3-.25.obj
    
    COMMAND meshes cube 2 cube-2.obj
    COMMAND meshes cube 10 cube-10.obj
    
    COMMAND meshes normals angle_weighted "${EXAMPLES}/cube_binary.obj" cube_binary-angle_weighted.obj
    COMMAND meshes normals_halfedge angle_weighted "${EXAMPLES}/cube_binary.obj" cube_binary-angle_weighted-halfedge.obj
    
    COMMAND meshes normals angle_weighted "${EXAMPLES}/bunny.obj" bunny-angle_weighted.obj
    COMMAND meshes normals_halfedge angle_weighted "${EXAMPLES}/bunny.obj" bunny-angle_weighted-halfedge.obj
    
    COMMAND meshes normals angle_weighted "${EXAMPLES}/cylinder_irregular.obj" cylinder_irregular-angle_weighted.obj
    COMMAND meshes normals_halfedge angle_weighted "${EXAMPLES}/cylinder_irregular.obj" cylinder_irregular-angle_weighted-halfedge.obj
    
    WORKING_DIRECTORY "${OUTPUT}"
    VERBATIM
    )
# This output target depends on the meshes target having been created.
add_dependencies(output meshes)

include("CMakeLists-zip.txt" OPTIONAL)
