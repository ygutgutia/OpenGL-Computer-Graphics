#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

// stores user input for all vertex coordinates
vector<float> x;
vector<float> y;
int n = 4; // number of vertices

GLfloat m1[16] = { // matrix defined for shearing along x
    1, 0, 0, 0,
    0.2, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};


GLfloat m2[16] = { // matrix defined for shearing along y
    1, 0.2, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

void initialiseVals(){ // initialise opengl window
    glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT); // clears existing buffer values
	glColor3f(1.0, 0.0, 0.0); // color red used for drawing
	glMatrixMode(GL_PROJECTION); // projection model used
	gluOrtho2D(0, 400, 0, 400); // orthographic window boundary defined
}

void rectangle(){ // function draws the rectangle
	glBegin(GL_LINE_LOOP); // inbuilt function to join all points defined inside
	glVertex2f(x[0], y[0]);
	glVertex2f(x[1], y[1]);
	glVertex2f(x[2], y[2]);
	glVertex2f(x[3], y[3]);
	glEnd();
	glFlush();  
}

void Keyboard(unsigned char c, int x, int y){

    glClear(GL_COLOR_BUFFER_BIT); // clears buffer before updating values
    // since we have used orthographic matrix type, all transformations are wrt to the window origin
    switch(c)
    {
        case 119: //W - Move up
            printf("Moving Up.\n");
            glTranslatef(0, 5, 0); // translate by 5 units up
            break;
        case 115: //S - Move Down
            printf("Moving Down.\n");
            glTranslatef(0, -5, 0); // translate by 5 units down
            break;
        case 97: //A - Move Left
            printf("Moving Left.\n");
            glTranslatef(-5, 0, 0); // translate by 5 units left
            break;
        case 100: //D - Move right
            printf("Moving Right.\n"); // translate by 5 units  right
            glTranslatef(5, 0, 0);
            break;
        case 113: //Q - Rotating ACW
            printf("Rotating anticlockwise.\n");
            glRotatef(10, 0, 0, 1); // rotating 10 degrees positive (acw)
            break;
        case 101: //E - Rotating CW
            printf("Rotating clockwise.\n");
            glRotatef(-10, 0, 0, 1); // rotating 10 degrees negative (cw)
            break;
        case 122: //Z - Scaling up
            printf("Scaling up.\n");
            glScalef(1.2, 1.2, 0); // scaling up the object
            break;
        case 120: //X - Scaling down
            printf("Scaling down.\n");
            glScalef(1/1.2, 1/1.2, 0); // scaling down the object
            break;
        case 99: //C - Shear along x
            printf("Shear along x.\n");
            glMultMatrixf(m1); // multiplying the orthographic matrix with the shear matrix for x
            break;
        case 118: //V - Shear along y
            printf("Shear along y.\n");
            glMultMatrixf(m2); // multiplying the orthographic matrix with the shear matrix for y
            break;
        case 102: //F - Exit
            printf("Exiting.\n");
            exit(1);
    }
    glutDisplayFunc(rectangle); // re-rendering the object
    glutPostRedisplay();
}


int main(int argc, char **argv){

	// taking vertex input from the user
    cout<<"Enter all coordinates in either clw or acw (x, y)\n";
	for(int i=0; i<n; i++){
		float temp1, temp2;
		cin >> temp1 >> temp2;
		x.push_back(temp1);
		y.push_back(temp2);
	}
	cout<<"Waiting for the render\n";
    // x = {150, 300, 300, 150}; //initialised for instant use
    // y = {150, 150, 300, 300};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(400, 400); // display window size
	glutInitWindowPosition(800, 50); // display window popping position
	glutCreateWindow("Rectangle Transformations"); // title of the window
	initialiseVals();
	glutDisplayFunc(rectangle); // calling the display function
    glutKeyboardFunc(Keyboard); // captures any keyboard input and renders window accordingly
	glutMainLoop(); // refreshes the window if there is any change
	return 0;
}