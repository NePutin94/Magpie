//
// Created by NePutin on 8/5/2021.
//

#ifndef PALKA_OBJECT_H
#define PALKA_OBJECT_H

#include "../palka_core/Sprite.h"

#ifdef REFLECTION_CORE
#include <rttr/type>
#include <rttr/wrapper_mapper.h>
#include <rttr/registration_friend>
#endif

namespace palka
{
    class Object
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
#endif
    public:
        std::string name;
        bool isActive;
        Texture texture;
        Sprite sprite;
        Vec2f position;

    public:
        explicit Object(std::string_view name) : name(name), isActive(true)
        {}

        explicit Object(Texture&& textrue, std::string_view name) : name(name), isActive(true)
        {
            setTexture(std::move(texture), {});
        }

        void setPosition(Vec2f pos)
        {
            position = pos;
            sprite.setPosition(position);
        }

        Vec2f getPosition()
        {
            return position;
        }

        bool IsActive() const
        {
            return isActive;
        }

        std::string getName() const
        {
            return name;
        }

        void setTexture(Texture&& txt, RectI rect)
        {
            texture = std::move(txt);
            sprite.setTexture(texture, rect);
        }

        operator Drawable&()
        {
            return sprite;
        }
    };
}
#endif //PALKA_OBJECT_H
