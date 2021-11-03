#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mesh.h"

#include <iostream>
#include "debugging.h"

using namespace graphics101;

const real eps = 1e-5;

real MaxAbsoluteValue( const mat4& mat ) {
    real result = 0.0;
    
    for( int i = 0; i < 4; ++i ) {
        for( int j = 0; j < 4; ++j ) {
            result = glm::max( result, glm::abs( mat[i][j] ) );
        }
    }
    
    return result;
}

TEST_CASE( "Normalizing Transformation Test Empty Mesh" ) {
    Mesh mesh;
    const mat4 T = mesh.normalizingTransformation();
    // Normalizing an empty mesh should return the identity matrix.
    const mat4 TrueTransform(1.0);
    
    const real totalDeviation = MaxAbsoluteValue( T - TrueTransform );
    
    // std::cout << "Your deviation: " << totalDeviation << '\n';
    MESSAGE( "Your deviation: ", totalDeviation );
    CHECK( totalDeviation < eps );
}

TEST_CASE( "Normalizing Transformation Test Max X Mesh" ) {
    Mesh mesh;
    mesh.positions.push_back( vec3( 1.0, -1.0, 0.5 ) );
    mesh.positions.push_back( vec3( 5.0, -3.0, 0.7 ) );
    
    const mat4 T = mesh.normalizingTransformation();
    // Normalizing an empty mesh should return the matrix that translates by -3 and then scales by 1/2.
    const real TrueScale = 0.5;
    mat4 TrueTransform = mat4( mat3( TrueScale ) );
    TrueTransform[3] = vec4( TrueScale*vec3( -3.0, 2.0, -0.6 ), 1.0 );
    
    const real totalDeviation = MaxAbsoluteValue( T - TrueTransform );
    
    // std::cout << "Your deviation: " << totalDeviation << '\n';
    MESSAGE( "Your deviation: ", totalDeviation );
    CHECK( totalDeviation < eps );
}

TEST_CASE( "Normalizing Transformation Test Max Y Mesh" ) {
    Mesh mesh;
    mesh.positions.push_back( vec3( 1.0, -10.0, 0.5 ) );
    mesh.positions.push_back( vec3( 5.0, -30.0, 0.7 ) );
    
    const mat4 T = mesh.normalizingTransformation();
    // Normalizing an empty mesh should return the matrix that translates by -3 and then scales by 1/2.
    const real TrueScale = 2.0/20.0;
    mat4 TrueTransform = mat4( mat3( TrueScale ) );
    TrueTransform[3] = vec4( TrueScale*vec3( -3.0, 20.0, -0.6 ), 1.0 );
    
    const real totalDeviation = MaxAbsoluteValue( T - TrueTransform );
    
    // std::cout << "Your matrix:\n" << T << '\n';
    // std::cout << "True matrix:\n" << TrueTransform << '\n';
    
    // std::cout << "Your deviation: " << totalDeviation << '\n';
    MESSAGE( "Your deviation: ", totalDeviation );
    CHECK( totalDeviation < eps );
}

TEST_CASE( "Normalizing Transformation Test Max Z Mesh" ) {
    Mesh mesh;
    mesh.positions.push_back( vec3( 1.0, -1.0, 500.0 ) );
    mesh.positions.push_back( vec3( 5.0, -3.0, 600.0 ) );
    
    const mat4 T = mesh.normalizingTransformation();
    // Normalizing an empty mesh should return the matrix that translates by -3 and then scales by 1/2.
    const real TrueScale = 2.0/100.0;
    mat4 TrueTransform = mat4( mat3( TrueScale ) );
    TrueTransform[3] = vec4( TrueScale*vec3( -3.0, 2.0, -550.0), 1.0 );
    
    const real totalDeviation = MaxAbsoluteValue( T - TrueTransform );
    
    // std::cout << "Your deviation: " << totalDeviation << '\n';
    MESSAGE( "Your deviation: ", totalDeviation );
    CHECK( totalDeviation < eps );
}
