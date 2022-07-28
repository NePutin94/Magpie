//
// Created by NePutin on 6/26/2021.
//
#include "Window.h"
#include "Sprite.h"
#include "Rect.h"
#include "Renderer.h"

void palka::Sprite::setTexture(Texture& tex, RectI rect)
{
    texture.set_data(&tex);
    auto sz = texture.get_data()->getSize();
    if (rect != RectI::Zero())
        setTextureRect(rect);
    else
        rect = {0, 0, sz.x, sz.y};
    setTextureRect({0, 0, sz.x, sz.y});
    vertex.add(Vertex{{0, 0},
                       {0, 0}});
    vertex.add(Vertex{{(float) rect.w, 0},
                       {1,              0}});
    vertex.add(Vertex{{(float) rect.w, (float) rect.h},
                       {1,              1}});
    vertex.add(Vertex{{0, (float) rect.h},
                       {0, 1}});
}

void palka::Sprite::setTextureRect(palka::RectI rect)
{
    src = rect;
    setCenter({src.w / 2.f, src.h / 2.f});
}

palka::RectI palka::Sprite::getTextureRect() const
{
    return src;
}

palka::RectI palka::Sprite::getLocalRect() const
{
    return {0, 0, src.w, src.h};
}

palka::RectF palka::Sprite::getGlobalRect() const
{
    return RectF{getPosition().x, getPosition().y, (float) src.w * getScale().x, (float) src.h * getScale().y};
}

void palka::Sprite::draw(Renderer& win) const
{
    RenderContext context;
    //context.texture = texture.get_data();
   // context.transform = getTransform();
    win.draw(vertex, context);
}

palka::Sprite::Sprite(palka::Texture& tex) : vertex(VertArray::Type::Quads)
{
    setTexture(tex);
}

//void palka::Sprite::draw(palka::Renderer& win, palka::BlendMode b) const
//{
//    RenderContext context;
//    context.blend = b;
//    context.texture = texture.get_data();
//    context.transform = getTransform();
//    win.draw(vertex, context);
//}

#ifdef REFLECTION_CORE

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Sprite>("Sprite")
            .constructor<>()
            .property("setRect", &palka::Sprite::getTextureRect, &palka::Sprite::setTextureRect)
            .property("texture", &palka::Sprite::texture)
            .property("vertex", &palka::Sprite::vertex)
            .property("src", &palka::Sprite::src);
}
#endif