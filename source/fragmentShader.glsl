#version 330 core

// Inputs
in vec2 UV;

// Outputs
out vec3 colour;

// Uniforms
uniform sampler2D diffuseMap;

void main()
{
    colour = vec3(texture(diffuseMap, UV));
}