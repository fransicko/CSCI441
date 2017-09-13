#ifndef __441_WORLDMAP_H__
#define __441_WORLDMAP_H__

// here's a fun fact....glfw internally includes gl.h
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>		// used for loading an image...we will get to this later on.  You can ignore it for now.

#include <stdio.h>

#include <sstream>			// I don't fully love C...let's use some C++
#include <string>
using namespace std;

//////////////////////////////////////////////////////////
//
//	Do not modify any of these values.  They handle
//	the world map operation.  We will eventually
//	discuss how textures work so that we can display
//	images like is being done here.

const GLint MAP_WIDTH = 6, MAP_HEIGHT = 6;				// sets the width and height of our 2D map array
GLint mapHandles[ MAP_WIDTH ][ MAP_HEIGHT ];			// a 2D storing the texture handles of our world map backgrounds
GLint currMapLocationX = 2, currMapLocationY = 2;		// our current location within the map

void bindCurrentMapLocation();							// binds the current texture
void drawMap( GLint windowWidth, GLint windowHeight );											// draws a rectangle with our world map background

// these next four methods update the current location and loads the correct map.  they will wrap
// around the edges, so the Hero can move through an infinite world
void moveDown();										// moves our world map down one square
void moveLeft();										// moves our world map left one square
void moveRight();										// moves our world map right one square
void moveUp();											// moves our world map up one square
//////////////////////////////////////////////////////////

void bindCurrentMapLocation() {
	glBindTexture( GL_TEXTURE_2D, mapHandles[ currMapLocationX ][ currMapLocationY ] );
}

// draw a quad with our texture mapped to it
void drawMap( GLint windowWidth, GLint windowHeight ) {
	glEnable( GL_TEXTURE_2D );
	glColor4f( 1, 1, 1, 1 );
	glBegin( GL_QUADS ); {
		glTexCoord2f( 0, 0 );	glVertex2f( 			      0, 	      			0 );
		glTexCoord2f( 1, 0 ); glVertex2f( windowWidth-1, 		      		0 );
		glTexCoord2f( 1, 1 );	glVertex2f( windowWidth-1, windowHeight-1 );
		glTexCoord2f( 0, 1 );	glVertex2f( 		        0, windowHeight-1 );
	}; glEnd();
	glDisable( GL_TEXTURE_2D );
}

// I hope you can figure out what's going on here
void moveDown() {
	currMapLocationY++;
	if( currMapLocationX >= MAP_HEIGHT )
		currMapLocationY = 0;
	bindCurrentMapLocation();
}

void moveLeft() {
	currMapLocationX--;
	if( currMapLocationX < 0 )
		currMapLocationX = MAP_WIDTH-1;
	bindCurrentMapLocation();
}

void moveRight() {
	currMapLocationX++;
	if( currMapLocationX >= MAP_WIDTH )
		currMapLocationX = 0;
	bindCurrentMapLocation();
}

void moveUp() {
	currMapLocationY--;
	if( currMapLocationY < 0 )
		currMapLocationY = MAP_HEIGHT-1;
	bindCurrentMapLocation();
}

// initialize our map
void initMap() {
	glEnable( GL_TEXTURE_2D );
	printf( "Loading map..." );
	fflush( stdout );

	// I'm going to leave this uncommented for now.  Trust that it works.  It controls
	// the loading of all the background images that get dispalyed
	for( GLint j = 0; j < MAP_HEIGHT; j++ ) {
		for( GLint i = 0; i < MAP_WIDTH; i++ ) {
			stringstream filenameSS;
			filenameSS << "./images/map" << (j*MAP_WIDTH + i) << ".png";
			string filename = filenameSS.str();

			GLint mapTexHandle = SOIL_load_OGL_texture( filename.c_str(),
													SOIL_LOAD_AUTO,
													SOIL_CREATE_NEW_ID,
													SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT );

			// failed to load image
			if( mapTexHandle == 0 ) {
				filename = "./images/default.png";
				mapTexHandle = SOIL_load_OGL_texture( filename.c_str(),
														SOIL_LOAD_AUTO,
														SOIL_CREATE_NEW_ID,
														SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT );
			}

			glBindTexture( GL_TEXTURE_2D, mapTexHandle );
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

			mapHandles[ i ][ j ] = mapTexHandle;
		}
		printf( "." );
		fflush( stdout );
	}
	printf( "...done!\n" );
	fflush( stdout );

	bindCurrentMapLocation();
	glDisable( GL_TEXTURE_2D );
}

#endif // __441_WORLDMAP_H__
