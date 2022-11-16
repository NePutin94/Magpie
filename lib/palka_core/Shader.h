//
// Created by NePutin on 9/7/2021.
//

#ifndef PALKA_SHADER_H
#define PALKA_SHADER_H

#include <string>
#include "Vec2.h"
#include "Transform.h"
#include "UniformBuffer.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <fstream>
#include <sstream>
#include "ConsoleLog.h"


namespace palka
{
    class _Shader
    {
    public:
        enum Type
        {
            FRAGMENT,
            VERTEX,
            GEOMETRY
        };
    private:
        Type type;
        std::string source;
        bool isCompiled;
        unsigned int shaderID;
    public:

        explicit _Shader(Type t) : type(t), isCompiled(false), shaderID(-1)
        {}

        explicit _Shader(std::string_view f_path, Type t, bool _compile = true) : type(t), shaderID(-1), isCompiled(false)
        {
            load(f_path, _compile);
        }

        constexpr auto ENUM_TO_GL(palka::_Shader::Type t)
        {
            switch(t)
            {
                case palka::_Shader::FRAGMENT:
                    return GL_FRAGMENT_SHADER;
                    break;
                case palka::_Shader::VERTEX:
                    return GL_VERTEX_SHADER;
                    break;
                case palka::_Shader::GEOMETRY:
                    return GL_GEOMETRY_SHADER;
                    break;
            }
            return -1;
        }

        void compile();

        void load(std::string_view file_path, bool _compile = true);

        void delShader();

        ~_Shader()
        {
            delShader();
        }

        auto getId() const
        {
            return shaderID;
        }
    };

    class Shader
    {
    public:
        enum Type
        {
            FRAGMENT,
            VERTEX,
            GEOMETRY
        };

        Shader(Type t = Type::FRAGMENT) : type(t)
        {}

        Shader(const Shader&) = delete;

        constexpr auto ENUM_TO_GL(palka::Shader::Type t)
        {
            switch(t)
            {
                case palka::Shader::FRAGMENT:
                    return GL_FRAGMENT_SHADER;
                    break;
                case palka::Shader::VERTEX:
                    return GL_VERTEX_SHADER;
                    break;
                case palka::Shader::GEOMETRY:
                    return GL_GEOMETRY_SHADER;
                    break;
            }
            return -1;
        }

        void load(std::string_view file_name);

        void setValue(std::string_view name, float value);

        void setValue(std::string_view name, int value)
        {}

        void setValue(std::string_view name, Transform t);

        void setValue(std::string_view name, glm::mat4 t);

        void setValue(std::string_view name, glm::mat4* t, int count)
        {
            GLint loc = glGetUniformLocation((GLuint) shaderID, name.data());
            if(loc != -1)
            {
                glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(*t));
            }
        }

        void setValue(std::string_view name, Vec3f t)
        {
            GLint loc = glGetUniformLocation((GLuint) shaderID, name.data());
            if(loc != -1)
            {
                glUniform3f(loc, t.x, t.y, t.z);
            }
        }

        void setValue(std::string_view name, glm::vec3* t, int count)
        {
            GLint loc = glGetUniformLocation((GLuint) shaderID, name.data());
            if(loc != -1)
            {
                glUniform3fv(loc, count, &(t[0].x));
            }
        }

        void setValue(std::string_view name, Vec2f value);

        void UseUbo(UniformBuffer& b) //need to do this using a static method or move it to a separate entity
        {
            ubo = &b;
            unsigned int uboIndex = glGetUniformBlockIndex(shaderID, "matrixBuffer");
            glUniformBlockBinding(shaderID, uboIndex, 0);
            // ubo.create(sizeof(float[16]) * 3);
            ubo->bindToPoint(0);
        }

        void updateUBO(float* data, int L, int R)
        {
            ubo->setData(data, R, L);
        }

        void loadVF(std::string_view file_name_fragment, std::string_view file_name_vertex);

        unsigned int getId() const
        {
            return shaderID;
        }

    private:
        UniformBuffer* ubo;
        unsigned int shaderID;
        std::string source;
        Type type;
        unsigned int UBO;

        void compile();
    };
}

#endif //PALKA_SHADER_H
