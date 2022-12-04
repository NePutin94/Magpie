#include "../include/Application.h"
#include "../include/Plot.h"

using namespace Magpie;

int main()
{
    Magpie::Ray r1{{0, 0, 0},
                   {1, 0, 1}};
    Magpie::Ray r2{{2,  0, 0},
                   {-1, 0, 1}};
    auto re = r1.containsPoint({-12, 0, -11});
    auto res23 = r1.intersexctRay(r2);
    auto res33 = r1.intersects(r2);
    Magpie::Plot3D<float> p1{1, 0, 1, 0};
    Magpie::Plot3D<float> p2{1, 0, 0, -1};
    Magpie::Plot3D<float> p3{1, -1, 0, 1};
    palka::Vec3f t1;
    palka::Vec3f t2;
    auto res = p1.isect_plane_plane_to_normal_ray(p2, t1, t2);
    auto res2 = p2.isect_plane_plane_to_normal_ray(p1, t1, t2);
    // auto res2 = Magpie::Plot3D<float>::threePlaneIntersection(p1, p2, p3);
    Magpie::Application app(Magpie::Config::WindowSize);
    app.run();

    return 0;
}
