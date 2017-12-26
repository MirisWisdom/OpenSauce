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

		// Returns a [element_t] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template <typename element_t>
		element_t* Elements()
		{
			return reinterpret_cast<element_t*>(address);
		}

		void* get_element(
			const int32 element_index);
		void delete_element(
			const int32 element_index);
		int32 add_element();
		bool resize(
			const int32 element_count);

		void* add_and_get_element();

		#if PLATFORM_IS_EDITOR
		size_t get_element_size() const;

		struct s_iterator_result
		{
			void* address;
			int32 index;

			s_iterator_result(
				void* ptr,
				int32 i) : address(ptr)
				, index(i) {}
		};

		// NOTE: Design assumes there's no concurrent element changing (adding or removing)
		struct s_iterator
		{
			byte* m_address;
			int32 m_element_index;
			size_t m_element_size;
		public:
			s_iterator(
				tag_block& block,
				size_t element_size,
				size_t element_index = 0)
				: m_address(CAST_PTR(byte*, block.address) + (element_size * element_index))
				, m_element_index(element_index)
				, m_element_size(element_size) { }

			bool operator!=(
				const s_iterator& other) const
			{
				return m_address != other.m_address;
			}

			s_iterator& operator++()
			{
				m_address += m_element_size;
				++m_element_index;
				return *this;
			}

			s_iterator_result operator*() const
			{
				return s_iterator_result(m_address, m_element_index);
			}
		};

		s_iterator begin()
		{
			return s_iterator(*this, this->get_element_size());
		}

		s_iterator end()
		{
			return s_iterator(*this, this->get_element_size(), CAST(size_t, this->count));
		}
		#endif
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT(sizeof(tag_block) == 0xC);
	#define pad_tag_block PAD32 PAD32 PAD32
	#else
	BOOST_STATIC_ASSERT(sizeof(tag_block) == 0x8);
	#define pad_tag_block PAD32 PAD32
	#endif
}
