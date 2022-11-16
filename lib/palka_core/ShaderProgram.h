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

        ~ShaderProgram()
        {
            deleteProgram();
        }

        void addShader(const _Shader& shader);

        void createProgram();

        void linkProgram();

        GLint getUniformLoc(std::string_view uniform_name);

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

        unsigned int getUBOIndex(std::string_view block_index);

        void UBOBindingTo(unsigned int ubo_index, unsigned int uniform_block_binding);

        void deleteProgram();

        void bind();

    private:
        unsigned int shaderProgramID;
        bool isLinked;
        bool isCreated;

        std::map<std::string, GLint> uniformCache;
    };
}

#endif //PALKA_SHADERPROGRAM_H
