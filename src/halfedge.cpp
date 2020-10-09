#include "halfedge.h"

// needed for implementation
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <algorithm>

namespace graphics101
{

void HalfEdgeTriMesh::vertex_face_neighbors( const Index vertex_index, Indices& result ) const
{
    result.clear();

    // Your code goes here.
    
    // Outline:
    // 1 Get the outgoing half-edge index for the vertex.
    // 2 Start the iteration at that half-edge index.
    // 3 Get the HalfEdge for the half-edge index.
    // 4 Store the face the half-edge points to if it's not -1.
    //   Call result.push_back() with each face index.
    //   To handle boundaries, don't .push_back() if the face index == -1.
    // 5 Increment to the next face.
    // 6 If we are back where we started, we've seen all vertices.
    
    
}
HalfEdgeTriMesh::Indices HalfEdgeTriMesh::vertex_face_neighbors( const Index vertex_index ) const
{
    Indices result;
    vertex_face_neighbors( vertex_index, result );
    return result;
}

void HalfEdgeTriMesh::vertex_vertex_neighbors( const Index vertex_index, Indices& result ) const
{
    result.clear();

    // Get the outgoing half-edge index for the vertex.
    const HalfEdgeIndex start_hei = outgoing_halfedge_index_for_vertex( vertex_index );
    // Start the iteration at that half-edge index.
    HalfEdgeIndex hei = start_hei;
    while( true )
    {
        // Get the HalfEdge for the half-edge index.
        const HalfEdge& he = halfedge( hei );
        // Store the vertex the half-edge points to.
        result.push_back( he.to_vertex );

        // Increment to the next vertex.
        hei = halfedge( he.opposite_he ).next_he;

        // If we are back where we started, we've seen all vertices.
        if( hei == start_hei ) break;
    }
}
HalfEdgeTriMesh::Indices HalfEdgeTriMesh::vertex_vertex_neighbors( const Index vertex_index ) const
{
    Indices result;
    vertex_vertex_neighbors( vertex_index, result );
    return result;
}

int HalfEdgeTriMesh::vertex_valence( const Index vertex_index ) const
{
    Indices neighbors;
    vertex_vertex_neighbors( vertex_index, neighbors );
    return neighbors.size();
}

std::vector< HalfEdgeTriMesh::Index > HalfEdgeTriMesh::boundary_vertices() const
{
    // Use an std::set<> if we wanted the returned sequence to be sorted.
    std::unordered_set< Index > result;
    for( const HalfEdge& he : m_halfedges )
    {
        if( -1 == he.face )
        {
            // result.extend( self.he_index2directed_edge( hei ) )
            result.insert( he.to_vertex );
            result.insert( halfedge( he.opposite_he ).to_vertex );
        }
    }

    return Indices( result.begin(), result.end() );
}

HalfEdgeTriMesh::Edges HalfEdgeTriMesh::boundary_edges() const
{
    Edges result;
    for( int hei = 0; hei < m_halfedges.size(); ++hei )
    {
        const HalfEdge& he = halfedge( HalfEdgeIndex( hei ) );

        if( -1 == he.face )
        {
            const auto de = he_index2directed_edge( HalfEdgeIndex( hei ) );
            result.push_back( de );
        }
    }

    return result;
}

bool HalfEdgeTriMesh::vertex_is_boundary( const Index vertex_index ) const
{
    return -1 == halfedge( outgoing_halfedge_index_for_vertex( vertex_index ) ).face;
}

}

namespace
{
// HalfEdgeTriMesh::directed_edge2index_map_t is a private typdef. Just redeclare it here.
typedef std::map< std::pair< graphics101::HalfEdgeTriMesh::Index, graphics101::HalfEdgeTriMesh::Index >, graphics101::HalfEdgeTriMesh::Index > directed_edge2index_map_t;
graphics101::HalfEdgeTriMesh::Index directed_edge2face_index( const directed_edge2index_map_t& de2fi, graphics101::HalfEdgeTriMesh::Index vertex_i, graphics101::HalfEdgeTriMesh::Index vertex_j )
{
    assert( !de2fi.empty() );

    const auto it = de2fi.find( std::make_pair( vertex_i, vertex_j ) );

    // If no such directed edge exists, then there's no such face in the mesh.
    // The edge must be a boundary edge.
    // In this case, the reverse orientation edge must have a face.
    if( it == de2fi.end() )
    {
        assert( de2fi.find( std::make_pair( vertex_j, vertex_i ) ) != de2fi.end() );
        return -1;
    }

    return it->second;
}
}

namespace graphics101
{

void HalfEdgeTriMesh::clear()
{
    m_halfedges.clear();
    m_vertex_halfedges.clear();
    m_face_halfedges.clear();
    m_edge_halfedges.clear();
    m_directed_edge2he_index.clear();
}

void HalfEdgeTriMesh::build( const unsigned long num_vertices, const TriangleVec& triangles, const Edges& edges )
{
    /*
    Generates all half edge data structures for the mesh given by its vertices 'self.vs'
    and faces 'self.faces'.

    Python version used heavily
    */

    directed_edge2index_map_t de2fi;
    for( int fi = 0; fi < triangles.size(); ++fi )
    {
        const Triangle& tri = triangles[fi];
        de2fi[ std::make_pair( tri[0], tri[1] ) ] = fi;
        de2fi[ std::make_pair( tri[1], tri[2] ) ] = fi;
        de2fi[ std::make_pair( tri[2], tri[0] ) ] = fi;
    }

    // Clear any previous data we store.
    clear();
    m_vertex_halfedges.resize( num_vertices, HalfEdgeIndex(-1) );
    m_face_halfedges.resize( triangles.size(), HalfEdgeIndex(-1) );
    m_edge_halfedges.resize( edges.size(), HalfEdgeIndex(-1) );
    m_halfedges.reserve( edges.size()*2 );

    for( int ei = 0; ei < edges.size(); ++ei )
    {
        const Edge& edge = edges[ei];

        // Add the HalfEdge structures to the end of the list.
        const HalfEdgeIndex he0index( m_halfedges.size() );
        m_halfedges.push_back( HalfEdge() );
        HalfEdge& he0 = m_halfedges.back();

        const HalfEdgeIndex he1index( m_halfedges.size() );
        m_halfedges.push_back( HalfEdge() );
        HalfEdge& he1 = m_halfedges.back();

        // The face will be -1 if it is a boundary half-edge.
        he0.face = directed_edge2face_index( de2fi, edge[0], edge[1] );
        he0.to_vertex = edge[1];
        he0.edge = ei;

        // The face will be -1 if it is a boundary half-edge.
        he1.face = directed_edge2face_index( de2fi, edge[1], edge[0] );
        he1.to_vertex = edge[0];
        he1.edge = ei;

        // Store the opposite half-edge index.
        he0.opposite_he = he1index;
        he1.opposite_he = he0index;

        // Also store the index in our m_directed_edge2he_index map.
        assert( m_directed_edge2he_index.find( std::make_pair( edge[0], edge[1] ) ) == m_directed_edge2he_index.end() );
        assert( m_directed_edge2he_index.find( std::make_pair( edge[1], edge[0] ) ) == m_directed_edge2he_index.end() );
        m_directed_edge2he_index[ std::make_pair( edge[0], edge[1] ) ] = he0index;
        m_directed_edge2he_index[ std::make_pair( edge[1], edge[0] ) ] = he1index;

        // If the vertex pointed to by a half-edge doesn't yet have an out-going
        // halfedge, store the opposite halfedge.
        // Also, if the vertex is a boundary vertex, make sure its
        // out-going halfedge is a boundary halfedge.
        // NOTE: Halfedge data structure can't properly handle butterfly vertices.
        //       If the mesh has butterfly vertices, there will be multiple outgoing
        //       boundary halfedges.  Because we have to pick one as the vertex's outgoing
        //       halfedge, we can't iterate over all neighbors, only a single wing of the
        //       butterfly.
        if( m_vertex_halfedges[ he0.to_vertex ] == -1 || -1 == he1.face )
        {
            m_vertex_halfedges[ he0.to_vertex ] = he0.opposite_he;
        }
        if( m_vertex_halfedges[ he1.to_vertex ] == -1 || -1 == he0.face )
        {
            m_vertex_halfedges[ he1.to_vertex ] = he1.opposite_he;
        }

        // If the face pointed to by a half-edge doesn't yet have a
        // halfedge pointing to it, store the halfedge.
        if( -1 != he0.face && m_face_halfedges[ he0.face ] == -1 )
        {
            m_face_halfedges[ he0.face ] = he0index;
        }
        if( -1 != he1.face && m_face_halfedges[ he1.face ] == -1 )
        {
            m_face_halfedges[ he1.face ] = he1index;
        }

        // Store one of the half-edges for the edge.
        assert( m_edge_halfedges[ ei ] == -1 );
        m_edge_halfedges[ ei ] = he0index;
    }

    // Now that all the half-edges are created, set the remaining next_he field.
    // We can't yet handle boundary halfedges, so store them for later.
    HalfEdgeIndices boundary_heis;
    for( int hei = 0; hei < m_halfedges.size(); ++hei )
    {
        HalfEdge& he = m_halfedges.at( hei );
        // Store boundary halfedges for later.
        if( -1 == he.face )
        {
            boundary_heis.push_back( HalfEdgeIndex( hei ) );
            continue;
        }

        const Triangle& face = triangles[ he.face ];
        const Index i = he.to_vertex;

        Index j = -1;
        if( face[0] == i ) j = face[1];
        else if( face[1] == i ) j = face[2];
        else if( face[2] == i ) j = face[0];
        assert( -1 != j );

        he.next_he = m_directed_edge2he_index[ std::make_pair(i,j) ];
    }

    // Make a map from vertices to boundary halfedges (indices) originating from them.
    // NOTE: There will only be multiple originating boundary halfedges at butterfly vertices.
    std::unordered_map< Index, std::set< HalfEdgeIndex > > vertex2outgoing_boundary_hei;
    for( const auto& hei : boundary_heis )
    {
        const Index originating_vertex = halfedge( halfedge( hei ).opposite_he ).to_vertex;
        vertex2outgoing_boundary_hei[ originating_vertex ].insert( hei );
        if( vertex2outgoing_boundary_hei[ originating_vertex ].size() > 1 )
        {
            std::cerr << "Butterfly vertex encountered.\n";
        }
    }

    // For each boundary halfedge, make its next_he one of the boundary halfedges
    // originating at its to_vertex.
    for( const auto& hei : boundary_heis )
    {
        HalfEdge& he = m_halfedges[ hei ];

        auto& outgoing = vertex2outgoing_boundary_hei[ he.to_vertex ];
        if( !outgoing.empty() )
        {
            auto outgoing_hei = outgoing.begin();
            he.next_he = *outgoing_hei;

            outgoing.erase( outgoing_hei );
        }
    }

#ifndef NDEBUG
    for( const auto& it : vertex2outgoing_boundary_hei ) { assert( it.second.empty() ); }
#endif
}

HalfEdgeTriMesh::HalfEdgeIndex HalfEdgeTriMesh::directed_edge2he_index( const Index i, const Index j ) const
{
    /// This isn't const, and doesn't handle the case where (i,j) isn't known:
    // return m_directed_edge2he_index[ std::make_pair( i,j ) ];

    auto result = m_directed_edge2he_index.find( std::make_pair( i,j ) );
    if( result == m_directed_edge2he_index.end() ) return HalfEdgeIndex(-1);

    return result->second;
}

void unordered_edges_from_triangles( const TriangleVec& triangles, HalfEdgeTriMesh::Edges& edges_out )
{
    typedef HalfEdgeTriMesh::Index Index;
    typedef HalfEdgeTriMesh::Edge Edge;
    typedef std::set< std::pair< Index, Index > > edge_set_t;
    edge_set_t edges;
    for( const auto& tri : triangles )
    {
        edges.insert( std::make_pair( std::min( tri[0], tri[1] ), std::max( tri[0], tri[1] ) ) );
        edges.insert( std::make_pair( std::min( tri[1], tri[2] ), std::max( tri[1], tri[2] ) ) );
        edges.insert( std::make_pair( std::min( tri[2], tri[0] ), std::max( tri[2], tri[0] ) ) );
    }

    edges_out.resize( edges.size() );
    int e = 0;
    for( const auto& it : edges )
    {
        edges_out.at(e) = Edge( it.first, it.second );
        ++e;
    }
}

}
