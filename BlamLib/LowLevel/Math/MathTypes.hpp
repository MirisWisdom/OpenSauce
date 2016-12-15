/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

#include <math/MathTypesCpp.hpp>

//////////////////////////////////////////////////////////////////////////
// Managed
__MCPP_CODE_START__
using namespace System;
namespace LowLevel { namespace Math {

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_point2d
	{
	mcpp_public
		mcpp_real X, Y;

		real_point2d(mcpp_real x, mcpp_real y) : X(x), Y(y) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_point3d
	{
	mcpp_public
		mcpp_real X, Y, Z;

		real_point3d(mcpp_real x, mcpp_real y, mcpp_real z) : X(x), Y(y), Z(z) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_vector2d
	{
	mcpp_public
		mcpp_real I, J;

		real_vector2d(mcpp_real i, mcpp_real j) : I(i), J(j) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_vector3d
	{
	mcpp_public
		mcpp_real I, J, K;

		real_vector3d(mcpp_real i, mcpp_real j, mcpp_real k) : I(i), J(j), K(k) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_quaternion
	{
	mcpp_public
		real_vector3d Vector;
		mcpp_real W;
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_euler_angles2d
	{
	mcpp_public
		mcpp_real Yaw, Pitch;

		real_euler_angles2d(mcpp_real y, mcpp_real p) : Yaw(y), Pitch(p) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_euler_angles3d
	{
	mcpp_public
		mcpp_real Yaw, Pitch, Roll;

		real_euler_angles3d(mcpp_real y, mcpp_real p, mcpp_real r) : Yaw(y), Pitch(p), Roll(r) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_plane2d
	{
	mcpp_public
		real_vector2d Normal;
		mcpp_real D;
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_plane3d
	{
	mcpp_public
		real_vector3d Normal;
		mcpp_real D;
	};


	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_orientation3d
	{
	mcpp_public
		real_quaternion Rotation;
		real_point3d Translation;
		mcpp_real Scale;
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_matrix3x3
	{
	mcpp_public
		real_vector3d Forward, Left, Up;

		real_matrix3x3(real_vector3d forward, real_vector3d left, real_vector3d up) : Forward(forward), Left(left), Up(up) {}
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_matrix4x3
	{
	mcpp_public
		mcpp_real Scale;
		real_vector3d Forward, Left, Up;
		real_point3d Position;

		inline real_matrix4x3(mcpp_bool to_identity) { if(to_identity) ToIdentity(); }
		real_matrix4x3(mcpp_real scale);

		inline void FromCppMath(const CppMath::real_matrix4x3* mat)
		{
			this->Scale = mat->Scale;

			this->Forward.I = mat->Forward.I;
			this->Forward.J = mat->Forward.J;
			this->Forward.K = mat->Forward.K;

			this->Left.I = mat->Left.I;
			this->Left.J = mat->Left.J;
			this->Left.K = mat->Left.K;

			this->Up.I = mat->Up.I;
			this->Up.J = mat->Up.J;
			this->Up.K = mat->Up.K;

			this->Position.X = mat->Position.X;
			this->Position.Y = mat->Position.Y;
			this->Position.Z = mat->Position.Z;
		}

		void ToIdentity();
	};

}; };
__MCPP_CODE_END__