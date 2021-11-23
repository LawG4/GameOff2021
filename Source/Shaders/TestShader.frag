#version 450

layout(location = 0) in vec2 vTexCoord;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D texSampler;
void main() {

    // Get the colour from the texutre
    outColor = vec4(texture(texSampler, vTexCoord));

    // If this sprite part is empty then discard
    if(outColor.a <= 0.001)
    {
        discard;
    }
}