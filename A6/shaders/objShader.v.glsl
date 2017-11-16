/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 330 core

in vec2 texCoord;

// TODO #B
in vec3 vPosition;

// TODO #F1
out vec3 theColor;
out vec2 texCoord2;
out vec3 finalColor;


// TODO #A
uniform mat4 mvpMatrix;

uniform vec3 lightDiff;
uniform vec3 lightAmb;
uniform vec3 lightSpec;
uniform vec3 matDiff;
uniform vec3 matAmb;
uniform vec3 matSpec;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float shine;
in vec3 normalHandle;


// TODO #G1
uniform float time;

void main() {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    // TODO #C
	gl_Position = mvpMatrix * vec4(vPosition, 1);
    
    // light magic
	// normalize the pos
	vec3 vPos = normalize(vPosition);
	vec3 cPos = normalize(camPos);
	vec3 ltPos = normalize(lightPos);
	vec3 light = normalize(lightPos - vPosition);
	
	vec3 diffInten = lightDiff * matDiff * max(0, dot(light, normalize(normalHandle)));
	vec3 ambInten = lightAmb * matAmb;
	vec3 specInten = lightSpec * matSpec * pow(max(0, dot((ltPos + (cPos - vPos)), normalize(normalHandle))), shine);
	
	finalColor = diffInten + ambInten + specInten;
	
	
	
	
	
	
}
