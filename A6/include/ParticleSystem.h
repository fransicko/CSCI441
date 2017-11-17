#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_ 1

#include <glm/glm.hpp>

#include <GL/glew.h>

class ParticleSystem {
public:
	ParticleSystem( glm::vec3 p, glm::vec3 v, double ml );

	// MISCELLANEOUS
	glm::vec3 emitter;
	double angle;
	float minVol;
	float maxVol;
	float minLife;
	float maxLife;
	
	void update();
	void draw();

private:
};

#endif