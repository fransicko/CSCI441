#include "ParticleSystem.h"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects3.hpp>

#include <math.h>
#include <stdlib.h>

ParticleSystem::ParticleSystem() {
	system = 'a';
	emitter = glm::vec3(0,0,0);
	angle = 0;
	minVol = 0;
	maxVol = 0;
	minLife = 0;
	maxLife = 0;
	num_part = 0;
}

ParticleSystem::ParticleSystem( char s, glm::vec3 e, double a, 
				float mV, float maVol, float minLif, 
				float maxLif, int numPart, double time ) {
					
	system = s;
	emitter = e;
	angle = a;
	minVol = mV;
	maxVol = maVol;
	minLife = minLif;
	maxLife = maxLif;
	num_part = numPart;
	
	textureHandle = CSCI441::TextureUtils::loadAndRegisterTexture( "../textures/snowflake.png" );
	timeI = time;
}

void ParticleSystem::update() {
	// LOOKHERE #2
	part.clear();
	float nAngle = (2*M_PI) / num_part;
	float xzAngle = nAngle;
	for( int i = 0; i < num_part; i++ ) {
		float vs = randNumber((int)maxVol, (int)minVol);	
		glm::vec3 speed = glm::vec3(vs*cos(xzAngle), vs*sin(angle * M_PI/180), vs*sin(xzAngle));
		part.push_back(Particle(emitter, speed, randNumber((int)maxLife, (int)minLife))); // life volocity position
		xzAngle += nAngle;
	}
}

GLint ParticleSystem::randNumber( int max, int min ) {
	//return rand() / (GLfloat)RAND_MAX * max * 2.0 - max;
	return rand()%((max - min) + 1) + min;
}

void ParticleSystem::draw(glm::vec3 eyePoint, glm::vec3 lookAtPoint, 
							GLint &snow_size, double eT,
							GLuint &pointsVAO, GLuint &pointsVBO) {
	glm::vec3 vVec = lookAtPoint - eyePoint;
    glm::normalize(vVec);
	glm::mat4 modelMtx;
	
	int orderedInd[num_part];
	double distances[num_part];
	for (int i = 0; i < num_part; ++i) {
		glm::vec4 p = glm::vec4(part.at(i).position.x, part.at(i).position.y, part.at(i).position.z, 1) * modelMtx;
		glm::vec4 ep = p - glm::vec4(eyePoint, 1);
		
		distances[i] = glm::dot(glm::vec4(vVec, 0), ep);
		orderedInd[i] = i;
	}
	
	for (int i = 0; i < num_part; ++i) {
		for (int j = i+1; j < num_part; ++j) {
			if (distances[i] < distances[j]) {
				double c = distances[j];
				distances[j] = distances[i];
				distances[i] = c;
				
				int cc = orderedInd[j];
				orderedInd[j] = orderedInd[i];
				orderedInd[i] = cc;
			}
		}
	}
	
	std::vector<Particle> orderedPoints;
	double endTime = eT;
	double timeD = endTime - timeI;
	timeI = endTime;
	for(int i = 0; i < num_part; ++i) {
		orderedPoints.push_back(part[ orderedInd[i] ]);
		part.at(orderedInd[i]).update(timeD);
	}
	
	/*
	glUniform1f(snow_size, 1.0f - (float)(part.at(0).currentAge / part.at(0).maxLife));
	glBindVertexArray( pointsVAO );
	
	// TODO #2 : send our sorted data
	glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(orderedPoints), orderedPoints);

	

	// LOOKHERE #4
	glBindTexture( GL_TEXTURE_2D, textureHandle );
	glDrawArrays( GL_POINTS, 0, num_part );
	*/
	std::cout << "\n Its in teh draw for ps" << std::endl;
	for (int i = 0; i < num_part; ++i) orderedPoints.at(i).draw(snow_size, pointsVAO, pointsVBO, textureHandle);
	// This will be the time for the snowflakes dieing so we make
	if (part[0].currentAge >= part[0].maxLife) {
		part.clear();
		update();;
		
	}
	
}