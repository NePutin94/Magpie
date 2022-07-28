//
// Created by NePutin on 8/5/2021.
//

#include "Object.h"

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Object>("Object")
            .constructor<std::string_view>()
            .property("sprite", &palka::Object::sprite)
            .property_readonly("name", &palka::Object::getName)
            .property_readonly("isActive", &palka::Object::IsActive);
}
#endif