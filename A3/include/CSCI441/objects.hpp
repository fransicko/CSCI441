#ifndef __CSCI441_OBJECTS_HPP__
#define __CSCI441_OBJECTS_HPP__

#ifdef __APPLE__
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

#include <assert.h>   // for assert()
#include <math.h>			// for cos(), sin()

#include <CSCI441/teapot.hpp> // for teapot()

////////////////////////////////////////////////////////////////////////////////////

namespace CSCI441 {
		void drawSolidCone( GLdouble base, GLdouble height, GLint stacks, GLint slices );
		void drawWireCone( GLdouble base, GLdouble height, GLint stacks, GLint slices );

    void drawSolidCube( GLdouble sideLength );
		void drawWireCube( GLdouble sideLength );

		void drawSolidCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices );
		void drawWireCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices );

		void drawSolidDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings );
		void drawWireDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings );

		void drawSolidPartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep );
		void drawWirePartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep );

		void drawSolidSphere( GLdouble radius, GLint stacks, GLint slices );
		void drawWireSphere( GLdouble radius, GLint stacks, GLint slices );

		void drawSolidTeapot( GLdouble size );
		void drawWireTeapot( GLdouble size );

		void drawSolidTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings );
		void drawWireTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Internal rendering implementations to stay consistent with solid and wire modes
//
// Cone is drawn with a cylinder
// Disk is drawn with a partial disk

namespace CSCI441_INTERNAL {
	void drawCube( GLdouble sideLength, GLenum renderMode );
	void drawCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices, GLenum renderMode );
	void drawPartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep, GLenum renderMode );
	void drawSphere( GLdouble radius, GLint stacks, GLint slices, GLenum renderMode );
	void drawTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings, GLenum renderMode );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Outward facing function implementations

void CSCI441::drawSolidCone( GLdouble base, GLdouble height, GLint stacks, GLint slices ) {
	assert( base > 0.0f );
	assert( height > 0.0f );
	assert( stacks > 0 );
	assert( slices > 2 );

	CSCI441_INTERNAL::drawCylinder( base, 0.0f, height, stacks, slices, GL_FILL );
}

void CSCI441::drawWireCone( GLdouble base, GLdouble height, GLint stacks, GLint slices ) {
	assert( base > 0.0f );
	assert( height > 0.0f );
	assert( stacks > 0 );
	assert( slices > 2 );

	CSCI441_INTERNAL::drawCylinder( base, 0.0f, height, stacks, slices, GL_LINE );
}

void CSCI441::drawSolidCube( GLdouble sideLength ) {
  assert( sideLength > 0.0f );

  CSCI441_INTERNAL::drawCube( sideLength, GL_FILL );
}

void CSCI441::drawWireCube( GLdouble sideLength ) {
  assert( sideLength > 0.0f );

  CSCI441_INTERNAL::drawCube( sideLength, GL_LINE );
}

void CSCI441::drawSolidCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices ) {
	assert( (base >= 0.0f && top > 0.0f) || (base > 0.0f && top >= 0.0f) );
	assert( height > 0.0f );
	assert( stacks > 0 );
	assert( slices > 2 );

	CSCI441_INTERNAL::drawCylinder( base, top, height, stacks, slices, GL_FILL );
}

void CSCI441::drawWireCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices ) {
	assert( (base >= 0.0f && top > 0.0f) || (base > 0.0f && top >= 0.0f) );
	assert( height > 0.0f );
	assert( stacks > 0 );
	assert( slices > 2 );

	CSCI441_INTERNAL::drawCylinder( base, top, height, stacks, slices, GL_LINE );
}

void CSCI441::drawSolidDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings ) {
	assert( inner >= 0.0f );
	assert( outer > 0.0f );
	assert( outer > inner );
	assert( slices > 2 );
	assert( rings > 0 );

	CSCI441_INTERNAL::drawPartialDisk( inner, outer, slices, rings, 0, 2*M_PI, GL_FILL );
}

void CSCI441::drawWireDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings ) {
	assert( inner >= 0.0f );
	assert( outer > 0.0f );
	assert( outer > inner );
	assert( slices > 2 );
	assert( rings > 0 );

	CSCI441_INTERNAL::drawPartialDisk( inner, outer, slices, rings, 0, 2*M_PI, GL_LINE );
}

void CSCI441::drawSolidPartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep ) {
	assert( inner >= 0.0f );
	assert( outer > 0.0f );
	assert( outer > inner );
	assert( slices > 2 );
	assert( rings > 0 );
	assert( start >= 0.0f );
	assert( sweep <= 360.0f );

	CSCI441_INTERNAL::drawPartialDisk( inner, outer, slices, rings, start, sweep * M_PI / 360.0f, GL_FILL );
}

void CSCI441::drawWirePartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep ) {
	assert( inner >= 0.0f );
	assert( outer > 0.0f );
	assert( outer > inner );
	assert( slices > 2 );
	assert( rings > 0 );
	assert( start >= 0.0f );
	assert( sweep <= 360.0f );

	CSCI441_INTERNAL::drawPartialDisk( inner, outer, slices, rings, start, sweep * M_PI / 360.0f, GL_LINE );
}

void CSCI441::drawSolidSphere( GLdouble radius, GLint stacks, GLint slices ) {
	assert( radius > 0.0f );
	assert( stacks > 1 );
	assert( slices > 2 );

	CSCI441_INTERNAL::drawSphere( radius, stacks, slices, GL_FILL );
}

void CSCI441::drawWireSphere( GLdouble radius, GLint stacks, GLint slices ) {
	assert( radius > 0.0f );
	assert( stacks > 1);
	assert( slices > 2 );

	CSCI441_INTERNAL::drawSphere( radius, stacks, slices, GL_LINE );
}

void CSCI441::drawSolidTeapot( GLdouble size ) {
	assert( size > 0.0f );

	CSCI441_INTERNAL::teapot( 8, size, GL_FILL, false );
}

void CSCI441::drawWireTeapot( GLdouble size ) {
	assert( size > 0.0f );

	CSCI441_INTERNAL::teapot( 8, size, GL_LINE, false );
}

void CSCI441::drawSolidTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings ) {
	assert( innerRadius > 0.0f );
	assert( outerRadius > 0.0f );
	assert( sides > 2 );
	assert( rings > 2 );

	CSCI441_INTERNAL::drawTorus( innerRadius, outerRadius, sides, rings, GL_FILL );
}

void CSCI441::drawWireTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings ) {
	assert( innerRadius > 0.0f );
	assert( outerRadius > 0.0f );
	assert( sides > 2 );
	assert( rings > 2 );

	CSCI441_INTERNAL::drawTorus( innerRadius, outerRadius, sides, rings, GL_LINE );
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
// Internal function rendering implementations

void CSCI441_INTERNAL::drawCube( GLdouble sideLength, GLenum renderMode ) {
	GLdouble cornerPoint = sideLength / 2.0f;

  glPushAttrib( GL_POLYGON_BIT ); {

		glPolygonMode( GL_FRONT_AND_BACK, renderMode );

		// Left Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f( -1.0f, 0.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -cornerPoint, -cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -cornerPoint,  cornerPoint,  cornerPoint );
	  }; glEnd();

		// Right Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f(  1.0f, 0.0f, 0.0f );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  cornerPoint,  cornerPoint,  cornerPoint );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint, -cornerPoint );
	  }; glEnd();

		// Top Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f(  0.0f, 1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -cornerPoint,  cornerPoint,  cornerPoint );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  cornerPoint,  cornerPoint,  cornerPoint );
	  }; glEnd();

		// Bottom Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f(  0.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint, -cornerPoint );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -cornerPoint, -cornerPoint, -cornerPoint );
	  }; glEnd();

		// Front Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f(  0.0f, 0.0f, 1.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint,  cornerPoint );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -cornerPoint,  cornerPoint,  cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  cornerPoint,  cornerPoint,  cornerPoint );
	  }; glEnd();

		// Back Face
	  glBegin( GL_TRIANGLE_STRIP ); {
	    glNormal3f(  0.0f, 0.0f, -1.0f );
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  cornerPoint, -cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -cornerPoint,  cornerPoint, -cornerPoint );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -cornerPoint, -cornerPoint, -cornerPoint );
	  }; glEnd();

	}; glPopAttrib();
}

void CSCI441_INTERNAL::drawCylinder( GLdouble base, GLdouble top, GLdouble height, GLint stacks, GLint slices, GLenum renderMode ) {
	double sliceStep = 2.0 * M_PI / slices;
	double stackStep = height / stacks;

	glPushAttrib( GL_POLYGON_BIT ); {

		glPolygonMode( GL_FRONT_AND_BACK, renderMode );

		for( int stackNum = 0; stackNum < stacks; stackNum++ ) {
			GLdouble botRadius = base*(stacks-stackNum)/stacks + top*stackNum/stacks;
			GLdouble topRadius = base*(stacks-stackNum-1)/stacks + top*(stackNum+1)/stacks;

			glBegin( GL_TRIANGLE_STRIP ); {
				for( int sliceNum = 0; sliceNum <= slices; sliceNum++ ) {
					glNormal3f( cos( sliceNum * sliceStep ), 0.0f, sin( sliceNum * sliceStep ) );

					glTexCoord2f( cos( sliceNum * sliceStep ), (double)stackNum / stacks );
					glVertex3f( cos( sliceNum * sliceStep )*botRadius, stackNum     * stackStep, sin( sliceNum * sliceStep )*botRadius );

					glTexCoord2f( cos( sliceNum * sliceStep ), (double)(stackNum+1) / stacks );
					glVertex3f( cos( sliceNum * sliceStep )*topRadius, (stackNum+1) * stackStep, sin( sliceNum * sliceStep )*topRadius );
				}
			}; glEnd();
		}

	}; glPopAttrib();
}

void CSCI441_INTERNAL::drawPartialDisk( GLdouble inner, GLdouble outer, GLint slices, GLint rings, GLdouble start, GLdouble sweep, GLenum renderMode ) {
	double sliceStep = (sweep - start) / slices;
	double ringStep = (outer - inner) / rings;

	glPushAttrib( GL_POLYGON_BIT ); {

		glPolygonMode( GL_FRONT_AND_BACK, renderMode );

		for( int ringNum = 0; ringNum < rings-1; ringNum++ ) {
			double currRadius = inner + ringNum*ringStep;
			double nextRadius = inner + (ringNum+1)*ringStep;

			glBegin( GL_TRIANGLE_STRIP ); {
				glNormal3f( 0.0f, 0.0f, 1.0f );

				for( double theta = start; theta <= start + sweep; theta += sliceStep ) {
					glTexCoord2f( cos(theta)*(currRadius/outer), sin(theta)*(currRadius/outer) );
					glVertex3f( cos(theta)*currRadius, sin(theta)*currRadius, 0.0f );

					glTexCoord2f( cos(theta)*(nextRadius/outer), sin(theta)*(nextRadius/outer) );
					glVertex3f( cos(theta)*nextRadius, sin(theta)*nextRadius, 0.0f );
				}
			}; glEnd();
		}

	}; glPopAttrib();
}

void CSCI441_INTERNAL::drawSphere( GLdouble radius, GLint stacks, GLint slices, GLenum renderMode ) {
	double sliceStep = 2.0 * M_PI / slices;
	double stackStep = M_PI / stacks;

	glPushAttrib( GL_POLYGON_BIT ); {

		glPolygonMode( GL_FRONT_AND_BACK, renderMode );

		// sphere top
		glBegin( GL_TRIANGLE_FAN ); {
			double phi = stackStep * stacks;
			double phiNext = stackStep * (stacks-1);

			glNormal3f( 0.0f, 1.0f, 0.0f );
			glTexCoord2f( 1.0f, 1.0f );
			glVertex3f( 0.0f, -cos( phi )*radius, 0.0f );

			for( int sliceNum = 0; sliceNum <= slices; sliceNum++ ) {
				double theta = sliceStep * sliceNum;

				glNormal3f( -cos( theta )*sin( phiNext ),        -cos( phiNext ),        sin( theta )*sin( phiNext )        );
				glTexCoord2f( -cos( theta )*sin( phiNext ), -cos( phiNext ) );
				glVertex3f( -cos( theta )*sin( phiNext )*radius, -cos( phiNext )*radius, sin( theta )*sin( phiNext )*radius );
			}
		}; glEnd();

		// sphere stacks
		for( int stackNum = 1; stackNum < stacks-1; stackNum++ ) {
			double phi = stackStep * stackNum;
			double phiNext = stackStep * (stackNum + 1);

			glBegin( GL_TRIANGLE_STRIP ); {
				for( int sliceNum = slices; sliceNum >= 0; sliceNum-- ) {
					double theta = sliceStep * sliceNum;

					glNormal3f( -cos( theta )*sin( phi ),            -cos( phi )*radius,     sin( theta )*sin( phi )            );
					glTexCoord2f( -cos( theta )*sin( phi ),     -cos( phi ) );
					glVertex3f( -cos( theta )*sin( phi )*radius,     -cos( phi )*radius,     sin( theta )*sin( phi )*radius     );

					glNormal3f( -cos( theta )*sin( phiNext ),        -cos( phiNext )*radius, sin( theta )*sin( phiNext )        );
					glTexCoord2f( -cos( theta )*sin( phiNext ), -cos( phiNext ) );
					glVertex3f( -cos( theta )*sin( phiNext )*radius, -cos( phiNext )*radius, sin( theta )*sin( phiNext )*radius );
				}
			}; glEnd();
		}

		// sphere bottom
		glBegin( GL_TRIANGLE_FAN ); {
			double phi = 0;
			double phiNext = stackStep;

			glNormal3f( 0.0f, -1.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f );
			glVertex3f( 0.0f, -cos( phi )*radius, 0.0f );

			for( int sliceNum = slices; sliceNum >= 0; sliceNum-- ) {
				double theta = sliceStep * sliceNum;

				glNormal3f( -cos( theta )*sin( phiNext ),        -cos( phiNext ),        sin( theta )*sin( phiNext )        );
				glTexCoord2f( -cos( theta )*sin( phiNext ), -cos( phiNext ) );
				glVertex3f( -cos( theta )*sin( phiNext )*radius, -cos( phiNext )*radius, sin( theta )*sin( phiNext )*radius );
			}
		}; glEnd();

	}; glPopAttrib();
}

void CSCI441_INTERNAL::drawTorus( GLdouble innerRadius, GLdouble outerRadius, GLint sides, GLint rings, GLenum renderMode ) {
	double sideStep = 2.0 * M_PI / sides;
	double ringStep = 2.0 * M_PI / rings;

	glPushAttrib( GL_POLYGON_BIT ); {

		glPolygonMode( GL_FRONT_AND_BACK, renderMode );

		for( int ringNum = 0; ringNum < rings; ringNum++ ) {
			double currTheta = ringStep * ringNum;
			double nextTheta = ringStep * (ringNum+1);

			glBegin( GL_TRIANGLE_STRIP ); {
				for( int sideNum = 0; sideNum < sides; sideNum++ ) {
					double currPhi = sideStep * sideNum;
					double nextPhi = sideStep * (sideNum+1);

					glNormal3f( cos( currPhi ) * cos( currTheta ), cos( currPhi ) * sin( currTheta ), sin( currPhi ) );
					glTexCoord2f( cos( currPhi ) * cos( currTheta ), cos( currPhi ) * sin( currTheta ) );
					glVertex3f( ( outerRadius + innerRadius * cos( currPhi ) ) * cos( currTheta ), ( outerRadius + innerRadius * cos( currPhi ) ) * sin( currTheta ), innerRadius * sin( currPhi ) );

					glNormal3f( cos( currPhi ) * cos( nextTheta ), cos( currPhi ) * sin( nextTheta ), sin( currPhi ) );
					glTexCoord2f( cos( currPhi ) * cos( nextTheta ), cos( currPhi ) * sin( nextTheta ) );
					glVertex3f( ( outerRadius + innerRadius * cos( currPhi ) ) * cos( nextTheta ), ( outerRadius + innerRadius * cos( currPhi ) ) * sin( nextTheta ), innerRadius * sin( currPhi ) );

					glNormal3f( cos( nextPhi ) * cos( currTheta ), cos( nextPhi ) * sin( currTheta ), sin( nextPhi ) );
					glTexCoord2f( cos( nextPhi ) * cos( currTheta ), cos( nextPhi ) * sin( currTheta ) );
					glVertex3f( ( outerRadius + innerRadius * cos( nextPhi ) ) * cos( currTheta ), ( outerRadius + innerRadius * cos( nextPhi ) ) * sin( currTheta ), innerRadius * sin( nextPhi ) );

					glNormal3f( cos( nextPhi ) * cos( nextTheta ), cos( nextPhi ) * sin( nextTheta ), sin( nextPhi ) );
					glTexCoord2f( cos( nextPhi ) * cos( nextTheta ), cos( nextPhi ) * sin( nextTheta ) );
					glVertex3f( ( outerRadius + innerRadius * cos( nextPhi ) ) * cos( nextTheta ), ( outerRadius + innerRadius * cos( nextPhi ) ) * sin( nextTheta ), innerRadius * sin( nextPhi ) );
				}
			}; glEnd();
		}

	}; glPopAttrib();
}

#endif // __CSCI441_OBJECTS_HPP__
