/** @file ShaderProgram3.hpp
  * @brief Class to work with OpenGL Shaders
	* @author Dr. Jeffrey Paone
	* @date Last Edit: 03 Nov 2017
	* @version 1.4
	*
	* @copyright MIT License Copyright (c) 2017 Dr. Jeffrey Paone
	*
	*	These functions, classes, and constants help minimize common
	*	code that needs to be written.
  */

#ifndef __CSCI441_SHADEREPROGRAM_3_H__
#define __CSCI441_SHADEREPROGRAM_3_H__

#include "ShaderUtils3.hpp"

#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////

namespace CSCI441 {

	class ShaderProgram {
	public:
		static void enableDebugMessages();
		static void disableDebugMessages();

		ShaderProgram( const char *vertexShaderFilename,
					   			 const char *fragmentShaderFilename );

		ShaderProgram( const char *vertexShaderFilename,
					   			 const char *tesselationControlShaderFilename,
					   	 		 const char *tesselationEvaluationShaderFilename,
					   	 		 const char *geometryShaderFilename,
					   	 		 const char *fragmentShaderFilename );

		ShaderProgram( const char *vertexShaderFilename,
					   			 const char *geometryShaderFilename,
					   	 		 const char *fragmentShaderFilename );

		~ShaderProgram();

		GLint getUniformLocation( const char *uniformName );

		GLint getUniformBlockIndex( const char *uniformBlockName );
		GLint getUniformBlockSize( const char *uniformBlockName );
		GLubyte* getUniformBlockBuffer( const char *uniformBlockName );
		GLint* getUniformBlockOffsets( const char *uniformBlockName );
		GLint* getUniformBlockOffsets( const char *uniformBlockName, const char *names[] );
		void setUniformBlockBinding( const char *uniformBlockName, GLuint binding );

		GLint getAttributeLocation( const char *attributeName );

		GLuint getSubroutineIndex( GLenum shaderStage, const char *subroutineName );

		GLuint getNumUniforms();
		GLuint getNumUniformBlocks();
		GLuint getNumAttributes();

		GLuint getShaderProgramHandle();

		void useProgram();
	private:
		ShaderProgram();

		static bool sDEBUG;

		GLuint _vertexShaderHandle;
		GLuint _tesselationControlShaderHandle;
		GLuint _tesselationEvaluationShaderHandle;
		GLuint _geometryShaderHandle;
		GLuint _fragmentShaderHandle;

		GLuint _shaderProgramHandle;

		bool registerShaderProgram( const char *vertexShaderFilename,
																const char *tesselationControlShaderFilename,
																const char *tesselationEvaluationShaderFilename,
																const char *geometryShaderFilename,
																const char *fragmentShaderFilename );

		GLint* getUniformBlockOffsets( GLint uniformBlockIndex );
		GLint* getUniformBlockOffsets( GLint uniformBlockIndex, const char *names[] );
	};

}

////////////////////////////////////////////////////////////////////////////////

bool CSCI441::ShaderProgram::sDEBUG = true;

void CSCI441::ShaderProgram::enableDebugMessages() {
	sDEBUG = true;
}
void CSCI441::ShaderProgram::disableDebugMessages() {
	sDEBUG = false;
}

CSCI441::ShaderProgram::ShaderProgram( const char *vertexShaderFilename, const char *fragmentShaderFilename ) {
	registerShaderProgram( vertexShaderFilename, "", "", "", fragmentShaderFilename );
}

CSCI441::ShaderProgram::ShaderProgram( const char *vertexShaderFilename, const char *tesselationControlShaderFilename, const char *tesselationEvaluationShaderFilename, const char *geometryShaderFilename, const char *fragmentShaderFilename ) {
	registerShaderProgram( vertexShaderFilename, tesselationControlShaderFilename, tesselationEvaluationShaderFilename, geometryShaderFilename, fragmentShaderFilename );
}

CSCI441::ShaderProgram::ShaderProgram( const char *vertexShaderFilename, const char *geometryShaderFilename, const char *fragmentShaderFilename ) {
	registerShaderProgram( vertexShaderFilename, "", "", geometryShaderFilename, fragmentShaderFilename );
}

bool CSCI441::ShaderProgram::registerShaderProgram( const char *vertexShaderFilename, const char *tesselationControlShaderFilename, const char *tesselationEvaluationShaderFilename, const char *geometryShaderFilename, const char *fragmentShaderFilename ) {
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	if( sDEBUG ) printf( "\n[INFO]: /--------------------------------------------------------\\\n");

	/* compile each one of our shaders */
	if( sDEBUG ) printf( "[INFO]: | Vertex Shader: %39s |\n", vertexShaderFilename );
	_vertexShaderHandle   = CSCI441_INTERNAL::ShaderUtils::compileShader( vertexShaderFilename,   GL_VERTEX_SHADER   );

	if( strcmp( tesselationControlShaderFilename, "" ) != 0 ) {
		if( sDEBUG ) printf( "[INFO]: | Tess Control Shader: %33s |\n", tesselationControlShaderFilename );
		if( major < 4 ) {
			printf( "[ERROR]:|   TESSELATION SHADER NOT SUPPORTED!!  UPGRADE TO v4.0+ |\n" );
			_tesselationControlShaderHandle = 0;
		} else {
			_tesselationControlShaderHandle = CSCI441_INTERNAL::ShaderUtils::compileShader( tesselationControlShaderFilename, GL_TESS_CONTROL_SHADER );
		}
	} else {
		_tesselationControlShaderHandle = 0;
	}

	if( strcmp( tesselationEvaluationShaderFilename, "" ) != 0 ) {
		if( sDEBUG ) printf( "[INFO]: | Tess Evaluation Shader: %30s |\n", tesselationEvaluationShaderFilename );
		if( major < 4 ) {
			printf( "[ERROR]:|   TESSELATION SHADER NOT SUPPORTED!!  UPGRADE TO v4.0+ |\n" );
			_tesselationEvaluationShaderHandle = 0;
		} else {
			_tesselationEvaluationShaderHandle = CSCI441_INTERNAL::ShaderUtils::compileShader( tesselationEvaluationShaderFilename, GL_TESS_EVALUATION_SHADER );
		}
	} else {
		_tesselationEvaluationShaderHandle = 0;
	}

	if( strcmp( geometryShaderFilename, "" ) != 0 ) {
		if( sDEBUG ) printf( "[INFO]: | Geometry Shader: %37s |\n", geometryShaderFilename );
		if( major < 3 || (major == 3 && minor < 2) ) {
			printf( "[ERROR]:|   GEOMETRY SHADER NOT SUPPORTED!!!    UPGRADE TO v3.2+ |\n" );
			_geometryShaderHandle = 0;
		} else {
			_geometryShaderHandle = CSCI441_INTERNAL::ShaderUtils::compileShader( geometryShaderFilename, GL_GEOMETRY_SHADER );
		}
	} else {
		_geometryShaderHandle = 0;
	}

	if( sDEBUG ) printf( "[INFO]: | Fragment Shader: %37s |\n", fragmentShaderFilename );
	_fragmentShaderHandle = CSCI441_INTERNAL::ShaderUtils::compileShader( fragmentShaderFilename, GL_FRAGMENT_SHADER );

	/* get a handle to a shader program */
	_shaderProgramHandle = glCreateProgram();

	/* attach the vertex and fragment shaders to the shader program */
	glAttachShader( _shaderProgramHandle, _vertexShaderHandle );
	if( _tesselationControlShaderHandle != 0 ) {
		glAttachShader( _shaderProgramHandle, _tesselationControlShaderHandle );
	}
	if( _tesselationEvaluationShaderHandle != 0 ) {
		glAttachShader( _shaderProgramHandle, _tesselationEvaluationShaderHandle );
	}
	if( _geometryShaderHandle != 0 ) {
		glAttachShader( _shaderProgramHandle, _geometryShaderHandle );
	}
	glAttachShader( _shaderProgramHandle, _fragmentShaderHandle );

	/* link all the programs together on the GPU */
	glLinkProgram( _shaderProgramHandle );

	if( sDEBUG ) printf( "[INFO]: | Shader Program: %41s", "|\n" );

	/* check the program log */
	CSCI441_INTERNAL::ShaderUtils::printLog( _shaderProgramHandle );

	GLint separable = GL_FALSE;
	glGetProgramiv( _shaderProgramHandle, GL_PROGRAM_SEPARABLE, &separable );

	if( sDEBUG ) printf( "[INFO]: | Program Separable: %35s |\n", (separable ? "Yes" : "No"));

	/* print shader info for uniforms & attributes */
	CSCI441_INTERNAL::ShaderUtils::printShaderProgramInfo( _shaderProgramHandle );

	/* return handle */
	return _shaderProgramHandle != 0;
}

GLint CSCI441::ShaderProgram::getUniformLocation( const char *uniformName ) {
	GLint uniformLoc = glGetUniformLocation( _shaderProgramHandle, uniformName );
	if( uniformLoc == -1 )
		fprintf( stderr, "[ERROR]: Could not find uniform %s\n", uniformName );
	return uniformLoc;
}

GLint CSCI441::ShaderProgram::getUniformBlockIndex( const char *uniformBlockName ) {
	GLint uniformBlockLoc = glGetUniformBlockIndex( _shaderProgramHandle, uniformBlockName );
	if( uniformBlockLoc == -1 )
		fprintf( stderr, "[ERROR]: Could not find uniform block %s\n", uniformBlockName );
	return uniformBlockLoc;
}

GLint CSCI441::ShaderProgram::getUniformBlockSize( const char *uniformBlockName ) {
	GLint blockSize;
	glGetActiveUniformBlockiv( _shaderProgramHandle, getUniformBlockIndex(uniformBlockName), GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize );
	return blockSize;
}

GLubyte* CSCI441::ShaderProgram::getUniformBlockBuffer( const char *uniformBlockName ) {
	GLubyte *blockBuffer;

	GLint blockSize = getUniformBlockSize( uniformBlockName );

	blockBuffer = (GLubyte*)malloc(blockSize);

	return blockBuffer;
}

GLint* CSCI441::ShaderProgram::getUniformBlockOffsets( const char *uniformBlockName ) {
	return getUniformBlockOffsets( getUniformBlockIndex(uniformBlockName) );
}

GLint* CSCI441::ShaderProgram::getUniformBlockOffsets( const char *uniformBlockName, const char *names[] ) {
	return getUniformBlockOffsets( getUniformBlockIndex(uniformBlockName), names );
}

GLint* CSCI441::ShaderProgram::getUniformBlockOffsets( GLint uniformBlockIndex ) {
	GLint numUniforms;
	glGetActiveUniformBlockiv( _shaderProgramHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms );

	GLuint *indices = (GLuint*)malloc(numUniforms*sizeof(GLuint));
	glGetActiveUniformBlockiv( _shaderProgramHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)indices);

	GLint *offsets = (GLint*)malloc(numUniforms*sizeof(GLint));
	glGetActiveUniformsiv( _shaderProgramHandle, numUniforms, indices, GL_UNIFORM_OFFSET, offsets );
	return offsets;
}

GLint* CSCI441::ShaderProgram::getUniformBlockOffsets( GLint uniformBlockIndex, const char *names[] ) {
	GLint numUniforms;
	glGetActiveUniformBlockiv( _shaderProgramHandle, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms );

	GLuint *indices = (GLuint*)malloc(numUniforms*sizeof(GLuint));
	glGetUniformIndices( _shaderProgramHandle, numUniforms, names, indices );

	GLint *offsets = (GLint*)malloc(numUniforms*sizeof(GLint));
	glGetActiveUniformsiv( _shaderProgramHandle, numUniforms, indices, GL_UNIFORM_OFFSET, offsets );
	return offsets;
}

void CSCI441::ShaderProgram::setUniformBlockBinding( const char *uniformBlockName, GLuint binding ) {
	glUniformBlockBinding( _shaderProgramHandle, getUniformBlockIndex(uniformBlockName), binding );
}

GLint CSCI441::ShaderProgram::getAttributeLocation( const char *attributeName ) {
	GLint attributeLoc = glGetAttribLocation( _shaderProgramHandle, attributeName );
	if( attributeLoc == -1 )
		fprintf( stderr, "[ERROR]: Could not find attribute %s\n", attributeName );
	return attributeLoc;
}

GLuint CSCI441::ShaderProgram::getSubroutineIndex( GLenum shaderStage, const char *subroutineName ) {
	GLuint subroutineIndex = glGetSubroutineIndex( _shaderProgramHandle, shaderStage, subroutineName );
	if( subroutineIndex == GL_INVALID_INDEX )
		fprintf( stderr, "[ERROR]: Could not find subroutine %s for %s\n", subroutineName, CSCI441_INTERNAL::ShaderUtils::GL_shader_type_to_string(shaderStage) );
	return subroutineIndex;
}

GLuint CSCI441::ShaderProgram::getNumUniforms() {
	int numUniform = 0;
	glGetProgramiv( _shaderProgramHandle, GL_ACTIVE_UNIFORMS, &numUniform );
	return numUniform;
}

GLuint CSCI441::ShaderProgram::getNumUniformBlocks() {
	int numUniformBlocks = 0;
	glGetProgramiv( _shaderProgramHandle, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks );
	return numUniformBlocks;
}

GLuint CSCI441::ShaderProgram::getNumAttributes() {
	int numAttr = 0;
	glGetProgramiv( _shaderProgramHandle, GL_ACTIVE_ATTRIBUTES, &numAttr );
	return numAttr;
}

GLuint CSCI441::ShaderProgram::getShaderProgramHandle() {
	return _shaderProgramHandle;
}

void CSCI441::ShaderProgram::useProgram() {
	glUseProgram( _shaderProgramHandle );
}

CSCI441::ShaderProgram::ShaderProgram() {}

CSCI441::ShaderProgram::~ShaderProgram() {
	glDeleteShader( _vertexShaderHandle );
	glDeleteShader( _tesselationControlShaderHandle );
	glDeleteShader( _tesselationEvaluationShaderHandle );
	glDeleteShader( _geometryShaderHandle );
	glDeleteShader( _fragmentShaderHandle );
	glDeleteProgram( _shaderProgramHandle );
}

#endif //__CSCI441_SHADEREPROGRAM_3_H__
