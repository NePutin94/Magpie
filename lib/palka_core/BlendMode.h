//
// Created by NePutin on 9/9/2021.
//

#ifndef PALKA_BLENDMODE_H
#define PALKA_BLENDMODE_H

#include <glad/gl.h>

namespace palka
{
    class BlendMode
    {
    public:
        enum Factor
        {
            ZERO,
            ONE,
            SRC_COLOR,
            ONE_MINUS_SRC_COLOR,
            DST_COLOR,
            ONE_MINUS_DST_COLOR,
            SRC_ALPHA,
            ONE_MINUS_SRC_ALPHA,
            DST_ALPHA,
            ONE_MINUS_DST_ALPHA
        };
        enum Operation
        {
            Add,
            Subtract,
            ReverseSubtract,
            Min,
            Max
        };

        Factor colorSrcFactor;
        Factor colorDstFactor;
        Operation colorEquation;
        Factor alphaSrcFactor;
        Factor alphaDstFactor;
        Operation alphaEquation;

        constexpr BlendMode() :
                colorSrcFactor(BlendMode::SRC_ALPHA),
                colorDstFactor(BlendMode::ONE_MINUS_SRC_ALPHA),
                colorEquation(BlendMode::Add),
                alphaSrcFactor(BlendMode::ONE),
                alphaDstFactor(BlendMode::ONE_MINUS_SRC_ALPHA),
                alphaEquation(BlendMode::Add)
        {

        }

        constexpr BlendMode(Factor sFactor, Factor dFactor, Operation bOperation = Add)
                : colorSrcFactor(sFactor),
                  colorDstFactor(dFactor),
                  colorEquation(bOperation),
                  alphaSrcFactor(sFactor),
                  alphaDstFactor(dFactor),
                  alphaEquation(bOperation)
        {

        }

        constexpr BlendMode(Factor sFactor, Factor dFactor, Operation bOperation, Factor asFactor, Factor adFactor, Operation abOperation)
                : colorSrcFactor(sFactor),
                  colorDstFactor(dFactor),
                  colorEquation(bOperation),
                  alphaSrcFactor(asFactor),
                  alphaDstFactor(adFactor),
                  alphaEquation(abOperation)
        {

        }

        static constexpr auto enumToGlConstant(Factor f)
        {
            switch (f)
            {
                case ZERO:
                    return GL_ZERO;
                    break;
                case ONE:
                    return GL_ONE;
                    break;
                case SRC_COLOR:
                    return GL_SRC_COLOR;
                    break;
                case ONE_MINUS_SRC_COLOR:
                    return GL_ONE_MINUS_SRC_COLOR;
                    break;
                case DST_COLOR:
                    return GL_DST_COLOR;
                    break;
                case ONE_MINUS_DST_COLOR:
                    return GL_ONE_MINUS_DST_COLOR;
                    break;
                case SRC_ALPHA:
                    return GL_SRC_ALPHA;
                    break;
                case ONE_MINUS_SRC_ALPHA:
                    return GL_ONE_MINUS_SRC_ALPHA;
                    break;
                case DST_ALPHA:
                    return GL_DST_ALPHA;
                    break;
                case ONE_MINUS_DST_ALPHA:
                    return GL_ONE_MINUS_DST_ALPHA;
                    break;
            }
            return GL_ZERO;
        }

        static constexpr auto enumToGlConstant(Operation o)
        {
            switch (o)
            {
                case Add:
                    return GL_FUNC_ADD;
                    break;
                case Subtract:
                    return GL_FUNC_SUBTRACT;
                    break;
                case ReverseSubtract:
                    return GL_FUNC_REVERSE_SUBTRACT;
                    break;
                case Min:
                    return GL_MIN;
                    break;
                case Max:
                    return GL_MAX;
                    break;
            }
            return GL_FUNC_ADD;
        }

        static consteval auto BlendAlpha()
        {
            return BlendMode{BlendMode::SRC_ALPHA, BlendMode::ONE_MINUS_SRC_ALPHA, BlendMode::Add,
                             BlendMode::ONE, BlendMode::ONE_MINUS_SRC_ALPHA, BlendMode::Add};
        }

        static consteval auto BlendAdd()
        {
            return BlendMode{BlendMode::SRC_ALPHA, BlendMode::ONE, BlendMode::Add,
                             BlendMode::ONE, BlendMode::ONE, BlendMode::Add};
        }

        static consteval auto BlendMultiply()
        {
            return BlendMode{BlendMode::DST_COLOR, BlendMode::ZERO, BlendMode::Add};
        }

        static consteval auto BlendMin()
        {
            return BlendMode{BlendMode::ONE, BlendMode::ONE, BlendMode::Min};
        }

        static consteval auto BlendMax()
        {
            return BlendMode{BlendMode::ONE, BlendMode::ONE, BlendMode::Max};
        }

    };
}
#endif //PALKA_BLENDMODE_H
