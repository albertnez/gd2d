
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
const short RADIUS = 8;

int K;
float t = 15;


//mouse
bool mouseMove = false;


bool mouseRightUp = false;
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
int firstNode = -1;
//global graph
Graph g;


void refresh(int value);

float Fa (const float& d) {
	return d*d/K;
}
float Fr (const float& d) {
	return (K*K)/d;
}


void exportGraph (const Graph &g) {
	cout << g.nodes << endl;
	cout << g.edges << endl;
	for (int i = 0; i < g.nodes; ++i) {
		if (i > 0) cout << " ";
		cout << g.degree[i];
	}
	cout << endl;
	for (int i = 0; i < 2 * g.edges + 1; ++i) {
		if (i > 0) cout << " ";
		cout << g.adj[i];
	}
	cout << endl;
	for (int i = 0; i < g.nodes +1; ++i) {
		if (i > 0) cout << " ";
		cout << g.ind[i];
	}
	cout << endl;
}
	

//*******************************
//******* END FORCE FUNCTIONS ***
//*********************************

//*********************
//****** OPEN GL ******
//*********************

void insert(vector<int> &v, const int& i, const int& k) {
	v.resize(v.size() + 1);
	for(int j = v.size(); j > i; --j){
    	v[j] = v[j - 1];
	}
	v[i] = k;
}


//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {    
	switch (key) {
    case 27: exit(0); break;
    case 's': t = 0; break;
    case 'a': t += 2; break;
    case 'd': t = max(0.0f, t-2); break;
    case 'e': exportGraph(g); break;
  }
}


int nearestNode(const Vec& pos, const Graph& g) {
	float dist = (pos - g.pos[0]).module();
	int min = 0;
	for (int i = 0; i < g.nodes; ++i) {
		if ( (pos - g.pos[i]).module() < dist) {
			dist = (pos - g.pos[i]).module();
			min = i;
		}
	}
	return min;
}


void mouseCB(int button, int state, int x, int y)
{
	
    mouseX = x;
    mouseY = y;

    mouseLeftDown = (button == GLUT_LEFT_BUTTON and state == GLUT_DOWN);
    mouseRightUp = (button == GLUT_RIGHT_BUTTON and state == GLUT_UP);
    mouseRightDown = (button = GLUT_RIGHT_BUTTON and state == GLUT_DOWN);

    //get node
    if(mouseLeftDown and dragNode < 0) {
    	int i = 0;
    	bool found = false;
    	Vec mousePos = Vec(x, y) - Vec(posX, posY);
    	while (i < g.nodes and !found) {
    		//cout << "Node: " << i << endl <<
    		//"Dist: " << (Vec(x, y) - g.pos[i]).module() << endl;

    		if (( mousePos - g.pos[i]).module() < RADIUS + 8 ) {
    			dragNode = i;
    			found = true;
    		}
    		++i;
    	}
    	
    }
    else dragNode = -1;


    if(mouseRightDown) {
    	firstNode = nearestNode(Vec(x, y), g);
    }

    if(mouseRightUp) {
    	if (!mouseMove) {
    	Vec mousePos = Vec(x, y) - Vec(posX, posY);
    	//add the node

    	//# nodes
    	g.ind.push_back(g.ind[g.nodes]);
    	++g.nodes;
    	//pos
    	g.pos.push_back(mousePos);
    	//disp
    	g.disp.push_back(Vec(0,0));
    	g.degree.push_back(0);

    	} 
    	//Unir arestes
    	else {
    		int second = nearestNode(Vec(x, y), g);
    		if (firstNode != second) {
	    		int first = min(firstNode, second);
	    		second = max(firstNode, second);
	    		++g.edges;
	    		++g.degree[first];
	    		++g.degree[second];
	    		insert(g.adj, g.ind[second], first);
	    		insert(g.adj, g.ind[first], second);
	    		
	    		for (int i = first+1; i <= g.nodes; ++i) {
	    			++g.ind[i];
	    		}
	    		for (int i = second+1; i <= g.nodes; ++i) {
	    			++g.ind[i];
	    		}
	    	}
    		mouseMove = false;

    	}

		firstNode = -1;

    }
    //if(button == 3) Zoom += 0.5;
    //else if(button == 4) Zoom -= 0.5;
	glutPostRedisplay();	
}

void mouseMotionCB(int x, int y)
{
	
    if(mouseLeftDown) {
    	if (dragNode >= 0) {
	        moveX = (x - mouseX)*2.5;
	        moveY = (y - mouseY)*2.5;
	        mouseX = x;
	        mouseY = y;
    	}
    	/*
    	else {
	    	posX += (x - mouseX);
	    	posY += (y - mouseY);
	    	mouseX = x;
	    	mouseY = y;
    	}
    	*/
    	glutPostRedisplay();
    }
    else if(mouseRightDown)
    {
    	if ((x - mouseX != 0 or y - mouseY != 0 ) and !mouseMove) {
    		mouseMove = true;
    	}
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
	float x, y;
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
				float m = dif.module();
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
			float m = dif.module();
			if (m != 0) {
				g.disp[i] = g.disp[i] - dif.unit()*Fa(m);
			}
		}
	}

	//Walls
	/*
	for (int i = 0; i < g.nodes; ++i) {

	}
	*/

	//temperature
	for (int i = 0; i < g.nodes; ++i) {
		if (i == dragNode) {
			g.pos[i] = g.pos[i] + Vec(moveX, moveY);
		}
		else {
			float m = g.disp[i].module();
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

	t *= 0.99;

	//Fix to walls!
	for (int i = 0; i < g.nodes; ++i) {
	g.pos[i].set (
		min(float(xSize - RADIUS), max(float(RADIUS), g.pos[i].getX())) ,
		min(float(ySize - RADIUS), max(float(RADIUS), g.pos[i].getY()))
		);
	}

	glutPostRedisplay();
	if (t > 0.06)  glutTimerFunc(25, update, 0);
	else {
		t = 0;
	 refresh(0);
	}
}


void refresh(int value) {
	g.pos[dragNode] = g.pos[dragNode] + Vec(moveX, moveY);
	glutPostRedisplay();
	if (t > 0) update(0);
	else glutTimerFunc(50, refresh, 0);
}
//Fix position
		//g.pos[i].y = min(ySize - RADIUS, max(0+RADIUS, g.pos[i].y));

//*******************************
//*********** MAIN **************
//*******************************

int main(int argc, char** argv) {
	srand(time(0));

	//Initialitze Graph
	initGraph(g);

	//Initialize K
	K = 0.5*sqrt((xSize*ySize)/g.nodes);

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









