/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

#include "Math/MathTypes.hpp"

#include "Math/MatrixMath.hpp"

//////////////////////////////////////////////////////////////////////////
// Native
__CPP_CODE_START__


namespace CppMath
{
};
__CPP_CODE_END__

//////////////////////////////////////////////////////////////////////////
// Managed
__MCPP_CODE_START__
using namespace System;

namespace LowLevel { namespace Math {
	public mcpp_class Util abstract sealed
	{
	mcpp_public

		static mcpp_uint CompressRealToShortN(mcpp_real value);

		static mcpp_uint CompressRealVector3DToHenDN3(real_quaternion vector);
	};
}; };
__MCPP_CODE_END__