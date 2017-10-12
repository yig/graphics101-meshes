#ifndef __halfedge_h__
#define __halfedge_h__

#include "mesh.h" // Triangle
#include <map>

namespace graphics101
{

class HalfEdgeTriMesh
{
public:
    // We need a signed integer type so we can use -1 for an invalid index.
    typedef long Index;
    typedef std::vector< Index > Indices;
    
    // A special type that acts like an integer index but requires the
    // user to explicitly create from an integer to prevent bugs.
    struct HalfEdgeIndex {
        HalfEdgeIndex() : value( -1 ) {}
        // Mark the constructor from an integer explicit
        // so that an integer can't be silently and automatically converted.
        // This is the primary additional type safety.
        explicit HalfEdgeIndex( const Index& val ) : value( val ) {}
        // Automatically convert back to an Index as needed.
        inline operator Index& () { return value; }
        inline operator const Index& () const { return value; }
        // TODO Q: Do we need to write an explicit operator==()?
        // operator==( const HalfEdgeIndex& rhs ) { return this->value == rhs.value; }
        // Return true if the index is valid, and false otherwise.
        bool valid() const { return value >= 0; }
        // The value itself.
        Index value;
    };
    typedef std::vector< HalfEdgeIndex > HalfEdgeIndices;
    
    typedef ivec2 Edge;
    typedef std::vector< Edge > Edges;
    
    struct HalfEdge
    {
        // Index into the vertex array for the vertex this half-edge points to.
        Index to_vertex;
        // Index into the face array for the face of this half-edge.
        Index face;
        // Index into the edges array for the edge this half-edge is part of.
        Index edge;
        // The index into the half-edges array of the opposite half-edge.
        // Call halfedge() to get the actual HalfEdge struct.
        HalfEdgeIndex opposite_he;
        // The index into the half-edges array of the next half-edge around the face.
        // Call halfedge() to get the actual HalfEdge struct.
        HalfEdgeIndex next_he;
        
        HalfEdge() :
            to_vertex( -1 ),
            face( -1 ),
            edge( -1 ),
            opposite_he( -1 ),
            next_he( -1 )
            {}
    };
    
    // Build the half-edge data structure from `triangles`.
    // Call unordered_edges_from_triangles( triangles ) to get a vector of undirected edges.
    void build( const unsigned long num_vertices, const TriangleVec& triangles, const Edges& edges );
    
    // Clear the data-structure. You can call build() again after this.
    void clear();
    
    // Get the HalfEdge struct for an index.
    const HalfEdge& halfedge( const HalfEdgeIndex& halfedge_index ) const { return m_halfedges.at( halfedge_index ); }
    HalfEdgeIndex outgoing_halfedge_index_for_vertex( const Index vertex_index ) const { return m_vertex_halfedges.at( vertex_index ); }
    
    // Returns the vertex neighbors (as indices) of the vertex 'vertex_index'.
    Indices vertex_vertex_neighbors( const Index vertex_index ) const;
    // Returns in 'result' the vertex neighbors (as indices) of the vertex 'vertex_index'.
    void vertex_vertex_neighbors( const Index vertex_index, Indices& result ) const;
    
    // Returns the valence (number of vertex neighbors) of vertex with index 'vertex_index'.
    int vertex_valence( const Index vertex_index ) const;
    
    // Returns the face neighbors (as indices) of the vertex 'vertex_index'.
    Indices vertex_face_neighbors( const Index vertex_index ) const;
    // Returns in 'result' the face neighbors (as indices) of the vertex 'vertex_index'.
    void vertex_face_neighbors( const Index vertex_index, Indices& result ) const;
    
    // Returns whether the vertex with given index is on the boundary.
    bool vertex_is_boundary( const Index vertex_index ) const;
    // Returns a vector of vertex indices that lie on the boundary of the mesh.
    Indices boundary_vertices() const;
    
    // Returns a list of undirected boundary edges (i,j).  If (i,j) is in the result, (j,i) will not be.
    Edges boundary_edges() const;
    
    // Given the index of a HalfEdge, returns the corresponding directed edge (i,j).
    Edge he_index2directed_edge( const HalfEdgeIndex he_index ) const {
        const HalfEdge& he = halfedge( he_index );
        return Edge( halfedge( he.opposite_he ).to_vertex, he.to_vertex );
    }
    // Given a directed edge (i,j), returns the index of the 'HalfEdge' in
    // halfedges().
    HalfEdgeIndex directed_edge2he_index( const Index i, const Index j ) const;
    HalfEdgeIndex directed_edge2he_index( const Edge& ij ) const { return directed_edge2he_index( ij[0], ij[1] ); }
    
private:
    std::vector< HalfEdge > m_halfedges;
    // Offsets into the 'halfedges' sequence, one per vertex.
    HalfEdgeIndices m_vertex_halfedges;
    // Offset into the 'halfedges' sequence, one per face.
    HalfEdgeIndices m_face_halfedges;
    // Offset into the 'halfedges' sequence, one per edge (unordered pair of vertex indices).
    HalfEdgeIndices m_edge_halfedges;
    // A map from an ordered edge (an std::pair of Index's) to an offset into the 'halfedge' sequence.
    typedef std::map< std::pair< Index, Index >, HalfEdgeIndex > directed_edge2halfedge_index_map_t;
    // A map from an ordered edge (an std::pair of Index's) to an offset into a different sequence.
    typedef std::map< std::pair< Index, Index >, Index > directed_edge2index_map_t;

    directed_edge2halfedge_index_map_t m_directed_edge2he_index;
};

void unordered_edges_from_triangles( const TriangleVec& triangles, HalfEdgeTriMesh::Edges& edges_out );

}

#endif /* __halfedge_h__ */
