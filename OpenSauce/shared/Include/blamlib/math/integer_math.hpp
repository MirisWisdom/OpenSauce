/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>

namespace Yelo
{
	// 2 shorts representing 'x' and 'y'
	struct point2d
	{
		int16 x;
		int16 y;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};
#define pad_point2d PAD16 PAD16

	// 4 shorts, Top, Left, Bottom, Right
	struct rectangle2d
	{
		// Top
		int16 top;

		// Left
		int16 left;

		// Bottom
		int16 bottom;

		// Right
		int16 right;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};
#define pad_rectangle2d PAD16 PAD16 PAD16 PAD16

	// bounding in short integer values
	struct short_bounds
	{
		int16 lower;
		int16 upper;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};
};