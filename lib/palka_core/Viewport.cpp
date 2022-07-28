//
// Created by NePutin on 7/5/2021.
//

#include "Viewport.h"


#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Viewport>("Viewport")
            .constructor<palka::RectF>()
            .property("center", &palka::Viewport::getCenter, &palka::Viewport::setCenter)
            .property("rect", &palka::Viewport::rect)
            .property("size", &palka::Viewport::getSize, &palka::Viewport::setSize)
            .property("t", &palka::Viewport::t)
            .property("rotation", &palka::Viewport::getRotation, &palka::Viewport::setRotation)
            .property("needUpdate", &palka::Viewport::needUpdate);
}
#endif