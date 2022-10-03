#ifndef MAGPIE_ISOLVER_H
#define MAGPIE_ISOLVER_H
#include <fstream>
namespace Magpie
{
    template<class T, class Result>
    class Solver
    {
    private:
    protected:
        virtual std::string serialize() = 0;

        virtual void deserialize(const std::string&) = 0;

    public:
        virtual Result solve() = 0;

        void save(std::string_view path)
        {
            std::ofstream out(path.data());
            out << serialize();
            out.close();
        }

        void load()
        {}

        void saveTask()
        {}

        void loadTask()
        {}
    };
}
#endif //MAGPIE_ISOLVER_H
