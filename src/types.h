#ifndef __types_h__
#define __types_h__

// We do this to get everything glm supports.
#include "glm/glm.hpp"
// We could only include the subset we need to speed up compile times.
/*
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp" // glm::mat4
*/

#include <vector>
#include <string>

#include <cassert>

#include <cmath> // std::sqrt()
#include <algorithm> // std::min(), std::max()
#include <limits> // infinity

namespace graphics101 {
    typedef float real;
    typedef glm::vec2 vec2;
    typedef glm::vec3 vec3;
    typedef glm::vec4 vec4;
    typedef glm::ivec2 ivec2;
    typedef glm::ivec3 ivec3;
    typedef glm::ivec4 ivec4;
    typedef glm::mat2 mat2;
    typedef glm::mat3 mat3;
    typedef glm::mat4 mat4;
    
    struct ray3
    {
        // point
        vec3 p;
        // direction
        vec3 d;
        
        ray3( const vec3& a_p, const vec3& a_d ) : p( a_p ), d( a_d ) {}
    };
    
    struct Triangle {
        // Indices for the three corners.
        int A = -1;
        int B = -1;
        int C = -1;
        
        // Array-like accessor for the three corners.
        int& operator[]( const int corner ) { return (&A)[corner]; }
        const int& operator[]( const int corner ) const { return (&A)[corner]; }
        
        // Construct a triangle from the three corner indices.
        Triangle( const int A_, const int B_, const int C_ ) : A( A_ ), B( B_ ), C( C_ ) {}
        
        // Compare two triangles.
        bool operator==( const Triangle& rhs ) const { return A == rhs.A && B == rhs.B && C == rhs.C; }
    };
    typedef std::vector< Triangle > TriangleVec;
    
    typedef std::string string;
    
    // Constants
    const real infinity = std::numeric_limits<real>::infinity();
    const real pi = 3.14159265358979323846264338327950288;
}

#endif /* __types_h__ */
