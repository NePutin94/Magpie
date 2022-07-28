#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 vertColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

layout(std140,binding=0) uniform matrixBuffer
{
    mat4 proj;
    mat4 view;
    mat4 model;
};

void main () {
    gl_Position =  proj  * view * model * vec4(aPos, 1.0);
    vertexColor = vertColor;
    texCoord = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}
