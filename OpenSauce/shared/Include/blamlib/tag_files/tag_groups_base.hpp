/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_max_tag_name_length = 255,
		};
	};

// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
// Halo2's uses a managed constant pool for tag names so we don't want to allow write-access
#if PLATFORM_IS_EDITOR && CHEAPE_PLATFORM == CHEAPE_PLATFORM_HALO1
	typedef char* tag_reference_name_t;
#else
	typedef const char* tag_reference_name_t;
#endif

	struct tag_reference
	{
		// group tag identifier for this reference
		tag group_tag;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_t name;
		// length of the reference name
		int32 name_length;
#endif
		// datum index of this reference in the tag index
		datum_index tag_index;

		API_INLINE operator datum_index() const { return tag_index; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x10 );
	#define pad_tag_reference PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x8 );
	#define pad_tag_reference PAD32 PAD32
#endif


	struct tag_block
	{
		// element count for this block
		int32 count;
		// elements pointer
		void* address;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer for this block instance
		const struct tag_block_definition* definition;
#endif

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template<typename T> inline T* Items() { return CAST_PTR(T*, address); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0xC );
	#define pad_tag_block PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0x8 );
	#define pad_tag_block PAD32 PAD32
#endif


	struct tag_data
	{
		// byte count of this data blob
		int32 size;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// unknown
		long_flags flags;
		// offset relative to the start of owner tag group?
		int32 data_offset;
#endif
		// data blob bytes pointer
		void* address;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer of this data blob instance
		const struct tag_data_definition* definition;
#endif

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template<typename T> inline T* Items() { return CAST_PTR(T*, address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		inline byte* Bytes() { return CAST_PTR(byte*, address); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x14 );
	#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x8 );
	#define pad_tag_data PAD32 PAD32
#endif
};