//
// Created by NePutin on 1/13/2023.
//

#ifndef MAGPIE_SPHEREMESH_H
#define MAGPIE_SPHEREMESH_H

#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "glm/common.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace palka
{
    class SphereMesh
    {
    private:
        VertexArrayObject vao;
        VertexBufferObject vbo;
        VertexBufferObject indecesVbo;
        float _radius;
        int _numSlices;
        int _numStacks;
        int _numVertices = 0;
        int _numIndices = 0;
        int _primitiveRestartIndex = 0;
        GLuint _numPoleIndices;
        GLuint _numBodyIndices;

        GLuint _northPoleIndexOffset;
        GLuint _bodyIndexOffset;
        GLuint _southPoleIndexOffset;

    public:
        SphereMesh(float radius, int numSlices, int numStacks, bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true)
                : _radius(radius), _numSlices(numSlices), _numStacks(numStacks), indecesVbo(BufferObject::BufferType::EAB)
        {

        }

        void init()
        {
            _numVertices = (_numStacks + 1) * (_numSlices + 1);

            const auto numBodyStacks = _numStacks - 2;
            const auto numBodyPrimitiveRestarts = std::max(0, numBodyStacks - 1);
            _numBodyIndices = 2 * numBodyStacks * (_numSlices + 1) + numBodyPrimitiveRestarts;

            _numPoleIndices = _numSlices * 3;

            _northPoleIndexOffset = 0;
            _bodyIndexOffset = _numPoleIndices;
            _southPoleIndexOffset = _bodyIndexOffset + _numBodyIndices;

            _numIndices = 2 * _numPoleIndices + _numBodyIndices;
            _primitiveRestartIndex = _numVertices;


            const auto sliceAngleStep = 2.0f * glm::pi<float>() / static_cast<float>(_numSlices);
            auto currentSliceAngle = 0.0f;
            std::vector<float> sliceSines, sliceCosines;
            for(auto i = 0; i <= _numSlices; i++)
            {
                sliceSines.push_back(sin(currentSliceAngle));
                sliceCosines.push_back(cos(currentSliceAngle));
                currentSliceAngle += sliceAngleStep;
            }
            const auto stackAngleStep = -glm::pi<float>() / static_cast<float>(_numStacks);
            auto currentStackAngle = glm::pi<float>() / 2.0f;
            std::vector<float> stackSines, stackCosines;
            for(auto i = 0; i <= _numStacks; i++)
            {
                stackSines.push_back(sin(currentStackAngle));
                stackCosines.push_back(cos(currentStackAngle));
                currentStackAngle += stackAngleStep;
            }


            palka::VertArray array;
            for(auto i = 0; i <= _numStacks; i++)
            {
                for(auto j = 0; j <= _numSlices; j++)
                {
                    const auto x = _radius * stackCosines[i] * sliceCosines[j];
                    const auto y = _radius * stackSines[i];
                    const auto z = _radius * stackCosines[i] * sliceSines[j];

                    const auto normalX = stackCosines[i] * sliceCosines[j];
                    const auto normalY = stackSines[i];
                    const auto normalZ = stackCosines[i] * sliceSines[j];
                    array.add(Vertex({x, y, z},Color(255, 0, 80), {}, {normalX, normalY, normalZ}));
                }
            }

            vao.create(_numVertices);
            vao.bind();
            vbo.create(sizeof(Vertex) * _numVertices);
            indecesVbo.create(sizeof(GLuint) * _numIndices);

            std::vector<GLuint> arr_indeces;
            for(auto i = 0; i < _numSlices; i++)
            {
                GLuint sliceIndex = i;
                GLuint nextSliceIndex = sliceIndex + _numSlices + 1;
                arr_indeces.emplace_back(static_cast<GLuint>(sliceIndex));
                arr_indeces.emplace_back(static_cast<GLuint>(nextSliceIndex));
                arr_indeces.emplace_back(static_cast<GLuint>(nextSliceIndex + 1));
            }

            // Then for body (triangle strip)
            GLuint currentVertexIndex = _numSlices + 1;
            for(auto i = 0; i < numBodyStacks; i++)
            {
                // Primitive restart triangle strip from second body stack on
                if(i > 0)
                {
                    arr_indeces.emplace_back(_primitiveRestartIndex);
                }

                for(auto j = 0; j <= _numSlices; j++)
                {
                    GLuint sliceIndex = currentVertexIndex + j;
                    GLuint nextSliceIndex = currentVertexIndex + _numSlices + 1 + j;
                    arr_indeces.emplace_back(sliceIndex);
                    arr_indeces.emplace_back(nextSliceIndex);
                }

                currentVertexIndex += _numSlices + 1;
            }

            // And finally south pole (triangles again)
            GLuint beforeLastStackIndexOffset = _numVertices - 2 * (_numSlices + 1);
            for(auto i = 0; i < _numSlices; i++)
            {
                GLuint sliceIndex = beforeLastStackIndexOffset + i;
                GLuint nextSliceIndex = sliceIndex + _numSlices + 1;
                arr_indeces.emplace_back(static_cast<GLuint>(sliceIndex));
                arr_indeces.emplace_back(static_cast<GLuint>(sliceIndex + 1));
                arr_indeces.emplace_back(static_cast<GLuint>(nextSliceIndex));
            }

            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);
            indecesVbo.setData(sizeof(GLuint) * arr_indeces.size(), &arr_indeces[0]);
            vao.linkEBO(indecesVbo);
            vao.linkVBO(vbo, sizeof(Vertex));
            vao.setPointer_dsa(vbo, 0, 3, GL_FLOAT, 0, false);
            vao.setPointer_dsa(vbo, 1, 2, GL_FLOAT, 32, false);
            vao.setPointer_dsa(vbo, 2, 3, GL_FLOAT, 40, false);

            vao.setPointers(vbo, sizeof(Vertex));
        }

        void render()
        {
            vao.bind();

            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(_primitiveRestartIndex);

            glDrawElements(GL_TRIANGLES, _numPoleIndices, GL_UNSIGNED_INT, (void*) (sizeof(GLuint) * _northPoleIndexOffset));

            glDrawElements(GL_TRIANGLE_STRIP, _numBodyIndices, GL_UNSIGNED_INT, (void*) (sizeof(GLuint) * _bodyIndexOffset));

            glDrawElements(GL_TRIANGLES, _numPoleIndices, GL_UNSIGNED_INT, (void*) (sizeof(GLuint) * _southPoleIndexOffset));

            glDisable(GL_PRIMITIVE_RESTART);
        }

    };
}

#endif //MAGPIE_SPHEREMESH_H
