
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <sstream>
#include <algorithm>

//OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Vec2d.h"
#include "Graph.h"

using namespace std;
typedef vector<int> vi;

const int xSize = 900;
const int ySize = 600;
const short RADIUS = 10;

int K;
double t = 2;


//mouse
bool mouseLeftDown = false;
bool mouseRightDown = false;

float mouseX = 0;
float mouseY = 0;

float posX = 0;
float posY = 0;

float moveX = 0;
float moveY = 0;

float Zoom = 0;

int dragNode = -1;
//global graph
Graph g;


double Fa (const double& d) {
	return d*d/K;
}
double Fr (const double& d) {
	return (K*K)/d;
}



//*******************************
//******* END FORCE FUNCTIONS ***
//*********************************

//*********************
//****** OPEN GL ******
//*********************

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {    //The current mouse coordinates
	switch (key) {
    case 27: exit(0); break;

    	default: cout << "Mouse position: " << x << " | " << y << endl; break;
  }
}

void mouseCB(int button, int state, int x, int y)
{
	
    mouseX = x;
    mouseY = y;

    mouseLeftDown = (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN);
    mouseRightDown = (button == GLUT_RIGHT_BUTTON and state == GLUT_DOWN);

    //get node
    if(mouseLeftDown) {
    	int i = 0;
    	bool found = false;
    	while (i < g.nodes and !found) {
    		//cout << "Node: " << i << endl <<
    		//"Dist: " << (Vec(x, y) - g.pos[i]).module() << endl;
    		if ( (Vec(x, y) - g.pos[i]).module() < RADIUS ) {
    			dragNode = i;
    			found = true;
    		}
    		++i;
    	}
    	
    }
    else dragNode = -1;
    if(button == 3) Zoom += 0.5;
    else if(button == 4) Zoom -= 0.5;


	glutPostRedisplay();	
	
}

void mouseMotionCB(int x, int y)
{
	
    if(mouseLeftDown and dragNode >= 0)
    {
        moveX = (x - mouseX)*5;
        moveY = (y - mouseY)*5;
        mouseX = x;
        mouseY = y;
    	glutPostRedisplay();

    }
    else if(mouseRightDown)
    {
    	posX += (x - mouseX);
    	posY += (y - mouseY);
    	mouseX = x;
    	mouseY = y;
    	glutPostRedisplay();
    }
    
}

//Initializes 3D rendering
void initRendering() {
  //Makes 3D drawing work when something is in front of something else
 // glViewport(0,0,1,1);
glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0, xSize, ySize, 0, 0, 1);
glMatrixMode(GL_MODELVIEW);
}





void drawCircle(float cx, float cy, short r) 
{  
	int num_segments = 10 * sqrtf(r);//change the 10 to a smaller/bigger number as needed 

	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tan(theta);//calculate the tangential factor 

	float radial_factor = cos(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 

	float y = 0; 
    
	glBegin(GL_TRIANGLE_FAN); 
	glVertex2f(cx, cy);
	for(int ii = 0; ii <= num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//calculate the tangential vector 
		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 

		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 

		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}

void drawGraph(const Graph& g) {
	//arestes
	glBegin(GL_LINES);
	glColor3f(0.5f, 0.5f, 0.5f);
	double x, y;
	int k;
	for (int i = 0; i < g.nodes; ++i) {
		x = g.pos[i].getX();
		y = g.pos[i].getY();

		for (int j = g.ind[i]; j < g.ind[i+1]; ++j) {
			k = g.adj[j];
			glVertex2f(x, y);
			glVertex2f(g.pos[k].getX(), g.pos[k].getY());

		}
	}
	glEnd();
	//nodes
	glColor3f(0.6, 0.0, 0.0);
	for (int i = 0; i < g.nodes; ++i) {
		drawCircle(g.pos[i].getX(), g.pos[i].getY(), RADIUS);
	}
}

void drawScene() {
  //Clear information from last draw
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity(); //Reset the drawing perspective

  //glScaled(0.25, 0.25, 0);
  glTranslatef(0, 0, Zoom);

  glTranslatef(posX, posY, 0);

  drawGraph(g);



  glutSwapBuffers();
}


//*******************************
//********** UPDATE *************
//*******************************

void update(int value) {
	//repulsion
	for (int i = 0; i < g.nodes; ++i) {
		g.disp[i] = Vec(0,0);
		for (int j = 0; j < g.nodes; ++j) {
			if (i != j) {
				Vec dif = g.pos[i] - g.pos[j];
				double m = dif.module();
				if (m != 0) {
					g.disp[i] = g.disp[i] + dif.unit()*Fr(m);
				}
			}
		}
	}
	//atraction
	for (int i = 0; i < g.nodes; ++i) {
		for (int j = g.ind[i]; j < g.ind[i+1]; ++j) {
			Vec dif = g.pos[i] - g.pos[g.adj[j]];
			double m = dif.module();
			if (m != 0) {
				g.disp[i] = g.disp[i] - dif.unit()*Fa(m);
			}
		}
	}
	//temperature
	for (int i = 0; i < g.nodes; ++i) {
		if (i == dragNode) {
			g.pos[i] = g.pos[i] + Vec(moveX, moveY);
		}
		else {
			double m = g.disp[i].module();
			if (m != 0) {
				if ( m < t) {
					g.pos[i] = g.pos[i] + g.disp[i];
				}
				else {
					g.pos[i] = g.pos[i] + g.disp[i].unit()*t;
				}
			}
		}
	}

	//t *= 0.99;
	//cout << "T: " << t << endl;


	glutPostRedisplay();
	if (t > 0.1) glutTimerFunc(25, update, 0);
}
//Fix position
		//g.pos[i].x = min(xSize - RADIUS, max(0+RADIUS, g.pos[i].x));
		//g.pos[i].y = min(ySize - RADIUS, max(0+RADIUS, g.pos[i].y));

//*******************************
//*********** MAIN **************
//*******************************

int main(int argc, char** argv) {
	srand(time(0));

	//Initialitze Graph
	initGraph(g);

	//Initialize K
	K = 0.3*sqrt((xSize*ySize)/g.nodes);
	cout << "K: " << K << endl;

	setNodesPosition(g, xSize, ySize);

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(xSize, ySize); //Set the window size
	  
	//Creating Window
	//stringstream title;
	//title << "Nodes: " << g.nodes << " | Arestes: " << g.arestes; 
	glutCreateWindow("Graph drawing in 2D using forces");
	initRendering(); //Initialize rendering
  
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
	// glutReshapeFunc(handleResize);
  
	glutTimerFunc(50, update, 0);
	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}









