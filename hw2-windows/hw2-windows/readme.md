HW2 needs implement transformation, shading, and a viewer for a scene specified in text file.

The overall goal is to take an input scene description that specifies the camera, scene width and height,
lights (up to 10 lights), object geometry (for simplicity, only spheres, teapots and cubes) and material
properties.

Because skeleton code is nearly complete, I just record the real work I have done.
* Understand OpenGL transformation stack
openGL applies transformation to an object in reverse order, for example, if you first "Translate (0,0,-5)", then "Scale (2,2,2)", then draw a cube with side length 1.0. the cube would been scaled to 2.0 first, then translate to 0 0 -5. So the final scene is a cube with side length 2.0 located at (0,0,5). If you reverse transformations, then the cube would first move to (0,0,5), then scale to (0,0,10), because point at (0,0,5.5) would scale to (0,0,11).

Another weird thing about OpenGL is the matrix are actually row major, so when multiply vectors, vectors are (x,y,z,w)instead of (x,y,z,w)^T. So the real multiplying process is the point you draw reversely multiply the transformations you made before, and these transformations should be row major! e.g, the translation component should locate at mat[3][0] mat[3][1] mat[3][2].

* Fill gluLookAt(eye, center, up) function, 
gluLookAt(eye, center, up) is very easy, the idea is you first move the camera back to origin, and than rotate
it to the right direction (point to -z axis), The translation matrix is T, and rotation matrix R can be built by
vector eye-center and up, this coordinate system is actually the rotation matrix (Every rotation is actually a new coordinate system), the result is R * T

* Fill gluPerspective(fovy, aspect, zNear, zFar)
gluPerspective is to convert a frustum to a 2*2*2 cube centered at origin, fovy defines the opening angle of your eyes
in y direction, once the xy opening angle defined, the project plane is decided, because the y length of project plane is 2, then you know cot(fovy / 2.0) = d, aspect is the ratio of height / width (use aspect to determine x opening angle). the frustum then is decided by the near and far plane in z direction, and we need to convert this frustum to a 2*2*2 cube.

* OpenGL shading
shading is computed on each pixar (or fragment). Traditionally there are 4 kinds lighting effects, which are ambient, diffuse, specular, emissive. Ambient is backgroud lighting, we don't consider emissive, diffuse considers the roughness of the surface, diffuse light is reflected everywhere with equal intensity, and specular considers the mirror relection of the surface, specular light is reflected to the range around exit angle with different intensity.






