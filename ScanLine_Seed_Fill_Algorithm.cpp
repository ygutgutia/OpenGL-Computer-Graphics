#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;
#define PI 3.14

// stores the coordinates of all the vertices
vector<int> x;
vector<int> y;
int vertex; // number of vertices
int radius = 50.0;
// we initialise the center of the screen around which the polygon will be constructed
// this can also be taken as a point inside of the polygon (to start the flood fill algorithm)
int centreX = 200.0;
int centreY = 200.0;

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

// check whether current pixel color is same as background color
bool colorEqualBoundary(Color currentColor){
	return (currentColor.green == boundColor.green && currentColor.blue == boundColor.blue && currentColor.red == boundColor.red);
}

// check whether current pixel color is same as fill color
bool colorEqualFill(Color currentColor){
	return (currentColor.green == insideColor.green && currentColor.blue == insideColor.blue && currentColor.red == insideColor.red);
}

Color getColorPixel(GLint pointX, GLint pointY){
	Color pointColor;
    // used to return pixel data
	glReadPixels(pointX, pointY, 1, 1, GL_RGB, GL_FLOAT, &pointColor);
	return pointColor;
}

bool isPixelValid(int x, int y){
	return (!colorEqualBoundary(getColorPixel(x, y)) && !colorEqualFill(getColorPixel(x, y)));
}

void fillColorPixel(GLint pointX, GLint pointY){
    // set color of a pixel to be the internal color
	glColor3f(insideColor.red, insideColor.green, insideColor.blue);
	glBegin(GL_POINTS);
	glVertex2i(pointX, pointY); // draw the point (internal to the polygon)
	glEnd();
	glFlush();
}

// fill inside of polygon using flood fill algorithm - stack based
void ScanLineSeedAlgoStackBased(){
	
	// we use stack to store points that need to be checked and filled
	stack<int> storeX;
	stack<int> storeY;
	storeX.push(centreX);
    storeY.push(centreY);

    // until all points are colored
    while(storeX.size() > 0){
		
		// selecting the top most point
		int currX = storeX.top();
		storeX.pop();
		int currY = storeY.top();
		storeY.pop();

		if(!isPixelValid(currX, currY))
			continue;

		// find right end point and meanwhile fill current scan line
		int currRightPixel = currX;
		while(!colorEqualBoundary(getColorPixel(currRightPixel, currY))){
			fillColorPixel(currRightPixel, currY);
			currRightPixel++;
		}
		currRightPixel--; // inside polygon

		// find left end point and meanwhile fill current scan line
		int currLeftPixel = currX;
		while(!colorEqualBoundary(getColorPixel(currLeftPixel, currY))){
			fillColorPixel(currLeftPixel, currY);
			currLeftPixel--;
		}
		currLeftPixel++; // inside polygon

		// scan above scan line
		int xtemp = currLeftPixel;
		int ytemp = currY + 1;
		bool findNewSeed = false;
		while(xtemp <= currRightPixel){
			findNewSeed = false;
			while(isPixelValid(xtemp, ytemp) && (xtemp < currRightPixel)){
				findNewSeed= true;
				xtemp++;
			}
			if(findNewSeed){
				if(isPixelValid(xtemp, ytemp) && (xtemp == currRightPixel)){
					storeX.push(xtemp);
					storeY.push(ytemp);
				}
				else{
					storeX.push(xtemp-1);
					storeY.push(ytemp);
				}
			}
			/* Skips the internal invalid point to the right (the case where there is an obstacle at the right end of the processing interval)*/
			int xspan = 0;
			while(colorEqualBoundary(getColorPixel(xtemp+xspan, ytemp)))
				xspan++;
			/* handles special cases to exit the while(x<=currRightPixel) loop*/
			xtemp += (xspan ==0) ? 1 : xspan;
		}

		// scan below scan line
		xtemp = currLeftPixel;
		ytemp = currY - 1;
		findNewSeed = false;
		while(xtemp <= currRightPixel){
			findNewSeed = false;
			while(isPixelValid(xtemp, ytemp) && (xtemp < currRightPixel)){
				findNewSeed= true;
				xtemp++;
			}
			if(findNewSeed){
				if(isPixelValid(xtemp, ytemp) && (xtemp == currRightPixel)){
					storeX.push(xtemp);
					storeY.push(ytemp);
				}
				else{
					storeX.push(xtemp-1);
					storeY.push(ytemp);
				}
			}
			/* Skips the internal invalid point to the right (the case where there is an obstacle at the right end of the processing interval)*/
			int xspan = 0;
			while(colorEqualBoundary(getColorPixel(xtemp+xspan, ytemp)))
				xspan++;
			/* handles special cases to exit the while(x<=currRightPixel) loop*/
			xtemp += (xspan == 0) ? 1 : xspan;
		}

	}	
}

// fill inside of polygon using flood fill algorithm - recursion based
void ScanLineSeedAlgoRecursionBased(int currX, int currY){
	
	if(!isPixelValid(currX, currY))
		return;

	// find right end point and meanwhile fill current scan line
	int currRightPixel = currX;
	while(!colorEqualBoundary(getColorPixel(currRightPixel, currY))){
		fillColorPixel(currRightPixel, currY);
		currRightPixel++;
	}
	currRightPixel--; // inside polygon

	// find left end point and meanwhile fill current scan line
	int currLeftPixel = currX;
	while(!colorEqualBoundary(getColorPixel(currLeftPixel, currY))){
		fillColorPixel(currLeftPixel, currY);
		currLeftPixel--;
	}
	currLeftPixel++; // inside polygon

	// scan above scan line
	int xtemp = currLeftPixel;
	int ytemp = currY + 1;
	bool findNewSeed = false;
	while(xtemp <= currRightPixel){
		findNewSeed = false;
		while(isPixelValid(xtemp, ytemp) && (xtemp < currRightPixel)){
			findNewSeed= true;
			xtemp++;
		}
		if(findNewSeed){
			if(isPixelValid(xtemp, ytemp) && (xtemp == currRightPixel))
				ScanLineSeedAlgoRecursionBased(xtemp, ytemp);
			else
				ScanLineSeedAlgoRecursionBased(xtemp-1, ytemp);
		}
		/* Skips the internal invalid point to the right (the case where there is an obstacle at the right end of the processing interval)*/
		int xspan = 0;
		while(colorEqualBoundary(getColorPixel(xtemp+xspan, ytemp)))
			xspan++;
		/* handles special cases to exit the while(x<=currRightPixel) loop*/
		xtemp += (xspan ==0) ? 1 : xspan;
	}

	// scan below scan line
	xtemp = currLeftPixel;
	ytemp = currY - 1;
	findNewSeed = false;
	while(xtemp <= currRightPixel){
		findNewSeed = false;
		while(isPixelValid(xtemp, ytemp) && (xtemp < currRightPixel)){
			findNewSeed= true;
			xtemp++;
		}
		if(findNewSeed){
			if(isPixelValid(xtemp, ytemp) && (xtemp == currRightPixel))
				ScanLineSeedAlgoRecursionBased(xtemp, ytemp);
			else
				ScanLineSeedAlgoRecursionBased(xtemp-1, ytemp);
		}
		/* Skips the internal invalid point to the right (the case where there is an obstacle at the right end of the processing interval)*/
		int xspan = 0;
		while(colorEqualBoundary(getColorPixel(xtemp+xspan, ytemp)))
			xspan++;
		/* handles special cases to exit the while(x<=currRightPixel) loop*/
		xtemp += (xspan == 0) ? 1 : xspan;
	}
}


// function used to display all points on the window
void disp(){
	glColor3f(boundColor.red, boundColor.green, boundColor.blue); // color red used for drawing the boundary of the polygon
    drawPolygon(); // draw the boundary polygon
	ScanLineSeedAlgoStackBased(); // fill the inside using Scanline Seed Fill algorithm - stack Based
	// ScanLineSeedAlgoRecursionBased(centreX, centreY); // fill the inside using Scanline Seed Fill algorithm - recursion Based
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
	glutCreateWindow("ScanLine Seed Fill Algorithm"); // title of the window

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