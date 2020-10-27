#include "mesh.h"
#include "shape.h"

#include <iostream> // std::cout and std::cerr
#include <cassert> // assert()
#include <sstream>

namespace
{
// Converts a string to the templated type.
template< typename T >
inline T strto( const std::string& str )
// Explicitly constructing the result (result = T(); instead of result;) means that
// built-in types (int, float, etc) will default to 0, and so return that in case of error.
{ std::istringstream converter( str ); T result = T(); converter >> result; return result; }
template< typename T >
inline T strto( const std::string& str, bool& success )
// Explicitly constructing the result (result = T(); instead of result;) means that
// built-in types (int, float, etc) will default to 0, and so return that in case of error.
// Optional output parameter `success` tells the caller this explicitly.
{ std::istringstream converter( str ); T result = T(); success = bool(converter >> result); return result; }

}


using namespace graphics101;

void usage( const char* argv0 ) {
    std::cerr << "Usage: " << argv0 << " cylinder #slices #stacks output.obj\n";
    std::cerr << "Usage: " << argv0 << " sphere   #slices #stacks output.obj\n";
    std::cerr << "Usage: " << argv0 << " cone     #slices #stacks output.obj\n";
    std::cerr << "Usage: " << argv0 << " torus    #slices_tube_circle #slices_cross_circle cross_radius output.obj\n";
    std::cerr << "Usage: " << argv0 << " cube     #slices_edge output.obj\n";
    std::cerr << '\n';
    std::cerr << "Usage: " << argv0 << " normals          weights input.obj output.obj (weights must be either 'unweighted' or 'angle_weighted')\n";
    std::cerr << "Usage: " << argv0 << " normals_halfedge weights input.obj output.obj (weights must be either 'unweighted' or 'angle_weighted')\n";
    std::cerr << '\n';
    std::cerr << "Usage: " << argv0 << " normalize input.obj output.obj\n";
    std::cerr << '\n';
    std::cerr << "Usage: " << argv0 << " transform M11 M21 M31 M41 M12 M22 M32 M42 M13 M23 M33 M43 M14 M24 M34 M44 input.obj output.obj (Mij are the elements of a 4x4 matrix)\n";
    std::exit(-1);
}

int main( int argc, char* argv[] ) {
    if( argc == 1 ) usage( argv[0] );
    
    const std::string command( argv[1] );
    if( command == "cylinder" ) {
        if( argc != 5 ) usage( argv[0] );
        
        bool success;
        const int slices = strto<int>( argv[2], success );
        if( !success || slices < 3 ) {
            std::cerr << "ERROR: slices must be >= 3.\n";
            usage( argv[0] );
        }
        const int stacks = strto<int>( argv[3], success );
        if( !success || stacks < 2 ) {
            std::cerr << "ERROR: stacks must be >= 2.\n";
            usage( argv[0] );
        }
        const char* outpath( argv[4] );
        
        Mesh mesh;
        Cylinder( slices, stacks ).tessellate( mesh );
        success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "sphere" ) {
        if( argc != 5 ) usage( argv[0] );
        
        bool success;
        const int slices = strto<int>( argv[2], success );
        if( !success || slices < 3 ) {
            std::cerr << "ERROR: slices must be >= 3.\n";
            usage( argv[0] );
        }
        const int stacks = strto<int>( argv[3], success );
        if( !success || stacks < 1 ) {
            std::cerr << "ERROR: stacks must be >= 1.\n";
            usage( argv[0] );
        }
        const char* outpath( argv[4] );
        
        Mesh mesh;
        Sphere( slices, stacks ).tessellate( mesh );
        success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "cone" ) {
        if( argc != 5 ) usage( argv[0] );
        
        bool success;
        const int slices = strto<int>( argv[2], success );
        if( !success || slices < 3 ) {
            std::cerr << "ERROR: slices must be >= 3.\n";
            usage( argv[0] );
        }
        const int stacks = strto<int>( argv[3], success );
        if( !success || stacks < 1 ) {
            std::cerr << "ERROR: stacks must be >= 1.\n";
            usage( argv[0] );
        }
        const char* outpath( argv[4] );
        
        Mesh mesh;
        Cone( slices, stacks ).tessellate( mesh );
        success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "torus" ) {
        if( argc != 6 ) usage( argv[0] );
        
        bool success;
        const int slices_tube_circle = strto<int>( argv[2], success );
        if( !success || slices_tube_circle < 3 ) {
            std::cerr << "ERROR: slices_tube_circle must be >= 3.\n";
            usage( argv[0] );
        }
        const int slices_cross_circle = strto<int>( argv[3], success );
        if( !success || slices_cross_circle < 3 ) {
            std::cerr << "ERROR: slices_cross_circle must be >= 3.\n";
            usage( argv[0] );
        }
        const real cross_radius = strto<real>( argv[4], success );
        if( !success || cross_radius <= 0.0 ) {
            std::cerr << "ERROR: cross_radius must be > 0\n";
            usage( argv[0] );
        }
        const char* outpath( argv[5] );
        
        Mesh mesh;
        Torus( slices_tube_circle, slices_cross_circle, cross_radius ).tessellate( mesh );
        success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "cube" ) {
        if( argc != 4 ) usage( argv[0] );
        
        bool success;
        const int slices_edge = strto<int>( argv[2], success );
        if( !success || slices_edge < 2 ) {
            std::cerr << "ERROR: slices_edge must be >= 2.\n";
            usage( argv[0] );
        }
        const char* outpath( argv[3] );
        
        Mesh mesh;
        Cube( slices_edge ).tessellate( mesh );
        success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "normals" || command == "normals_halfedge" ) {
        if( argc != 5 ) usage( argv[0] );
        
        const std::string weights = argv[2];
        const char* inpath( argv[3] );
        const char* outpath( argv[4] );
        
        Mesh mesh;
        mesh.loadFromOBJ( inpath );
        
        Mesh::MeshNormalStrategy strategy = Mesh::Unweighted;
        if( weights == "unweighted" ) {
            strategy = Mesh::Unweighted;
        }
        else if( weights == "angle_weighted" ) {
            strategy = Mesh::AngleWeighted;
        }
        else {
            std::cerr << "ERROR: Unknown normal weight type: " << weights << '\n';
            usage( argv[0] );
        }
        
        if( command == "normals" ) mesh.computeNormals( strategy );
        else mesh.computeNormalsHalfEdge( strategy );
        
        const bool success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "normalize" ) {
        if( argc != 4 ) usage( argv[0] );
        
        const char* inpath( argv[2] );
        const char* outpath( argv[3] );
        
        Mesh mesh;
        mesh.loadFromOBJ( inpath );
        const mat4 transform = mesh.normalizingTransformation();
        mesh.applyTransformation( transform );
        
        const bool success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else if( command == "transform" ) {
        if( argc != 20 ) usage( argv[0] );
        
        const char* inpath( argv[argc-2] );
        const char* outpath( argv[argc-1] );
        
        Mesh mesh;
        mesh.loadFromOBJ( inpath );
        mat4 transform;
        for( int j = 0; j < 4; ++j ) {
            for( int i = 0; i < 4; ++i ) {
                bool success;
                const real Mij = strto<real>( argv[2+i+4*j], success );
                if( !success ) {
                    std::cerr << "ERROR: Mij must be floating point numbers.\n";
                    usage( argv[0] );
                }
                transform[i][j] = Mij;
            }
        }
        mesh.applyTransformation( transform );
        
        const bool success = mesh.writeToOBJ( outpath );
        if( !success ) {
            std::cerr << "ERROR: Unable to write OBJ to path: " << outpath << '\n';
        } else {
            std::cout << "Saved: " << outpath << std::endl;
        }
    }
    else {
        std::cerr << "ERROR: Unknown command: " << command << '\n';
        usage( argv[0] );
    }
    
    return 0;
}
