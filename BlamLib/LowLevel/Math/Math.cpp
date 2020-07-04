/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#include "Precompile.hpp"
#include "Math/Math.hpp"

//////////////////////////////////////////////////////////////////////////
// Native
__CPP_CODE_START__
#include <string.h>


cpp_enum {
	_offset_real_vector3d_I = 0x0,
	_offset_real_vector3d_J = 0x4,
	_offset_real_vector3d_K = 0x8,
};

cpp_enum {
	_offset_real_point3d_X = 0x0,
	_offset_real_point3d_Y = 0x4,
	_offset_real_point3d_Z = 0x8,
};

cpp_enum {
	_offset_real_quaternion_Vector = 0x0,
	_offset_real_quaternion_W = 0xC,
};

cpp_enum {
	_offset_real_matrix4x3_Scale = 0x0,
	_offset_real_matrix4x3_Forward = 0x4,
	_offset_real_matrix4x3_Left = 0x10,
	_offset_real_matrix4x3_Up = 0x1C,
	_offset_real_matrix4x3_Position = 0x28,

	_sizeof_real_matrix4x3 = sizeof(CppMath::real_matrix4x3)
};

static const cpp_real K_REAL_ZERO = 0.0f;
static const cpp_real K_REAL_HALF = 0.5f;
static const cpp_real K_REAL_ONE = 1.0f;
static const cpp_real K_REAL_TWO = 2.0f;
static const cpp_real K_REAL_PI = 3.1415927f;
static const cpp_real K_REAL_PI_TWO = 6.2831855f;


#pragma region halo2 floating point compression
API_FUNC_NAKED static cpp_uint /*sub_441D20*/floor_it(cpp_real value)
{
#ifndef _WIN64
	API_FUNC_NAKED_START()
		push    ebx
		push    esi
		mov     esi, /*[esp+8+arg_0]*/ value
		mov     eax, esi
		sar     eax, 0x17
		and     eax, 0xFF       // exponent = (value >> 23) & 0xFF
		push    edi
		lea     edi, [eax-0x7F]
		mov     ecx, 0x96
		sub     ecx, eax
		mov     eax, esi
		and     eax, 0x7FFFFF
		or      eax, 0x800000
		sar     eax, cl
		mov     edx, esi
		sar     edx, 0x1F
		sar     edi, 0x1F
		xor     eax, edx
		mov     ebx, edi
		sub     eax, edx
		not     ebx
		and     eax, ebx
		test    esi, 0x7FFFFFFF
		jz      /*short*/ loc_441D8E
		test    edx, edx
		jz      /*short*/ loc_441D8A
		test    edi, edi
		jnz     /*short*/ loc_441D7F
		mov     edx, 1
		shl     edx, cl
		sub     edx, 1
		and     edx, esi
		test    edx, 0x7FFFFF
		jz      /*short*/ loc_441D8A
loc_441D7F:
		pop     edi
		mov     ecx, 1
		pop     esi
		sub     eax, ecx
		pop     ebx
		retn
		// ---------------------------------------------------------------------------
loc_441D8A:
		xor     ecx, ecx
		sub     eax, ecx
loc_441D8E:
		pop     edi
		pop     esi
		pop     ebx
	API_FUNC_NAKED_END(1)

#else
	return 0;
#endif
}

static cpp_uint compress_real_to_shortn(cpp_real value)
{
	cpp_real calc;
	if(value < -1.0)		calc = -1.0;
	else if(value > 1.0)	calc = 1.0;
	else					calc = value;
	calc *= 32767.5;

	return floor_it(calc);
}

static cpp_uint compress_real_vector3d_to_HenDN3(cpp_real* values)
{
	CppMath::real_vector3d* vector = reinterpret_cast<CppMath::real_vector3d*>(values);

	cpp_real calc;
	cpp_uint comp_x, comp_y, comp_z;

	if(vector->I < -1.0)		calc = -1.0;
	else if(vector->I > 1.0)	calc = 1.0;
	else						calc = vector->I;
	calc *= 1023.5;
	comp_x = floor_it(calc) & 0x7FF;

	if(vector->J < -1.0)		calc = -1.0;
	else if(vector->J > 1.0)	calc = 1.0;
	else						calc = vector->J;
	calc *= 1023.5;
	comp_y = floor_it(calc) & 0x7FF;

	if(vector->K < -1.0)		calc = -1.0;
	else if(vector->K > 1.0)	calc = 1.0;
	else						calc = vector->K;
	calc *= 511.5;
	comp_z = floor_it(calc) & 0x3FF;

	// Packed 11:11:10 Format
	return comp_x |
		(comp_y << 11) |
		(comp_z << 22);
}
#pragma endregion

__CPP_CODE_END__


#include "Math/MatrixMath.inl"


//////////////////////////////////////////////////////////////////////////
// Managed
__MCPP_CODE_START__

namespace LowLevel { namespace Math {
	mcpp_uint Util::CompressRealToShortN(mcpp_real value)						{ return compress_real_to_shortn(value); }

	mcpp_uint Util::CompressRealVector3DToHenDN3(Math::real_quaternion vector)	{ return compress_real_vector3d_to_HenDN3(&vector.Vector.I); }

}; };

__MCPP_CODE_END__