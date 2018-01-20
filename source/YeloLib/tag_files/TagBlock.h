#pragma once

namespace Yelo
{
	struct tag_block;
	struct tag_block_definition;

	// Template'd tag block for allowing more robust code.
	template <typename t_type>
	struct TagBlock
	{
		typedef t_type* iterator_t;
		typedef const t_type* const_iterator_t;
		typedef t_type value_type_t;
		typedef t_type& reference_t;
		typedef const t_type& const_reference_t;
		typedef t_type* pointer_t;
		typedef const t_type* const_pointer_t;

		int32 count;
		union
		{
			void* address;
			t_type* definitions;
		};
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const tag_block_definition* block_definition;
		#endif

		size_t total_size() const
		{
			return sizeof(t_type) * count;
		}

		tag_block* to_tag_block()
		{
			return reinterpret_cast<tag_block*>(&this->count);
		}

		const tag_block* to_tag_block() const
		{
			return reinterpret_cast<const tag_block*>(&this->count);
		}

		TagBlock<t_type>& shallow_copy_from(
			const tag_block& block)
		{
			this->count = block.count;
			this->address = block.address;

			#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
			this->block_definition = block.definition;
			#endif

			return this;
		}

		// Indexer for getting a definition reference via the definition's index in the block
		t_type& operator[](
			int32 index)
		{
			#if PLATFORM_IS_EDITOR
			return *blam::tag_block_get_element(*this, index);
			#else
			YELO_ASSERT(index >= 0 && index < count);
			return this->definitions[index];
			#endif
		}

		// Indexer for getting a (const) definition reference via the definition's index in the block
		const t_type& operator[](
			int32 index) const
		{
			#if PLATFORM_IS_EDITOR
			return *blam::tag_block_get_element(*this, index);
			#else
			YELO_ASSERT(index >= 0 && index < count);
			return this->definitions[index];
			#endif
		}

		#if PLATFORM_IS_EDITOR
		t_type* get_element(
			int32 element)
		{
			return blam::tag_block_get_element(*this, index);
		}

		void delete_element(
			int32 element)
		{
			blam::tag_block_delete_element(*this, element);
		}

		bool delete_all_elements()
		{
			return blam::tag_block_delete_all_elements(*this);
		}

		int32 add_element()
		{
			return blam::tag_block_add_element(*this);
		}

		bool resize(
			int32 element_count)
		{
			return blam::tag_block_resize(*this, element_count);
		}

		t_type* add_and_get_element()
		{
			return blam::tag_block_add_and_get_element(*this);
		}
		#endif

		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		const_iterator_t begin() const
		{
			return definitions;
		}

		iterator_t begin()
		{
			return definitions;
		}

		const_iterator_t const_begin() const
		{
			return definitions;
		}

		const_iterator_t const_begin()
		{
			return definitions;
		}

		const_iterator_t end() const
		{
			return definitions + count;
		}

		iterator_t end()
		{
			return definitions + count;
		}

		const_iterator_t const_end() const
		{
			return definitions + count;
		}

		const_iterator_t const_end()
		{
			return definitions + count;
		}

		bool empty() const
		{
			return count == 0;
		}

		size_t size() const
		{
			return static_cast<size_t>(count);
		}
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	ASSERT_SIZE(TagBlock<byte>, 0xC);
	#else
	ASSERT_SIZE(TagBlock<byte>, 0x8);
	#endif
}
