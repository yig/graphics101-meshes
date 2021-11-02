#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "mesh.h"
#include "shape.h"

#include <iostream>
#include "debugging.h"

using namespace graphics101;

const real eps = 1e-5;

#define GENERATE_DATA 0
#if GENERATE_DATA
#include "meshdebug.h"
#else
#include "shape_test_cylinder_5_10.h"
#include "shape_test_sphere_5_10.h"
#endif

// Helper function used by all tests
namespace {
real hausdorff_distance( const std::vector< vec3 >& ptsA, const std::vector< vec3 >& ptsB ) {
    // The Hausdorff distance is the maximum minimum distance of any point in A to any point in B
    // or the maximum minimum distance of any point in B to any point in A, whichever is larger.
    
    if( ptsA.size() == 0 && ptsB.size() == 0 ) return 0.0;
    else if( ptsA.size() == 0 || ptsB.size() == 0 ) return infinity;
    
    // Find the maximum distance from any point in ptsA to its closest point in ptsB.
    real maxAtoB = 0.;
    for( const auto& pA : ptsA ) {
        real minToB = infinity;
        for( const auto& pB : ptsB ) {
            minToB = std::min( minToB, length( pA - pB ) );
        }
        maxAtoB = std::max( maxAtoB, minToB );
    }
    
    real maxBtoA = 0.;
    for( const auto& pB : ptsB ) {
        real minToA = infinity;
        for( const auto& pA : ptsA ) {
            minToA = std::min( minToA, length( pA - pB ) );
        }
        maxBtoA = std::max( maxBtoA, minToA );
    }
    
    const real result = std::max( maxAtoB, maxBtoA );
    MESSAGE( "Hausdorff distance: ", result );
    return result;
}
}

TEST_CASE( "Cylinder Test" ) {
    Mesh truth;
    const int slices = 5;
    const int stacks = 10;
#if GENERATE_DATA
    Cylinder( slices, stacks ).tessellate( truth );
    PrintToInclude( "getCylinder_5_10", "shape_test_cylinder_5_10.h", truth );
#else
    truth = getCylinder_5_10();
#endif
    
    Mesh mesh;
    Cylinder( slices, stacks ).tessellate( mesh );
    
#if !GENERATE_DATA
    // There should be a position for each slice * stack
    CHECK( mesh.positions.size() == slices*stacks );
    // There should be no more normals than positions. There could be less.
    CHECK( mesh.normals.size() <= slices*stacks );
    
    if( truth.positions.size() != slices*stacks ) FAIL("Ground Truth data is corrupt.");
    
    // The Hausdorff distance of positions and normals should be zero.
    // This assumes normals are all normalized.
    CHECK( hausdorff_distance( mesh.positions, truth.positions ) < eps );
    CHECK( hausdorff_distance( mesh.normals, truth.normals ) < eps );
    // Cylinder normals should all have z = 0 and x**2 + y**2 = 1.
    // The hausdorff distance should check this.
    
    // There should be a known number of faces.
    CHECK( mesh.face_positions.size() == truth.face_positions.size() );
    // There should be the same number of normal faces as position faces.
    CHECK( mesh.face_normals.size() == mesh.face_positions.size() );
    for( int fi = 0; fi < mesh.face_positions.size(); ++fi ) {
        // The face should index into positions.
        const auto& fp = mesh.face_positions.at(fi);
        CHECK( fp.A >= 0 );
        CHECK( fp.A < mesh.positions.size() );
        CHECK( fp.B >= 0 );
        CHECK( fp.B < mesh.positions.size() );
        CHECK( fp.C >= 0 );
        CHECK( fp.C < mesh.positions.size() );
        
        const auto& fn = mesh.face_normals.at(fi);
        CHECK( fn.A >= 0 );
        CHECK( fn.A < mesh.normals.size() );
        CHECK( fn.B >= 0 );
        CHECK( fn.B < mesh.normals.size() );
        CHECK( fn.C >= 0 );
        CHECK( fn.C < mesh.normals.size() );
        
        // The face position should match its normal but with z=0.
        CHECK( length( vec3( vec2( mesh.positions.at( fp.A ) ), 0.0 ) - mesh.normals.at( fn.A ) ) < eps );
        CHECK( length( vec3( vec2( mesh.positions.at( fp.B ) ), 0.0 ) - mesh.normals.at( fn.B ) ) < eps );
        CHECK( length( vec3( vec2( mesh.positions.at( fp.C ) ), 0.0 ) - mesh.normals.at( fn.C ) ) < eps );
        
        // Are faces CCW?
        CHECK( dot(
            mesh.positions.at(fp.B),
            cross(mesh.positions.at(fp.B)-mesh.positions.at(fp.A),mesh.positions.at(fp.C)-mesh.positions.at(fp.A))
            ) > eps );
    }
#endif
}

TEST_CASE( "Sphere Test" ) {
    Mesh truth;
    const int slices = 5;
    const int stacks = 10;
#if GENERATE_DATA
    Sphere( slices, stacks ).tessellate( truth );
    PrintToInclude( "getSphere_5_10", "shape_test_sphere_5_10.h", truth );
#else
    truth = getSphere_5_10();
#endif
    
    Mesh mesh;
    Sphere( slices, stacks ).tessellate( mesh );
    
#if !GENERATE_DATA
    // There should be a position for each slice * stack + 2 for the north and south poles
    CHECK( mesh.positions.size() == slices*stacks + 2 );
    // There should an equal number of normals and positions.
    CHECK( mesh.normals.size() == slices*stacks + 2 );
    
    if( truth.positions.size() != slices*stacks + 2 ) FAIL("Ground Truth data is corrupt.");
    
    // The Hausdorff distance of positions and normals should be zero.
    // This assumes normals are all normalized.
    CHECK( hausdorff_distance( mesh.positions, truth.positions ) < eps );
    CHECK( hausdorff_distance( mesh.normals, truth.normals ) < eps );
    // Cylinder normals should all have z = 0 and x**2 + y**2 = 1.
    // The hausdorff distance should check this.
    
    // There should be a known number of faces.
    CHECK( mesh.face_positions.size() == truth.face_positions.size() );
    // There should be the same number of normal faces as position faces.
    CHECK( mesh.face_normals.size() == mesh.face_positions.size() );
    for( int fi = 0; fi < mesh.face_positions.size(); ++fi ) {
        // The face should index into positions.
        const auto& fp = mesh.face_positions.at(fi);
        CHECK( fp.A >= 0 );
        CHECK( fp.A < mesh.positions.size() );
        CHECK( fp.B >= 0 );
        CHECK( fp.B < mesh.positions.size() );
        CHECK( fp.C >= 0 );
        CHECK( fp.C < mesh.positions.size() );
        
        const auto& fn = mesh.face_normals.at(fi);
        CHECK( fn.A >= 0 );
        CHECK( fn.A < mesh.normals.size() );
        CHECK( fn.B >= 0 );
        CHECK( fn.B < mesh.normals.size() );
        CHECK( fn.C >= 0 );
        CHECK( fn.C < mesh.normals.size() );
        
        // The face position should match its normal.
        CHECK( length( mesh.positions.at( fp.A ) - mesh.normals.at( fn.A ) ) < eps );
        CHECK( length( mesh.positions.at( fp.B ) - mesh.normals.at( fn.B ) ) < eps );
        CHECK( length( mesh.positions.at( fp.C ) - mesh.normals.at( fn.C ) ) < eps );
        
        // Are faces CCW?
        CHECK( dot(
            mesh.positions.at(fp.B),
            cross(mesh.positions.at(fp.B)-mesh.positions.at(fp.A),mesh.positions.at(fp.C)-mesh.positions.at(fp.A))
            ) > eps );
    }
#endif
}
