# DESCRIPTION
Some concepts taught as a part of Computer Graphics course in OpenGL.

# DEPENDENCIES
Open GL Library
_Change the header used in each file according to the location where you have stored the OpenGL library files._

# HOW TO RUN
To run any file, it must be compiled using:
'''
g++ filename.cpp -lopengl32 -lglu32 -lfreeglut
'''
Then execute the executable created. This is for Windows Users.

# CODES

## Bresenham Line Drawing Algorithm
When the code is executed, it asks the user to enter the two enpoints of the line, and then generates all intermediate points using the Bresenham Line Algorithm, and displays it on the screen.

## Bresenham Polygon Drawing Algorithm
Asks the user to enter the number of vertices of the polygon, and then inputs the coordinates of all the vertices. It then generates all intermediate points between any two vertices using the Bresenham Line Algorithm of the previous program, and displays it on the screen.

## Midpoint Circle Drawing algorithm
Asks the user to enter the coordinates of the centre of the circle, and its radius, and then generates all points of the circle using the Midpoint Circle Drawing Algorithm, and finally displays it on the screen.

## Boundary Fill Algorithm
Although the code can be modified to enter random vertices of a polygon, but then we need to find an internal point of the polygon to begin the fill algorithms. therefore for simplicity, I have taken a regular polygon with a defined centre.
The code asks the user to enter the number of vertices of the regular polygon, creates the regular polygon, anf then fills color inside it using the **boundary fill algorithm**. There is code for both Recursive and stack based approach. One of them must be commented when running the program.

## Flood Fill Algorithm
Although the code can be modified to enter random vertices of a polygon, but then we need to find an internal point of the polygon to begin the fill algorithms. therefore for simplicity, I have taken a regular polygon with a defined centre.
The code asks the user to enter the number of vertices of the regular polygon, creates the regular polygon, anf then fills color inside it using the **flood fill algorithm**.

## Scanline Seed Fill Algorithm
Although the code can be modified to enter random vertices of a polygon, but then we need to find an internal point of the polygon to begin the fill algorithms. therefore for simplicity, I have taken a regular polygon with a defined centre.
The code asks the user to enter the number of vertices of the regular polygon, creates the regular polygon, anf then fills color inside it using the **Scanline Seed fill algorithm** which is different from scaline fill algorithm - [Refer here](https://www.csee.umbc.edu/~ebert/435/notes/435_ch5.html). There is code for both Recursive and stack based approach. One of them must be commented when running the program.

## Transformations on a 2D object
The user is asked to enter the coordinates of a rectangle. Then the rectangle is printed on the screen.
Then using keyboard keys, the suer can perform transformations of the 2D object:

- 'W': Move the object up (translate along positive y axis)
- 'S': Move the object down (translate along negative y axis)
- 'A': Move the object left (translate along positive x axis)
- 'D': Move the object right (translate along negative x axis)
- 'Q': Rotate the object anti-clockwise by 10 degrees
- 'E': Rotate the object clockwise by 10 degrees
- 'Z': Scale up the object 1.2 times along both x and y axis
- 'X': Scale down the object 1.2 times along both x and y axis
- 'C': Shear along x-axis with shearing constant = 0.2
- 'V': Shear along y-axis with shearing constant = 0.2
- 'F': Exit

## Rotating a 3D object with Mouse, Adding textures, colors, Text, and lightning
The camera position is kept fixed. The user can press left mouse button and rotate the 3D cube wrt x and y axis.
Using the scroll up and scroll down, diffused light intensity can be varied.
Both ambient and diffused lights have equal intensity in R, G, and B.
Two sides have colors with text written, while the other four sides have textures(texture image is added).
The text is a bit buggy and appears after the cube is slightly rotated.
The light position is also fixed wrt to the camera and no mouse movement would rotate light. Therefore all surfaces appear equally bright.
If the light position is to be kept fixed wrt the world coordinates, then this change in code is required:
'''
void disp(){

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear buffers
    glViewport(0, 0, (GLint)windowWidth, (GLint)windowHeight); // optional
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // replaces current matrix ith identity matrix

    // viewing angle, aspect ratio, near plane, far plane
    gluPerspective(60, windowWidth/windowHeight , 0.1, 100);
    glMatrixMode(GL_MODELVIEW); // perspective projection
    glLoadIdentity();

    gluLookAt( // camera-position
        0, 0, 5, // eye-position - on the z-axis
        0, 0, 0, // point on the near plane(for VRP)
        0, 1, 0 // up-vector
    );

    // ambient light
    GLfloat ambientLight[] = {amb_white_light, amb_white_light, amb_white_light, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    // diffused light
    GLfloat diffuseLight[] = {diff_white_light, diff_white_light, diff_white_light, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    glRotatef(rotateX, 1.0, 0.0, 0.0); // rotate abt x-axis
    glRotatef(rotateY, 0.0, 1.0, 0.0); // rotate abt y-axis

    // position of light source (diffused)
    // light is fixed relative to the world coordinate
    GLfloat position[] = {0, 0, 5, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    colorcube();
    glutSwapBuffers(); // use instead of glFlush()
}
'''

# Conclusion
If certain improvements can be done to the done, I shall be glad to know. Or if you wish to know anything related, you can find me on LinkedIn @ygutgutia