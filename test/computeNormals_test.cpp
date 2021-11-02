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
#include "computeNormals_test_input.h"
#include "computeNormals_test_output_unweighted.h"
#include "computeNormals_test_output_weighted.h"
#endif

// Helper function used by all tests
namespace {
real compareMeshToTruth( const Mesh& mesh, const Mesh& truth ) {
    // computeNormals() shouldn't touch face positions or normals data
    CHECK( mesh.face_positions == truth.face_positions );
    CHECK( mesh.face_texcoords == truth.face_texcoords );
    
    // computeNormals() should make the face_normals the same as the face_positions.
    CHECK( mesh.face_normals == truth.face_positions );
    
    // computeNormals() shouldn't touch positions or texcoords
    CHECK( mesh.positions == truth.positions );
    CHECK( mesh.texcoords == truth.texcoords );
    
    // positions() should create a normal per position.
    CHECK( mesh.normals.size() == truth.positions.size() );
    
    // Let's find the maximum deviation of any normal and use that as the score.
    real score = 0.0;
    for( int i = 0; i < mesh.normals.size(); ++i ) {
        const real l = length( mesh.normals.at(i) - truth.normals.at(i) );
        score = std::max( score, l );
        CHECK( l < eps );
    }
    const real score_out_of_100 = (100.0*(1.0-score));
    // std::cerr << "Score: " << (100.0*(1.0-score)) << '\n';
    MESSAGE( "Score: ", score_out_of_100 );
    return score;
}
}

TEST_CASE( "Compute Normals Test (Not HalfEdge, Unweighted)" ) {
    Mesh mesh;
#if GENERATE_DATA
    mesh.loadFromOBJ( "../examples/cylinder_irregular.obj" );
    PrintToInclude( "getComputeNormalsInput", "computeNormals_test_input.h", mesh );
#else
    mesh = getComputeNormalsInput();
#endif
    
    mesh.computeNormals( Mesh::Unweighted );
    
#if GENERATE_DATA
    PrintToInclude( "getComputeNormalsOutputUnweighted", "computeNormals_test_output_unweighted.h", mesh );
#else
    const Mesh truth = getComputeNormalsOutputUnweighted();
    compareMeshToTruth( mesh, truth );
#endif
}

TEST_CASE( "Compute Normals Test (Not HalfEdge, Weighted)" ) {
    Mesh mesh;
#if GENERATE_DATA
    mesh.loadFromOBJ( "../examples/cylinder_irregular.obj" );
    PrintToInclude( "getComputeNormalsInput", "computeNormals_test_input.h", mesh );
#else
    mesh = getComputeNormalsInput();
#endif
    
    mesh.computeNormals( Mesh::AngleWeighted );
    
#if GENERATE_DATA
    PrintToInclude( "getComputeNormalsOutputWeighted", "computeNormals_test_output_weighted.h", mesh );
#else
    const Mesh truth = getComputeNormalsOutputWeighted();
    compareMeshToTruth( mesh, truth );
#endif
}

#if !GENERATE_DATA

TEST_CASE( "Compute Normals Test (HalfEdge, Unweighted)" ) {
    Mesh mesh;
    mesh = getComputeNormalsInput();
    
    mesh.computeNormalsHalfEdge( Mesh::Unweighted );
    
    const Mesh truth = getComputeNormalsOutputUnweighted();
    compareMeshToTruth( mesh, truth );
}

TEST_CASE( "Compute Normals Test (HalfEdge, Weighted)" ) {
    Mesh mesh;
    mesh = getComputeNormalsInput();
    
    mesh.computeNormalsHalfEdge( Mesh::AngleWeighted );
    
    const Mesh truth = getComputeNormalsOutputWeighted();
    compareMeshToTruth( mesh, truth );
}

#endif
