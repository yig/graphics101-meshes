add_executable( normalize_test normalize_test.cpp )
target_include_directories(normalize_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(normalize_test meshlib)
add_test( NAME normalize COMMAND normalize_test )

add_executable( applytransform_test applytransform_test.cpp )
target_include_directories(applytransform_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(applytransform_test meshlib)
add_test( NAME applytransform COMMAND applytransform_test )

add_executable( computeNormals_test computeNormals_test.cpp )
target_include_directories(computeNormals_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(computeNormals_test meshlib)
add_test( NAME computeNormals COMMAND computeNormals_test )

add_executable( shape_test shape_test.cpp )
target_include_directories(shape_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(shape_test meshlib)
add_test( NAME shape COMMAND shape_test )
