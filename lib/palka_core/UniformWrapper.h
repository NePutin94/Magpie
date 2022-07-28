//
// Created by dimka on 11/17/2021.
//

#ifndef PALKA_UNIFORMWRAPPER_H
#define PALKA_UNIFORMWRAPPER_H

#include "Vec2.h"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace palka
{
    template<class T>
    class UniformWrapper
    {
    public:
        template<class U>
        static void setValue(const U& value, GLint loc)
        requires std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<U>>>, T>
        {
            set(&value, loc, 1);
        }

        template<class U>
        static void setValue(const U* value, GLint loc, int count)
        requires std::is_same_v<std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<U>>>, T>
        {
            set(value, loc, count);
        }

    private:
        static void set(const int* val, GLint loc, int count)
        {
            glUniform1iv(loc, count, val);
        }

        static void set(const bool* val, GLint loc, int count)
        {
            set((int*) val, loc, count);
        }

        static void set(const float* val, GLint loc, int count)
        {
            glUniform1fv(loc, count, val);
        }

        static void set(const Vec2f* val, GLint loc, int count)
        {
            glUniform2fv(loc, count, glm::value_ptr(*val));
        }

        static void set(const Vec2i* val, GLint loc, int count)
        {
            glUniform2iv(loc, count, glm::value_ptr(*val));
        }

        static void set(const Vec3f* val, GLint loc, int count)
        {
            glUniform3fv(loc, count, glm::value_ptr(*val));
        }

        static void set(const Vec3i* val, GLint loc, int count)
        {
            glUniform3iv(loc, count, glm::value_ptr(*val));
        }

        static void set(const Mat2f* val, GLint loc, int count)
        {
            glUniformMatrix2fv(loc, count, GL_FALSE, glm::value_ptr(*val));
        }

        static void set(const Mat3f* val, GLint loc, int count)
        {
            glUniformMatrix3fv(loc, count, GL_FALSE, glm::value_ptr(*val));
        }

        static void set(const Mat4f* val, GLint loc, int count)
        {
            glUniformMatrix4fv(loc, count, GL_FALSE, glm::value_ptr(*val));
        }
    };
}

#endif //PALKA_UNIFORMWRAPPER_H
