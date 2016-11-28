/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <cseries/MacrosCpp.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			// character count in a [tag_string] type
			k_tag_string_length = 31,
			// character count in a [long_string] type
			k_long_string_length = 255,

			k_string_id_length = 127,

			k_string_64_length = 63,
			k_string_128_length = 127,
			k_string_256_length = 255,
		};
	};

	namespace Flags
	{
		enum {
			_alignment_16_bit = 1,
			_alignment_32_bit,
			_alignment_64_bit,
			_alignment_128_bit,
		};
	};

	#define pad_bool PAD8

	// # from 0 to 255
	typedef unsigned char byte;
	// # from -128 to 127
	typedef signed char sbyte;
#define pad_byte PAD8

	// # from 0 to 65,535
	typedef unsigned short uint16;
	// # from -32,768 to 32,767
	typedef signed short int16;
#define pad_int16 PAD16

	// # from 0 to 4,294,967,295
	typedef unsigned long uint32;
#define pad_uint32 PAD32
	// # from -2147483648 to 2147483647
	typedef signed long int32;
#define pad_int32 PAD32

	// # from 0 to 18,446,744,073,709,551,615
	typedef unsigned __int64 uint64;
	// # from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
	typedef signed __int64 int64;
#define pad_int64 PAD32 PAD32

	// a floating-point # from 1.175494351e-38F to 3.402823466e+38F
	typedef float real;
#define pad_real PAD32


	// Constant '\0' terminated ascii string
	typedef const char* cstring;
	// '\0\0' terminated unicode string
	typedef wchar_t* wstring;
	// Constant '\0\0' terminated unicode string
	typedef const wchar_t* wcstring;

	// null value for a [string] or [cstring]
#define CSTRING_NULL	"\0"
	// null value for a [wstring] or [wcstring]
#define WSTRING_NULL	L"\0"

	// 32 character ASCII string, with null terminator
	typedef char tag_string[Yelo::Enums::k_tag_string_length+1];
#define pad_tag_string PAD32 PAD32 PAD32 PAD32 PAD32 PAD32 PAD32 PAD32
	// 256 character ASCII string, with null terminator
	typedef char long_string[Yelo::Enums::k_long_string_length+1];
#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128

	typedef char string_id_value[Yelo::Enums::k_string_id_length+1];
	typedef uint32 string_id;
#define pad_string_id PAD32

	// 128 character ASCII string, with null terminator
	typedef char string128[Yelo::Enums::k_string_128_length+1];
	// 256 character ASCII string, with null terminator
	typedef char string256[Yelo::Enums::k_string_256_length+1];

	// 64 character UNICODE string (128 bytes), with null terminator
	typedef wchar_t wstring64[Yelo::Enums::k_string_64_length+1];
	// 256 character UNICODE string (512 bytes), with null terminator
	typedef wchar_t wstring256[Yelo::Enums::k_string_256_length+1];

	struct string_list
	{
		typedef cstring*		iterator;
		typedef const cstring*	const_iterator;

		int32 count;
		cstring* strings;

		const_iterator	begin() const	{ return strings; }
		iterator		begin()			{ return strings; }
		const_iterator	end() const		{ return strings + count; }
		iterator		end()			{ return strings + count; }

		cstring operator [](int32 index) const
		{
			assert(index>=0 && index<count);

			return strings[index];
		}
	}; BOOST_STATIC_ASSERT( sizeof(string_list) == 0x8 );


	// a tag group id
	typedef unsigned long tag;

	// an enumerated value in a 1 byte range
	typedef signed char byte_enum;
#define pad_byte_enum PAD8

	// an enumerated value in a 2 byte range
	typedef signed short _enum;
#define pad_enum PAD16

	// an enumerated value in a 4 byte range (not an officially used type in halo 1 tags, at least not up front anyway)
	typedef signed long long_enum;
#define pad_long_enum PAD32

	// bit flags in a 1 byte range
	typedef unsigned char byte_flags;
#define pad_byte_flags PAD8

	// bit flags in a 2 byte range
	typedef unsigned short word_flags;
#define pad_word_flags PAD16

	// bit flags in a 4 byte range
	typedef unsigned long long_flags;
#define pad_long_flags PAD32
};

#include <blamlib/math/integer_math.hpp>
#include <blamlib/math/real_math.hpp>
#include <blamlib/math/color_math.hpp>
#include <blamlib/math/matrix_math.hpp>

#include <blamlib/memory/datum_index.hpp>