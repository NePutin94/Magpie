//
// Created by NePutin on 8/15/2021.
//

#include "Color.h"
#include "config.h"

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    rttr::registration::class_<palka::Color>("Color")
            .property("red", &palka::Color::r)
            .property("green", &palka::Color::g)
            .property("blue", &palka::Color::b);
}
#endif