/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/math/real_math.hpp>

namespace Yelo
{
	struct real_matrix3x3
	{
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
	};
	#define pad_real_matrix3x3	PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d)

	struct real_matrix4x3
	{
		real Scale;
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
		real_point3d Position;

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		void ConvertTo4x4(D3DMATRIX& d3dmat) const
		{
			d3dmat._11 = Forward.i;		d3dmat._12 = Forward.j;		d3dmat._13 = Forward.k;		d3dmat._14 = 0.0f;
			d3dmat._21 = Left.i;		d3dmat._22 = Left.j;		d3dmat._23 = Left.k;		d3dmat._24 = 0.0f;
			d3dmat._31 = Up.i;			d3dmat._32 = Up.j;			d3dmat._33 = Up.k;			d3dmat._34 = 0.0f;
			d3dmat._41 = Position.x;	d3dmat._42 = Position.y;	d3dmat._43 = Position.z;	d3dmat._44 = Scale;
		}
#endif
	};
	#define pad_real_matrix4x3	PAD32 \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_point3d)

	struct real_matrix3x4
	{
		float m[4][3];
	};
};