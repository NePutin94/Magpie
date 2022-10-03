#ifndef MAGPIE_SIMPLEXMETHODVIEW_H
#define MAGPIE_SIMPLEXMETHODVIEW_H

#include "UiView.h"
#include "SimplexMethod.h"

namespace Magpie
{
    class SimplexMethodView : public UiView
    {
    private:
        SimplexMethod<double> s;
    public:
        SimplexMethodView(MatrixStorage<double> store)
        {
            s.init(store);
        }

        ~SimplexMethodView() override = default;

        void render() override;

        void update() override;

        void setEvents() override;
    };
}

#endif //MAGPIE_SIMPLEXMETHODVIEW_H
