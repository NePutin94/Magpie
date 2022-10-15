#ifndef MAGPIE_SOLVERMEMENTO_H
#define MAGPIE_SOLVERMEMENTO_H

#include <memory>
#include <ConsoleLog.h>
#include <rttr/type>
#include <rttr/wrapper_mapper.h>
#include <rttr/registration_friend>
namespace Magpie
{
    template<class T>
    class IMementoData
    {
    public :
        virtual T createMemento() const = 0;

        virtual bool reinstateMemento(const T& mem) = 0;
    };

    template<class T>
    class MementoHistory
    {
        static constexpr short size = 16;
        bool makeSnapshot;

        int incIter()
        {
            undo_cap = undo_cap >= size - 1 ? size - 1 : ++undo_cap;
            redo_cap = 0;

            if(current + 1 >= size)
            {
                if(iter + 1 < size)
                    iter = current = iter + 1;
                else
                {
                    iter = current = 0;
                }

                return current;
            }
            iter++;
            return ++current;
        }

        int undOffset()
        {
            if(undo_cap <= 0)
                return -1;
            ++redo_cap;
            --undo_cap;
            if(iter == 0)
                return iter = size - 1;
            return --iter;
        }

        int redOffset()
        {
            if(redo_cap <= 0)
                return -1;
            --redo_cap;
            ++undo_cap;
            ++iter;
            if(iter >= size) iter = 0;
            return iter;
        }
    public:
        int current = -1; //index of the free-to-write cell
        int iter = -1; //index of the cell where the current state is stored
        int redo_cap;
        int undo_cap;
        std::array<std::unique_ptr<T>, size> curr;

        MementoHistory() = default;

        MementoHistory(const MementoHistory<T>& ot) = delete;

        MementoHistory(MementoHistory&&) = default;

        MementoHistory& operator=(MementoHistory&&) = default;

        void undo(T& mem)
        {
            if(auto i = undOffset(); i >= 0)
            {
                if(!mem.reinstateMemento(curr[i])) redOffset();
                else
                    palka::Console::fmt_log("Undo success, iter %i, redo_cap %i, undo_cap %i", palka::Console::system, iter, redo_cap, undo_cap);
            }
        }

        void redo(T& mem)
        {
            if(auto i = redOffset(); i >= 0)
            {
                if(!mem.reinstateMemento(curr[i])) undOffset();
                else
                    palka::Console::fmt_log("Redo success, iter %i, redo_cap %i, undo_cap %i", palka::Console::system, iter, redo_cap, undo_cap);
            }
        }

        void NeedSnapshot() //called when the asset state changes
        { makeSnapshot = true; }

        void update(const T& mem)
        {
            curr[incIter()] = std::make_unique<T>(mem.createMemento());
        }
    };
}

#endif //MAGPIE_SOLVERMEMENTO_H
