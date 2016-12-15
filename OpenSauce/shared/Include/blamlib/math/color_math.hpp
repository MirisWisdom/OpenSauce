/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/math/integer_math.hpp>
#include <blamlib/math/real_math.hpp>

namespace Yelo
{
	struct argb_color;
	// 0 - 255, represents a color
	struct rgb_color
	{
		PAD8;
		byte red;
		byte green;
		byte blue;

		OVERRIDE_OPERATOR_CAST_THIS(argb_color);
		OVERRIDE_OPERATOR_CAST_THIS(uint32);
	};
#define pad_rgb_color PAD32

	// 0 - 255, represents a color
	struct argb_color
	{
		byte alpha;
		byte red;
		byte green;
		byte blue;

		OVERRIDE_OPERATOR_CAST_THIS(rgb_color);
		OVERRIDE_OPERATOR_CAST_THIS(uint32);
	};
#define pad_argb_color PAD32

	// represents a color, range: 0 - 1
	struct real_rgb_color
	{
		real red;
		real green;
		real blue;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};
	#define pad_real_rgb_color PAD32 PAD32 PAD32

	// represents a color, range: 0 - 1
	struct real_argb_color
	{
		real alpha;
		real red;
		real green;
		real blue;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS_BY_FIELD(real_rgb_color, red);
	};
	#define pad_real_argb_color PAD32 PAD32 PAD32 PAD32
};