#include "Application.h"
#include "config.h"
using namespace Magpie;

int main()
{
    Magpie::Application app(Magpie::Config::WindowSize);
    app.run();

    return 0;
}
