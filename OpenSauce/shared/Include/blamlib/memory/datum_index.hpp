/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>

namespace Yelo
{
	/// <summary>	Handle to data allocated by the engine's data-array construct. </summary>
	struct datum_index
	{
		typedef int16 index_t;
		typedef int16 salt_t;

		static const datum_index null;

		union {
			uint32 handle;
			struct {
				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>
				/// 	absolute index Note: This is actually suppose to be a unsigned short but it's not until
				/// 	Halo 3 where data starts breaking past the 0x7FFF cap, so for Halo 1 and Halo 2 projects
				/// 	this should still hold safe. Maybe I'll update the code to uint16 one day...
				/// </summary>
				index_t index;
				/// <summary>	salted index, gives the handle a certain uniqueness. </summary>
				salt_t salt;
			};
		};

		bool IsNull() const { return null == handle; }

		static datum_index Create(index_t index, salt_t salt)
		{
			datum_index result; result.handle = (CAST(uint32, salt) << 16) | index;
			return result;
		}
		// [header] should be a pointer to the start of a datum instance in a data array
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new datum_index based on an absolute index and datum pointer. </summary>
		///
		/// <param name="index"> 	Zero-based index of the datum. </param>
		/// <param name="header">	[in] should be a pointer to the start of a datum instance in a data array </param>
		///
		/// <returns>	[datum_index::null] if this fails </returns>
		static datum_index Create(index_t index, const void* header);

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		struct std_hash : public std::unary_function<datum_index, size_t>
		{
			// logic copied and pasted from xstddef's _Bitwise_hash
			size_t operator()(const datum_index& _Keyval) const
			{	// hash _Keyval to size_t value by pseudorandomizing transform
				return std::_Hash_seq((const unsigned char *)&_Keyval, sizeof(datum_index));
			}
		};
#endif

		OVERRIDE_OPERATOR_CAST_THIS(uint32);
		OVERRIDE_OPERATOR(=, datum_index&, uint32 arg) { this->handle = arg; return *this; }
		OVERRIDE_OPERATOR_MATH_BOOL(datum_index, handle, ==);
		OVERRIDE_OPERATOR_MATH_BOOL(datum_index, handle, !=);
	private:
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, handle, ==);
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, handle, !=);
	}; BOOST_STATIC_ASSERT( sizeof(datum_index) == 0x4 );
#define pad_datum_index PAD16 PAD16
#define DATUM_INDEX_TO_IDENTIFIER(datum)		(datum & 0xFFFF0000)
#define DATUM_INDEX_TO_ABSOLUTE_INDEX(datum)	(datum & 0x0000FFFF)
};