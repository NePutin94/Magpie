//
// Created by NePutin on 9/8/2021.
//
#include <glad/gl.h>
#include "config.h"
#include "VertexData.h"

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Vertex>("Vertex")
            .constructor<palka::Vec2f, palka::Color, palka::Vec2f>()
            .property("pos", &palka::Vertex::pos)
            .property("color", &palka::Vertex::color)
            .property("texture pos", &palka::Vertex::texCoord);

    registration::class_<palka::VertArray>("VertArray")
            .constructor<>()
            .property("array", &palka::VertArray::vertecies);
}
#endif