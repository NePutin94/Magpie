#ifndef MAGPIE_SCENEMANAGER_H
#define MAGPIE_SCENEMANAGER_H

#include "DataStorage.h"
#include "MenuView.h"
#include "InputView.h"
#include "InputMatrix.h"
#include "MagicInput.h"
#include "ArtificialBasisView.h"
#include "GraphicalMethodView.h"
#include "SimplexMethodView.h"
#include "GraphicalMethod3DView.h"
#include <stack>
#include "SimplexMethodAllView.h"

namespace Magpie
{

    class SceneManager
    {
        void fill(int var, DataStorage& storage)
        {
            switch(var)
            {
                case 0:
                    storage.data.alloc_matrix(3, 6);
                    storage.data.get(0, 0) = -2;
                    storage.data.get(1, 0) = -1;
                    storage.data.get(2, 0) = -3;
                    storage.data.get(3, 0) = -1;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 2;
                    storage.data.get(2, 1) = 5;
                    storage.data.get(3, 1) = -1;
                    storage.data.get(4, 1) = 0;
                    storage.data.get(5, 1) = 4;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -1;
                    storage.data.get(2, 2) = -1;
                    storage.data.get(3, 2) = 2;
                    storage.data.get(4, 2) = 0;
                    storage.data.get(5, 2) = 1;
                    break;
                case 1: //open
                    storage.data.alloc_matrix(4, 4);
                    storage.data.get(0, 0) = -3;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 2;
                    storage.data.get(2, 1) = 1;
                    storage.data.get(3, 1) = 7;

                    storage.data.get(0, 2) = 2;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = 1;
                    storage.data.get(3, 2) = 8;

                    storage.data.get(0, 3) = 0;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = 1;
                    storage.data.get(3, 3) = 3;
                    break;
                case 2://open
                    storage.data.alloc_matrix(4, 4);
                    storage.data.get(0, 0) = 1;
                    storage.data.get(1, 0) = 2;

                    storage.data.get(0, 1) = 2;
                    storage.data.get(1, 1) = -1;
                    storage.data.get(2, 1) = -1;
                    storage.data.get(3, 1) = -1;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -2;
                    storage.data.get(2, 2) = 1;
                    storage.data.get(3, 2) = 0;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = -1;
                    storage.data.get(3, 3) = 1;
                    break;
                case 3:
                    storage.data.alloc_matrix(5, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 2;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = 1;
                    storage.data.get(3, 1) = 5;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = 2;
                    storage.data.get(2, 2) = 1;
                    storage.data.get(3, 2) = 8;

                    storage.data.get(0, 3) = 5;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = 1;
                    storage.data.get(3, 3) = 9;

                    storage.data.get(0, 4) = 6;
                    storage.data.get(1, 4) = -1;
                    storage.data.get(2, 4) = 1;
                    storage.data.get(3, 4) = 9;
                    break;
                case 4:
                    storage.data.alloc_matrix(6, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 2;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 1) = 5;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = 2;
                    storage.data.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 2) = 8;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 3) = 1;

                    storage.data.get(0, 4) = -15;
                    storage.data.get(1, 4) = 2;
                    storage.data.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 4) = -8;

                    storage.data.get(0, 5) = -10;
                    storage.data.get(1, 5) = 5;
                    storage.data.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 5) = -8;
                    break;
                case 5:
                    storage.data.alloc_matrix(6, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 4;
                    storage.data.get(1, 1) = -1;
                    storage.data.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 1) = -2;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -2;
                    storage.data.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 2) = 0;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = 2;
                    storage.data.get(2, 3) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 3) = 1;

                    storage.data.get(0, 4) = -3;
                    storage.data.get(1, 4) = 4;
                    storage.data.get(2, 4) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 4) = -1;

                    storage.data.get(0, 5) = -3;
                    storage.data.get(1, 5) = 3;
                    storage.data.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 5) = -3;
                    break;
                case 6:
                    storage.data.alloc_matrix(7, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 2;
                    storage.data.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 1) = 8;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = 4;
                    storage.data.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 2) = 14;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = -2;
                    storage.data.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 3) = 1;

                    storage.data.get(0, 4) = -2;
                    storage.data.get(1, 4) = 4;
                    storage.data.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 4) = 12;

                    storage.data.get(0, 5) = 1;
                    storage.data.get(1, 5) = 1;
                    storage.data.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 5) = 1;

                    storage.data.get(0, 6) = 8;
                    storage.data.get(1, 6) = 1;
                    storage.data.get(2, 6) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 6) = 30;
                    break;
                case 7://open
                    storage.data.alloc_matrix(4, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = -1;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 1) = -2;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -10;
                    storage.data.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 2) = -30;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 3) = 12;
                    break;
                case 8://open
                    storage.data.alloc_matrix(4, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 1) = 1;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -1;
                    storage.data.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 2) = -2;

                    storage.data.get(0, 3) = 0;
                    storage.data.get(1, 3) = 1;
                    storage.data.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 3) = 3;
                case 9://open
                    storage.data.alloc_matrix(8, 4);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = -2;

                    storage.data.get(0, 1) = 6;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 1) = 2;

                    storage.data.get(0, 2) = 0;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 2) = 0;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = -4;
                    storage.data.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 3) = 5;

                    storage.data.get(0, 4) = 1;
                    storage.data.get(1, 4) = -2;
                    storage.data.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 4) = 6;

                    storage.data.get(0, 5) = 1;
                    storage.data.get(1, 5) = -1;
                    storage.data.get(2, 5) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 5) = 8;

                    storage.data.get(0, 6) = -2;
                    storage.data.get(1, 6) = 1;
                    storage.data.get(2, 6) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 6) = -26;

                    storage.data.get(0, 7) = -10;
                    storage.data.get(1, 7) = 1;
                    storage.data.get(2, 7) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 7) = 8;
                    break;
                case 10://open
                    storage.data.alloc_matrix(9, 4);
                    storage.data.get(0, 0) = 1;
                    storage.data.get(1, 0) = 2;

                    storage.data.get(0, 1) = 6;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 1) = 2;

                    storage.data.get(0, 2) = 0;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 2) = 0;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = -4;
                    storage.data.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 3) = 5;

                    storage.data.get(0, 4) = 1;
                    storage.data.get(1, 4) = -2;
                    storage.data.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 4) = 6;

                    storage.data.get(0, 5) = 1;
                    storage.data.get(1, 5) = -1;
                    storage.data.get(2, 5) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 5) = 8;

                    storage.data.get(0, 6) = -2;
                    storage.data.get(1, 6) = 1;
                    storage.data.get(2, 6) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 6) = -26;

                    storage.data.get(0, 7) = 2;
                    storage.data.get(1, 7) = 1;
                    storage.data.get(2, 7) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(3, 7) = 6;

                    storage.data.get(0, 8) = -9;
                    storage.data.get(1, 8) = 1;
                    storage.data.get(2, 8) = (int) Sign::LESSOREQUAL;
                    storage.data.get(3, 8) = 18;
                    break;
                case 11://3d
                    storage.data.alloc_matrix(3, 5);
                    storage.data.get(0, 0) = 1;
                    storage.data.get(1, 0) = 1;
                    storage.data.get(2, 0) = -1;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = 1;
                    storage.data.get(3, 1) = (int) Sign::LESSOREQUAL;
                    storage.data.get(4, 1) = 3;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = 3;
                    storage.data.get(3, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(4, 2) = 2;
                    break;
                case 12: //2d
                    storage.data.alloc_matrix(3, 6);
                    storage.data.get(0, 0) = -1;
                    storage.data.get(1, 0) = 0;
                    storage.data.get(2, 0) = 0;
                    storage.data.get(3, 0) = 1;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = 2;
                    storage.data.get(3, 1) = 1;
                    storage.data.get(4, 1) = (int) Sign::EQUAL;
                    storage.data.get(5, 1) = 5;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = -1;
                    storage.data.get(2, 2) = -1;
                    storage.data.get(3, 2) = 2;
                    storage.data.get(4, 2) = (int) Sign::EQUAL;
                    storage.data.get(5, 2) = 1;
                    break;
                case 13: //-24.2
                    storage.data.alloc_matrix(4, 7);
                    storage.data.get(0, 0) = -2;
                    storage.data.get(1, 0) = 2;
                    storage.data.get(2, 0) = -3;
                    storage.data.get(3, 0) = 3;
                    storage.data.get(4, 0) = 0;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = -2;
                    storage.data.get(2, 1) = 0;
                    storage.data.get(3, 1) = 1;
                    storage.data.get(4, 1) = 0;
                    storage.data.get(5, 1) = (int) Sign::EQUAL;
                    storage.data.get(6, 1) = 3;

                    storage.data.get(0, 2) = 0;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = 1;
                    storage.data.get(3, 2) = -2;
                    storage.data.get(4, 2) = 0;
                    storage.data.get(5, 2) = (int) Sign::EQUAL;
                    storage.data.get(6, 2) = 5;

                    storage.data.get(0, 3) = 0;
                    storage.data.get(1, 3) = 3;
                    storage.data.get(2, 3) = 0;
                    storage.data.get(3, 3) = 1;
                    storage.data.get(4, 3) = 1;
                    storage.data.get(5, 3) = (int) Sign::EQUAL;
                    storage.data.get(6, 3) = 4;
                    break;
                case 14://3d
                    storage.data.alloc_matrix(4, 5);
                    storage.data.get(0, 0) = 1;
                    storage.data.get(1, 0) = 1;
                    storage.data.get(2, 0) = -1;

                    storage.data.get(0, 1) = 1;
                    storage.data.get(1, 1) = 1;
                    storage.data.get(2, 1) = 1;
                    storage.data.get(3, 1) = (int) Sign::LESSOREQUAL;
                    storage.data.get(4, 1) = 8;

                    storage.data.get(0, 2) = 1;
                    storage.data.get(1, 2) = 1;
                    storage.data.get(2, 2) = 1;
                    storage.data.get(3, 2) = (int) Sign::GREATEROREQUAL;
                    storage.data.get(4, 2) = 2;

                    storage.data.get(0, 3) = 1;
                    storage.data.get(1, 3) = 0;
                    storage.data.get(2, 3) = 0;
                    storage.data.get(3, 3) = (int) Sign::LESSOREQUAL;
                    storage.data.get(4, 3) = 3;
                    break;
            }
        }

        std::unique_ptr<UiView> curr;
        std::stack<States> hist;
        DataStorage globData;
    public:
        void init()
        {
            create(States::InputNumber);
        }

        void updateScene()
        {
            auto st = curr->getState();
            if(curr->isGoToState())
            {
                while(hist.top() != st)
                    hist.pop();
                hist.pop();
            }
            create(st);
        }

        template<class T>
        void addView(T&& view)
        {
            curr = std::move(std::make_unique<T>(std::forward<T>(view)));
            curr->setCallback([this]()
                              { updateScene(); });
            curr->setData(globData);
            curr->setEvents();
        }

        void create(States s, bool save = true)
        {
            if(save && s != States::Back)
                hist.emplace(s);
            switch(s)
            {
                case States::Menu:
                    addView<MenuView>(MenuView{"Menu", palka::Vec2f{0.9, 0.9}});
                    break;
                case States::InputNumber:
                    addView<InputView>(InputView{"InputNumber", palka::Vec2f{0.9, 0.9}});
                    break;
                case States::InputRestriction:
                    addView<InputMatrix>(InputMatrix{"InputMatrix", palka::Vec2f{0.9, 0.9}});
                    break;
                case States::InputRestrictionGraph:
                    addView<MagicInput>(MagicInput{"MagicInput", palka::Vec2f{0.9, 0.9}});
                    break;
                case States::Back:
                    hist.pop();
                    create(hist.top(), false);
                    break;
                case States::SolverGraphics:
                    switch(globData.type)
                    {
                        case DataStorage::DOUBLE:
                            addView<GraphicalMethodView<double>>(GraphicalMethodView<double>{"GraphicalMethodView",
                                                                                             palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FLOAT:
                            addView<GraphicalMethodView<float>>(GraphicalMethodView<float>{"GraphicalMethodView",
                                                                                           palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FRACTUS:
                            addView<GraphicalMethodView<Fractus>>(GraphicalMethodView<Fractus>{"GraphicalMethodView",
                                                                                               palka::Vec2f{0.9, 0.9}});
                            break;
                    }
                    break;
                case States::SolverGraphics3D:
                    switch(globData.type)
                    {
                        case DataStorage::DOUBLE:
                            addView<GraphicalMethod3DView<double>>(GraphicalMethod3DView<double>{"GraphicalMethod3DView",
                                                                                                 palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FLOAT:
                            addView<GraphicalMethod3DView<float>>(GraphicalMethod3DView<float>{"GraphicalMethod3DView",
                                                                                               palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FRACTUS:
                            addView<GraphicalMethod3DView<Fractus>>(GraphicalMethod3DView<Fractus>{"GraphicalMethod3DView",
                                                                                                   palka::Vec2f{0.9, 0.9}});
                            break;
                    }
                    break;
                case States::SolverSimplex:
                    switch(globData.type)
                    {
                        case DataStorage::DOUBLE:
                            addView<SimplexMethodView<double>>(SimplexMethodView<double>{palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FLOAT:
                            addView<SimplexMethodView<float>>(SimplexMethodView<float>{palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FRACTUS:
                            addView<SimplexMethodView<Fractus>>(SimplexMethodView<Fractus>{palka::Vec2f{0.9, 0.9}});
                            break;
                    }
                    break;
                case States::SolverSimplexArtificialBasis:
                    switch(globData.type)
                    {
                        case DataStorage::DOUBLE:
                            addView<ArtificialBasisView<double>>(ArtificialBasisView<double>{palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FLOAT:
                            addView<ArtificialBasisView<float>>(ArtificialBasisView<float>{palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FRACTUS:
                            addView<ArtificialBasisView<Fractus>>(ArtificialBasisView<Fractus>{palka::Vec2f{0.9, 0.9}});
                            break;
                    }
                    break;
                case States::SolverSimplexOnePass:
                    switch(globData.type)
                    {
                        case DataStorage::DOUBLE:
                            addView<SimplexMethodAllView<double>>(SimplexMethodAllView<double>{palka::Vec2f{0.9, 0.9}});
                            break;
                        case DataStorage::FLOAT:

                            break;
                        case DataStorage::FRACTUS:
                            break;
                    }

                    break;
            }
        }

        void render(palka::Window& w)
        {
            curr->render(w);
        }

        void update()
        {
            curr->update();
        }

        void events()
        {

        }
    };
}

#endif //MAGPIE_SCENEMANAGER_H
