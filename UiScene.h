//
// Created by dimka on 2/3/2022.
//

#ifndef BILLET_UISCENE_H
#define BILLET_UISCENE_H

#include "UiView.h"
#include "DockWrapper.h"
#include <ranges>
#include <vector>

namespace Billet
{
    class UiScene
    {
    private:
        using cont_type =  std::map<std::string, std::unique_ptr<UiView>>;
        cont_type w_inst;
    protected:
        template<class T>
        void addView(const std::string& name, T&& view)
        {
            w_inst.emplace(name, std::make_unique<T>(view));
        }

        cont_type& views()
        {
            return w_inst;
        }

    public:
        virtual void init() = 0;

        virtual void render() = 0;

        virtual void update() = 0;
    };
}

#endif //BILLET_UISCENE_H
