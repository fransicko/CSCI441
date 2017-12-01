#version 330 core

in vec2 texCoord;

uniform sampler2D fbo;

out vec4 fragColorOut;

void main() {
    vec4 texel = texture( fbo, texCoord );
    
    vec4 finalColor = texel;
    
    // TODO #A
    
    float texelSum = texel.r + texel.g + texel.b;
    texelSum /= 3.0;
    finalColor = vec4(texelSum,texelSum,texelSum,1);
    
    fragColorOut = clamp(finalColor, 0.0, 1.0);
}
