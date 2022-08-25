#ifndef BILLET_UISCENE_H
#define BILLET_UISCENE_H

#include "UiView.h"
#include "DockWrapper.h"
#include <ranges>
#include <vector>

namespace Magpie
{
    class UiScene
    {
    private:
        std::unique_ptr<UiView> curr;
    protected:
        virtual void Next() = 0;

        template<class T>
        void addView(T&& view)
        {
            curr = std::move(std::make_unique<T>(view));
            curr->setCallback([this]()
                              { Next(); });
        }

        auto views()
        {
            return curr.get();
        }

        template<class T>
        T* getView()
        {
            return static_cast<T*>(curr.get());
        }

    public:
        virtual void init() = 0;

        virtual void render() = 0;

        virtual void update() = 0;
    };
}

#endif //BILLET_UISCENE_H
