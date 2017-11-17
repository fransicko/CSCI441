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
}

void Particle::update() {
	// We wil update the x and z direction using the same method
	// x
	
	float yV = velocity.y - 9.81*currentAge;
	
	float xPos = velocity.x * currentAge;
	float zPos = velocity.z * currentAge;
	float yPos = velocity.y * currentAge - (0.5)*9.81*(currentAge*currentAge);
    
	position = glm::vec3(xPos, yPos, zPos);
	currentAge += 0.0025;
}

