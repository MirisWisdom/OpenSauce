/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>

namespace Yelo
{
	// Handle to data allocated by the engine's data-array construct
	struct datum_index
	{
		typedef int16 index_t;
		typedef int16 salt_t;

		static const datum_index null;

		union {
			uint32 handle;
			struct {
				// absolute index
				// Note: This is actually suppose to be a unsigned short but it's not 
				// until Halo 3 where data starts breaking past the 0x7FFF cap, so for 
				// Halo 1 and Halo 2 projects this should still hold safe.
				// Maybe I'll update the code to uint16 one day...
				index_t index;
				// salted index, gives the handle a certain uniqueness
				salt_t salt;
			};
		};

		API_INLINE bool IsNull() const { return null == handle; }

		static datum_index Create(index_t index, salt_t salt);
		// [header] should be a pointer to the start of a datum instance in a data array
		static datum_index Create(index_t index, void* header);

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