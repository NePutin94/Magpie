#version 420 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 texCoord;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
// texture samplers
//uniform sampler2D texture1;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    // FragColor = texture(texture1, texCoord) * vertexColor;
    FragColor = vertexColor;
}

