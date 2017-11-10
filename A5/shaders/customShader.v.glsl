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

// TODO #A
uniform mat4 mvpMatrix;

// TODO #G1
uniform float time;

void main() {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    // TODO #C
	gl_Position = mvpMatrix * vec4(vPosition, 1);
    
    // TODO #F2
	texCoord2 = texCoord;
}
