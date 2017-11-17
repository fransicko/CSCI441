#include "Particle.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects3.hpp>

#include <math.h>
#include <stdlib.h>

Particle::Particle() {
	maxLife = 0;
	currentAge = 0;
	size = 1;
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(1, 1, 1); // This will contain the veleocty going in the three directions
}

Particle::Particle( glm::vec3 p, glm::vec3 v, double ml ) : maxLife(ml), velocity(v), position(p) {
	currentAge = 0;
	size = 1;
	positionI = p;
}

void Particle::update(double time) {
	// We wil update the x and z direction using the same method
	// x
	
	float yV = velocity.y - 9.81*currentAge;
	
	float xPos = positionI.x + velocity.x * currentAge;
	float zPos = positionI.z + velocity.z * currentAge;
	float yPos = positionI.y + yV * currentAge - (0.5f)*9.81*(currentAge*currentAge);
    
	position = glm::vec3(xPos, yPos, zPos);
	currentAge += time;
}

