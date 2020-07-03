/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#pragma once
#include <cseries/MacrosClr.hpp>

//////////////////////////////////////////////////////////////////////////
// Native
__CPP_CODE_START__
namespace CppMath
{
#include <PshPack1.h>
	cpp_struct real_point2d
	{
		cpp_real X, Y;
	};

	cpp_struct real_point3d
	{
		cpp_real X, Y, Z;
	};

	cpp_struct real_vector2d
	{
		cpp_real I, J;
	};

	cpp_struct real_vector3d
	{
		cpp_real I, J, K;
	};

	cpp_struct real_quaternion
	{
		real_vector3d Vector;
		cpp_real W;
	};

	cpp_struct real_euler_angles2d
	{
		cpp_real Yaw, Pitch;
	};

	cpp_struct real_euler_angles3d
	{
		cpp_real Yaw, Pitch, Roll;
	};

	cpp_struct real_plane2d
	{
		real_vector2d Normal;
		cpp_real D;
	};

	cpp_struct real_plane3d
	{
		real_vector3d Normal;
		cpp_real D;
	};


	cpp_struct real_orientation3d
	{
		real_quaternion Rotation;
		real_point3d Translation;
		cpp_real Scale;
	};

	cpp_struct real_matrix3x3
	{
		real_vector3d Forward, Left, Up;
	};

	cpp_struct real_matrix4x3
	{
		cpp_real Scale;
		real_vector3d Forward, Left, Up;
		real_point3d Position;
	};
#include <PopPack.h>
};
__CPP_CODE_END__