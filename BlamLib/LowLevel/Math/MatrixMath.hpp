/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

#include "Math/MathTypes.hpp"

namespace CppMath
{
	void matrix4x3_identity(real_matrix4x3* mat);

	void matrix4x3_transpose(real_matrix4x3* mat);


	void matrix4x3_scale(real_matrix4x3* mat, const float scale);
};