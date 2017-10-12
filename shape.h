#ifndef __shape_h__
#define __shape_h__

#include "types.h"
#include "mesh.h"
#include <cassert>

namespace graphics101 {

class Shape {
public:
    virtual ~Shape() {}
    
    // Fills out the position, normal, and texcoord properties of `mesh_out`,
    // as well as the accompanying face_* properties.
    virtual void tessellate( Mesh& mesh_out ) = 0;
};

// A cylinder whose axis runs from (0,0,0) to (0,0,1) with radius 1.
// The cylinder has no top or bottom cap.
// The first texture coordinates runs around the circle of the cylinder.
// The second texture coordinate runs up the cylinder. It is equal to z.
class Cylinder : public Shape {
public:
    Cylinder( int slices, int stacks ) : m_slices( slices ), m_stacks( stacks ) {
        assert( slices >= 3 );
        assert( stacks >= 2 );
    }
    Cylinder() = default;
    
    void tessellate( Mesh& mesh_out );
    
private:
    int m_slices = 10;
    int m_stacks = 2;
};

// A sphere whose center is (0,0,0) with radius 1.
// The first texture coordinate follows the slices.
// The second texture coordinate follows the stacks. It is 0 when z = -1 and 1 when z = 1.
class Sphere : public Shape {
public:
    Sphere( int slices, int stacks ) : m_slices( slices ), m_stacks( stacks ) {
        assert( slices >= 3 );
        assert( stacks >= 1 );
    }
    Sphere() = default;
    
    void tessellate( Mesh& mesh_out );
    
private:
    int m_slices = 10;
    int m_stacks = 10;
};

// A cone whose axis runs from (0,0,0) to (0,0,1) with radius 1.
// The cone has no bottom cap.
// The first texture coordinates runs around the circle of the cone.
// The second texture coordinate runs up the cone. It is equal to z.
class Cone : public Shape {
public:
    Cone( int slices, int stacks ) : m_slices( slices ), m_stacks( stacks ) {
        assert( slices >= 3 );
        assert( stacks >= 1 );
    }
    Cone() = default;
    
    void tessellate( Mesh& mesh_out );
    
private:
    int m_slices = 10;
    int m_stacks = 2;
};

// A torus (donut). The center of the doughy tube is the unit circle
// in the xy plane with radius 1. The cross section has radius m_cross_radius.
// You can think of it as if dough with constant thickness m_cross_radius was
// added to the unit circle in the xy plane.
// The first texture coordinate runs around each cross section circle.
// The second texture coordinate runs around the tube.
class Torus : public Shape {
public:
    Torus( int slices_tube_circle, int slices_cross_circle, real cross_radius )
        : m_slices_tube_circle( slices_tube_circle ), m_slices_cross_circle( slices_cross_circle ), m_cross_radius( cross_radius )
    {
        assert( slices_tube_circle >= 3 );
        assert( slices_cross_circle >= 3 );
        assert( cross_radius > 0.0 );
    }
    Torus() = default;
    
    void tessellate( Mesh& mesh_out );
    
private:
    int m_slices_tube_circle = 10;
    int m_slices_cross_circle = 6;
    real m_cross_radius = 0.25;
};

// A unit cube whose sides are axis-aligned and whose corners are (-1,-1,-1) and (1,1,1).
// Each face should have texture coordinates that map the entire texture from 0,0 to 1,1
// onto the square.
class Cube : public Shape {
public:
    Cube( int slices_edge ) : m_slices_edge( slices_edge ) {
        assert( slices_edge >= 2 );
    }
    Cube() = default;
    
    void tessellate( Mesh& mesh_out );
    
private:
    int m_slices_edge = 2;
};

}

#endif /* __shape_h__ */
