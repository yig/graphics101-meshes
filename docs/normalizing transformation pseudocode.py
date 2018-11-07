def normalizing_transformation( mesh ):
    positions = mesh.positions
    
    ## Find the minimum and maximum x, y, or z value in the mesh.
    vec3 minimum(positions[0])
    vec3 maximum(positions[0])
    for p in positions:
        minimum.x = min( minimum.x, p.x )
        minimum.y = min( minimum.y, p.y )
        minimum.z = min( minimum.z, p.z )
        
        maximum.x = max( maximum.x, p.x )
        maximum.y = max( maximum.y, p.y )
        maximum.z = max( maximum.z, p.z )
    
    ## find the midpoint
    midpoint = ( minimum + maximum )/2
    
    T = translation matrix so that midpoint becomes the origin (0,0,0)
    
    ## Find the biggest difference across the three coordinates (x, y, or z)
    ## between the maximum and minimum values.
    difference = maximum - minimum
    maxcoord = max( difference.x, difference.y, difference.z )
    ## We want to scale such that maxcoord*scale = 2.
    scale = 2/maxcoord
    
    S = uniform scale matrix by scale
    
    ## The result is the product of the matrices so that we translate and then scale.
    result = S * T
