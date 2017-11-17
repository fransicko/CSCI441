#ifndef _PARTICLE_H_
#define _PARTICLE_H_ 1

#include <glm/glm.hpp>

#include <GL/glew.h>

class Particle {
public:

	Particle();
	Particle( glm::vec3 p, glm::vec3 v, double ml );

	// MISCELLANEOUS
  double maxLife;
  double currentAge;
  float size;
  glm::vec3 position;
  glm::vec3 velocity; // This will contain the veleocty going in the three directions
  
  void update();


private:
};

#endif