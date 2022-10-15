#ifndef MAGPIE_SIMPLEXMETHODVIEW_H
#define MAGPIE_SIMPLEXMETHODVIEW_H

#include "UiView.h"
#include "SimplexMethod.h"
#include "SolverMemento.h"

namespace Magpie
{
    class SimplexMethodView : public UiView
    {
    private:
        SimplexMethod<double> s;
        SimplexMethod2<double> s2;
        MementoHistory<SimplexMethod<double>> h;
    public:
        SimplexMethodView(SimplexMethodView&&) = default;

        SimplexMethodView& operator=(SimplexMethodView&&) = default;
        SimplexMethodView(MatrixStorage<double> store)
        {
            s.init(store);
            s2.init(store);
            s2.simplex_iterative();
        }

        ~SimplexMethodView() override = default;

        void render() override;

        void update() override;

        void setEvents() override;

    };
}

#endif //MAGPIE_SIMPLEXMETHODVIEW_H
