#pragma once

namespace Yelo
{
	// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
	typedef char* tag_reference_name_reference;

	struct tag_reference
	{
		enum
		{
			k_debug_data_size = sizeof(tag_reference_name_reference) + sizeof(int32),
		};

		tag group_tag;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference name;
		// length of the reference name
		int32 name_length;
		#endif
		datum_index tag_index;

		operator datum_index() const;

		void clear();

		void set(
			tag group_tag,
			cstring name);

		template <typename t_type>
		void set(
			const cstring name)
		{
			this->set(t_type::k_group_tag, name);
		}
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	ASSERT_SIZE(tag_reference, 0x10);
	#define pad_tag_reference PAD32 PAD32 PAD32 PAD32
	#else
	ASSERT_SIZE(tag_reference, 0x8);
	#define pad_tag_reference PAD32 PAD32
	#endif
}
