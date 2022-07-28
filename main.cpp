#include "Application.h"

using namespace Billet;

using f_pair = std::pair<float, float>;

auto intersection(f_pair A, f_pair B, f_pair C)
{
    f_pair out;
    float det = A.first * B.second - A.second * B.first;
    out.first = -(B.first * C.second - B.second * C.first) / det;
    out.second = -(A.second * C.first - A.first * C.second) / det;
    return out;
}

int main()
{
    std::vector<Billet::Plot2<float>> testArr;
    Billet::Plot2<float> A{1, 0, 0};
    Billet::Plot2<float> B{0, 1, 0};
    Billet::Plot2<float> C{1, 2, 3};
    Billet::Plot2<float> F{2, 1, 8};
    Billet::Plot2<float> Y{1, 2, 7};
    testArr.emplace_back(C);
    testArr.emplace_back(Y);
    testArr.emplace_back(A);
    testArr.emplace_back(B);
    testArr.emplace_back(F);
    std::vector<palka::Vec2f> points;
    for(int i = 0; i<testArr.size();++i)
    {
        for(int j = i+1;j<testArr.size();++j)
        {
            auto& v = testArr[i];
            auto& v2= testArr[j];
            if(v != v2)
            {
                auto in = intersection({v.a, v2.a}, {v.b, v2.b}, {v.c, v2.c});
                if(in.first >= 0 && in.second >= 0)
                {
                    points.emplace_back(palka::Vec2f {in.first, in.second});
                }
            }
        }
    }
    std::sort(testArr.begin(), testArr.end(), [](Billet::Plot2<float>& l, Billet::Plot2<float>& r)
    {
        auto in = intersection({l.a, r.a}, {l.b, r.b}, {r.c, l.c});
        bool ret = false;
        if(in.first >= 0 && in.second >= 0)
        {
            for(float i = 0.01; i < 0.2; i += 0.01f)
            {
                in.first += i;
                auto lv = l.getValueAtY(in.first);
                auto rv = r.getValueAtY(in.first);
                if(lv > rv)
                    ret = true;
            }
        }
        else{
            for(float i = 0; i < 0.2; i += 0.01f)
            {

                auto lv = l.getValueAtY(i);
                auto rv = r.getValueAtY(i);
                if(lv < rv)
                    ret = true;
            }
        }
        return ret;
    });

    Billet::Application app({1280, 720});
    app.run();
    return 0;
}
