//
// Created by dimka on 10/21/2021.
//

#ifndef PALKA_STATICMESH_H
#define PALKA_STATICMESH_H

#include <glm/trigonometric.hpp>
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"

namespace palka
{
    class StaticMesh
    {
    public:
        StaticMesh() : indecesVbo(BufferObject::BufferType::EAB)
        {
            mainSegments = 20;
            tubeSegments = 20;
            mainRadius = 3.0f;
            tubeRadius = 1.5f;
        }

        void init()
        {
            numVertices = (mainSegments + 1) * (tubeSegments + 1);
            primitiveRestartIndex = numVertices;
            numIndices = (mainSegments * 2 * (tubeSegments + 1)) + mainSegments - 1;
            vao.create(numVertices);
            vao.bind();
            vbo.create(sizeof(Vertex) * numVertices);
            indecesVbo.create(sizeof(GLuint) * numIndices);
            const auto mainSegmentAngleStep = glm::radians(360.0f / static_cast<float>(mainSegments));
            const auto tubeSegmentAngleStep = glm::radians(360.0f / static_cast<float>(tubeSegments));

            palka::VertArray array;
            auto currentMainSegmentAngle = 0.0f;
            for(auto i = 0; i <= mainSegments; i++)
            {
                // Calculate sine and cosine of main segment angle
                const auto sinMainSegment = sin(currentMainSegmentAngle);
                const auto cosMainSegment = cos(currentMainSegmentAngle);
                auto currentTubeSegmentAngle = 0.0f;
                for(auto j = 0; j <= tubeSegments; j++)
                {
                    // Calculate sine and cosine of tube segment angle
                    const auto sinTubeSegment = sin(currentTubeSegmentAngle);
                    const auto cosTubeSegment = cos(currentTubeSegmentAngle);

                    // Calculate vertex position on the surface of torus
                    auto surfacePosition = glm::vec3(
                            (mainRadius + tubeRadius * cosTubeSegment) * cosMainSegment,
                            (mainRadius + tubeRadius * cosTubeSegment) * sinMainSegment,
                            tubeRadius * sinTubeSegment);

                    array.add(surfacePosition);

                    // Update current tube angle
                    currentTubeSegmentAngle += tubeSegmentAngleStep;
                }
                // Update main segment angle
                currentMainSegmentAngle += mainSegmentAngleStep;
            }

            int z = 0;

            const auto mainSegmentTextureStep = 2.0f / static_cast<float>(mainSegments);
            const auto tubeSegmentTextureStep = 2.0f / static_cast<float>(tubeSegments);

            auto currentMainSegmentTexCoordV = 0.0f;
            for(auto i = 0; i <= mainSegments; i++)
            {
                auto currentTubeSegmentTexCoordU = 0.0f;
                for(auto j = 0; j <= tubeSegments; j++)
                {
                    auto textureCoordinate = glm::vec2(currentTubeSegmentTexCoordU, currentMainSegmentTexCoordV);
                    array[z++].texCoord = textureCoordinate;
                    currentTubeSegmentTexCoordU += tubeSegmentTextureStep;
                }

                // Update texture coordinate of main segment
                currentMainSegmentTexCoordV += mainSegmentTextureStep;
            }

            z = 0;
            currentMainSegmentAngle = 0.0f;
            for(auto i = 0; i <= mainSegments; i++)
            {
                // Calculate sine and cosine of main segment angle
                const auto sinMainSegment = sin(currentMainSegmentAngle);
                const auto cosMainSegment = cos(currentMainSegmentAngle);
                auto currentTubeSegmentAngle = 0.0f;
                for(auto j = 0; j <= tubeSegments; j++)
                {
                    // Calculate sine and cosine of tube segment angle
                    const auto sinTubeSegment = sin(currentTubeSegmentAngle);
                    const auto cosTubeSegment = cos(currentTubeSegmentAngle);

                    auto normal = glm::vec3(
                            cosMainSegment * cosTubeSegment,
                            sinMainSegment * cosTubeSegment,
                            sinTubeSegment
                    );
                    int _z = z++;
                    array[_z].normal = normal;
                    array[_z].color = Color(normal.x * 255, normal.y * 255, normal.z * 255);
                    // Update current tube angle
                    currentTubeSegmentAngle += tubeSegmentAngleStep;
                }

                // Update main segment angle
                currentMainSegmentAngle += mainSegmentAngleStep;
            }

            std::vector<GLuint> arr;
            GLuint currentVertexOffset = 0;
            for(auto i = 0; i < mainSegments; i++)
            {
                for(auto j = 0; j <= tubeSegments; j++)
                {
                    GLuint vertexIndexA = currentVertexOffset;
                    arr.emplace_back(vertexIndexA);
                    GLuint vertexIndexB = currentVertexOffset + tubeSegments + 1;
                    arr.emplace_back(vertexIndexB);
                    currentVertexOffset++;
                }
                if(i != mainSegments - 1)
                {
                    arr.emplace_back(primitiveRestartIndex);
                }
            }
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);
            indecesVbo.setData(sizeof(GLuint) * arr.size(), &arr[0]);
            vao.linkEBO(indecesVbo);
            vao.linkVBO(vbo, sizeof(Vertex));
            vao.setPointer_dsa(vbo, 0, 3, GL_FLOAT, 0, false);
            vao.setPointer_dsa(vbo, 1, 2, GL_FLOAT, 32, false);
            vao.setPointer_dsa(vbo, 2, 3, GL_FLOAT, 40, false);

            vao.setPointers(vbo, sizeof(Vertex));

            vbo.unbind();
            indecesVbo.unbind();
            vao.unbind();
        }

        void render()
        {
            vao.bind();
            glEnable(GL_PRIMITIVE_RESTART);
            glPrimitiveRestartIndex(primitiveRestartIndex);

            glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0);

            glDisable(GL_PRIMITIVE_RESTART);
        }

    private:
        int numVertices;
        int mainSegments;
        int primitiveRestartIndex;
        int numIndices;
        int tubeSegments;
        float mainRadius;
        float tubeRadius;

        VertexArrayObject vao;
        VertexBufferObject vbo;
        VertexBufferObject indecesVbo;
    };
}

#endif //PALKA_STATICMESH_H
