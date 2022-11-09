#ifndef MAGPIE_CONFIG_H
#define MAGPIE_CONFIG_H

#include "Vec2.h"
#include <Color.h>

namespace Magpie
{
    struct Config
    {
        static inline palka::Vec2f WindowSize = {1600, 900};
    };
    namespace Colors
    {
        /////////////////////
        // s  - selected  //
        // us - unselected//
        // h  - hovered   //
        // r  - rect      //
        ////////////////////
        constexpr palka::Color OutLine_r = palka::Color(39, 221, 240, 255);
        constexpr palka::Color Rect_s = palka::Color(59, 114, 228, 255);
        constexpr palka::Color Rect_us = palka::Color(221, 35, 67, 255);
        constexpr palka::Color Node = palka::Color(255, 228, 226, 255);
        constexpr palka::Color Tool_us = palka::Color(47, 35, 221, 255);
        constexpr palka::Color Tool_h = palka::Color(167, 35, 93, 255);
        constexpr palka::Color Tool_s = palka::Color(228, 28, 93, 255);
    }
}
#endif //MAGPIE_CONFIG_H
