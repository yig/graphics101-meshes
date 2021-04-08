## Make a mesh of a rectangular grid in the xy plane with z = 0. x and y will run from [0,1].
def make_mesh_grid( int num_x_vertices, int num_y_vertices ):
    ## We will fill out an array of 3D positions and an array of indices into those
    ## positions defining the triangles.
    positions = float3[]
    faces_positions = int3[]
    
    ## We will also fill out an array of 3D normals and an array of indices into those
    ## normals for the triangles.
    ## There had better be the same number of triangles
    ## in `faces_positions` as in `faces_normals` when we are done!
    normals = float3[]
    faces_normals = int3[]
    
    ## There's only one normal.
    normals.append( ( 0, 0, 1 ) )
    
    ## Let's iterate over x and y from the bottom-left (0,0) to the top-right (1,1)
    ## and create positions:
    for( int xi = 0; xi < num_x_vertices; ++xi ):
        ## The xi-th coordinate will be x.
        float x = float(xi)/(num_x_vertices-1)
        
        for( int yi = 0; yi < num_y_vertices; ++yi ):
            ## The yi-th coordinate will be y.
            float y = float(yi)/(num_y_vertices-1)
            
            ## Create the position.
            positions.append( ( x, y, 0 ) )
            
            ## If we aren't the bottom-most row of vertices or left-most column
            ## of vertices, we have a vertices to our left and below us.
            if xi > 0 and yi > 0:
                ## Each "square" of 4 vertices will be like this:
                ##    ( xi-1, yi   )   ( xi, yi   )
                ##    ( xi-1, yi-1 )   ( xi, yi-1 )
                ## Because there are `num_y_vertices` in each inner-loop, the vertex at position
                ## ( xi, yi ) is at index `num_y_vertices*xi + yi` in the positions array.
                ## Let's connect these four vertices with two triangles.
                ## Don't forget about counter-clockwise orientation.
                ## The above is illustrated in the file "make_mesh pseudocode diagram".
                faces_positions.append( (
                    ## upper-right
                    num_y_vertices*xi     + yi
                    ## upper-left
                    num_y_vertices*(xi-1) + yi,
                    ## lower-left
                    num_y_vertices*(xi-1) + yi-1,
                    ) )
                faces_positions.append( (
                    ## upper-right
                    num_y_vertices*xi     + yi,
                    ## lower-left
                    num_y_vertices*(xi-1) + yi-1,
                    ## lower-right
                    num_y_vertices*xi     + yi-1,
                    ) )
                ## The corresponding normal indices are simple, since all faces have
                ## the same normal.
                faces_normals.append( ( 0, 0, 0 ) )
                faces_normals.append( ( 0, 0, 0 ) )
    
    ## Make sure we have the same number of triangles in both faces arrays.
    assert faces_positions.size() == faces_normals.size()
    
    return positions, faces_positions, normals, faces_normals
