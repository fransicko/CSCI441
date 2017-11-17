/*
 *  CSCI 441, Computer Graphics, Fall 2017
 *
 *  Project: lab09
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the basic setup to work with GLSL shaders.
 *
 *  Author: Dr. Paone, Colorado School of Mines, 2017
 *
 */

//*************************************************************************************

// LOOKHERE #2
#include <GL/glew.h>        // include GLEW to get our OpenGL 3.0+ bindings
#include <GLFW/glfw3.h>			// include GLFW framework header

// include GLM libraries and matrix functions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL/SOIL.h>		// for image loading
#include <stdio.h>				// for printf functionality
#include <stdlib.h>				// for exit functionality
#include <time.h>					// for time functionality

// note that all of these headers end in *3.hpp
// these class library files will only work with OpenGL 3.0+
#include <CSCI441/modelLoader3.hpp> // to load in OBJ models
#include <CSCI441/OpenGLUtils3.hpp> // to print info about OpenGL
#include <CSCI441/objects3.hpp>     // to render our 3D primitives
#include <CSCI441/ShaderProgram3.hpp>   // our shader helper functions
#include <CSCI441/TextureUtils.hpp>   // our texture helper functions

#include "include/Shader_Utils.h"   // our shader helper functions
#include "include/Particle.h"

//*************************************************************************************
//
// Global Parameters

int windowWidth, windowHeight;

bool controlDown = false;
bool leftMouseDown = false;
glm::vec2 mousePosition( -9999.0f, -9999.0f );

glm::vec3 cameraAngles( 1.82f, 2.01f, 15.0f );
glm::vec3 eyePoint(   10.0f, 10.0f, 10.0f );
glm::vec3 lookAtPoint( 0.0f,  0.0f,  0.0f );
glm::vec3 upVector(    0.0f,  1.0f,  0.0f );

int objectIndex = 2;

// A6 globals for spawn location, angle of launch
glm::vec3 spawn( 0.0f,  2.0f,  0.0f ); // TODO #A6 This will need to change later
float angle = M_PI / 2;
CSCI441::ModelLoader* model = NULL;
double timeI;

GLuint shaderProgramHandle = 0;
GLint mvp_uniform_location = -1, time_uniform_location = -1;
GLint vpos_attrib_location = -1, text_coor_location = -1;

// Lighting uniforms
GLint lightDiff = -1;
GLint lightAmb = -1;
GLint lightSpec = -1;
GLint matDiff = -1;
GLint matAmb = -1;
GLint matSpec = -1;
GLint lightPos = -1;
GLint camPos = -1;
GLint shine = -1;
GLint normalHandle = -1;



GLuint objectProgramHandle = 0;
GLint obj_mvp_uniform_location = -1, obj_time_uniform_location = -1;
GLint obj_vpos_attrib_location = -1, obj_text_coor_location = -1;

GLuint platformTextureHandle, frontTextureHandle, backTextureHandle, ceilTextureHandle, leftTextureHandle, rightTextureHandle;
GLuint vaods;
GLuint vaodCeil;
GLuint vaodFloor;
GLuint vaodFront;
GLuint vaodBack;
GLuint vaodRight;
GLuint vaodLeft;
GLuint vbod;

// Lab 10
CSCI441::ShaderProgram *snowShaderProgram = NULL;
GLint snow_modelview_uniform_location, snow_projection_uniform_location;
GLint snow_vpos_attrib_location;

const int NUM_POINTS = 3;
struct Vertex { GLfloat x, y, z; };
Vertex points[NUM_POINTS];
Particle part[NUM_POINTS];
GLuint pointsVAO, pointsVBO;

GLuint textureHandle;

//******************************************************************************
//
// Helper Functions

// convertSphericalToCartesian() ///////////////////////////////////////////////
//
// This function updates the camera's position in cartesian coordinates based
//  on its position in spherical coordinates. Should be called every time
//  cameraAngles is updated.
//
////////////////////////////////////////////////////////////////////////////////
void convertSphericalToCartesian() {
	eyePoint.x = cameraAngles.z * sinf( cameraAngles.x ) * sinf( cameraAngles.y );
	eyePoint.y = cameraAngles.z * -cosf( cameraAngles.y );
	eyePoint.z = cameraAngles.z * -cosf( cameraAngles.x ) * sinf( cameraAngles.y );
}

GLfloat randNumber( int max ) {
	//return rand() / (GLfloat)RAND_MAX * max * 2.0 - max;
	return rand()%((max - 3) + 1) + 3;
}

// loadAndRegisterTexture() ////////////////////////////////////////////////////
//
// Load and register a texture with OpenGL
//
////////////////////////////////////////////////////////////////////////////////
GLuint loadAndRegisterTexture( const char *filename ) {
	GLuint texHandle = SOIL_load_OGL_texture( filename,
																					 SOIL_LOAD_AUTO,
																					 SOIL_CREATE_NEW_ID,
																					 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT );

	if( texHandle == 0 ) {
			printf( "[ERROR]: Could not load texture \"%s\"\n[SOIL]: %s\n", filename, SOIL_last_result() );
	} else {
			printf( "[INFO]: Successfully loaded texture \"%s\"\n[SOIL]: %s\n", filename, SOIL_last_result() );
			glBindTexture(   GL_TEXTURE_2D,  texHandle );
			glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     GL_REPEAT );
	}

	return texHandle;
}

//******************************************************************************
//
// Event Callbacks

// error_callback() ////////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's error callback.
//	When an error within GLFW occurs, GLFW will tell us by calling
//	this function.  We can then print this info to the terminal to
//	alert the user.
//
////////////////////////////////////////////////////////////////////////////////
static void error_callback(int error, const char* description) {
	fprintf(stderr, "[ERROR]: %s\n", description);
}

// key_callback() //////////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's keypress callback.
//	Responds to key presses and key releases
//
////////////////////////////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == 'Q') && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

  if( action == GLFW_PRESS ) {
    switch( key ) {
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, GLFW_TRUE );
        break;

      case GLFW_KEY_1:
      case GLFW_KEY_2:
      case GLFW_KEY_3:
      case GLFW_KEY_4:
      case GLFW_KEY_5:
      case GLFW_KEY_6:
      case GLFW_KEY_7:
        objectIndex = key - 49; // 49 is GLFW_KEY_1.  they go in sequence from there
        break;
    }
  }
}

// mouse_button_callback() /////////////////////////////////////////////////////
//
//		We will register this function as GLFW's mouse button callback.
//	Responds to mouse button presses and mouse button releases.  Keeps track if
//	the control key was pressed when a left mouse click occurs to allow
//	zooming of our arcball camera.
//
////////////////////////////////////////////////////////////////////////////////
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
		leftMouseDown = true;
	} else {
		leftMouseDown = false;
		mousePosition.x = -9999.0f;
		mousePosition.y = -9999.0f;
	}
  controlDown = mods & GLFW_MOD_CONTROL;
}

// cursor_callback() ///////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's cursor movement callback.
//	Responds to mouse movement.  When active motion is used with the left
//	mouse button an arcball camera model is followed.
//
////////////////////////////////////////////////////////////////////////////////
static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	// make sure movement is in bounds of the window
	// glfw captures mouse movement on entire screen
	if( xpos > 0 && xpos < windowWidth ) {
		if( ypos > 0 && ypos < windowHeight ) {
			// active motion
			if( leftMouseDown ) {
				if( (mousePosition.x - -9999.0f) < 0.001f ) {
					mousePosition.x = xpos;
					mousePosition.y = ypos;
				} else {
					if( !controlDown ) {
						cameraAngles.x += (xpos - mousePosition.x)*0.005f;
						cameraAngles.y += (ypos - mousePosition.y)*0.005f;

						if( cameraAngles.y < 0 ) cameraAngles.y = 0.0f + 0.001f;
						if( cameraAngles.y >= M_PI ) cameraAngles.y = M_PI - 0.001f;
					} else {
						double totChgSq = (xpos - mousePosition.x) + (ypos - mousePosition.y);
						cameraAngles.z += totChgSq*0.01f;

						if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
						if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;
					}
					convertSphericalToCartesian();

					mousePosition.x = xpos;
					mousePosition.y = ypos;
				}
			}
			// passive motion
			else {

			}
		}
	}
}

// scroll_callback() ///////////////////////////////////////////////////////////
//
//		We will register this function as GLFW's scroll wheel callback.
//	Responds to movement of the scroll where.  Allows zooming of the arcball
//	camera.
//
////////////////////////////////////////////////////////////////////////////////
static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset ) {
	double totChgSq = yOffset;
	cameraAngles.z += totChgSq*0.01f;

	if( cameraAngles.z <= 2.0f ) cameraAngles.z = 2.0f;
	if( cameraAngles.z >= 50.0f ) cameraAngles.z = 50.0f;

	convertSphericalToCartesian();
}

//******************************************************************************
//
// Setup Functions




// setupGLFW() /////////////////////////////////////////////////////////////////
//
//		Used to setup everything GLFW related.  This includes the OpenGL context
//	and our window.
//
////////////////////////////////////////////////////////////////////////////////
GLFWwindow* setupGLFW() {
	// set what function to use when registering errors
	// this is the ONLY GLFW function that can be called BEFORE GLFW is initialized
	// all other GLFW calls must be performed after GLFW has been initialized
	glfwSetErrorCallback(error_callback);

	// initialize GLFW
	if (!glfwInit()) {
		fprintf( stderr, "[ERROR]: Could not initialize GLFW\n" );
		exit(EXIT_FAILURE);
	} else {
		fprintf( stdout, "[INFO]: GLFW initialized\n" );
	}

	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );						// request forward compatible OpenGL context
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );	// request OpenGL Core Profile context
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );		// request OpenGL 3.x context
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );		// request OpenGL 3.3 context

	// create a window for a given size, with a given title
	GLFWwindow *window = glfwCreateWindow(640, 480, "Lab09: GLSL Shaders", NULL, NULL);
	if( !window ) {						// if the window could not be created, NULL is returned
		fprintf( stderr, "[ERROR]: GLFW Window could not be created\n" );
		glfwTerminate();
		exit( EXIT_FAILURE );
	} else {
		fprintf( stdout, "[INFO]: GLFW Window created\n" );
	}

	glfwMakeContextCurrent(	window );	// make the created window the current window
	glfwSwapInterval( 1 );				    // update our screen after at least 1 screen refresh

	glfwSetKeyCallback( 			  window, key_callback				  );	// set our keyboard callback function
	glfwSetMouseButtonCallback( window, mouse_button_callback );	// set our mouse button callback function
	glfwSetCursorPosCallback(	  window, cursor_callback  			);	// set our cursor position callback function
	glfwSetScrollCallback(			window, scroll_callback			  );	// set our scroll wheel callback function

	return window;										// return the window that was created
}

// setupOpenGL() ///////////////////////////////////////////////////////////////
//
//      Used to setup everything OpenGL related.
//
////////////////////////////////////////////////////////////////////////////////
void setupOpenGL() {
	glEnable( GL_DEPTH_TEST );					// enable depth testing
	glDepthFunc( GL_LESS );							// use less than depth test

	glEnable(GL_BLEND);									// enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// use one minus blending equation

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black
}

// setupGLEW() /////////////////////////////////////////////////////////////////
//
//      Used to initialize GLEW
//
////////////////////////////////////////////////////////////////////////////////
void setupGLEW() {
	// LOOKHERE #1B
	glewExperimental = GL_TRUE;
	GLenum glewResult = glewInit();

	/* check for an error */
	if( glewResult != GLEW_OK ) {
		printf( "[ERROR]: Error initalizing GLEW\n");
		/* Problem: glewInit failed, something is seriously wrong. */
  	fprintf( stderr, "[ERROR]: %s\n", glewGetErrorString(glewResult) );
		exit(EXIT_FAILURE);
	} else {
		 fprintf( stdout, "[INFO]: GLEW initialized\n" );
		 fprintf( stdout, "[INFO]: Status: Using GLEW %s\n", glewGetString(GLEW_VERSION) );
	}

	if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
		printf( "[ERROR]: OpenGL not version 2.0+.  GLSL not supported\n" );
		exit(EXIT_FAILURE);
	}
}

// setupShaders() //////////////////////////////////////////////////////////////
//
//      Create our shaders.  Send GLSL code to GPU to be compiled.  Also get
//  handles to our uniform and attribute locations.
//
////////////////////////////////////////////////////////////////////////////////
void setupShaders( const char *vertexShaderFilename, const char *fragmentShaderFilename ) {
	// TODO #7
	 shaderProgramHandle = createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
	
	// TODO #8A
	mvp_uniform_location = glGetUniformLocation(shaderProgramHandle, "mvpMatrix");
	
  // TODO #10A
  time_uniform_location = glGetUniformLocation(shaderProgramHandle, "time");
	// TODO #8B
	vpos_attrib_location = glGetAttribLocation(shaderProgramHandle, "vPosition");
	text_coor_location = glGetAttribLocation(shaderProgramHandle, "texCoord");
}

void setupObjShaders( const char *vertexShaderFilename, const char *fragmentShaderFilename ) {
	objectProgramHandle = createShaderProgram(vertexShaderFilename, fragmentShaderFilename);
	 
	 // NOTE: Should really check to make sure that the stuff is greater then 0
	
	// TODO #8A
	obj_mvp_uniform_location = glGetUniformLocation(objectProgramHandle, "mvpMatrix");
	
	// TODO #10A
	obj_time_uniform_location = glGetUniformLocation(objectProgramHandle, "time");
	// TODO #8B
	obj_vpos_attrib_location = glGetAttribLocation(objectProgramHandle, "vPosition");
	obj_text_coor_location = glGetAttribLocation(objectProgramHandle, "texCoord");

	lightDiff = glGetUniformLocation(objectProgramHandle, "lightDiff");
	lightSpec = glGetUniformLocation(objectProgramHandle, "lightSpec");
	lightAmb = glGetUniformLocation(objectProgramHandle, "lightAmb");
	matDiff = glGetUniformLocation(objectProgramHandle, "matDiff");
	matSpec = glGetUniformLocation(objectProgramHandle, "matSpec");
	matAmb = glGetUniformLocation(objectProgramHandle, "matAmb");
	lightPos = glGetUniformLocation(objectProgramHandle, "lightPos");
	camPos = glGetUniformLocation(objectProgramHandle, "camPos");
	shine = glGetUniformLocation(objectProgramHandle, "shine");
	normalHandle = glGetAttribLocation(objectProgramHandle, "normalHandle");
}

// setupSnowShaders() //////////////////////////////////////////////////////////////
//
//      Create our shaders.  Send GLSL code to GPU to be compiled.  Also get
//  handles to our uniform and attribute locations.
//
////////////////////////////////////////////////////////////////////////////////
void setupSnowShaders() {
	// LOOKHERE #1
	snowShaderProgram = new CSCI441::ShaderProgram( "shaders/billboardQuadShader.v.glsl",
																							"shaders/billboardQuadShader.g.glsl",
																						  "shaders/billboardQuadShader.f.glsl" );
	snow_modelview_uniform_location  = snowShaderProgram->getUniformLocation( "mvMatrix" );
	snow_projection_uniform_location = snowShaderProgram->getUniformLocation( "projMatrix" );
	snow_vpos_attrib_location			  = snowShaderProgram->getAttributeLocation( "vPos" );
}

// setupTextures() /////////////////////////////////////////////////////////////
//
//      Load and register all the tetures for our program
//
////////////////////////////////////////////////////////////////////////////////
void setupTextures() {
	platformTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-ground.jpg" );
	frontTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-castle.jpg" );
	backTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-forest.png" );
	rightTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-forest.png" );
	leftTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-forest.png" );
	ceilTextureHandle = loadAndRegisterTexture( "textures/skybox/dark-cloud.jpg" );
	textureHandle = CSCI441::TextureUtils::loadAndRegisterTexture( "textures/snowflake.png" );
}


void setupObject(const char *objectFilename) {
	model = new CSCI441::ModelLoader();
	model->loadModelFile( objectFilename );
}
// setupBuffers() //////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
////////////////////////////////////////////////////////////////////////////////
void setupBuffers() {
  

	// TODO #01 - create our struct here
	struct VertexTextured {
		float x, y, z, s, t;
	};

	// Plate
	// TODO #02: create our platform vertex array
	VertexTextured vertexArr[4] = {
		{-5, -1, -5, 0, 0}, 
		{5, -1, -5, 1, 0},
		{5, -1, 5, 1, 1},
		{-5, -1, 5, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArray[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaods);
	glBindVertexArray(vaods);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArr), vertexArr, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW );
	
	
	// FLOOR
	VertexTextured vertexArrFl[4] = {
		{-20, -20, -20, 0, 0}, 
		{20, -20, -20, 1, 0},
		{20, -20, 20, 1, 1},
		{-20, -20, 20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayFl[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodFloor);
	glBindVertexArray(vaodFloor);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrFl), vertexArrFl, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayFl), indexArrayFl, GL_STATIC_DRAW );
	
	
	// CEIL
	VertexTextured vertexArrC[4] = {
		{-20, 20, -20, 0, 0}, 
		{20, 20, -20, 1, 0},
		{20, 20, 20, 1, 1},
		{-20, 20, 20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayC[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodCeil);
	glBindVertexArray(vaodCeil);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrC), vertexArrC, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayC), indexArrayC, GL_STATIC_DRAW );
	
	
	// FRONT
	VertexTextured vertexArrFr[4] = {
		{-20, -20, 20, 0, 0}, 
		{20, -20, 20, 1, 0},
		{20, 20, 20, 1, 1},
		{-20, 20, 20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayFr[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodFront);
	glBindVertexArray(vaodFront);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrFr), vertexArrFr, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayFr), indexArrayFr, GL_STATIC_DRAW );
	
	
	//BACK
	VertexTextured vertexArrBr[4] = {
		{-20, -20, -20, 0, 0}, 
		{20, -20, -20, 1, 0},
		{20, 20, -20, 1, 1},
		{-20, 20, -20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayBr[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodBack);
	glBindVertexArray(vaodBack);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrBr), vertexArrBr, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayBr), indexArrayBr, GL_STATIC_DRAW );
	
	
	//RIGHT
	VertexTextured vertexArrR[4] = {
		{-20, -20, -20, 0, 0}, 
		{-20, -20, 20, 1, 0},
		{-20, 20, 20, 1, 1},
		{-20, 20, -20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayR[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodRight);
	glBindVertexArray(vaodRight);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrR), vertexArrR, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayR), indexArrayR, GL_STATIC_DRAW );
	
	
	//LEFT
	VertexTextured vertexArrL[4] = {
		{20, -20, -20, 0, 0}, 
		{20, -20, 20, 1, 0},
		{20, 20, 20, 1, 1},
		{20, 20, -20, 0, 1}
	};

  // TODO #03: create the index array
  unsigned short indexArrayL[6] = {0, 1, 2, 2, 3, 0};

	// TODO #04B: generate and bind the VAO
	glGenVertexArrays(1, &vaodLeft);
	glBindVertexArray(vaodLeft);
	
	// TODO #05A: generate and bind the array VBO
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ARRAY_BUFFER, vbod);

	// TODO #05B: send the vertex data to the GPU
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertexArrL), vertexArrL, GL_STATIC_DRAW );

	// TODO #06A: state where the vertex position is located within our array data
	glEnableVertexAttribArray( vpos_attrib_location );
	glVertexAttribPointer( vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)0 );

	// TODO #06B: state where the texture coordinate is located within our array data
	glEnableVertexAttribArray( text_coor_location );
	glVertexAttribPointer( text_coor_location, 2, GL_FLOAT, GL_FALSE, sizeof( VertexTextured ), (void*)(sizeof(float) * 3));

	// TODO #07: generate and bind the element array VBO.  send data to the GPU
	glGenBuffers(1, &vbod);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbod);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArrayL), indexArrayL, GL_STATIC_DRAW );
}

// setupSnowBuffers() //////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
////////////////////////////////////////////////////////////////////////////////
void setupSnowBuffers() {
	// LOOKHERE #2
	float nAngle = (2*M_PI) / NUM_POINTS;
	float xzAngle = nAngle;
	for( int i = 0; i < NUM_POINTS; i++ ) {
		Vertex v = { spawn.x, spawn.y, spawn.z };
		points[i] = v;
		float vs = randNumber(5);	
		glm::vec3 speed = glm::vec3(vs*cos(xzAngle), vs*sin(angle), vs*sin(xzAngle));
		part[i] = Particle(spawn, speed, 1.0); // life volocity position
		xzAngle += nAngle;
	}

	glGenVertexArrays( 1, &pointsVAO );
	glBindVertexArray( pointsVAO );

	glGenBuffers( 1, &pointsVBO );
	glBindBuffer( GL_ARRAY_BUFFER, pointsVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	glEnableVertexAttribArray( snow_vpos_attrib_location );
	glVertexAttribPointer( snow_vpos_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );
}

//******************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

// renderScene() ///////////////////////////////////////////////////////////////
//
//		This method will contain all of the objects to be drawn.
//
////////////////////////////////////////////////////////////////////////////////
void renderScene( glm::mat4 viewMtx, glm::mat4 projMtx ) {
  // stores our model matrix
  glm::mat4 modelMtx;

  // use our shader program
  // Draw the skybox
	// TODO #9A
	glUseProgram(shaderProgramHandle);

  // precompute our MVP CPU side so it only needs to be computed once
  glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;;
  // send MVP to GPU
	// TODO #9B
	glUniformMatrix4fv(mvp_uniform_location, 1, GL_FALSE, &mvpMtx[0][0]);
  // send time to GPU
	// TODO #10B
  glUniform1f(time_uniform_location, glfwGetTime());

  // draw all the cool stuff!
  
  /*
  switch( objectIndex ) {
    case 0: CSCI441::drawSolidTeapot( 2.0 );                            break;
    case 1: CSCI441::drawSolidCube( 4.0 );                              break;
    case 2: CSCI441::drawSolidSphere( 3.0, 16, 16 );                    break;
    case 3: CSCI441::drawSolidTorus( 1.0, 2.0, 16, 16 );                break;
    case 4: CSCI441::drawSolidCone( 2.0, 4.0, 16, 16 );                 break;
    case 5: CSCI441::drawSolidCylinder( 2.0, 2.0, 4.0, 16, 16 );        break;
    case 6: model->draw( vpos_attrib_location );                        break;
  }
  */
  
  // Draw the skybox
	//glEnable(GL_TEXTURE);
	glBindTexture(   GL_TEXTURE_2D,  platformTextureHandle );
	glBindVertexArray(vaods);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	// using the same texture for the ground and platform 
	glBindTexture(   GL_TEXTURE_2D,  platformTextureHandle );
	glBindVertexArray(vaodFloor);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	// using the same texture for the ground and platform 
	glBindTexture(   GL_TEXTURE_2D,  frontTextureHandle );
	glBindVertexArray(vaodFront);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	glBindTexture(   GL_TEXTURE_2D,  ceilTextureHandle );
	glBindVertexArray(vaodCeil);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	glBindTexture(   GL_TEXTURE_2D,  backTextureHandle );
	glBindVertexArray(vaodBack);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	glBindTexture(   GL_TEXTURE_2D,  rightTextureHandle );
	glBindVertexArray(vaodRight);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	glBindTexture(   GL_TEXTURE_2D,  leftTextureHandle );
	glBindVertexArray(vaodLeft);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
	
	
	// The object shading
	  // Draw the suzen
  glUseProgram(objectProgramHandle);

  // precompute our MVP CPU side so it only needs to be computed once
  glm::mat4 obj_mvpMtx = projMtx * viewMtx * modelMtx;
  // send MVP to GPU
	// TODO #9B
	glUniformMatrix4fv(obj_mvp_uniform_location, 1, GL_FALSE, &obj_mvpMtx[0][0]);
	
	//glUniformMatrix4fv(obj_mvp_uniform_location, 1, GL_FALSE, &mvpMtx[0][0]);
  
	glUniform3f(matAmb, 0.1f, 0.18725f, 0.1745f);
	glUniform3f(matDiff, 0.396f, 0.74151f, .69102f);
	glUniform3f(matSpec, 0.297254f, 0.30829f, 0.306678f);
	glUniform1f(shine, 0.1f);
	
	glUniform3f(lightAmb, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightDiff, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightSpec, 1.0f, 1.0f, 1.0f);
	
	glUniform3f(lightPos, 10.0f, 10.0f, 10.0f);
	glUniform3f(camPos, eyePoint.x, eyePoint.y, eyePoint.z);
	
  //model->draw( obj_vpos_attrib_location, normalHandle );
  
  
  model->draw(obj_vpos_attrib_location, normalHandle, obj_text_coor_location, matDiff, matSpec, shine, matAmb);
	
	
	
}

void renderSnowScene( glm::mat4 viewMtx, glm::mat4 projMtx ) {
	// LOOKHERE #3

  // stores our model matrix
  glm::mat4 modelMtx;

  // use our shader program
	snowShaderProgram->useProgram();

  // precompute our MVP CPU side so it only needs to be computed once
  glm::mat4 mvMtx = viewMtx * modelMtx;;
  // send MVP to GPU
  glUniformMatrix4fv(snow_modelview_uniform_location, 1, GL_FALSE, &mvMtx[0][0]);
  glUniformMatrix4fv(snow_projection_uniform_location, 1, GL_FALSE, &projMtx[0][0]);

	// TODO #1 : sort!
	glm::vec3 vVec = lookAtPoint - eyePoint;
    glm::normalize(vVec);
	
	int orderedInd[NUM_POINTS];
	double distances[NUM_POINTS];
	for (int i = 0; i < NUM_POINTS; ++i) {
		glm::vec4 p = glm::vec4(part[i].position.x, part[i].position.y, part[i].position.z, 1) * modelMtx;
		glm::vec4 ep = p - glm::vec4(eyePoint, 1);
		
		distances[i] = glm::dot(glm::vec4(vVec, 0), ep);
		orderedInd[i] = i;
	}
	
	for (unsigned int i = 0; i < NUM_POINTS; ++i) {
		for (unsigned int j = i+1; j < NUM_POINTS; ++j) {
			if (distances[i] < distances[j]) {
				double c = distances[j];
				distances[j] = distances[i];
				distances[i] = c;
				
				int cc = orderedInd[j];
				orderedInd[j] = orderedInd[i];
				orderedInd[i] = cc;
			}
		}
	}
	
	Vertex orderedPoints[NUM_POINTS];
	double endTime = glfwGetTime();
	double timeD = endTime - timeI;
	timeI = endTime;
	for( unsigned int i = 0; i < NUM_POINTS; ++i) {
		orderedPoints[i] = { part[ orderedInd[i] ].position.x, part[ orderedInd[i] ].position.y, part[ orderedInd[i] ].position.z };
		part[ orderedInd[i] ].update(timeD);
	}
	
	glBindVertexArray( pointsVAO );
	
	// TODO #2 : send our sorted data
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(orderedPoints), orderedPoints);

	

	// LOOKHERE #4
	glBindTexture( GL_TEXTURE_2D, textureHandle );
	glDrawArrays( GL_POINTS, 0, NUM_POINTS );
	
	// This will be the time for the snowflakes dieing so we make
	if (part[0].currentAge >= part[0].maxLife) setupSnowBuffers();
}

///*****************************************************************************
//
// Our main function

// main() ///////////////////////////////////////////////////////////////
//
//		Really you should know what this is by now.
//
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ) {
  // ensure proper number of arguments provided at runtime
  /*
	if( argc != 2 ) {
    // we need a vertex and fragment shader
		fprintf( stderr, "Usage: ./a5 <file.obj>\n" );
		exit(EXIT_FAILURE);
	}
	
	*/
	
	// Need to read in a file
  // GLFW sets up our OpenGL context so must be done first
	GLFWwindow *window = setupGLFW();	// initialize all of the GLFW specific information releated to OpenGL and our window
	timeI = glfwGetTime();
	setupOpenGL();										// initialize all of the OpenGL specific information
	// LOOKHERE #1A
	setupGLEW();											// initialize all of the GLEW specific information

  CSCI441::OpenGLUtils::printOpenGLInfo();

	setupShaders( "shaders/customShader.v.glsl", "shaders/customShader.f.glsl" ); // load our shader program into memory
	setupObjShaders( "shaders/objShader.v.glsl", "shaders/objShader.f.glsl" ); // load our shader program into memory
	setupObject( "models/suzanne.obj" );
	setupBuffers();										// load all our VAOs and VBOs into memory
	setupSnowShaders(); 									// load our shader program into memory
	setupSnowBuffers();										// load all our VAOs and VBOs into memory
	
	setupTextures();
	
  // needed to connect our 3D Object Library to our shader
	// LOOKHERE #3
  CSCI441::setVertexAttributeLocations( vpos_attrib_location );
  CSCI441::setVertexAttributeLocations( snow_vpos_attrib_location );

	convertSphericalToCartesian();		// set up our camera position

  //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
	//	until the user decides to close the window and quit the program.  Without a loop, the
	//	window will display once and then the program exits.
	while( !glfwWindowShouldClose(window) ) {	// check if the window was instructed to be closed
    glDrawBuffer( GL_BACK );				// work with our back frame buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

		// Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
		// when using a Retina display the actual window can be larger than the requested window.  Therefore
		// query what the actual size of the window we are rendering to is.
		glfwGetFramebufferSize( window, &windowWidth, &windowHeight );

		// update the viewport - tell OpenGL we want to render to the whole window
		glViewport( 0, 0, windowWidth, windowHeight );

		// set the projection matrix based on the window size
		// use a perspective projection that ranges
		// with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
		glm::mat4 projectionMatrix = glm::perspective( 45.0f, windowWidth / (float) windowHeight, 0.001f, 100.0f );

		// set up our look at matrix to position our camera
		glm::mat4 viewMatrix = glm::lookAt( eyePoint,lookAtPoint, upVector );

		// draw everything to the window
		// pass our view and projection matrices as well as deltaTime between frames
		renderScene( viewMatrix, projectionMatrix );
		renderSnowScene( viewMatrix, projectionMatrix );

		glfwSwapBuffers(window);// flush the OpenGL commands and make sure they get rendered!
		glfwPollEvents();				// check for any events and signal to redraw screen
	}

  glfwDestroyWindow( window );// clean up and close our window
	glfwTerminate();						// shut down GLFW to clean up our context

	return EXIT_SUCCESS;
}
