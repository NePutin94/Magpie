//
// Created by NePutin on 9/28/2021.
//

#ifndef PALKA_SHADERPROGRAM_H
#define PALKA_SHADERPROGRAM_H

#include "Shader.h"
#include "ConsoleLog.h"
#include <glad/gl.h>
#include <type_traits>
#include "UniformWrapper.h"

namespace palka
{
    class ShaderProgram
    {
    public:

        explicit ShaderProgram(bool create = false) : isLinked(false), shaderProgramID(-1), isCreated(false)
        {
            if(create) createProgram();
        }

        ShaderProgram(ShaderProgram&& ot)
        {
            if(this == &ot)
                return;
            shaderProgramID = ot.shaderProgramID;
            isLinked = ot.isLinked;
            isCreated = ot.isCreated;
            uniformCache = std::move(ot.uniformCache);

            ot.shaderProgramID = -1;
            ot.isLinked = false;
            ot.isCreated = false;
        }

        ~ShaderProgram()
        {
            deleteProgram();
        }

        void addShader(const _Shader& shader)
        {
            if(isCreated)
                glAttachShader(shaderProgramID, shader.getId());
        }

        void createProgram()
        {
            shaderProgramID = glCreateProgram();
            isCreated = true;
        }

        void linkProgram()
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

        GLint getUniformLoc(std::string_view uniform_name)
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

        template<class T>
        void setUniform(std::string_view uniform_name, const T& value)
        {
            GLint loc = getUniformLoc(uniform_name);
            if(loc != -1)
                UniformWrapper<T>::setValue(value, loc);
        }

        template<class T>
        void setUniform(std::string_view uniform_name, const T* value, int count)
        {
            GLint loc = getUniformLoc(uniform_name);
            if(loc != -1)
                UniformWrapper<T>::setValue(value, loc, count);
        }

        unsigned int getUBOIndex(std::string_view block_index)
        {
            if(isLinked)
            {
                auto res = glGetUniformBlockIndex(shaderProgramID, block_index.data());
                return (res == GL_INVALID_INDEX) ? -1 : res;
            }
            return -1;
        }

        void UBOBindingTo(unsigned int ubo_index, unsigned int uniform_block_binding)
        {
            if(isLinked)
                glUniformBlockBinding(shaderProgramID, ubo_index, uniform_block_binding);
        }

        void deleteProgram()
        {
            if(!isLinked)
                return;
            glDeleteProgram(shaderProgramID);
            isLinked = false;
        }

        void bind()
        {
            glUseProgram(shaderProgramID);
        }

    private:
        unsigned int shaderProgramID;
        bool isLinked;
        bool isCreated;

        std::map<std::string, GLint> uniformCache;
    };
}

#endif //PALKA_SHADERPROGRAM_H
