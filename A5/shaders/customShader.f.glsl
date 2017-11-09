/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 330 core

uniform sampler2D textureMap;	// uniform set to bound texture
in vec2 texCoord2;

// TODO #F3
in vec3 theColor;

// TODO #D
out vec4 fragColorOut;

void main() {

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/
	
	vec4 texel = texture( textureMap, texCoord2 );
    
    // TODO #E
	fragColorOut = texel;//vec4(texCoord2,1,1);
	
    // TODO #F4
	//fragColorOut = vec4(theColor, 1.0);
}
