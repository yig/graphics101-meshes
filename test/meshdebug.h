#ifndef __meshdebug_h__
#define __meshdebug_h__

#include <iostream>
#include <iomanip>
#include <fstream>
#include "mesh.h"

// Prints the mesh data as a C++ file that can be included.
inline void PrintToInclude( const char* functionName, const char* outpath, const graphics101::Mesh& mesh ) {
    std::ofstream out( outpath );
    // https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout
    out.precision(17);
    
    out << "#ifndef __" << functionName << "_h__\n";
    out << "#define __" << functionName << "_h__\n";
    out << "\n";
    out << "#include \"mesh.h\"\n";
    out << "\n";
    out << "inline graphics101::Mesh " << functionName << "() {\n";
    out << "    graphics101::Mesh result;\n";
    out << "    \n";
    out << "    result.positions.reserve( " << mesh.positions.size() << " );\n";
    out << "    result.face_positions.reserve( " << mesh.face_positions.size() << " );\n";
    out << "    \n";
    out << "    result.normals.reserve( " << mesh.normals.size() << " );\n";
    out << "    result.face_normals.reserve( " << mesh.face_normals.size() << " );\n";
    out << "    \n";
    out << "    result.texcoords.reserve( " << mesh.texcoords.size() << " );\n";
    out << "    result.face_texcoords.reserve( " << mesh.face_texcoords.size() << " );\n";
    out << "    \n";
    
    for( const auto& p : mesh.positions ) {
        out << "    result.positions.emplace_back( " << p[0] << ", " << p[1] << ", " << p[2] << " );\n";
    }
    out << "    \n";
    for( const auto& p : mesh.normals ) {
        out << "    result.normals.emplace_back( " << p[0] << ", " << p[1] << ", " << p[2] << " );\n";
    }
    out << "    \n";
    for( const auto& p : mesh.texcoords ) {
        out << "    result.texcoords.emplace_back( " << p[0] << ", " << p[1] << " );\n";
    }
    out << "    \n";
    for( const auto& p : mesh.face_positions ) {
        out << "    result.face_positions.emplace_back( " << p[0] << ", " << p[1] << ", " << p[2] << " );\n";
    }
    out << "    \n";
    for( const auto& p : mesh.face_normals ) {
        out << "    result.face_normals.emplace_back( " << p[0] << ", " << p[1] << ", " << p[2] << " );\n";
    }
    out << "    \n";
    for( const auto& p : mesh.face_texcoords ) {
        out << "    result.face_texcoords.emplace_back( " << p[0] << ", " << p[1] << ", " << p[2] << " );\n";
    }
    
    out << "    \n";
    
    out << "    return result;\n";
    out << "}\n";
    out << "\n";
    out << "#endif";
}

#endif
