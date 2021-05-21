#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

// stores the centre and the radius of the circle
int xcentre, ycentre, radius;

void draw_pixel(int x, int y) { // to render a point on the screen
	glBegin(GL_POINTS); // inbuilt function to render the point defined inside
	glVertex2i(x + xcentre, y + ycentre); // renders points wrt to the centre of the circle
	glEnd();
}

void initialiseVals(){ // initialise opengl window
	glClear(GL_COLOR_BUFFER_BIT); // clears existing buffer values
	glClearColor(1.0, 1.0, 1.0, 0.0); // color set for the window
	glColor3f(1.0, 0.0, 0.0); // color red is used to draw
	glMatrixMode(GL_PROJECTION); // projection model used
	gluOrtho2D(0, 400, 0, 400); // orthographic window boundary defined
}

// using the midpoint circle algorithm to render all points
void midPointCircleAlgo(){
	// calculating circle coordinates wrt to (0, 0) starting at (0, r).
	// during render we shall shift them wrt to the given centre.
	int x = 0; // x-coordinate of initial point (0, r) [wrt to (0, 0)]
	int y = radius; // initial y-coordinate of point
	int di = 5 - 4*radius; // intitial value of d (i = 0)
	draw_pixel(x, y); // render the intiial point

	// only computing for the 2nd octant and using symmetry property
	while (y > x){
		x++; // x will always increment by 1 in the 2nd octant starting from (r, 0).
		if (di < 0) // if di<0
			di += 8*x+4; // new di value
		else{ // if di>=0
			y--; // always decrement y value (since second octant)
			di += 8*(x-y)+4; // update di
		}
		// using symmetry property of circle to reduce re-computation
		draw_pixel(x, y); // second octant
		draw_pixel(x, -y); // seventh octant
		draw_pixel(-x, y); // third octant
		draw_pixel(-x, -y); // sixth octant
		draw_pixel(y, x); // first octant
		draw_pixel(-y, x); // eighth octant
		draw_pixel(y, -x); // fourth octant
		draw_pixel(-y, -x); // fifth octant
	}
	glFlush();
}

int main(int argc, char** argv){	

	// taking the centre coordinates, and the radius as an input from the user
	cout << "Enter the x-coordinate for the center of the circle:\n";
    cin >> xcentre;
	cout << "Enter the y-coordinate for the center of the circle:\n";
    cin >> ycentre;
	cout << "Enter the radius of the circle:\n";
    cin >> radius;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400); // display window size
	glutInitWindowPosition(800, 50); // display window popping position
	glutCreateWindow ("Circle Drawing midpoint algorithm"); // title of the window
	initialiseVals();
	glutDisplayFunc(midPointCircleAlgo); // calling the display function
	glutMainLoop(); // refreshes the window if there is any change
    return 0;
}