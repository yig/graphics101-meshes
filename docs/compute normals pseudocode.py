def compute_normals_halfedge():
    ## allocate space for the normals
    normals = new vec3[ vertices.length() ]
    
    ## for each vertex, for each face of the vertex, accumulate normals
    for v in vertices:
        ## Initialize to zero
        n = vec3(0)
        for face neighbor of v:
            n += face neighbor normal
        normals[ v index ] = normalize(n)

def compute_normals_without_halfedge():
    ## allocate space for the normals
    normals = new vec3[ vertices.length() ]
    ## initialize to zero
    for n in normals: n = vec3(0)
    
    ## for each face, for each vertex of the face, accumulate normals
    for face in faces:
        n = face normal
        # iterate over the three vertices in the triangle
        for v in face:
            normals[ v index ] += face normal
    
    ## normalize
    for n in normals:
        n = normalize(n)
