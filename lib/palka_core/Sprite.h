//
// Created by NePutin on 6/26/2021.
//

#ifndef PALKA_SPRITE_H
#define PALKA_SPRITE_H

#include <utility>
#include "Drawable.h"
#include "Texture.h"
#include "Rect.h"
#include "VertexData.h"
#include "Transform.h"
#include "TransformObject.h"

#ifdef REFLECTION_CORE
#include <rttr/type>
#include <rttr/wrapper_mapper.h>
#include <rttr/registration_friend>
#include "RawPtrWrapper.h"
#endif

namespace palka
{
    class Sprite : public Drawable, public TransformObject
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE(TransformObject)
        RTTR_REGISTRATION_FRIEND
#endif
    private:
        Raw_Ptr<Texture> texture;
        RectI src;
        VertArray vertex;
        void draw(Renderer& win) const override;
       // void draw(Renderer& win, BlendMode b) const override;
    public:

        Sprite() : vertex(VertArray::Quads) {};

        explicit Sprite(Texture& tex);

        void setTexture(Texture& tex, RectI rect = {});

        void setTextureRect(RectI rect);

        RectI getTextureRect() const;

        RectI getLocalRect() const;

        RectF getGlobalRect() const;

        friend class Window;
    };
}
#endif //PALKA_SPRITE_H
