#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(location = 0) out vec2 vTexCoord;

layout(push_constant) uniform ModelUbo
{
    mat4 model;
}ubo;

void main() {

    // Pass the vertex coordinates onto the fragment shader
    vTexCoord = inTexCoord;

    // Pass on the vertex position over to the rasterizer
    gl_Position = ubo.model * vec4(inPosition, 1.0);    
}