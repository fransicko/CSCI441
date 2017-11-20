#ifndef _PARTICLE_H_
#define _PARTICLE_H_ 1

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <iostream>
using namespace std;

class Particle {
public:

	Particle();
	Particle( glm::vec3 p, glm::vec3 v, double ml );

	// MISCELLANEOUS
	struct Vertex { GLfloat x, y, z; };
	Vertex point[1];
	
	// Particle variables
  double maxLife;
  double currentAge;
  float size;
  glm::vec3 position;
  glm::vec3 positionI;
  glm::vec3 velocity; // This will contain the veleocty going in the three directions
  
  void draw(GLint &snow_size, GLuint &pointsVAO, GLuint &pointsVBO, GLuint textureHandle);
  void update(double time);


private:
};

#endif