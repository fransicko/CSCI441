/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 330 core

// TODO #B
in vec3 vPosition;

// TODO #F1

// TODO #A
uniform mat4 mvpMatrix;

// TODO #G1

void main() {
    /*****************************************/
    /********* Vertex Calculations  **********/
    /*****************************************/
    
    // TODO #G2
    
    // TODO #C
	gl_Position = mvpMatrix * vec4(vPosition, 1);
    
    // TODO #F2
}
