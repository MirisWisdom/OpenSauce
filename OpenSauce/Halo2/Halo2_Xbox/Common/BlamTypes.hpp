/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			// character count in a [long_string] type
			k_long_string_length = 255,

			k_string_id_length = 127,
		};
	};


#pragma region String types / utils

	// 255 character ASCII string, no null terminator
	typedef char long_string[Yelo::Enums::k_long_string_length+1];
#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128

	typedef char string_id_value[Yelo::Enums::k_string_id_length+1];

	typedef uint32 string_id;
#define pad_string_id PAD32

#pragma endregion


	struct TypeIndex
	{
		// Value
		uint32 Ptr;

		// Get value
		inline operator uint32() { return Ptr; }
		inline uint32 operator = (uint32 value) { return Ptr = value; }
		inline uint32 Type() { return Ptr >> 30; }
		inline uint32 Index() { return Ptr & 0x3FFFFFFF; }

		static inline TypeIndex Create(uint32 type, uint32 index) { TypeIndex ret = {(uint32)((type<<30) | (index&0x3FFFFFFF))}; return ret; }
	};

	// Map resource pointer
	struct ResourcePtr : TypeIndex
	{
		enum Location {
			_CacheNone,
			_CacheMainMenu,
			_CacheShared,
			_CacheSharedSp,
		};

		// Return the location of the resource
		inline uint32 Map()		{ Type(); }
		// Get the offset of the resource
		inline uint32 Offset()	{ Index(); }
		// Returns true if the resource is in the cache the pointer is from
		inline bool Internal()	{ return Type() == _CacheNone; }
		// Returns true if the resource is in the main menu cache
		inline bool Mainmenu()	{ return Type() == _CacheMainMenu; }
		// Returns true if the resource is in the shared cache
		inline bool Shared()	{ return Type() == _CacheShared; }
		// Returns true if the resource is in the shared single player cache
		inline bool SharedSp()	{ return Type() == _CacheSharedSp; }
	};

};