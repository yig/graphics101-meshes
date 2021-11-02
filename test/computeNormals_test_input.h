#ifndef __getComputeNormalsInput_h__
#define __getComputeNormalsInput_h__

#include "mesh.h"

inline graphics101::Mesh getComputeNormalsInput() {
    graphics101::Mesh result;
    
    result.positions.reserve( 0 );
    result.face_positions.reserve( 0 );
    
    result.normals.reserve( 0 );
    result.face_normals.reserve( 0 );
    
    result.texcoords.reserve( 0 );
    result.face_texcoords.reserve( 0 );
    
    
    
    
    
    
    
    return result;
}

#endif