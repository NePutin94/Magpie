#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 vertColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec2 texCoord;
out vec3 Normal;

layout(std140, binding=0) uniform matrixBuffer
{
    mat4 proj;
    mat4 view;
    mat4 model;
};

uniform mat4 offsets[125];

void main () {
    gl_Position =  proj  * view * model * vec4(aPos, 1.0);//gl_ModelViewProjectionMatrix proj * viewMat * model
    vertexColor = vertColor;
    texCoord = aTexCoord;
    Normal = aNormal;
}
