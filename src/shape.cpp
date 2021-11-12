#include "shape.h"

#include "debugging.h"

#include <cmath> // std::sin(), std::cos()

using namespace glm;

namespace graphics101 {

void Cylinder::tessellate( Mesh& mesh )
{
    mesh.clear();
    
    // Your code goes here.
    auto ij2index = [&]( int i, int j ) { return -1; };
}

void Sphere::tessellate( Mesh& mesh )
{
    mesh.clear();
    
    // Your code goes here.
    auto ij2index = [&]( int i, int j ) { return -1; };
}

void Cone::tessellate( Mesh& mesh )
{
    mesh.clear();
    
    // Your code goes here.
    auto ij2index = [&]( int i, int j ) { return -1; };
}

void Torus::tessellate( Mesh& mesh )
{
    mesh.clear();
    
    // Your code goes here.
    auto ij2index = [&]( int i, int j ) { return -1; };
}

void Cube::tessellate( Mesh& mesh )
{
    mesh.clear();
    
    // Your code goes here.
}

}
