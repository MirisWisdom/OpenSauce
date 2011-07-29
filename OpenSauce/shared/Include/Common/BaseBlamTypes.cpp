/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <Common/Precompile.hpp>
#include <Common/BaseBlamTypes.hpp>

namespace Yelo
{
	const sbyte K_SBYTE_MIN	=	CAST(sbyte, -128);
	const sbyte K_SBYTE_MAX	=	CAST(sbyte, 127);
	const byte K_BYTE_MIN =		CAST(byte, 0);
	const byte K_BYTE_MAX =		CAST(byte, 255);
	const int16 K_INT16_MIN	=	CAST(int16, -32768);
	const int16 K_INT16_MAX	=	CAST(int16, 32767);
	const uint16 K_UINT16_MIN =	CAST(uint16, 0);
	const uint16 K_UINT16_MAX =	CAST(uint16, 65535);
	const int32 K_INT32_MIN	=	CAST(int32, -2147483646);
	const int32 K_INT32_MAX	=	CAST(int32, 2147483647);
	const uint32 K_UINT32_MIN =	CAST(uint32, 0);
	const uint32 K_UINT32_MAX =	CAST(uint32, 4294967295);
	const int64 K_INT64_MIN	=	CAST(int64, -9223372036854775806);
	const int64 K_INT64_MAX	=	CAST(int64, 9223372036854775807);
	const uint64 K_UINT64_MIN =	CAST(uint64, 0);
	const uint64 K_UINT64_MAX =	CAST(uint64, 18446744073709551615);
	const real K_REAL_MIN	=	CAST(real, 1.175494351e-38F);
	const real K_REAL_MAX	=	CAST(real, 3.402823466e+38F);

	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide)
	{
		int32 x = 0;
		while(wide[x] && string_length--)
			string[x] = (char)wide[x++];

		return string;
	}

	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii)
	{
		int32 x = 0;
		while(ascii[x] && string_length--)
			string[x] = (wchar_t)ascii[x++];

		return string;
	}


	const datum_index datum_index::null = { CAST(uint32, NONE) };
};