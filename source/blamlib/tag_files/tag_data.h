#pragma once

namespace Yelo
{
	struct tag_data_definition;

	struct tag_data
	{
		enum
		{
			k_debug_data_size = sizeof(long_flags) + sizeof(int32) +
			sizeof(struct tag_block_definition*),
		};

		int32 size;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		long_flags flags;
		// offset in the source tag file (relative to the start of the definition bytes)
		int32 stream_position;
		#endif
		void* address;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const tag_data_definition* definition;
		#endif

		// Returns a [t_type] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template <typename t_type>
		t_type* Elements()
		{
			return reinterpret_cast<t_type*>(address);
		}

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		byte* Bytes() const;

		bool resize(
			int32 new_size = 0);
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT(sizeof(tag_data) == 0x14);
	#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32
	#else
	BOOST_STATIC_ASSERT(sizeof(tag_data) == 0x8);
	#define pad_tag_data PAD32 PAD32
	#endif
}
