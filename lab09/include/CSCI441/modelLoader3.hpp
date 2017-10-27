/** @file modelLoader3.hpp
  * @brief Helper functions to draw 3D OpenGL 3.0+ objects
	* @author Dr. Jeffrey Paone
	* @date Last Edit: 26 Oct 2017
	* @version 1.3
	*
	* @copyright MIT License Copyright (c) 2017 Dr. Jeffrey Paone
	*
	*	This class will load and render object files.  Currently supports:
	*		.obj
	*
	*	@warning NOTE: This header file will only work with OpenGL 3.0+
	*	@warning NOTE: This header file depends upon GLEW
  */

#include <GL/glew.h>

#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////////

/** @namespace CSCI441
  * @brief CSCI441 Helper Functions for OpenGL
	*/
namespace CSCI441 {

	/** @class ModelLoader
		* @brief Loads object models from file and renders using VBOs/VAOs
		*/
	class ModelLoader {
	public:
		/** @brief Creates an empty model
			*/
		ModelLoader();
		/** @brief Loads a model from the given file
			* @param const char* filename	- file to load model from
			*/
		ModelLoader( const char* filename );
		/** @brief Frees memory associated with model on both CPU and GPU
			*/
		~ModelLoader();

		/** @brief Loads a model from the given file
			* @param const char* filename	- file to load model from
			* @param bool INFO						- flag to control if informational messages should be displayed
			* @param bool ERRORS					- flag to control if error messages should be displayed
			* @return true if load succeeded, false otherwise
			*/
		bool loadModelFile( const char* filename, bool INFO = true, bool ERRORS = true );
		/** @brief Renders a model
			* @param GLint positionLocation	- attribute location of vertex position
			* @param GLint normalLocation		- attribute location of vertex normal
			* @param GLint texCoordLocation	- attribute location of vertex texture coordinate
			* @param GLint matDiffLocation	- attribute location of material diffuse component (currently not used but in place for forward compatibility)
			* @param GLint matSpecLocation	- attribute location of material specular component (currently not used but in place for forward compatibility)
			* @param GLint matShinLocation	- attribute location of material shininess component (currently not used but in place for forward compatibility)
			* @param GLint matAmbLocation		- attribute location of material ambient component (currently not used but in place for forward compatibility)
			* @return true if draw succeeded, false otherwise
			*/
		bool draw( GLint positionLocation, GLint normalLocation = -1, GLint texCoordLocation = -1,
							 GLint matDiffLocation = -1, GLint matSpecLocation = -1, GLint matShinLocation = -1, GLint matAmbLocation = -1 );

	private:
		void _init();
		bool _loadOBJFile( bool INFO, bool ERRORS );
		vector<string> _tokenizeString( string input, string delimiters );

		char* _filename;

		GLuint _vaod;
		GLuint _vbods[2];

		GLfloat* _vertices;
		GLfloat* _texCoords;
		GLfloat* _normals;
		unsigned short* _indices;
		unsigned long int _uniqueIndex;
		unsigned long int _numIndices;

		bool _hasVertexTexCoords;
		bool _hasVertexNormals;
	};
}

CSCI441::ModelLoader::ModelLoader() {
	_init();
}

CSCI441::ModelLoader::ModelLoader( const char* filename ) {
	_init();
	loadModelFile( filename );
}

CSCI441::ModelLoader::~ModelLoader() {
	if( _vertices ) 			free( _vertices );
	if( _texCoords ) 			free( _texCoords );
	if( _normals ) 				free( _normals );
	if( _indices ) 				free( _indices );
	if( _filename ) 			free( _filename );

	glDeleteBuffers( 1, &_vaod );
	glDeleteBuffers( 2, _vbods );
}

void CSCI441::ModelLoader::_init() {
	_hasVertexTexCoords = false;
	_hasVertexNormals = false;

	glGenVertexArrays( 1, &_vaod );
	glGenBuffers( 2, _vbods );
}

bool CSCI441::ModelLoader::loadModelFile( const char* filename, bool INFO, bool ERRORS ) {
	bool result = true;
	_filename = (char*)malloc(sizeof(char)*strlen(filename));
	strcpy( _filename, filename );
	if( strstr( _filename, ".obj" ) != NULL ) {
		result = _loadOBJFile( INFO, ERRORS );
	}
	else {
		result = false;
		if (ERRORS) fprintf( stderr, "[ERROR]:  Unsupported file format for file: %s\n", _filename );
	}

	return result;
}

bool CSCI441::ModelLoader::draw( GLint positionLocation, GLint normalLocation, GLint texCoordLocation,
						 										 GLint matDiffLocation, GLint matSpecLocation, GLint matShinLocation, GLint matAmbLocation ) {
  bool result = true;

	glBindVertexArray( _vaod );
	glBindBuffer( GL_ARRAY_BUFFER, _vbods[0] );

	glEnableVertexAttribArray( positionLocation );
	glVertexAttribPointer( positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glEnableVertexAttribArray( normalLocation );
	glVertexAttribPointer( normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat) * _uniqueIndex * 3) );

	glEnableVertexAttribArray( texCoordLocation );
	glVertexAttribPointer( texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(GLfloat) * _uniqueIndex * 6) );

	glDrawElements( GL_TRIANGLES, _numIndices, GL_UNSIGNED_SHORT, (void*)0 );

	return result;
}

// Read in a WaveFront *.obj File

bool CSCI441::ModelLoader::_loadOBJFile( bool INFO, bool ERRORS ) {
	bool result = true;

	if (INFO ) printf( "[.obj]: -=-=-=-=-=-=-=- BEGIN %s Info -=-=-=-=-=-=-=- \n", _filename );

	time_t start, end;
	time(&start);

	ifstream in( _filename );
	if( !in.is_open() ) {
		if (ERRORS) fprintf( stderr, "[.obj]: [ERROR]: Could not open \"%s\"\n", _filename );
		if ( INFO ) printf( "[.obj]: -=-=-=-=-=-=-=-  END %s Info  -=-=-=-=-=-=-=- \n", _filename );
		return false;
	}

	unsigned long int numObjects = 0, numGroups = 0;
	unsigned long int numVertices = 0, numTexCoords = 0, numNormals = 0;
	unsigned long int numFaces = 0, numTriangles = 0;
	double minX = 999999, maxX = -999999, minY = 999999, maxY = -999999, minZ = 999999, maxZ = -999999;
	string line;

	map<string, unsigned long int> uniqueCounts;
	_uniqueIndex = 0;

	int progressCounter = 0;

	while( getline( in, line ) ) {
		line.erase( line.find_last_not_of( " \n\r\t" ) + 1 );

		vector< string > tokens = _tokenizeString( line, " \t" );
		if( tokens.size() < 1 ) continue;

		//the line should have a single character that lets us know if it's a...
		if( !tokens[0].compare( "#" ) || tokens[0].find_first_of("#") == 0 ) {								// comment ignore
		} else if( !tokens[0].compare( "o" ) ) {						// object name ignore
			numObjects++;
		} else if( !tokens[0].compare( "g" ) ) {						// polygon group name ignore
			numGroups++;
		} else if( !tokens[0].compare( "v" ) ) {						//vertex
			numVertices++;

			double x = atof( tokens[1].c_str() ),
				     y = atof( tokens[2].c_str() ),
				     z = atof( tokens[3].c_str() );

			if( x < minX ) minX = x;
			if( x > maxX ) maxX = x;
			if( y < minY ) minY = y;
			if( y > maxY ) maxY = y;
			if( z < minZ ) minZ = z;
			if( z > maxZ ) maxZ = z;
		} else if( !tokens.at(0).compare( "vn" ) ) {                    //vertex normal
			numNormals++;
		} else if( !tokens.at(0).compare( "vt" ) ) {                    //vertex tex coord
			numTexCoords++;
		} else if( !tokens.at(0).compare( "f" ) ) {                     //face!

			//now, faces can be either quads or triangles (or maybe more?)
			//split the string on spaces to get the number of verts+attrs.
			vector<string> faceTokens = _tokenizeString(line, " ");

			for(long unsigned int i = 1; i < faceTokens.size(); i++) {
				if( uniqueCounts.find( faceTokens[i] ) == uniqueCounts.end() ) {
					uniqueCounts.insert( pair<string,unsigned long int>(faceTokens[i],_uniqueIndex) );
					_uniqueIndex++;
				}

				//need to use both the tokens and number of slashes to determine what info is there.
				vector<string> groupTokens = _tokenizeString(faceTokens[i], "/");
				int numSlashes = 0;
				for( long unsigned int j = 0; j < faceTokens[i].length(); j++ ) {
					if(faceTokens[i][j] == '/') numSlashes++;
				}

				//based on combination of number of tokens and slashes, we can determine what we have.
				if(groupTokens.size() == 2 && numSlashes == 1) {
					_hasVertexTexCoords = true;
				} else if(groupTokens.size() == 2 && numSlashes == 2) {
					_hasVertexNormals = true;
				} else if(groupTokens.size() == 3) {
					_hasVertexTexCoords = true;
					_hasVertexNormals = true;
				} else if(groupTokens.size() != 1) {
					if (ERRORS) fprintf(stderr, "[.obj]: [ERROR]: Malformed OBJ file, %s.\n", _filename);
					return false;
				}
			}

			numTriangles += (faceTokens.size() - 1 - 3 + 1);

			numFaces++;
		} else {

		}

		if (INFO) {
			progressCounter++;
			if( progressCounter % 5000 == 0 ) {
				printf("\33[2K\r");
				switch( progressCounter ) {
					case 5000:	printf("[.obj]: scanning %s...\\", _filename);	break;
					case 10000:	printf("[.obj]: scanning %s...|", _filename);	break;
					case 15000:	printf("[.obj]: scanning %s.../", _filename);	break;
					case 20000:	printf("[.obj]: scanning %s...-", _filename);	break;
				}
				fflush(stdout);
			}
			if( progressCounter == 20000 )
				progressCounter = 0;
		}
	}
	in.close();

	if (INFO) {
		printf( "\33[2K\r" );
		printf( "[.obj]: scanning %s...done!\n", _filename );
		printf( "[.obj]: Vertices:  \t%lu\tNormals:  \t%lu\tTex Coords:\t%lu\n", numVertices, numNormals, numTexCoords );
		printf( "[.obj]: Faces:     \t%lu\tTriangles:\t%lu\n", numFaces, numTriangles );
		printf( "[.obj]: Objects:   \t%lu\tGroups:   \t%lu\n", numObjects, numGroups );
		printf( "[.obj]: Dimensions:\t(%f, %f, %f)\n", (maxX - minX), (maxY - minY), (maxZ - minZ) );
	}

	_vertices = (GLfloat*)malloc(sizeof(GLfloat) * _uniqueIndex * 3);
	_texCoords = (GLfloat*)malloc(sizeof(GLfloat) * _uniqueIndex * 2);
	_normals = (GLfloat*)malloc(sizeof(GLfloat) * _uniqueIndex * 3);
	_indices = (unsigned short*)malloc(sizeof(unsigned short) * numTriangles * 3);

	GLfloat* v = (GLfloat*)malloc(sizeof(GLfloat) * numVertices * 3);
	GLfloat* vt = (GLfloat*)malloc(sizeof(GLfloat) * numTexCoords * 2);
	GLfloat* vn = (GLfloat*)malloc(sizeof(GLfloat) * numNormals * 3);

	uniqueCounts.clear();
	_uniqueIndex = 0;
	_numIndices = 0;

	in.open( _filename );

	unsigned long int vSeen = 0, vtSeen = 0, vnSeen = 0, indicesSeen = 0;

	while( getline( in, line ) ) {
		line.erase( line.find_last_not_of( " \n\r\t" ) + 1 );

		vector< string > tokens = _tokenizeString( line, " \t" );
		if( tokens.size() < 1 ) continue;

		//the line should have a single character that lets us know if it's a...
		if( !tokens[0].compare( "#" ) || tokens[0].find_first_of("#") == 0 ) {								// comment ignore
		} else if( !tokens[0].compare( "o" ) ) {						// object name ignore

		} else if( !tokens[0].compare( "g" ) ) {						// polygon group name ignore

		} else if( !tokens[0].compare( "mtllib" ) ) {					// material library

		} else if( !tokens[0].compare( "usemtl" ) ) {					// use material library

		} else if( !tokens[0].compare( "s" ) ) {						// smooth shading

		} else if( !tokens[0].compare( "v" ) ) {						//vertex
			double x = atof( tokens[1].c_str() ),
				     y = atof( tokens[2].c_str() ),
				     z = atof( tokens[3].c_str() );

			v[vSeen*3 + 0] = x;
			v[vSeen*3 + 1] = y;
			v[vSeen*3 + 2] = z;

			vSeen++;
		} else if( !tokens.at(0).compare( "vn" ) ) {                    //vertex normal
			double x = atof( tokens[1].c_str() ),
				     y = atof( tokens[2].c_str() ),
				     z = atof( tokens[3].c_str() );

			vn[vnSeen*3 + 0] = x;
			vn[vnSeen*3 + 1] = y;
			vn[vnSeen*3 + 2] = z;

			vnSeen++;
		} else if( !tokens.at(0).compare( "vt" ) ) {                    //vertex tex coord
			double s = atof( tokens[1].c_str() ),
						 t = atof( tokens[2].c_str() );

		  vt[vtSeen*2 + 0] = s;
			vt[vtSeen*2 + 1] = t;

			vtSeen++;
		} else if( !tokens.at(0).compare( "f" ) ) {                     //face!

			//now, faces can be either quads or triangles (or maybe more?)
			//split the string on spaces to get the number of verts+attrs.
			vector<string> faceTokens = _tokenizeString(line, " ");

			for(long unsigned int i = 1; i < faceTokens.size(); i++) {
				if( uniqueCounts.find( faceTokens[i] ) == uniqueCounts.end() ) {
					uniqueCounts.insert( pair<string,unsigned long int>(faceTokens[i],_uniqueIndex) );

					//need to use both the tokens and number of slashes to determine what info is there.
					vector<string> groupTokens = _tokenizeString(faceTokens[i], "/");
					int numSlashes = 0;
					for( long unsigned int j = 0; j < faceTokens[i].length(); j++ ) {
						if(faceTokens[i][j] == '/') numSlashes++;
					}

					//regardless, we always get a vertex index.
					int vI = atoi(groupTokens[0].c_str());
					if( vI < 0 )
						vI = vSeen + vI + 1;
					_vertices[ _uniqueIndex*3 + 0 ] = v[ ((vI - 1) * 3) + 0 ];
					_vertices[ _uniqueIndex*3 + 1 ] = v[ ((vI - 1) * 3) + 1 ];
					_vertices[ _uniqueIndex*3 + 2 ] = v[ ((vI - 1) * 3) + 2 ];

					//based on combination of number of tokens and slashes, we can determine what we have.
					if(groupTokens.size() == 2 && numSlashes == 1) {
						int vtI = atoi(groupTokens[1].c_str());
						if( vtI < 0 )
							vtI = vtSeen + vtI + 1;
						vtI--;
						_texCoords[ _uniqueIndex*2 + 0 ] = vt[ ((vtI - 1) * 2) + 0 ];
						_texCoords[ _uniqueIndex*2 + 1 ] = vt[ ((vtI - 1) * 2) + 1 ];
					} else if(groupTokens.size() == 2 && numSlashes == 2) {
						int vnI = atoi(groupTokens[1].c_str());
						if( vnI < 0 )
							vnI = vnSeen + vnI + 1;
						vnI--;
						_normals[ _uniqueIndex*3 + 0 ] = vn[ ((vnI - 1) * 3) + 0 ];
						_normals[ _uniqueIndex*3 + 1 ] = vn[ ((vnI - 1) * 3) + 1 ];
						_normals[ _uniqueIndex*3 + 2 ] = vn[ ((vnI - 1) * 3) + 2 ];
					} else if(groupTokens.size() == 3) {
						int vtI = atoi(groupTokens[1].c_str());
						if( vtI < 0 )
							vtI = vtSeen + vtI + 1;
						vtI--;
						_texCoords[ _uniqueIndex*2 + 0 ] = vt[ ((vtI - 1) * 2) + 0 ];
						_texCoords[ _uniqueIndex*2 + 1 ] = vt[ ((vtI - 1) * 2) + 1 ];

						int vnI = atoi(groupTokens[2].c_str());
						if( vnI < 0 )
							vnI = vnSeen + vnI + 1;
						vnI--;
						_normals[ _uniqueIndex*3 + 0 ] = vn[ ((vnI - 1) * 3) + 0 ];
						_normals[ _uniqueIndex*3 + 1 ] = vn[ ((vnI - 1) * 3) + 1 ];
						_normals[ _uniqueIndex*3 + 2 ] = vn[ ((vnI - 1) * 3) + 2 ];
					}

					_uniqueIndex++;
				}
			}

			for(long unsigned int i = 2; i < faceTokens.size()-1; i++) {
				_indices[ indicesSeen++ ] = uniqueCounts.find( faceTokens[1] )->second;
				_indices[ indicesSeen++ ] = uniqueCounts.find( faceTokens[i] )->second;
				_indices[ indicesSeen++ ] = uniqueCounts.find( faceTokens[i+1] )->second;

				_numIndices += 3;
			}

		} else {
			if (INFO) printf( "[.obj]: ignoring line: %s\n", line.c_str() );
		}

		if (INFO) {
			progressCounter++;
			if( progressCounter % 5000 == 0 ) {
				printf("\33[2K\r");
				switch( progressCounter ) {
					case 5000:	printf("[.obj]: parsing %s...\\", _filename);	break;
					case 10000:	printf("[.obj]: parsing %s...|", _filename);	break;
					case 15000:	printf("[.obj]: parsing %s.../", _filename);	break;
					case 20000:	printf("[.obj]: parsing %s...-", _filename);	break;
				}
				fflush(stdout);
			}
			if( progressCounter == 20000 )
				progressCounter = 0;
		}
	}

	in.close();

	if (INFO) {
		printf( "\33[2K\r" );
		printf( "[.obj]: parsing %s...done!\n", _filename );
	}

	glBindVertexArray( _vaod );
	glBindBuffer( GL_ARRAY_BUFFER, _vbods[0] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(GLfloat) * _uniqueIndex * 8, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, 																sizeof(GLfloat) * _uniqueIndex * 3, _vertices );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat) * _uniqueIndex * 3,sizeof(GLfloat) * _uniqueIndex * 3, _normals );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(GLfloat) * _uniqueIndex * 6,sizeof(GLfloat) * _uniqueIndex * 2, _texCoords );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _vbods[1] );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indicesSeen, _indices, GL_STATIC_DRAW );

	time(&end);
	double seconds = difftime( end, start );

	if (INFO) {
		printf( "[.obj]: Completed in %.3fs\n", seconds );
		printf( "[.obj]: -=-=-=-=-=-=-=-  END %s Info  -=-=-=-=-=-=-=- \n\n", _filename );
	}

	return result;
}

//
//  vector<string> tokenizeString(string input, string delimiters)
//
//      This is a helper function to break a single string into std::vector
//  of strings, based on a given set of delimiter characters.
//
vector<string> CSCI441::ModelLoader::_tokenizeString(string input, string delimiters) {
	if(input.size() == 0)
		return vector<string>();

	vector<string> retVec = vector<string>();
	size_t oldR = 0, r = 0;

	//strip all delimiter characters from the front and end of the input string.
	string strippedInput;
	int lowerValidIndex = 0, upperValidIndex = input.size() - 1;
	while((unsigned int)lowerValidIndex < input.size() && delimiters.find_first_of(input.at(lowerValidIndex), 0) != string::npos)
		lowerValidIndex++;

	while(upperValidIndex >= 0 && delimiters.find_first_of(input.at(upperValidIndex), 0) != string::npos)
		upperValidIndex--;

	//if the lowest valid index is higher than the highest valid index, they're all delimiters! return nothing.
	if((unsigned int)lowerValidIndex >= input.size() || upperValidIndex < 0 || lowerValidIndex > upperValidIndex)
		return vector<string>();

	//remove the delimiters from the beginning and end of the string, if any.
	strippedInput = input.substr(lowerValidIndex, upperValidIndex-lowerValidIndex+1);

	//search for each instance of a delimiter character, and create a new token spanning
	//from the last valid character up to the delimiter character.
	while((r = strippedInput.find_first_of(delimiters, oldR)) != string::npos)
	{
		if(oldR != r)           //but watch out for multiple consecutive delimiters!
			retVec.push_back(strippedInput.substr(oldR, r-oldR));
		oldR = r+1;
	}
	if(r != 0)
		retVec.push_back(strippedInput.substr(oldR, r-oldR));

	return retVec;
}
