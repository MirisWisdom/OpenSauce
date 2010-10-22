/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_point3d
	{
	mcpp_public
		mcpp_real X, Y, Z;
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_vector2d
	{
	mcpp_public
		mcpp_real I, J;
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_vector3d
	{
	mcpp_public
		mcpp_real I, J, K;
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
	};

	[System::Runtime::InteropServices::StructLayoutAttribute(System::Runtime::InteropServices::LayoutKind::Sequential)]
	public mcpp_struct real_euler_angles3d
	{
	mcpp_public
		mcpp_real Yaw, Pitch, Roll;
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