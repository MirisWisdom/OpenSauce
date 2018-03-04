#pragma once

namespace Yelo
{
	struct tag_block_definition;

	struct tag_block
	{
		enum
		{
			k_debug_data_size = sizeof(struct tag_block_definition*),
		};

		int32 count;
		void* address;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const tag_block_definition* definition;
		#endif

		int32 add_element();
		void* add_and_get_element();
		void delete_element(
			const int32 element_index);
		void* get_element(
			const int32 element_index);
		bool resize(
			const int32 element_count);

		// Returns a [t_element] pointer that is the same as [address]
		template <typename t_element>
		t_element* elements()
		{
			return reinterpret_cast<t_element*>(address);
		}

		#if PLATFORM_IS_EDITOR
		size_t get_element_size() const;

		struct s_iterator_result
		{
			void* const address;
			const int32 index;

			s_iterator_result(
				void* const address,
				const int32 index);
		};

		// NOTE: Design assumes there's no concurrent element changing (adding or removing)
		struct s_iterator
		{
			byte* m_address;
			int32 m_element_index;
			const size_t m_element_size;

			s_iterator(
				tag_block& block,
				const size_t element_size,
				const size_t element_index = 0);

			bool operator!=(
				const s_iterator& other) const;
			s_iterator& operator++();
			s_iterator_result operator*() const;
		};

		s_iterator begin();
		s_iterator end();
		#endif
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	ASSERT_SIZE(tag_block, 0xC);
	#define pad_tag_block PAD32 PAD32 PAD32
	#else
	ASSERT_SIZE(tag_block, 0x8);
	#define pad_tag_block PAD32 PAD32
	#endif
}
