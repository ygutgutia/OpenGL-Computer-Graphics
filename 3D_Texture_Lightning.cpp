#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

bool isLMBclicked = false; // check if LMB is clicked
int windowWidth = 800; // size of window
int windowHeight = 800;
double rotateX = 0; // how much x, y have been rotated using mouse
double rotateY = 0;
double diffX = 0.0f; // temp variables
double diffY = 0.0f;
float amb_white_light = 0.2; // ambient light intensity
float diff_white_light = 0.8; // diffused light intensity
int xText = 0, yText = 0;

// surface colors
GLfloat mat_refl1[] = {0.35, 0.65, 0.0, 1}; // top surface
GLfloat mat_refl2[] = {0.0, 0.4, 0.5, 1}; // bottom surface
GLfloat mat_refl3[] = {1.0, 1.0, 1.0, 1.0}; // other surfaces

// standard cube vertices
float vertex[8][3] = { {-1.0, -1.0, 1.0},
                    {-1.0, 1.0, 1.0},
                    {1.0, 1.0, 1.0},
                    {1.0, -1.0, 1.0},
                    {-1.0, -1.0, -1.0},
                    {-1.0, 1.0, -1.0},
                    {1.0, 1.0, -1.0},
                    {1.0, -1.0, -1.0}, };


// load text on one face
void load_text(GLfloat x, GLfloat y, GLfloat z, char *format, ...)
{
    glDisable(GL_LIGHTING); // to not affect text color
    char buffer[200], *p;
    va_list args;

    va_start(args, format);
        vsprintf(buffer, format, args);
    va_end(args);
    glPushMatrix();
        glTranslatef(x, y, z);
        glTranslatef(xText, yText, 0);
        glScalef(0.001, 0.001, 0.001);
        if (z == -1) // else text will be inverted
            glRotatef(180, 0, 1, 0);   
        glLineWidth(4); // line widths
        glPointSize(1);
        for (p = buffer; *p; p++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

/* Loads in a bitmap as a GL texture */
void LoadTexture(const char *filename)
{
    GLuint textureTemp;
    int width, height;
    unsigned char * data;

    FILE * file;
    file = fopen(filename, "rb");

    if (file == NULL) return;
    width = 800; // resolution of bmp file
    height = 800;
    data = (unsigned char *)malloc(width * height * 3); // each pixel has RGB colors
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for(int i=0; i<width * height; ++i)
    {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];

        data[index] = R;
        data[index+2] = B;
    }

    glGenTextures(1, &textureTemp);
    glBindTexture(GL_TEXTURE_2D, textureTemp); // binding texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); // mipmap to select best resolution
    free(data);
}

// to display the textured sides
void TexturedSide(int a, int b, int c, int d){
    glEnable(GL_TEXTURE_2D); // enable texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); // texture relative coordinate
    glVertex3fv(vertex[a]); // vertex coordinate
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(vertex[b]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(vertex[c]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(vertex[d]);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// to display non-textured sides
void NormalSide(int a, int b, int c, int d){
    glBegin(GL_QUADS);
    glVertex3fv(vertex[a]);
    glVertex3fv(vertex[b]);
    glVertex3fv(vertex[c]);
    glVertex3fv(vertex[d]);
    glEnd();
}

// displaying tbhe entire cube
void colorcube(){
    TexturedSide(6, 2, 3, 7); // x = 1 surface
    TexturedSide(1, 5, 4, 0); // x = -1 surface
    TexturedSide(1, 2, 6, 5); // y = 1 surface
    TexturedSide(0, 4, 7, 3); // y = -1 surface

    glColorMaterial(GL_FRONT, GL_DIFFUSE); // for glColor3f
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_refl1); // set material property
    glColor3f(mat_refl1[0], mat_refl1[1], mat_refl1[2]); // less expensive than glMatetialfv(GL_DIFFUSE)
    NormalSide(2, 1, 0, 3); // draw colored side
    glColor3f(0, 0, 0); // black text
    load_text(-0.5, 0, 1, "FRONT FACE"); // draw text

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_refl2); // set material property
    glColor3f(mat_refl2[0], mat_refl2[1], mat_refl2[2]);
    NormalSide(6, 5, 4, 7); // draw colored side
    glColor3f(0, 0, 0); // black text
    load_text(0.5, 0, -1, "BACK FACE"); // draw text

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_refl3); // reset property to not affect other sides
    glColor3f(mat_refl3[0], mat_refl3[1], mat_refl3[2]); // to prevent texture tinting
    glDisable(GL_COLOR_MATERIAL);
}

// act on mouse movement
void mouseMotion(int currX, int currY){
    if (isLMBclicked){
        rotateX = currY - diffY;
        rotateY = currX - diffX;
        glutPostRedisplay();
    }
}

// detect mouse actions
void mouseFunc(int button, int state, int currX, int currY){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        isLMBclicked = true; // set LMB true
        diffX = currX - rotateY;
        diffY = currY - rotateX;
    }
    else
        isLMBclicked = false;

    // Wheel reports as button 3(scroll up)
    if (button == 3){
        xText+=0.5;
        yText+=0.5;
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        diff_white_light = diff_white_light * 1.1; // increase diffused light intensity
        if(diff_white_light > 1) // intensity <= 1
            diff_white_light = 1;
        glutPostRedisplay();
    }
    // Wheel reports as button 4(scroll down)
    if (button == 4){
        xText-=0.5;
        yText-=0.5;
        if (state == GLUT_DOWN) return; // Disregard redundant GLUT_DOWN events
        diff_white_light = diff_white_light * 0.9; // decrease diffused light intensity
        glutPostRedisplay();
    }
}


void disp(){

    // position of light source (diffused)
    // setting position before model view transformations, so that position doesnt change
    // light is fixed relative to eye
    GLfloat position[] = {0, 0, 5, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);

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
    colorcube();
    glutSwapBuffers(); // use instead of glFlush()
}

void init(){

    LoadTexture("texture_800_800.bmp"); // Load in the texture
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    glEnable(GL_DEPTH_TEST); // use depth buffer
    glEnable(GL_LIGHTING); // use lighting
    glEnable(GL_LIGHT0);

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    // GLUT_DOUBLE - uses double buffer
    // Use glutSwapBuffers() at end not glFlush()
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(windowWidth, windowHeight); // set window size
    glutCreateWindow("Cube Rendering"); // generating window with a title

    init();
    glutDisplayFunc(disp);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    return 0;
}