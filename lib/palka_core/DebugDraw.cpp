//
// Created by NePutin on 6/30/2021.
//

#include "DebugDraw.h"
#include <rttr/registration>

palka::Color palka::DebugDraw::point_color{80,220,255};
palka::Color palka::DebugDraw::line_color{255,0,89};

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::DebugDraw>("DebugDraw")
            .property("line color", &palka::DebugDraw::line_color)
            .property("point color", &palka::DebugDraw::point_color);
}

