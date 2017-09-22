/** @file OpenGLUtils.hpp
  * @brief Helper functions to work with OpenGL
	* @author Dr. Jeffrey Paone
	* @date Last Edit: 21 Sep 2017
	* @version 1.1
	*
	* @copyright MIT License Copyright (c) 2017 Dr. Jeffrey Paone
	*
	*	These functions
	*
	*	@warning NOTE: This header file will depends upon glm
  */

#ifndef __CSCI441_OPENGLUTILS_H__
#define __CSCI441_OPENGLUTILS_H__

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include <glm/glm.hpp>

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////

/** @namespace CSCI441
  * @brief CSCI441 Helper Functions for OpenGL
	*/
namespace CSCI441 {
		static const glm::vec3 X_AXIS( 1.0f, 0.0f, 0.0f );					// constant for postive X_AXIS
		static const glm::vec3 Y_AXIS( 0.0f, 1.0f, 0.0f );					// constant for postive Y_AXIS
		static const glm::vec3 Z_AXIS( 0.0f, 0.0f, 1.0f );					// constant for postive Z_AXIS
		static const glm::vec3 X_AXIS_NEG( -1.0f,  0.0f,  0.0f );		// constant for negative X_AXIS
		static const glm::vec3 Y_AXIS_NEG(  0.0f, -1.0f,  0.0f );		// constant for negative Y_AXIS
		static const glm::vec3 Z_AXIS_NEG(  0.0f,  0.0f, -1.0f );		// constant for negative Z_AXIS

		/**	@brief Multiplies current matrix by given matrix
			*
			* @param glm::mat4 mtx		- matrix to multiply the current matrix by
		  */
		void pushMatrix( glm::mat4 mtx );

    /**	@brief Multiplies current matrix by inverse of given matrix
			*
			* @param glm::mat4 mtx		- matrix to multiply the current matrix by the inverse of
		  */
		void popMatrix( glm::mat4 mtx );

    /** @class OpenGLUtils
      * @brief static class containing OpenGL Utilities
      */
    class OpenGLUtils {
    	public:
        /** @brief Prints information about our OpenGL context
          *
          */
    		static void printOpenGLInfo();

    	private:
    		OpenGLUtils();
  	};
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Internal definitions

namespace CSCI441_INTERNAL {
  void printOpenGLParamHeader( const int major, const int minor );
  void printOpenGLParam( const char *format, GLenum name );
  void printOpenGLParam2( const char *format, GLenum name );
  void printOpenGLParam3( const char *format, GLenum name );
  void printOpenGLParam4( const char *format, GLenum name );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Outward facing function implementations

inline void CSCI441::pushMatrix( glm::mat4 mtx ) {
	glMultMatrixf( &mtx[0][0] );
}

inline void CSCI441::popMatrix( glm::mat4 mtx ) {
	glMultMatrixf( &( glm::inverse(mtx) )[0][0] );
}

inline void CSCI441::OpenGLUtils::printOpenGLInfo() {
	float version;
	sscanf((char*)glGetString(GL_VERSION), "%f", &version);

	char versionStr[3];
	sprintf(versionStr, "%.1f", version);
	int major = atoi( &versionStr[0] );
	int minor = atoi( &versionStr[2] );

	fprintf( stdout, "\n[INFO]: /--------------------------------------------------------\\\n" );
	fprintf( stdout, "[INFO]: | OpenGL Information                                     |\n" );
	fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n" );
	fprintf( stdout, "[INFO]: |   OpenGL Version:  %35s |\n", glGetString(GL_VERSION) );
	fprintf( stdout, "[INFO]: |   OpenGL Renderer: %35s |\n", glGetString(GL_RENDERER) );
	fprintf( stdout, "[INFO]: |   OpenGL Vendor:   %35s |\n", glGetString(GL_VENDOR) );

	if( (major >= 2 && minor >= 0) || major > 2 ) {
		CSCI441_INTERNAL::printOpenGLParamHeader( 2, 0 );
		CSCI441_INTERNAL::printOpenGLParam( "[INFO]: |   Max # Lights:    %35d |\n", 									GL_MAX_LIGHTS );
		CSCI441_INTERNAL::printOpenGLParam( "[INFO]: |   Max # Color Attachments:  %26d |\n", 				GL_MAX_COLOR_ATTACHMENTS );
	}

	fprintf( stdout, "[INFO]: \\--------------------------------------------------------/\n\n");
}

inline CSCI441::OpenGLUtils::OpenGLUtils() {}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Internal function implementations

inline void CSCI441_INTERNAL::printOpenGLParamHeader( const int major, const int minor ) {
	fprintf( stdout, "[INFO]: >--------------------------------------------------------<\n");
	fprintf( stdout, "[INFO]: | OpenGL %d.%d Settings                                    |\n", major, minor);
	fprintf( stdout, "[INFO]: |--------------------------------------------------------|\n");
}

inline void CSCI441_INTERNAL::printOpenGLParam( const char *format, GLenum name ) {
	GLint value = 0;
	glGetIntegerv( name, &value );
	fprintf( stdout, format, value );
}

inline void CSCI441_INTERNAL::printOpenGLParam2( const char *format, GLenum name ) {
	GLint values[2] = {0,0};
	glGetIntegerv( name, values );
	fprintf( stdout, format, values[0], values[1] );
}

inline void CSCI441_INTERNAL::printOpenGLParam3( const char *format, GLenum name ) {
	GLint values[3] = {0,0,0};
	glGetIntegerv( name, values );
	fprintf( stdout, format, values[0], values[1], values[2] );
}

inline void CSCI441_INTERNAL::printOpenGLParam4( const char *format, GLenum name ) {
	GLint values[4] = {0,0,0,0};
	glGetIntegerv( name, values );
	fprintf( stdout, format, values[0], values[1], values[2], values[3] );
}

#endif // __CSCI441_OPENGLUTILS_H__
