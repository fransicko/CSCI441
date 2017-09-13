/*
 *  CSCI 441 Computer Graphics, Fall 2017
 *
 *  Project: a2
 *  File: main.cpp
 *
 *  Author: Dr. Jeffrey Paone - Fall 2017
 *
 *  Description:
 *		This is the shell code for Assignment 2.  It contains predefined
 *		methods that simply need to be called properly to have your Hero
 *		move throughout the map.
 */

#include <GLFW/glfw3.h>		// include GLFW framework header

#ifdef __APPLE__			// if compiling on Mac OS
	#include <OpenGL/gl.h>
#else						// else compiling on Linux OS
	#include <GL/gl.h>
#endif

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>				// for printf functionality
#include <stdlib.h>				// for exit functionality
#include <cmath>

// include everything necessary to make our world map work
// depends upon SOIL library
#include "WorldMap.h"

//*************************************************************************************
//
// Global Parameters

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;
int HEAD_START = 200, HEAD_MOVE = 1; // The initial position of the head
int ADDITIONAL_X = 0, ADDITIONAL_Y = 0; // These will be added to the character to change its location

double ScreenRed = 0, ScreenBlue = 0, ScreenGreen = 0; // we will change the color based on the position of the mouse
double mouseX = 0, mouseY = 0; // this is for the position of the mouse
//*************************************************************************************
//
// Event Callbacks

static void error_callback(int error, const char* description) {
	fprintf(stderr, "[ERROR]: %s\n", description);
}

//*************************************************************************************
//
// Setup Functions
void moveCharLeft() {
	if (ADDITIONAL_X == -270) {
		ADDITIONAL_X = 270;
		moveLeft();
	}
	else ADDITIONAL_X -= 10;
}

void moveCharRight() {
	if (ADDITIONAL_X == 270) {
		ADDITIONAL_X = -270;
		moveRight();
	}
	else ADDITIONAL_X += 10;
}

void moveCharUp() {
	if (ADDITIONAL_Y == 310) {
		ADDITIONAL_Y = -200;
		moveUp();
	}
	else ADDITIONAL_Y += 10;
}

void moveCharDown() {
	if (ADDITIONAL_Y == -200) {
		ADDITIONAL_Y = 310;
		moveDown();
	}
	else ADDITIONAL_Y -= 10;
}

void keyboard_callback( GLFWwindow *win, int key, int scancode, int action, int mods ) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		exit(EXIT_SUCCESS);
	}
	
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		moveCharLeft();
	}
	
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		moveCharRight();
	}
	
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		moveCharUp();
	}
	
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		moveCharDown();
	}
	
}

void mouse_button_callback( GLFWwindow *window, int button, int action, int mods ) {
	double XX = 0, YY = 0;
	glfwGetCursorPos(window, &XX, &YY);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			ScreenRed = XX / 512;
			ScreenGreen = YY / 512;
			ScreenBlue = (ScreenRed + ScreenGreen) / 2;
		}
	}
}

void cursor_callback( GLFWwindow *window, double x, double y ) {
	mouseX = x;
	mouseY = WINDOW_HEIGHT - y;
}

//
//  void setupGLFW()
//
//      Used to setup everything GLFW related.  This includes the OpenGL context
//	and our window.
//
GLFWwindow* setupGLFW() {
	// set what function to use when registering errors
	// this is the ONLY GLFW function that can be called BEFORE GLFW is initialized
	// all other GLFW calls must be performed after GLFW has been initialized
	glfwSetErrorCallback( error_callback );

	// initialize GLFW
	if (!glfwInit()) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit(EXIT_FAILURE);
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE  );	// use double buffering
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );	// request OpenGL v2.X
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );	// request OpenGL v2.1
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );		// do not allow our window to be able to be resized

	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 2", NULL, NULL );
	if( !window ) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	glfwSetKeyCallback( window, keyboard_callback);
	glfwSetMouseButtonCallback( window, mouse_button_callback );
	glfwSetCursorPosCallback( window, cursor_callback );
	glfwMakeContextCurrent(window);		// make the created window the current window
	glfwSwapInterval(1);				// update our window after at least 1 screen refresh

	return window;						// return the window that was created
}

//
//  void setupOpenGL()
//
//      Used to setup everything OpenGL related.  For now, the only setting
//	we need is what color to make the background of our window when we clear
//	the window.  In the future we will be adding many more settings to this
//	function.
//
void setupOpenGL() {
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// set the clear color to black
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

// This will modify the color of the orc based on the mouse position based on when it was clicked
double colorAdd(double color, char a) {
	double Screen = 0;
	switch(a) {
		case 'r':
			Screen = ScreenRed;
		case 'b':
			Screen = ScreenBlue;
		case 'g':
			Screen = ScreenGreen;
	}
	
	if ((Screen+ color) >= 1.0) color = 1.0;
	else return (Screen + color);
	
	return color;
}

// This is function is to have the eyes follow the mouse.
double eyePosition(double eye, char axis) {
	// calculate the difference between hardcoded eye and the mouse.
	double pos = 0;
	double dif = 0;
	switch (axis) {
		case 'x':
			pos = mouseX - (HEAD_START + ADDITIONAL_X);
			break;
		case 'y':
			pos = mouseY - (200 + ADDITIONAL_Y);
			break;
	}
	
	if ((dif = (pos - eye)) <= 5 && dif >= -5) return pos;
	else if (dif < -5 ) return eye - 5;
	else if (dif > 5) return eye + 5;
	
	return 0;
}


void drawHair() {
	// Draw the head hair
	glBegin(GL_TRIANGLES);
	glColor3f(colorAdd(0.000, 'r'), colorAdd(0.000, 'g'), colorAdd(0.545, 'b'));
	glVertex2f(0, 60);
	glVertex2f(60, 60);
	glVertex2f(30, 40);
	glEnd();
	
	// Draw beard
	glBegin(GL_TRIANGLES);
	glColor3f(colorAdd(0.000, 'r'), colorAdd(0.000, 'g'), colorAdd(0.545, 'b'));
	glVertex2f(0, 0);
	glVertex2f(10, -10);
	glVertex2f(20, 0);
	glEnd();
	
	glBegin(GL_TRIANGLES);
	glColor3f(colorAdd(0.000, 'r'), colorAdd(0.000, 'g'), colorAdd(0.545, 'b'));
	glVertex2f(20, 0);
	glVertex2f(30, -10);
	glVertex2f(40, 0);
	glEnd();
	
	glBegin(GL_TRIANGLES);
	glColor3f(colorAdd(0.000, 'r'), colorAdd(0.000, 'g'), colorAdd(0.545, 'b'));
	glVertex2f(40, 0);
	glVertex2f(50, -10);
	glVertex2f(60, 0);
	glEnd();
}

void drawEye() {
	int radius = 5;
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(colorAdd(1.000, 'r'), colorAdd(0.549, 'g'), colorAdd(0.000, 'b'));
	glVertex2f(0, 0);
	
	// using a loop to get a circle
	for (int i = 0; i < 360; ++i) {
		glVertex2f(radius*cos(i), radius*sin(i));
	}
	glEnd();
}

void drawTeeth() {
	glBegin(GL_TRIANGLES);
	glColor3f(1.000, 1.000, 1.000);
	glVertex2f(0, 0);
	glVertex2f(5, 15);
	glVertex2f(10, 0);
	glEnd();
}

void drawHead() {
	// Draw the head first
	glBegin(GL_QUADS);
	
	glColor3f(colorAdd(0, 'r'), colorAdd(0.502, 'g'), colorAdd(0, 'b'));
	glVertex2f(0, 0);
	glVertex2f(60, 0);
	glVertex2f(60, 60);
	glVertex2f(0, 60);
	glEnd();
	
	// draw his hair
	drawHair();
	
	// draw his eyes
	
	// Right eye
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(eyePosition(15, 'x'), eyePosition(30, 'y'), 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawEye();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// left eye
	transMtx = glm::translate(glm::mat4(), glm::vec3(eyePosition(45, 'x'), eyePosition(30, 'y'), 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawEye();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw teeth
	transMtx = glm::translate(glm::mat4(), glm::vec3(5, 5, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawTeeth();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );	
	
	transMtx = glm::translate(glm::mat4(), glm::vec3(45, 5, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawTeeth();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );	
}

void drawArm() {
	// Arm
	glBegin(GL_QUADS);
	
	glColor3f(colorAdd(0, 'r'), colorAdd(0.502, 'g'), colorAdd(0, 'b'));
	glVertex2f(0, 0);
	glVertex2f(30, 0);
	glVertex2f(30, 100);
	glVertex2f(0, 100);
	glEnd();
	
	// draw shoulder
	glBegin(GL_QUADS);
	
	glColor3f(0.863, 0.078, 0.235);
	glVertex2f(0, 100);
	glVertex2f(30, 100);
	glVertex2f(30, 140);
	glVertex2f(0, 140);
	glEnd();
}

void drawLeg() {
	glBegin(GL_QUADS); {
		glColor3f(0.545, 0.271, 0.075);
		glVertex2f(0, 0);
		glVertex2f(30, 0);
		glVertex2f(30, 90);
		glVertex2f(0, 90);
	} glEnd();
}

void drawBody() {
	// Body
	glBegin(GL_QUADS); {
		glColor3f(0.863, 0.078, 0.235);
		glVertex2f(0, 0);
		glVertex2f(70, 0);
		glVertex2f(70, 100);
		glVertex2f(0, 100);
	} glEnd();
	
	//draw right arm
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(-30, -40, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawArm();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	//draw left arm
	transMtx = glm::translate(glm::mat4(), glm::vec3(70, -40, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawArm();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw right leg
	transMtx = glm::translate(glm::mat4(), glm::vec3(00, -90, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLeg();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	// draw left leg
	transMtx = glm::translate(glm::mat4(), glm::vec3(40, -90, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLeg();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
}

// Constant animation
void headSide() {
	if (HEAD_START == 185) HEAD_MOVE = -HEAD_MOVE;
	else if (HEAD_START == 215) HEAD_MOVE = -HEAD_MOVE;

	HEAD_START += HEAD_MOVE;
}

// Draw character
void DrawCharacter() {
	// Draw body before head
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(195 + ADDITIONAL_X, 100 + ADDITIONAL_Y, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawBody();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// Draw head
	transMtx = glm::translate(glm::mat4(), glm::vec3(HEAD_START + ADDITIONAL_X, 200 + ADDITIONAL_Y, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawHead();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
}


//
//	void renderScene()
//
//		This method will contain all of the objects to be drawn.
//
void renderScene() {
	  // draw our World Map to the screen.  this MUST be your first drawing call
    drawMap( WINDOW_WIDTH, WINDOW_HEIGHT );	// DO NOT REMOVE THIS LINE

    //////////////////////////////////////////////////////////
    /////					Add Your Drawing Here		 /////
    //////////////////////////////////////////////////////////
	DrawCharacter();
}

//*************************************************************************************
//
// Our main function

//
//	int main( int argc, char *argv[] )
//
//		Really you should know what this is by now.  We will make use of the parameters later
//
int main( int argc, char* argv[] ) {
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
										// GLFW sets up our OpenGL context so must be done first
	setupOpenGL();						// initialize all of the OpenGL specific information

	initMap();							// initialize our map

	//  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {
		glDrawBuffer( GL_BACK );		// ensure we are drawing to the back buffer
		glClear( GL_COLOR_BUFFER_BIT );	// clear the current color contents in the buffer
		headSide();

		// update the projection matrix based on the window size
		// the GL_PROJECTION matrix governs properties of the view coordinates;
		// i.e. what gets seen - use an Orthographic projection that ranges
		// from [0, windowWidth] in X and [0, windowHeight] in Y. (0,0) is the lower left.
		glm::mat4 projMtx = glm::ortho( 0.0f, (GLfloat)WINDOW_WIDTH, 0.0f, (GLfloat)WINDOW_HEIGHT );
		glMatrixMode( GL_PROJECTION );	// change to the Projection matrix
		glLoadIdentity();				// set the matrix to be the identity
		glMultMatrixf( &projMtx[0][0] );// load our orthographic projection matrix into OpenGL's projection matrix state

		// Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
		// when using a Retina display the actual window can be larger than the requested window.  Therefore
		// query what the actual size of the window we are rendering to is.
		GLint framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize( window, &framebufferWidth, &framebufferHeight );

		// update the viewport - tell OpenGL we want to render to the whole window
		glViewport( 0, 0, framebufferWidth, framebufferHeight );

		glMatrixMode( GL_MODELVIEW );	// make the ModelView matrix current to be modified by any transformations
		glLoadIdentity();				// set the matrix to be the identity

		renderScene();					// draw everything to the window

		glfwSwapBuffers(window);		// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

	return 0;
}
