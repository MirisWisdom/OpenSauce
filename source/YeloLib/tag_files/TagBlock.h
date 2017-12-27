#pragma once

namespace Yelo
{
	struct tag_block;

	// Template'd tag block for allowing more robust code.
	template <typename t_type>
	struct TagBlock
	{
		typedef t_type* iterator;
		typedef const t_type* const_iterator;
		typedef t_type value_type;
		typedef t_type& reference;
		typedef const t_type& const_reference;
		typedef t_type* pointer;

		typedef const t_type* const_pointer;

		// Element count for this block
		int32 Count;

		// Anonymous union for allowing less code for converting, and less 
		// "#pragma warning" code entries
		union
		{
			// Pointer to the first tag block definition element
			void* Address;

			// Pointer to a specific block of object(s) of type t_type
			t_type* Definitions;
		};

		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer for this block instance
		const struct tag_block_definition* BlockDefinition;
		#endif

		// Using the class's template 't_type' parameter, calculates the total
		// size, in bytes, the elements assume in memory
		size_t SizeOf() const
		{
			return sizeof(t_type) * Count;
		}

		tag_block* to_tag_block()
		{
			return CAST_PTR(tag_block*, &this->Count);
		}

		const tag_block* to_tag_block() const
		{
			return CAST_PTR(const tag_block*, &this->Count);
		}

		// Sets this object to equal that of a anonymous tag block object. 
		TagBlock<t_type>& Copy(
			const tag_block& block)
		{
			this->Count = block.count;
			this->Address = block.address;

			#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
			this->BlockDefinition = block.definition;
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
			YELO_ASSERT(index >= 0 && index < Count);
			return this->Definitions[index];
			#endif
		}

		// Indexer for getting a (const) definition reference via the definition's index in the block
		const t_type& operator[](
			int32 index) const
		{
			#if PLATFORM_IS_EDITOR
			return *blam::tag_block_get_element(*this, index);
			#else
			YELO_ASSERT(index >= 0 && index < Count);
			return this->Definitions[index];
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
		const_iterator begin() const
		{
			return Definitions;
		}

		iterator begin()
		{
			return Definitions;
		}

		const_iterator const_begin() const
		{
			return Definitions;
		}

		const_iterator const_begin()
		{
			return Definitions;
		}

		const_iterator end() const
		{
			return Definitions + Count;
		}

		iterator end()
		{
			return Definitions + Count;
		}

		const_iterator const_end() const
		{
			return Definitions + Count;
		}

		const_iterator const_end()
		{
			return Definitions + Count;
		}

		bool empty() const
		{
			return Count == 0;
		}

		size_t size() const
		{
			return CAST(size_t, Count);
		}
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT(sizeof(TagBlock<byte>) == 0xC);
	#else
	BOOST_STATIC_ASSERT(sizeof(TagBlock<byte>) == 0x8);
	#endif
}
