/*
 *  CSCI 441, Computer Graphics, Fall 2017
 *
 *  Project: lab02
 *  File: main.cpp
 *
 *	Author: Dr. Jeffrey Paone - Fall 2015
 *	Modified: Michael Villafuerte - Fall 2017 for GLFW
 *
 *  Description:
 *      Contains the base code for a basic flight simulator.
 *
 */

// include the OpenGL library header
#ifdef __APPLE__					// if compiling on Mac OS
 	#include <OpenGL/gl.h>
#else										// if compiling on Linux or Windows OS
 	#include <GL/gl.h>
#endif

#include <GLFW/glfw3.h>	// include GLFW framework header

#include <CSCI441/objects.hpp> // for our 3D objects

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>				// for cos(), sin() functionality
#include <stdio.h>			// for printf functionality
#include <stdlib.h>			// for exit functionality
#include <time.h>			  // for time() functionality

//*************************************************************************************
//
// Global Parameters

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 640, windowHeight = 480;

int leftMouseButton;    	 									// status of the mouse button
glm::vec2 mousePos;			              		  // last known X and Y of the mouse

glm::vec3 camPos;            							 	// camera position in cartesian coordinates
float cameraTheta, cameraPhi;               // camera DIRECTION in spherical coordinates
glm::vec3 camDir; 			                    // camera DIRECTION in cartesian coordinates

GLuint environmentDL;                       // display list for the 'city'

//*************************************************************************************
//
// Helper Functions

// getRand() ///////////////////////////////////////////////////////////////////
//
//  Simple helper function to return a random number between 0.0f and 1.0f.
//
////////////////////////////////////////////////////////////////////////////////
float getRand() { return rand() / (float)RAND_MAX; }

// recomputeOrientation() //////////////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based
//  on its position in spherical coordinates. Should be called every time
//  cameraTheta or cameraPhi is updated.
//
////////////////////////////////////////////////////////////////////////////////
void recomputeOrientation() {
    // TODO #5: Convert spherical coordinates into a cartesian vector
    // see Wednesday's slides for equations.  Extra Hint: Slide #70
	camDir = glm::vec3(sin((double)cameraTheta) * sin((double)cameraPhi), -cos((double)cameraPhi), -cos((double)cameraTheta) * sin((double)cameraPhi));

    // and NORMALIZE this directional vector!!!
	camDir = glm::normalize(camDir);

}

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

static void keyboard_callback( GLFWwindow *window, int key, int scancode, int action, int mods ) {
	if( action == GLFW_PRESS ||  action == GLFW_REPEAT) {
		switch( key ) {
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				exit(EXIT_SUCCESS);
			case GLFW_KEY_W:
				camPos = camPos + camDir*((float)2);
				break;
			case GLFW_KEY_S:
				camPos = camPos - camDir*((float)2);
				break;
		}
	}
}

// cursor_callback() ///////////////////////////////////////////////////////////
//
//  GLFW callback for mouse movement. We update cameraPhi and/or cameraTheta
//      based on how much the user has moved the mouse in the
//      X or Y directions (in screen space) and whether they have held down
//      the left or right mouse buttons. If the user hasn't held down any
//      buttons, the function just updates the last seen mouse X and Y coords.
//
////////////////////////////////////////////////////////////////////////////////
static void cursor_callback( GLFWwindow *window, double x, double y ) {
	if( leftMouseButton == GLFW_PRESS ) {
			cameraTheta = cameraTheta + 0.005*(x - mousePos.x);
			cameraPhi = cameraPhi + 0.005*(mousePos.y - y);
			if (cameraPhi > M_PI) cameraPhi = M_PI-0.00001;
			else if (cameraPhi < 0) cameraPhi = 0.00001;
			recomputeOrientation();     // update camera (x,y,z) based on (theta,phi)
	}

	mousePos.x = x;
	mousePos.y = y;
}

// mouse_button_callback() /////////////////////////////////////////////////////
//
//  GLFW callback for mouse clicks. We save the state of the mouse button
//      when this is called so that we can check the status of the mouse
//      buttons inside the motion callback (whether they are up or down).
//
////////////////////////////////////////////////////////////////////////////////
static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods ) {
	if( button == GLFW_MOUSE_BUTTON_LEFT ) {
		leftMouseButton = action;
	}
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

// drawGrid() //////////////////////////////////////////////////////////////////
//
//  Function to draw a grid in the XZ-Plane using OpenGL 2D Primitives (GL_LINES)
//
////////////////////////////////////////////////////////////////////////////////
void drawGrid() {
    /*
     *	We will get to why we need to do this when we talk about lighting,
     *	but for now whenever we want to draw something with an OpenGL
     *	Primitive - like a line, quad, point - we need to disable lighting
     *	and then reenable it for use with the CSCI441 3D Objects.
     */
    glDisable( GL_LIGHTING );

    /** TODO #3: DRAW A GRID IN THE XZ-PLANE USING GL_LINES **/
	
	// range from -50 to 50 in the x and z, seperate by 10
	// The spacing of the lines
	int n = 1;
	for (float x = -50; x < 50; x+=n) {
		for (float z = -50; z < 50; z+=n) { // we'll have to draw the squares
			glColor3f(1, 1, 1);
			glBegin(GL_LINES);
			// pair one
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+n);
			
			// pair two
			glVertex3f(x, 0, z+n);
			glVertex3f(x+n, 0, z+n);
			
			// pair three
			glVertex3f(x+n, 0, z+n);
			glVertex3f(x+n, 0, z);
			
			// pair four
			glVertex3f(x+n, 0, z);
			glVertex3f(x, 0, z);
			
			glEnd();
		}
	}

    /*
     *	As noted above, we are done drawing with OpenGL Primitives, so we
     *	must turn lighting back on.
     */
    glEnable( GL_LIGHTING );
}

// drawCity() //////////////////////////////////////////////////////////////////
//
//  Function to draw a random city using CSCI441 3D Cubes
//
////////////////////////////////////////////////////////////////////////////////
void drawCity() {
    // TODO #4: Randomly place buildings of varying heights with random colors
	
	// The spacing of the lines
	int n = 1; 
	for (int x = -50; x < 50; x+=n) {
		for (int z = -50; z < 50; z+=n) { // we'll have to draw the squares
			if (x%2 == 0 && z%2 == 0 && getRand() < 0.4) {
				int h = (rand() % 10) + 1;
				int tranH = h/2;
				glm::mat4 transCube = glm::translate( glm::mat4(), glm::vec3( x, tranH, z ) );
				glMultMatrixf( &transCube[0][0] ); {
					glm::mat4 scaleTri = glm::scale( glm::mat4(), glm::vec3( 1.0f, (float)h, 1.0f ) );
					glMultMatrixf( &scaleTri[0][0] ); {
						glColor3f(getRand(), getRand(), getRand());
						CSCI441::drawSolidCube(1);
					}; glMultMatrixf( &(glm::inverse( scaleTri ))[0][0] );
				}; glMultMatrixf( &(glm::inverse( transCube ))[0][0] );
				
			}
			//glBegin(GL_LINES);
			
			
			//glEnd();
		}
	}
}

// generateEnvironmentDL() /////////////////////////////////////////////////////
//
//  This function creates a display list with the code to draw a simple
//      environment for the user to navigate through.
//
//  And yes, it uses a global variable for the display list.
//  I know, I know! Kids: don't try this at home. There's something to be said
//      for object-oriented programming after all.
//
////////////////////////////////////////////////////////////////////////////////
void generateEnvironmentDL() {
    // TODO #1 Create a Display List & Call our Drawing Functions
	environmentDL = glGenLists(1);
	
	glNewList( environmentDL, GL_COMPILE);
	
	drawCity();
	drawGrid();
	
	glEndList();

}

//
//	void renderScene()
//
//		This method will contain all of the objects to be drawn.
//
void renderScene(void)  {
    // TODO #2: REMOVE TEAPOT & CREATE A CITY SCENE ON A GRID...but call it's display list!
	glCallList( environmentDL );
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
	GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "Flight Simulator v 0.31", NULL, NULL );
	if( !window ) {						// if the window could not be created, NULL is returned
		fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	glfwMakeContextCurrent(window);		// make the created window the current window
	glfwSwapInterval(1);				     	// update our screen after at least 1 screen refresh

	glfwSetKeyCallback( window, keyboard_callback );							// set our keyboard callback function
	glfwSetCursorPosCallback( window, cursor_callback );					// set our cursor position callback function
	glfwSetMouseButtonCallback( window, mouse_button_callback );	// set our mouse button callback function

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
	// tell OpenGL to perform depth testing with the Z-Buffer to perform hidden
	//		surface removal.  We will discuss this more very soon.
  glEnable( GL_DEPTH_TEST );

	//******************************************************************
  // this is some code to enable a default light for the scene;
  // feel free to play around with this, but we won't talk about
  // lighting in OpenGL for another couple of weeks yet.
  float lightCol[4] = { 1, 1, 1, 1};
  float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
  float lPosition[4] = { 10, 10, 10, 1 };
  glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
  glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
  glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
  glEnable( GL_LIGHTING );
  glEnable( GL_LIGHT0 );

  // tell OpenGL not to use the material system; just use whatever we
	// pass with glColor*()
  glEnable( GL_COLOR_MATERIAL );
  glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	//******************************************************************

  // tells OpenGL to blend colors across triangles. Once lighting is
  // enabled, this means that objects will appear smoother - if your object
  // is rounded or has a smooth surface, this is probably a good idea;
  // if your object has a blocky surface, you probably want to disable this.
  glShadeModel( GL_SMOOTH );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// set the clear color to black
}

//
//  void setupScene()
//
//      Used to setup everything scene related.  Give our camera an
//	initial starting point and generate the display list for our city
//
void setupScene() {
	// give the camera a scenic starting point.
	camPos.x = 60;
	camPos.y = 40;
	camPos.z = 30;
	cameraTheta = -M_PI / 3.0f;
	cameraPhi = M_PI / 2.8f;
	recomputeOrientation();

	srand( time(NULL) );	// seed our random number generator
	generateEnvironmentDL();
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
	// GLFW sets up our OpenGL context so must be done first
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
	setupOpenGL();										// initialize all of the OpenGL specific information
	setupScene();											// initialize objects in our scene

	//  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {	// check if the window was instructed to be closed
    glDrawBuffer( GL_BACK );				// work with our back frame buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

		// update the projection matrix based on the window size
		// the GL_PROJECTION matrix governs properties of the view coordinates;
		// i.e. what gets seen - use a perspective projection that ranges
		// with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
		glm::mat4 projMtx = glm::perspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.001f, 1000.0f );
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
		glLoadIdentity();							// set the matrix to be the identity

		// set up our look at matrix to position our camera
		// TODO #6: Change how our lookAt matrix gets constructed
		glm::mat4 viewMtx = glm::lookAt( camPos,		// camera is located at (10, 10, 10)
								 										 camPos+camDir,		// camera is looking at (0, 0, 0,)
							 	 									 	 glm::vec3(  0,  1,  0 ) );	// up vector is (0, 1, 0) - positive Y
		// multiply by the look at matrix - this is the same as our view martix
		glMultMatrixf( &viewMtx[0][0] );

		renderScene();					// draw everything to the window

		glfwSwapBuffers(window);// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

	glfwDestroyWindow( window );// clean up and close our window
	glfwTerminate();						// shut down GLFW to clean up our context

	return EXIT_SUCCESS;				// exit our program successfully!
}
