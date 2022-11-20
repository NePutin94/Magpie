//
// Created by NePutin on 9/7/2021.
//

#include <fstream>
#include <sstream>
#include <glad/gl.h>

#include "Shader.h"
#include "ConsoleLog.h"



void palka::Shader::load(std::string_view file_name)
{
    std::ifstream file(file_name.data());
    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        source = buffer.str();
    }
    file.close();
    compile();
}

void palka::Shader::compile()
{
    GLuint shader = glCreateShader(ENUM_TO_GL(type));
    const GLchar* src = (const GLchar*) source.c_str();
    glShaderSource(shader, 1, &src, 0);
    glCompileShader(shader);
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetShaderInfoLog(shader, logLen, &written, &log[0]);
            log.insert(0, "\n");
            Console::addLog(log, Console::error);
        }
        return;
    }
    shaderID = glCreateProgram();
    glAttachShader(shaderID, shader);
    glLinkProgram(shaderID);
    GLint isLinked = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, (int*) &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        std::string log(maxLength, ' ');
        glGetProgramInfoLog(shaderID, maxLength, &maxLength, &log[0]);
        Console::addLog(log, Console::error);
        log.insert(0, "\n");
        glDeleteProgram(shaderID);
        glDeleteShader(shader);
        return;
    }
    glDetachShader(shaderID, shader);
}

void palka::Shader::setValue(std::string_view name, float value)
{
    GLint loc = glGetUniformLocation((GLuint)shaderID, name.data());
    if (loc != -1)
    {
        glUniform1f(loc, value);
    }
}

void palka::Shader::setValue(std::string_view name, palka::Vec2f value)
{
    GLint loc = glGetUniformLocation((GLuint)shaderID, name.data());
    if (loc != -1)
    {
        glUniform2f(loc, value.x,value.y);
    }
}

void palka::Shader::loadVF(std::string_view file_name_fragment, std::string_view file_name_vertex)
{
    //LOAD SOURCE
    std::string source_f;
    std::string source_v;
    std::ifstream file1(file_name_fragment.data());
    if (file1.is_open())
    {
        std::stringstream buffer;
        buffer << file1.rdbuf();
        source_f = buffer.str();
    }
    file1.close();
    std::ifstream file2(file_name_vertex.data());
    if (file2.is_open())
    {
        std::stringstream buffer;
        buffer << file2.rdbuf();
        source_v = buffer.str();
    }
    file2.close();
    //COMPILE1
    GLuint shader_f = glCreateShader(ENUM_TO_GL(FRAGMENT));
    {
        const GLchar* src_f = (const GLchar*) source_f.c_str();
        glShaderSource(shader_f, 1, &src_f, 0);
        glCompileShader(shader_f);
        GLint isCompiled = 0;
        glGetShaderiv(shader_f, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint logLen;
            glGetShaderiv(shader_f, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0)
            {
                std::string log(logLen, ' ');
                GLsizei written;
                glGetShaderInfoLog(shader_f, logLen, &written, &log[0]);
                log.insert(0, "\n");
                Console::addLog(log, Console::error);
            }
            return;
        }
    }

    GLuint shader_v = glCreateShader(ENUM_TO_GL(VERTEX));
    {
        const GLchar* src_v = (const GLchar*) source_v.c_str();
        glShaderSource(shader_v, 1, &src_v, 0);
        glCompileShader(shader_v);
        GLint isCompiled = 0;
        glGetShaderiv(shader_v, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint logLen;
            glGetShaderiv(shader_v, GL_INFO_LOG_LENGTH, &logLen);
            if (logLen > 0)
            {
                std::string log(logLen, ' ');
                GLsizei written;
                glGetShaderInfoLog(shader_v, logLen, &written, &log[0]);
                log.insert(0, "\n");
                Console::addLog(log, Console::error);
            }
            return;
        }
    }

    shaderID = glCreateProgram();
    glAttachShader(shaderID, shader_v);
    glAttachShader(shaderID, shader_f);
    glLinkProgram(shaderID);

    GLint isLinked = 0;
    glGetProgramiv(shaderID, GL_LINK_STATUS, (int*) &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        std::string log(maxLength, ' ');
        glGetProgramInfoLog(shaderID, maxLength, &maxLength, &log[0]);
        Console::addLog(log, Console::error);
        log.insert(0, "\n");
        glDeleteProgram(shaderID);
        return;
    }
    glDeleteShader(shader_f);
    glDeleteShader(shader_v);
}


