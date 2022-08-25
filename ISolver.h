#ifndef MAGPIE_ISOLVER_H
#define MAGPIE_ISOLVER_H
namespace Magpie
{
    template<class T, class Result>
    class Solver
    {
    private:
    protected:
    public:
        virtual Result solve() = 0;

        void save()
        {}

        void load()
        {}

        void saveTask()
        {}

        void loadTask()
        {}
    };
}
#endif //MAGPIE_ISOLVER_H
