#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int a1, b1, a2, b2; // stores the endpoints of the line

void draw_pixel(int x, int y){ // to render a point on the screen
	glBegin(GL_POINTS); // inbuilt function to render the point defined inside
	glVertex2i(x, y);
	glEnd();
}

void initialiseVals(){ // initialise opengl window
	glClear(GL_COLOR_BUFFER_BIT); // clears existing buffer values
	glClearColor(0.0, 0.0, 0.0, 1.0); // color set for the window
	glMatrixMode(GL_PROJECTION); // projection model used
	gluOrtho2D(0, 400, 0, 400); // orthographic window boundary defined
}

void bresenham(){ // applying the bresenham algorithm

	// we start with (a1, b1) and move toward (a2, b2)
	int x = a1;
	int y = b1;
	//dx and  dy are absolute values
	int dx = (a2 < a1) ? a1-a2 : a2-a1;
	int dy = (b2 < b1) ? b1-b2 : b2-b1;
	// incrementX and incrementY decides the direction of movement for x or y in each iteration
	int incrementX =  (a2 < a1) ? -1 : 1;
	int incrementY =  (b2 < b1) ? -1 : 1;

	if (dx > dy){ // absolute value of slope < 1
		draw_pixel(x, y); // to render the intial point
		int pi = 2*dy-dx; // initial p (i = 1)
		int changeD1 = 2*(dy-dx); // increment in pi, if pi > 0
		int changeD2 = 2*dy; // increment in pi, if pi < 0
		for (int i=0; i<dx; i++){ // loop until it reaches the second point (a2, b2)
			if(pi<0) // checking the value of pi
				pi += changeD2; // if pi < 0, change it by D2 value.
			else{
				y += incrementY; // increment y by 1 or -1
				pi += changeD1; // if pi < 0, change it by D1 value.
			}
			x += incrementX; // x will always increment by 1 or -1 since |m|<1
			draw_pixel(x, y); // render new point obtained
		}

	}else{ // absolute value of slope > 1
		draw_pixel(x, y); // to render the intial point
		int pi = 2*dx-dy; // initial p (i = 1)
		int changeD1 = 2*(dx-dy); // increment in pi, if pi > 0
		int changeD2 = 2*dx; // increment in pi, if pi < 0
		for (int i=0; i<dy; i++){ // loop until it reaches the second point (a2, b2)
			if(pi<0) // checking the value of pi
				pi += changeD2; // if pi < 0, change it by D2 value.
			else{ // if pi < 0, change by D1 value.
				x += incrementX; // increment x by 1 or -1
				pi += changeD1; // if pi < 0, change it by D1 value.
			}
			y += incrementY; // y will always increment by 1 or -1 since |m|>1
			draw_pixel(x, y); // render new point obtained
		}
	}
	glFlush();
}

int main(int argc, char **argv){
 
	// taking input from the user
	cout << "Enter the two points as a1, b1, a2, b2\n";
	cin >> a1 >> b1 >> a2 >> b2;
	cout << "Wait for the render...";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(400, 400); // display window size
	glutInitWindowPosition(800, 50); // display window popping position
	glutCreateWindow("Bresenham's Line Algorithm"); // title of the window
	initialiseVals();
	glutDisplayFunc(bresenham); // calling the display function
	glutMainLoop(); // refreshes the window if there is any change
	return 0;
}