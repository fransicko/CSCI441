/*
 *  CSCI 441, Computer Graphics, Fall 2017
 *
 *  Project: lab01
 *  File: main.cpp
 *
 *	Author: Dr. Jeffrey Paone - Fall 2017
 *
 *  Description:
 *      Contains the code for a simple interactive and animated example
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
 #include <glm/gtx/transform.hpp>

 #include <stdio.h>				// for printf functionality
 #include <stdlib.h>			// for exit functionality

 //*************************************************************************************
 //
 // Global Parameters

 // global variables to keep track of window width and height.
 // set to initial values for convenience, but we need variables
 // for later on in case the window gets resized.
int windowWidth = 512, windowHeight = 512;

float triforceAngle = 20.0f;
bool EVIL_TRIFORCE = false;
double xPosition = 100.0, yPostion = 100.0;

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

/** TODO #1 **/

//*************************************************************************************
//
// Setup Functions
void keyboard_callback( GLFWwindow *win, int key, int scancode, int action, int mods ) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		exit(EXIT_SUCCESS);
	}
	
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		EVIL_TRIFORCE = !(EVIL_TRIFORCE);
	}
}

void mouse_button_callback( GLFWwindow *window, int button, int action, int mods ) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			EVIL_TRIFORCE = true;
		}
		else if (action == GLFW_RELEASE) {
			EVIL_TRIFORCE = false;
		}
	}
}

void cursor_callback( GLFWwindow *window, double x, double y ) {
	xPosition = x;
	yPostion = windowHeight - y;
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
	if( !glfwInit() ) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );	// request OpenGL v2.X
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );	// request OpenGL v2.1
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );		// do not allow our window to be able to be resized
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	
	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "Lab01", NULL, NULL );
	if( !window ) {						// if the window could not be created, NULL is returned
		fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	// TODO #2
	glfwSetKeyCallback( window, keyboard_callback);
	glfwSetMouseButtonCallback( window, mouse_button_callback );
	glfwSetCursorPosCallback( window, cursor_callback );
	glfwMakeContextCurrent(window);		// make the created window the current window
	glfwSwapInterval(1);				     // update our screen after at least 1 screen refresh

	return window;						       // return the window that was created
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

//
//  void drawTriangle()
//
//		Issues a series of OpenGL commands to draw a triangle
//
void drawTriangle() {
	// tell OpenGL we want to draw triangles
	glBegin( GL_TRIANGLES ); {
		glVertex2f( -2.5f, -2.0f );		// lower left corner
		glVertex2f(  2.5f, -2.0f );		// lower right corner
		glVertex2f(  0.0f,  2.0f );		// top corner
	}; glEnd();	// tell OpenGL we are done drawing triangles
}

//
//  void drawTriforce()
//
//      Issues a series of OpenGL commands to draw three triangles in a
//  triangle shape that all fit inside a larger triangle.
//
void drawTriforce() {
	// make all the vertices be a nice, golden color.
	if (EVIL_TRIFORCE) {
		glColor3f( 1, 0.0, 0.0 );
	}
	else {
		glColor3f( 0.9, 0.8, 0.1 );
	}

  // the triangle will get rendered around the origin,
  // so move the origin to (-2.5, -2.0)
  glm::mat4 transTri = glm::translate( glm::mat4(), glm::vec3( -2.5f, -2.0f, 0.0f ) );
  glMultMatrixf( &transTri[0][0] ); {
		// draw the triangle
		drawTriangle();
	}; glMultMatrixf( &(glm::inverse( transTri ))[0][0] );

	// the triangle will get rendered around the origin,
	// so move the origin to (2.5, -2.0)
  transTri = glm::translate( glm::mat4(), glm::vec3( 2.5f, -2.0f, 0.0f ) );
  glMultMatrixf( &transTri[0][0] ); {
		// draw the triangle
		drawTriangle();
	}; glMultMatrixf( &(glm::inverse( transTri ))[0][0] );

  // the triangle will get rendered around the origin,
	// so move the origin to (0.0, 2.0)
  transTri = glm::translate( glm::mat4(), glm::vec3( 0.0f, 2.0f, 0.0f ) );
  glMultMatrixf( &transTri[0][0] ); {
		// draw the triangle
		drawTriangle();
	}; glMultMatrixf( &(glm::inverse( transTri ))[0][0] );
}

//
//	void renderScene()
//
//		This method will contain all of the objects to be drawn.
//
void renderScene() {
  // the triforce will get rendered around the origin,
	// so move the origin to (200,300)...
  glm::mat4 transTri = glm::translate( glm::mat4(), glm::vec3( xPosition, yPostion, 0.0f ) );
  glMultMatrixf( &transTri[0][0] ); {
    // and then rotate it by 45 degrees
    glm::mat4 rotTri = glm::rotate( glm::mat4(), triforceAngle, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    glMultMatrixf( &rotTri[0][0] ); {
  		// and then scale it 10X in x and 10X in y
      glm::mat4 scaleTri = glm::scale( glm::mat4(), glm::vec3( 10.0f, 10.0f, 1.0f ) );
      glMultMatrixf( &scaleTri[0][0] ); {
    		// finally send OpenGL a series of instructions that render our triforce
    		drawTriforce();
      }; glMultMatrixf( &(glm::inverse( scaleTri ))[0][0] );
    }; glMultMatrixf( &(glm::inverse( rotTri ))[0][0] );
  }; glMultMatrixf( &(glm::inverse( transTri ))[0][0] );
}

///*************************************************************************************
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
    // TODO #3
		glClear( GL_COLOR_BUFFER_BIT );	// clear the current color contents in the window

		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT);
		triforceAngle = triforceAngle + 0.05f;
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

		glfwSwapBuffers(window);// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

	glfwDestroyWindow( window );		// clean up and close our window
	glfwTerminate();					// shut down GLFW to clean up our context

	return EXIT_SUCCESS;				// exit our program successfully!
}
