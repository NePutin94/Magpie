//
// Created by NePutin on 9/28/2021.
//

#include "ShaderProgram.h"

void palka::ShaderProgram::createProgram()
{
    shaderProgramID = glCreateProgram();
    isCreated = true;
}

void palka::ShaderProgram::linkProgram()
{
    glLinkProgram(shaderProgramID);
    GLint _isLinked = 0;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int*) &_isLinked);
    if(_isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);
        std::string log(maxLength, ' ');
        glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &log[0]);
        Console::addLog(log, Console::error);
        log.insert(0, "\n");
        glDeleteProgram(shaderProgramID);
    } else
        isLinked = true;
}

void palka::ShaderProgram::addShader(const palka::_Shader& shader)
{
    if(isCreated)
        glAttachShader(shaderProgramID, shader.getId());
}

unsigned int palka::ShaderProgram::getUBOIndex(std::string_view block_index)
{
    if(isLinked)
    {
        auto res = glGetUniformBlockIndex(shaderProgramID, block_index.data());
        return (res == GL_INVALID_INDEX) ? -1 : res;
    }
    return -1;
}

void palka::ShaderProgram::UBOBindingTo(unsigned int ubo_index, unsigned int uniform_block_binding)
{
    if(isLinked)
        glUniformBlockBinding(shaderProgramID, ubo_index, uniform_block_binding);
}

GLint palka::ShaderProgram::getUniformLoc(std::string_view uniform_name)
{
    GLint loc = -1;
    if(uniformCache.contains(uniform_name.data()))
        loc = uniformCache[uniform_name.data()];
    else
    {
        loc = glGetUniformLocation((GLuint) shaderProgramID, uniform_name.data());
        uniformCache.emplace(uniform_name.data(), loc);
    }
    return loc;
}

void palka::ShaderProgram::deleteProgram()
{
    if(!isLinked)
        return;
    glDeleteProgram(shaderProgramID);
    isLinked = false;
}

void palka::ShaderProgram::bind()
{
    glUseProgram(shaderProgramID);
}
