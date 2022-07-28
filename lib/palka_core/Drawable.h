//
// Created by NePutin on 7/25/2021.
//

#ifndef PALKA_DRAWABLE_H
#define PALKA_DRAWABLE_H

namespace palka
{
    class Renderer;

    class Drawable
    {
    private:
        virtual void draw(Renderer& win) const = 0;

    public:
        virtual ~Drawable() = default;

        friend class Renderer;
    };
}
#endif //PALKA_DRAWABLE_H
