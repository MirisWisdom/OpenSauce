/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
    struct argb_color;

    struct rgb_color
    {
        PAD8;
        byte red;
        byte green;
        byte blue;

        friend bool operator==(const rgb_color& lhs, const rgb_color& rhs)
        {
            return lhs.red == rhs.red
                && lhs.green == rhs.green
                && lhs.blue == rhs.blue;
        }

        friend bool operator!=(const rgb_color& lhs, const rgb_color& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(argb_color);

        OVERRIDE_OPERATOR_CAST_THIS(uint32);
    };

    ASSERT_SIZE(rgb_color, 0x4);

    struct argb_color
    {
        byte alpha;
        byte red;
        byte green;
        byte blue;

        friend bool operator==(const argb_color& lhs, const argb_color& rhs)
        {
            return lhs.alpha == rhs.alpha
                && lhs.red == rhs.red
                && lhs.green == rhs.green
                && lhs.blue == rhs.blue;
        }

        friend bool operator!=(const argb_color& lhs, const argb_color& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(rgb_color);

        OVERRIDE_OPERATOR_CAST_THIS(uint32);
    };

    ASSERT_SIZE(argb_color, 0x4);

    struct real_rgb_color
    {
        real red;
        real green;
        real blue;

        friend bool operator==(const real_rgb_color& lhs, const real_rgb_color& rhs)
        {
            return lhs.red == rhs.red
                && lhs.green == rhs.green
                && lhs.blue == rhs.blue;
        }

        friend bool operator!=(const real_rgb_color& lhs, const real_rgb_color& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
    };

    ASSERT_SIZE(real_rgb_color, 0xC);

    struct real_argb_color
    {
        real alpha;
        real red;
        real green;
        real blue;

        friend bool operator==(const real_argb_color& lhs, const real_argb_color& rhs)
        {
            return lhs.alpha == rhs.alpha
                && lhs.red == rhs.red
                && lhs.green == rhs.green
                && lhs.blue == rhs.blue;
        }

        friend bool operator!=(const real_argb_color& lhs, const real_argb_color& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS_BY_FIELD(real_rgb_color, red);
    };

    ASSERT_SIZE(real_argb_color, 0x10);

#define pad_rgb_color           PAD32
#define pad_argb_color          PAD32
#define pad_real_rgb_color      PAD32 PAD32 PAD32
#define pad_real_argb_color     PAD32 PAD32 PAD32 PAD32
};
