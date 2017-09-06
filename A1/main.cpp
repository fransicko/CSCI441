/*
 *  CSCI 441, Computer Graphics, Fall 2017
 *
 *  Project: A1
 *  File: main.cpp
 *
 *	Author: Michael Villafuerte - Fall 2017
 f
 *
 *  Description:
 *      this is my heros sign
 *
 */

// include the OpenGL library header
#ifdef __APPLE__				// if compiling on Mac OS
	#include <OpenGL/gl.h>
#else							// if compiling on Linux or Windows OS
	#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>			// include GLFW framework header

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>				// for printf functionality

//*************************************************************************************
//
// Global Parameters

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 700, windowHeight = 150;

int windowId;					// global variable to keep track of the window id

//*************************************************************************************
//
// Event Callbacks

//
//	void error_callback( int error, const char* description )
//
//		We will register this function as GLFW's error callback.
//	When an error within GLFW occurs, GLFW will tell us by calling
//	this function.  We can then print this info to the terminal to
//	alert the user.
//
static void error_callback( int error, const char* description ) {
	fprintf( stderr, "[ERROR]: %s\n", description );
}

//*************************************************************************************
//
// Setup Functions

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
	if( !glfwInit() ) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );	// request OpenGL v2.X
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );	// request OpenGL v2.1
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );		// do not allow our window to be able to be resized

	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "Lab00A", NULL, NULL );
	if( !window ) {						// if the window could not be created, NULL is returned
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

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
// 645 is 64 is allowed for each letter

void drawLetterF() {
	//draws the F line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(32, 0);
	glVertex2f(32, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the top line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 90);
	glVertex2f(64, 90);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the second line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 40);
	glVertex2f(64, 40);
	glVertex2f(64, 75);
	glVertex2f(0, 75);
	glEnd();
}

void drawLetterR() {
	//draws the R line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(20, 0);
	glVertex2f(20, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the top line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 100);
	glVertex2f(64, 100);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the second line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 40);
	glVertex2f(64, 40);
	glVertex2f(64, 70);
	glVertex2f(0, 70);
	glEnd();
	
	//Draws the second vertical line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(48, 100);
	glVertex2f(64, 100);
	glVertex2f(64, 70);
	glVertex2f(48, 70);
	glEnd();
	
	// The second R line
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(32, 0);
	glVertex2f(48, 0);
	glVertex2f(48, 40);
	glVertex2f(32, 40);
	glEnd();	
}

void drawLetterA() {
	// draws the outline of the A
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// the center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 0);
	glVertex2f(48, 0);
	glVertex2f(48, 55);
	glVertex2f(16, 55);
	glEnd();	
	
	// the center box
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 65);
	glVertex2f(48, 65);
	glVertex2f(48, 100);
	glVertex2f(16, 100);
	glEnd();		
}

void drawLetterN() {
	// draws the outline of the N
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// the center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 0);
	glVertex2f(48, 0);
	glVertex2f(48, 100);
	glVertex2f(16, 100);
	glEnd();		
}

void drawLetterS() {
	// draws the outline of the S
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the top line
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 70);
	glVertex2f(64, 70);
	glVertex2f(64, 100);
	glVertex2f(16, 100);
	glEnd();
	
	// draws the second line
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(0, 16);
	glVertex2f(48, 16);
	glVertex2f(48, 40);
	glVertex2f(0, 40);
	glEnd();	
}

void drawLetterI() {
	// draws the outline of the I
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the left space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(0, 16);
	glVertex2f(24, 16);
	glVertex2f(24, 100);
	glVertex2f(0, 100);
	glEnd();
	
	// draws the right space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(40, 16);
	glVertex2f(64, 16);
	glVertex2f(64, 100);
	glVertex2f(40, 100);
	glEnd();	
}

void drawLetterC() { 
	// draws the outline of the C
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 16);
	glVertex2f(48, 16);
	glVertex2f(48, 100);
	glVertex2f(16, 100);
	glEnd();
	
	// draws the opening
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(48, 32);
	glVertex2f(64, 32);
	glVertex2f(64, 64);
	glVertex2f(48, 64);
	glEnd();	
}

void drawLetterK() { 
	// draws the outline of the K
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// the bottom center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 0);
	glVertex2f(36, 0);
	glVertex2f(36, 70);
	glVertex2f(16, 70);
	glEnd();
	
	// the top center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 95);
	glVertex2f(36, 95);
	glVertex2f(36, 120);
	glVertex2f(16, 120);
	glEnd();	
	
	// draw teh center blank
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(46, 40);
	glVertex2f(64, 40);
	glVertex2f(64, 90);
	glVertex2f(46, 90);
	glEnd();		
	
}

void drawLetterO() {
	// draws the outline of the 0
	glBegin(GL_QUADS);
	
	glColor3f(0.529, 0.808, 0.922);
	glVertex2f(0, 0);
	glVertex2f(64, 0);
	glVertex2f(64, 120);
	glVertex2f(0, 120);
	glEnd();
	
	// draws the center space
	glBegin(GL_QUADS);
	
	glColor3f(0, 0, 0);
	glVertex2f(16, 16);
	glVertex2f(48, 16);
	glVertex2f(48, 100);
	glVertex2f(16, 100);
	glEnd();
}

void drawMyTriangle() {
	glBegin(GL_TRIANGLES);
	
	glColor3f(0.373, 0.620, 0.627);
	glVertex2f(0, 0);
	glVertex2f(34, 0);
	glVertex2f(17, 40);
	glEnd();
}

void drawMyCrest() {
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(14, 80, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawMyTriangle();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// The center piece
	transMtx = glm::translate(glm::mat4(), glm::vec3(14, 40, 0));
	glMultMatrixf(&transMtx[0][0]); 
	glm::mat4 scaleMtx = glm::scale(glm::mat4(), glm::vec3(0.531, 0.333, 1));
	glMultMatrixf( &scaleMtx[0][0] );{
		glColor3f(1.000, 0.549, 0.000);
		drawLetterO();
	}; glMultMatrixf( &(glm::inverse( scaleMtx ))[0][0] );
	glMultMatrixf( &(glm::inverse( transMtx ))[0][0] ); 
	
	// Bottom triangle rotated
	transMtx = glm::translate(glm::mat4(), glm::vec3(48, 40, 0));
	glMultMatrixf(&transMtx[0][0]); 
	glm::mat4 rotMtx = glm::rotate( glm::mat4(), 160.225f, glm::vec3(0, 0, 1) ); 
	glMultMatrixf( &rotMtx[0][0] );{
		drawMyTriangle();
	}; glMultMatrixf( &(glm::inverse( rotMtx ))[0][0] );
	glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// left line
	transMtx = glm::translate(glm::mat4(), glm::vec3(0, 46, 0));
	glMultMatrixf(&transMtx[0][0]); {
		glBegin(GL_QUADS);
	
		glColor3f(0.000, 0.545, 0.545);
		glVertex2f(0, 0);
		glVertex2f(10, 0);
		glVertex2f(10, 26);
		glVertex2f(0, 26);
		glEnd();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	
	// right line
	transMtx = glm::translate(glm::mat4(), glm::vec3(52, 46, 0));
	glMultMatrixf(&transMtx[0][0]); {
		glBegin(GL_QUADS);
	
		glColor3f(0.180, 0.545, 0.341);
		glVertex2f(0, 0);
		glVertex2f(10, 0);
		glVertex2f(10, 26);
		glVertex2f(0, 26);
		glEnd();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	
	
}

// The max width allowed for each plus the space
int space(int i) {
	return (70 * i) + 5;
}
//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

//
//	void renderScene()
//
//		This method will contain all of the objects to be drawn.
//
void renderScene() {
	// This is for some easyer translations
	int i = 0;
	
	// draw F
	glm::mat4 transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterF();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw R
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterR();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw A
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterA();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw N
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterN();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw S
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterS();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );
	
	// draw I
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterI();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );	
	
	// draw C
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterC();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );		
	
	// draw K
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterK();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );		
	
	// draw O
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawLetterO();
		++i;
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );		
	

	
	// Draw Crest
	transMtx = glm::translate(glm::mat4(), glm::vec3(space(i), 10, 0));
	glMultMatrixf(&transMtx[0][0]); {
		drawMyCrest();
	}; glMultMatrixf( &(glm::inverse( transMtx ))[0][0] );		

}

//glm::mat4 tMtx = glm::translate( glm::mat4 mMtx, glm::vec3( float x, float y, float z ));

//*************************************************************************************
//
// Our main function

//
//	int main( int argc, char *argv[] )
//
//		Really you should know what this is by now.  We will make use of the parameters later
//
int main( int argc, char *argv[] ) {
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
										// GLFW sets up our OpenGL context so must be done first
	setupOpenGL();						// initialize all of the OpenGL specific information

	//  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {	// check if the window was instructed to be closed
		glClear( GL_COLOR_BUFFER_BIT );	// clear the current color contents in the window

		// update the projection matrix based on the window size
		// the GL_PROJECTION matrix governs properties of the view coordinates;
		// i.e. what gets seen - use an Orthographic projection that ranges
		// from [0, windowWidth] in X and [0, windowHeight] in Y. (0,0) is the lower left.
		glm::mat4 projMtx = glm::ortho( 0.0f, (GLfloat)windowWidth, 0.0f, (GLfloat)windowHeight );
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

	glfwDestroyWindow( window );		// clean up and close our window
	glfwTerminate();					// shut down GLFW to clean up our context

	return EXIT_SUCCESS;				// exit our program successfully!
}
