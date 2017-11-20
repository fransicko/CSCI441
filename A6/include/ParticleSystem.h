#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_ 1

#include <glm/glm.hpp>

#include <GL/glew.h>
#include <CSCI441/TextureUtils.hpp>   // our texture helper functions
#include "Particle.h"
#include <vector>

using namespace std;

class ParticleSystem {
public:


	struct Vertex { GLfloat x, y, z; };
	ParticleSystem();
	
	ParticleSystem(char s, glm::vec3 e, double a, 
				float mV, float maVol, float minLif, 
				float maxLif, int numPart, double time);

	// MISCELLANEOUS
	std::vector<Particle> part;
	char system;
	glm::vec3 emitter;
	double angle;
	float minVol;
	float maxVol;
	float minLife;
	float maxLife;
	int num_part;
	
	
	// Textures
	double timeI;
	GLint snow_size;
	GLuint textureHandle;
	
	void update();
	void draw(glm::vec3 eyePoint, glm::vec3 lookAtPoint, 
				GLint &snow_size, double eT,
				GLuint &pointsVAO, GLuint &pointsVBO);
	GLint randNumber( int max, int min );

private:
};

#endif