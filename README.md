Computer Graphics - Homework Assignment 5 - Meshes
==================================================

Goals:
------

* Understand how to create and process triangle meshes.

* Understand how to tessellate surfaces, including position, normal, and
texture coordinates.

* Understand how to compute the bounding box of a mesh.

* Increase familiarity with transformation matrices.

* Understand how to compute normals from a mesh with only surface
positions.

* Understand how to work with a half-edge data structure.

* Become more comfortable with C++.

Getting Started & Handing In:
-----------------------------

* This is a programming assignment. The code framework is provided here.

* The code will be written in C++. You are encouraged to write helper
functions as you like.

* The program is a command line program. The code for this
project makes use of only the C++ standard library and `glm`.
There is a Qt Creator project file (`meshes.pro`) so you can write,
compile, debug, and run your program in an IDE you are familiar with.
Although it doesn't use any of Qt, the Qt Creator development
environment is still convenient.

* You should have already successfully installed the open source version
of the Qt environment from the last assignment:
<https://www.qt.io/download-open-source>
(At the time of writing, version 5.11 is the newest version. Any 5.x
version should work. The installer, by default, includes all versions of
Qt. Save space by installing only the most recent version and a
compiler.) Mac users with [Homebrew](https://brew.sh/)
can alternatively install via: `brew install qt` and `brew cask install qt-creator`.

* Download the assignment. This will create a folder named `meshes`. Open the
file named `meshes.pro`. This should launch the Qt Creator development
environment (IDE).

* Build and run the code. The code should compile, but it will complain
when running about not having enough arguments. You should see a message
like:

        Usage: ./meshes cylinder #slices #stacks output.obj
        Usage: ./meshes sphere   #slices #stacks output.obj
        Usage: ./meshes cone     #slices #stacks output.obj
        Usage: ./meshes torus    #slices_big_circle #slices_small_circle tube_radius output.obj
        Usage: ./meshes cube     #slices_edge output.obj
        
        Usage: ./meshes normals          weights input.obj output.obj (weights must be either 'unweighted' or 'angle_weighted')
        Usage: ./meshes normals_halfedge weights input.obj output.obj (weights must be either 'unweighted' or 'angle_weighted')
        
        Usage: ./meshes normalize input.obj output.obj

* Add your code to `mesh.cpp`, `shape.cpp`, and `halfedge.cpp`. You may add
helper functions if you wish.

* Build and run and test that it is working correctly. Qt Creator has a
great debugger.

* Run the following commands. Copy your output `.obj` files into a new
output subdirectory.

        ./meshes cylinder 5 10 cylinder-5-10.obj
        ./meshes cylinder 30 2 cylinder-30-2.obj
        ./meshes cylinder 3 2 cylinder-3-2.obj
        ./meshes sphere 5 30 sphere-5-30.obj
        ./meshes sphere 30 30 sphere-30-30.obj
        ./meshes sphere 3 1 sphere-3-1.obj
        ./meshes normals unweighted cube_binary.obj cube_binary-unweighted.obj
        ./meshes normals angle_weighted cube_binary.obj cube_binary-angle_weighted.obj
        ./meshes normals_halfedge unweighted cube_binary.obj cube_binary-unweighted-halfedge.obj
        ./meshes normals_halfedge angle_weighted cube_binary.obj cube_binary-angle_weighted-halfedge.obj
        ./meshes normalize bunny.obj bunny-normalize.obj

    (the following are bonus)

        ./meshes cone 5 10 cone-5-10.obj
        ./meshes cone 30 1 cone-30-1.obj
        ./meshes cone 3 1 cone-3-1.obj
        ./meshes torus 5 10 .25 torus-5-10-.25.obj
        ./meshes torus 20 3 .1 torus-20-3-.1.obj
        ./meshes torus 3 3 .25 torus-3-3-.25.obj
        ./meshes cube 2 square-2.obj
        ./meshes cube 10 square-10.obj

* When done, zip your entire `meshes` directory along with the output
subdirectory and a `Notes.txt` file as `hw05_lastname_firstname.zip`
and upload your solution to Blackboard before the deadline. Your
`Notes.txt` should describe any known issues or extra features. Your
`Notes.txt` should also note the names of people in the class who
deserve a star for helping you (not by giving your their code!).

* **THIS IS AN INDIVIDUAL, NOT A GROUP ASSIGNMENT. That means all code
written for this assignment should be original! Although you are
permitted to consult with each other while working on this assignment,
code that is substantially the same will be considered cheating.** In your
`Notes.txt`, please note who deserves a star (who helped you with the
assignment).

Overview:
---------

In this assignment, you will be implementing a bit of mesh processing.
You will tessellate a couple of shapes (cylinder and sphere), which
means that you will create triangle meshes for them. You will compute
the axis-aligned bounding box of a mesh and transform it to the unit
cube. You will implement an algorithm for computing normals for meshes
which don't have them. You will interact with a half-edge data
structure. Your tessellation will create shapes like this:

![sphere with five lines of longitude](docs/images/sphere-5-100-snapshot.png)
![earth](docs/images/sphere-earth.png)
![cylinder](docs/images/cylinder-100-2.png)
![cone](docs/images/cone-30-3-snapshot.png)
![torus](docs/images/torus-30-30-.25-snapshot.png)

Rubric:
-------

* **(60 points)** Tessellation. Create a triangle mesh for a shape,
including positions, normals, and (optionally) texture coordinates. Your
code goes in `shape.cpp`. The function signature is

        void Shape::tessellate( Mesh& mesh_out );

    For each type of shape, you will fill out the `Mesh` object passed
    into the function. Its properties are:

    * **(20 points)**  
    `mesh.positions`: the array of 3D positions  
    `mesh.face_positions`: the array of triangles indexing `mesh.positions`

    * **(10 points)**  
    `mesh.normals`: the array of 3D normals  
    `mesh.face_normals`: the array of triangles indexing `mesh.normals`

    * **(bonus 10 points)**  
    `mesh.texcoords`: the array of 2D texture coordinates (aka explicit
coordinates). Texture coordinates range from [0,1] in each dimension
and are used to look up values in an image as a fraction of the image
width and height.  
    `mesh.face_texcoords`: the array of triangles indexing `mesh.texcoords`

    The shapes are:

    * Cylinder. The cylinder axis runs along the z-axis, from (0,0,0) to
(0,0,1). The cylinder has radius 1. The cylinder has no top or bottom
cap. The first texture coordinate runs around the circle of the cylinder
and is equal to the angle as a fraction of 2π. The second texture
coordinate runs up the cylinder. It is equal to z.
There are two member variables available to the function, `int m_slices`
and `int m_stacks`. Your cylinder should have `m_slices` vertices around
the circle (so three is the minimum) and `m_stacks` vertices along the
axis (so two is the minimum). There will be a total of
`m_slices*m_stacks` positions, `m_slices` normals (since each stack has
the same normals), and `(m_slices+1)*m_stacks` texture coordinates
(since the last texture coordinate of each circle must be
2π/2π = 1 and so can't be 0 like the first or else the texture
would tear).

    * Sphere. The sphere is the unit circle centered at the origin (0,0,0)
with radius 1. The first texture coordinate should be the longitude
angle as a fraction of 2π. The second texture coordinate should be
the latitude angle as a fraction of π. There are two member
variables available to the function, `int m_slices` and `int m_stacks`.
Your sphere should have `m_slices` lines of longitude (so three is the
minimum) and `m_stacks` lines of latitude (not counting the north and
south poles, so 1 is the minimum). There will be a total of
`m_slices*m_stacks + 2` positions (the +2 comes from the north and
south pole), the same number of normals as positions, and
`(m_slices+1)*m_stacks + 2` texture coordinates (for the same reason as
the cylinder, +2 for the north and south poles).

    * **(bonus, half credit)** Cone. The cone whose axis runs from (0,0,0) to
(0,0,1) with radius 1. The cone has no bottom cap. The first texture
coordinates runs around the circle of the cone. The second texture
coordinate runs up the cone. It is equal to z.
There are two member variables available to the function, `int m_slices`
and `int m_stacks`. Your cone should have `m_slices` vertices around the
circle (three is the minimum) and `m_stacks` vertices along the axis (not
counting the top point, so 1 is the minimum). There will be a total of
`m_slices*m_stacks + 1` positions (the +1 is the top point), `m_slices`
normals (since each stack has the same normals), and
`(m_slices+1)*m_stacks + 1` texture coordinates (for the same reason as
the cylinder and sphere, +1 for the top point).

    * **(bonus, half credit)** Torus. A torus is a donut. For our "unit"
torus, the center of the doughy tube is the unit circle in the xy plane
with radius 1. If you cut into the tube of the donut, you will cut
through a circular cross section. The cross section has radius
`m_cross_radius`. You can think of it as if dough with constant
thickness `m_cross_radius` was added to the unit circle in the xy plane.
The first texture coordinate runs around each cross section circle. The
second texture coordinate runs around the tube.
There are three member variables. `real m_cross_radius` was already
mentioned. The other two are `int m_slices_tube_circle` and
`int m_slices_cross_circle`. Your torus should have
`m_slices_cross_circle` vertices around each cross-section of the tube,
and `m_slices_tube_circle` vertices around the entire tube. (Three is
the minimum for each.) There will be a total of
`m_slices_tube_circle*m_slices_cross_circle` positions and normals,
and `(m_slices+1)*(m_stacks+1)` texture coordinates (since the last
texture coordinate of each circle, tube and cross section, must be
2π/2π = 1 and so can't be 0 like the first or else the texture
would tear).

    * **(bonus, half credit)** Cube. A unit cube whose sides are axis-aligned
and whose corners are (-1,-1,-1) and (1,1,1). Each square face should
have the entire texture mapped onto it. There is a single member
variable, `int m_slices_edge`. Each edge of your cube should have
`m_slices_edge` vertices (so two is the minimum), and each face of your
cube should have `m_slices_edge*m_slices_edge` vertices. There will
be a total of `(m_slices_edge-2)*(m_slices_edge-2)*6 + (m_slices_edge-2)*12 + 8`
positions (since each edge is shared by two
faces, and each corner is shared by three faces). There will be a total
of 6 normals (one per face). There will be a total of
`m_slices_edge*m_slices_edge` texture coordinates.

* **(30 points total)** Mesh normals. Your code goes in `mesh.cpp` and (a tiny
bit) `halfedges.cpp`. Given a Mesh object with positions but no normals,
compute a normal for each position by averaging the (flat) normal of
each triangle touching the vertex. You can do this by initializing a
running tally of the average normal for each vertex. Each triangle uses
its positions to compute a normal via the cross product. Add this normal
to the running tally for. You must support two modes, unweighted, and
angle-weighted. If the strategy parameter is `Unweighted`, simply
normalize the normal before adding it to the running tally. If the
strategy parameter is `AngleWeighted`, normalize the normal and then scale
it by the angle of the triangle at the vertex.

    * **(10 points)** Without a half-edge data structure. Iterate over each
triangle. Add its normal, unweighted or angle-weighted, to the running
tally for each of its three vertices. The function signature is:

            void Mesh::computeNormals( MeshNormalStrategy strategy )

    * **(20 points)** With a half-edge data structure. Iterate over each
vertex. Call `halfedges.vertex_face_neighbors( vertex_index )` to
obtain the triangles touching a vertex as a vector of indices into
`.face_positions`. Then iterate over the triangles and add their normals,
unweighted or angle-weighted, to the running tally for the vertex. The
function signature is:

            void Mesh::computeNormalsHalfEdge( MeshNormalStrategy strategy )

    Note that the half-edge function `.vertex_face_neighbors()` is not
written for you. You must fill in the code for this function in
`halfedges.cpp`. It is a very short function. Follow the provided outline.
Consult `HalfEdgeTriMesh::vertex_vertex_neighbors()` for reference; it
differs from `.vertex_face_neighbors()` by exactly one line.

* **(10 points)** Mesh normalization. Your code goes in `mesh.cpp`.

    * Compute the 4x4 transformation matrix that, if applied to the .positions
of the mesh, translates and uniformly scales it to tightly fit within
the unit cube. The unit cube is the cube with axis-aligned faces
centered at the origin with side length 2, aka corners at (-1,-1,-1) and
(1,1,1). The function signature is:

            mat4 Mesh::normalizingTransformation() const

        `normalizingTransformation()` does not affect any vertex positions. (That
is `applyTransformation()`, described below.) You can break down the job
of `normalizingTransformation()` into two parts:

        1. Compute the axis-aligned bounding box for all positions in the mesh. To
do this, find the minimum and maximum x, the minimum and maximum y, and
the minimum and maximum z among all positions in the mesh. The minimum
x,y,z is one corner of the bounding box and the maximum x,y,z is the
other corner.

        2. Compute the transformation that translates this bounding box to be
centered at the origin. (Hint: The center of the bounding box is 0.5*(
min_xyz + max_xyz ).) Compute the transformation that uniformly scales
the bounding box to fit inside the [-1,1] cube, which has edge length 2.
(Hint: max_xyz - min_xyz is the size of the bounding box in all
three dimensions. Scale so that the maximum size in any dimension equals
2.) If your translation matrix is T and your scale matrix is S and a
vertex is v, the transformation you want to apply is S * T * v, which
means that the box will first be centered at the origin and then scaled
so that its maximum dimension exactly fits within [-1,1].

    * You must also write a function which applies a transformation matrix to
the mesh. You apply the transformation matrix itself to .positions.
Don't forget that you apply the inverse transpose of the upper-left 3x3
portion of the matrix to the normals. The function signature is:

            void Mesh::applyTransformation( const mat4& transform )

Tips
----

* For debugging an OBJ that you program saves, I highly recommend the free
program [meshLab](http://www.meshlab.net/) ([http://www.meshlab.net/](http://www.meshlab.net/)):

    ![meshLab visualizing a cylinder](docs/images/meshlab-cylinder.png)
    ![meshLab visualizing a sphere](docs/images/meshlab-sphere.png)

    * A visualization like the above tells you a lot about your mesh. With a
few viewer settings, you can see the UV coordinates. You can see the
x,y,z axes and tell that the mesh is running along +z. You can see the
normals (face and vertex). You can see that the faces have
counter-clockwise orientation because "back-faces culling" is on and the
back-faces are missing. To create a similar visualization, open your OBJ
file with meshLab and then, under the Render menu, choose:

        * Show Normal/Curvature
        * Show Axis
        * Show UV Tex Param
        * Render Mode > BackFace Culling

    * To load a texture, like `uv_grid.png` or `earth.png`, go to Filters >
Texture > Set Texture.

    * To debug normals, it can be helpful to view with mesh with "reflection
lines", which renders the mesh as if it were a mirror in a long, striped
corridor. In meshLab, choose "Render > Render Mode > Smooth" and
choose "Render > Shaders > reflexion_lines".

* A cylindrical coordinate system is *(r,φ,z)*, where *r* is the
radius, *φ* is the angle around the circle of the cylinder, and
*z* is the cartesian *z* coordinate. For your cylinder, *r* = 1, slices
evenly sample *φ*, and stacks evenly sample *z*.

    * To convert cylindrical coordinates to Cartesian coordinates for your
positions:  
    *x = r cos( φ )*, *y = r sin( φ )*, and *z* = *z*.

    * A cylinder's normal at a position has the same *x,y* as the position, but *z* = 0.

    * A cylinder's texture coordinates are: *( φ/(2π), z )*.

* A spherical coordinate system is *(r,θ,φ)*, where *r* is the
radius, *θ* ∈ [0,π] is the angle away from the
north pole (like latitude), and *φ* ∈ [0,2π] is
the angle around the sphere like longitude. For your sphere, *r* = 1,
slices evenly sample *φ*, and stacks evenly sample *θ*.

    * To convert spherical coordinates to Cartesian coordinates for your
positions: *x = r sin(θ) cos(φ)*, *y = r sin(θ) sin(φ)*,
and *z = r cos(θ)*.

    * A sphere's normal at a position has the same *x,y,z* as the position.

    * A sphere's texture coordinates are
( φ/(2π), 1 - θ/π ).

* For a torus, a convenient coordinate system is 2D polar coordinates
*(r,θ)* along with a cartesian *z* coordinate. In this way, the
formula for a torus given *u,v* parameters is
*(r,θ,z) = ( 1 + p cos( 2 π u ), 2πv, - p sin( 2 π u ) )*,
where *p* = `m_cross_radius`. The *u,v* parameters go from 0 to 1
around the cross-section and tube, respectively. You can convert the 2D
polar coordinates to 2D cartesian coordinates as *x = r cos(θ)* and
*y = r sin(θ)*. The normal is the position after translating the
center of the cross section to the origin, which means subtracting 1
from *r*. To get a unit normal, drop the *p* scale factor. (You can verify
that it is unit by making use of the identity
*cos²(θ) + sin²(θ) = 1*.)

* When writing `Mesh::normalizingTransformation()`, you can use
`glm::translate(mat4(1.0), vec3(x,y,z))` to get a translation matrix that
translates by *x,y,z*. You can use `glm::scale(mat4(1.0), vec3(s,s,s))` to
get a scale matrix that scales uniformly by *s*. You can also build
these matrices yourself quite easily:

    * A uniform scale matrix is a matrix whose diagonal is the value to scale
by. You can create a 3x3 uniform scale matrix via `mat3( scale_factor )`,
where `scale_factor` is a number. You can create a `mat4` from a `mat3 m` via
the constructor `mat4(m)`. You can create an identity `mat4` via `mat4(1.0)`.
You can create a translation matrix by setting the fourth column of an
identity `mat4 t` via `t[3] = vec4( x,y,z,1.0 )` or, equivalently,
`t[3] = vec4( p,1.0 )`, where `p` is a `vec3`.

* `glm::min()` and `glm::max()` operate component-wise. That means that
`glm::min( vec2( 5.0, -3.0, vec2( 1.0, 0.0 ) )` is `vec2( 1.0, -3.0 )`. This
is useful for computing the axis-aligned bounding box of a set of
points, which is what you need to do in
`Mesh::normalizingTransformation()`.

* When computing angle weighted normals, you need to compute the angle at
each corner of the triangle. Recall that the dot product of two vectors
**a** · **b** = cos(θ) ||**a**|| ||**b**||,
where θ is the angle between them. So you can obtain the angle
between two vectors via the inverse cosine (arccosine) of the normalized
vectors. The function that computes arccosine is `acos()`. Call it on the
dot product of two of the triangle's edges, normalized. Note that `acos()`
is very strict about its input being between -1 and 1. Due to floating
point issues, sometimes the dot product will produce a number epsilon
out of range. Use `clamp()` or `min()` and `max()` to ensure the value you
pass to acos() is between -1 and 1.

* You can access the constant pi aka π = 3.14... via the constant `M_PI`
(defined in the `#include <cmath>` header) or by calling `glm::pi()`.

Qt functions you need for this assignment
-----------------------------------------

**None**
