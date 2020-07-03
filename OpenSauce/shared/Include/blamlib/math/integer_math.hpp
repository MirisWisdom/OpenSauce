/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
    struct point2d
    {
        int16 x;
        int16 y;

        friend bool operator==(const point2d& lhs, const point2d& rhs)
        {
            return lhs.x == rhs.x
                && lhs.y == rhs.y;
        }

        friend bool operator!=(const point2d& lhs, const point2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(int16);
    };

    ASSERT_SIZE(point2d, 0x4);

    struct rectangle2d
    {
        int16 top;
        int16 left;
        int16 bottom;
        int16 right;

        friend bool operator==(const rectangle2d& lhs, const rectangle2d& rhs)
        {
            return lhs.top == rhs.top
                && lhs.left == rhs.left
                && lhs.bottom == rhs.bottom
                && lhs.right == rhs.right;
        }

        friend bool operator!=(const rectangle2d& lhs, const rectangle2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(int16);
    };

    ASSERT_SIZE(rectangle2d, 0x8);

    struct short_bounds
    {
        int16 lower;
        int16 upper;

        friend bool operator==(const short_bounds& lhs, const short_bounds& rhs)
        {
            return lhs.lower == rhs.lower
                && lhs.upper == rhs.upper;
        }

        friend bool operator!=(const short_bounds& lhs, const short_bounds& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(int16);
    };

    ASSERT_SIZE(short_bounds, 0x4);

#define pad_point2d PAD16 PAD16
#define pad_rectangle2d PAD16 PAD16 PAD16 PAD16
#define pad_short_bounds PAD16 PAD16
};
