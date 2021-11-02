#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mesh.h"

#include <iostream>
#include "debugging.h"

using namespace graphics101;

const real eps = 1e-5;

#define GENERATE_DATA 0
#if GENERATE_DATA
#include "meshdebug.h"
#else
#include "applytransform_test_input.h"
#include "applytransform_test_output.h"
#endif

TEST_CASE( "Apply Transformation Test" ) {
    Mesh mesh;
#if GENERATE_DATA
    mesh.loadFromOBJ( "../examples/sphere-5-30.obj" );
    PrintToInclude( "getApplyTransformInput", "applytransform_test_input.h", mesh );
#else
    mesh = getApplyTransformInput();
#endif
    
    const mat4 T( 1, 2, -3, 4, -5, 16, 70, .8, 91, 102, 121, 1.2, 113, 1.4, .15, .016 );
    mesh.applyTransformation( T );
    
#if GENERATE_DATA
    PrintToInclude( "getApplyTransformOutput", "applytransform_test_output.h", mesh );
#else
    const Mesh truth = getApplyTransformOutput();
    
    // applyTransformation() shouldn't touch face data
    CHECK( mesh.face_positions == truth.face_positions );
    CHECK( mesh.face_normals == truth.face_normals );
    CHECK( mesh.face_texcoords == truth.face_texcoords );
    
    // applyTransformation() shouldn't touch texcoords
    CHECK( mesh.texcoords == truth.texcoords );
    
    // applyTransformation() shouldn't change the number of positions and normals.
    CHECK( mesh.positions.size() == truth.positions.size() );
    CHECK( mesh.normals.size() == truth.normals.size() );
    
    // Let's find the maximum deviation of any position or normal and use that as the score.
    real score = 0.0;
    for( int i = 0; i < mesh.positions.size(); ++i ) {
        const real l = length( mesh.positions.at(i) - truth.positions.at(i) );
        score = std::max( score, l );
        CHECK( l < eps );
    }
    for( int i = 0; i < mesh.normals.size(); ++i ) {
        const real l = length( mesh.normals.at(i) - truth.normals.at(i) );
        score = std::max( score, l );
        CHECK( l < eps );
    }
    const real score_out_of_100 = (100.0*(1.0-score));
    // std::cerr << "Score: " << score_out_of_100 << '\n';
    MESSAGE( "Score: ", score_out_of_100 );
#endif
}
