#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;
#define PI 3.14

// stores the coordinates of all the vertices
vector<int> x;
vector<int> y;
int vertex; // number of vertices
int radius = 50;
// we initialise the center of the screen around which the polygon will be constructed
// this can also be taken as a point inside of the polygon (to start the boundary fill algorithm)
int centreX = 200;
int centreY = 200;

struct Color{
    GLfloat red;
    GLfloat green;
    GLfloat blue;
};
Color boundColor = {1.0, 0.0, 0.0}; // red boundary
Color backgroundColor = {0.0, 0.0, 0.0}; // black background
Color insideColor = {1.0, 1.0, 1.0}; // fill inside with white color

void generatePolygonPoints(){
    // generate all the vertices of the polygon
    for(int i=1; i<=vertex; i++){
		int currx = (int)(centreX + radius * cos(((i-1)*2.0*PI)/vertex));
		int curry = (int)(centreX + radius * sin(((i-1)*2.0*PI)/vertex));
        x.push_back(currx); // store them in x, y
        y.push_back(curry);
    }
}

// function draws a line between two points
void lineDraw(int a1, int b1, int a2, int b2){
	glBegin(GL_LINES); // inbuilt function to draw a line
	glVertex2i(a1, b1);
	glVertex2i(a2, b2);
	glEnd();
}

// for each pair of consecutive points, draw a line
void drawPolygon(){
    for (int i=0; i<(vertex-1); i++)
        lineDraw(x[i], y[i], x[i+1], y[i+1]); // passing consecutive pair of points
	// connects the last vertex to the first vertex
    lineDraw(x[vertex-1], y[vertex-1], x[0], y[0]);
}

// check whether current color is neither inside color nor boundary color
int colorNotFilled(Color currColor){
	return ((currColor.red != boundColor.red || currColor.green != boundColor.green || currColor.blue != boundColor.blue)
			&& (currColor.red != insideColor.red || currColor.green != insideColor.green || currColor.blue != insideColor.blue));
}

// check whether current pixel color is same as boundary color
int colorEqual(Color currentColor){
	return (currentColor.green == boundColor.green && currentColor.blue == boundColor.blue && currentColor.red == boundColor.red);
}

Color getColorPixel(GLint pointX, GLint pointY){
	Color pointColor;
    // used to return pixel data
	glReadPixels(pointX, pointY, 1, 1, GL_RGB, GL_FLOAT, &pointColor);
	return pointColor;
}

void fillColorPixel(GLint pointX, GLint pointY){
    // set color of a pixel to be the internal color
	glColor3f(insideColor.red, insideColor.green, insideColor.blue);
	glBegin(GL_POINTS);
	glVertex2i(pointX, pointY); // draw the point (internal to the polygon)
	glEnd();
	glFlush();
}

// fill inside of polygon using boundary fill algorithm
void BoundFillStackBased(){

    // we use stack to store points that need to be checked and filled
    stack<int> storeX;
    stack<int> storeY;
    storeX.push(centreX);
    storeY.push(centreY);

    int currX, currY;

    // until all points are colored
    while(storeX.size() > 0){

        // selecting the top most point
		currX = storeX.top();
		storeX.pop();
		currY = storeY.top();
		storeY.pop();

		// check if there is a boundary in adjacent pixel
        if(colorEqual(getColorPixel(currX+1, currY)) || colorEqual(getColorPixel(currX, currY+1))
			|| colorEqual(getColorPixel(currX-1, currY)) || colorEqual(getColorPixel(currX, currY-1)))
            continue;

        Color currColor = getColorPixel(currX, currY);
		if(colorNotFilled(currColor)){ // checking if its already filled

			fillColorPixel(currX, currY); // fill current pixel color

			// push adjacent 4 neighbours in the stack
			storeX.push(currX+1);
			storeY.push(currY);
			storeX.push(currX-1);
			storeY.push(currY);
			storeX.push(currX);
			storeY.push(currY+1);
			storeX.push(currX);
			storeY.push(currY-1);
		}
    }
}

// fill inside of polygon using boundary fill algorithm
void BoundFillRecursionBased(int currX, int currY){

	// check if there is a boundary in adjacent pixel
	if(colorEqual(getColorPixel(currX+1, currY)) || colorEqual(getColorPixel(currX, currY+1))
		|| colorEqual(getColorPixel(currX-1, currY)) || colorEqual(getColorPixel(currX, currY-1)))
		return;

	Color currColor = getColorPixel(currX, currY);
	if(colorNotFilled(currColor)){ // checking if its already filled

		fillColorPixel(currX, currY); // fill current pixel color

		// Recurse the 4-neighbours
		BoundFillRecursionBased(currX+1, currY);
		BoundFillRecursionBased(currX-1, currY);
		BoundFillRecursionBased(currX, currY+1);
		BoundFillRecursionBased(currX, currY-1);
	}

}

// function used to display all points on the window
void disp(){
	glColor3f(boundColor.red, boundColor.green, boundColor.blue); // color red used for drawing the boundary of the polygon
    drawPolygon(); // draw the boundary polygon
	// fill the inside using Boundary Fill algorithm
	//BoundFillStackBased(); // Stack based
	BoundFillRecursionBased(centreX, centreY); // Recursion Based - uncomment to use this
    glFlush();
}

int main(int argc, char **argv){

    cout<<"Enter the number of vertices of the regular polygon.\n";
    cin>>vertex;
    generatePolygonPoints(); // center of screen

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(centreX*2, centreY*2); // display window size
	glutInitWindowPosition(800, 50); // display window popping position
	glutCreateWindow("Boundary Fill Algorithm"); // title of the window

    // initialise opengl window
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION); // projection model used
	gluOrtho2D(0, centreX*2, 0, centreY*2); // orthographic window boundary defined
	glClearColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue, 0); // specifies values used by glClear to clear the buffer
	glClear(GL_COLOR_BUFFER_BIT); // clears existing buffer values

	glutDisplayFunc(disp); // calling the display function
	glutMainLoop(); // refreshes the window if there is any change
	return 0;
}