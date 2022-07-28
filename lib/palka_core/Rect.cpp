//
// Created by NePutin on 6/23/2021.
//

#include "Rect.h"

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::RectI>("RectI")
            .constructor<palka::RectI>()
            .property("left", &palka::RectI::left)
            .property("top", &palka::RectI::top)
            .property("w", &palka::RectI::w)
            .property("h", &palka::RectI::h);

    registration::class_<palka::RectF>("RectF")
            .constructor<palka::RectF>()
            .property("left", &palka::RectF::left)
            .property("top", &palka::RectF::top)
            .property("w", &palka::RectF::w)
            .property("h", &palka::RectF::h);
}
#endif