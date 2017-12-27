#pragma once

namespace Yelo
{
	struct tag_data;

	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template <typename t_type>
	struct TagData
	{
		typedef t_type* iterator;
		typedef const t_type* const_iterator;
		typedef t_type value_type;
		typedef t_type& reference;
		typedef const t_type& const_reference;
		typedef t_type* pointer;
		typedef const t_type* const_pointer;

		// Size, in bytes, of the data blob
		int32 Size;

		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		long_flags Flags;
		int32 StreamPosition;
		#endif

		union
		{
			// Address of the data blob
			void* Address;

			t_type* Definitions;
		};

		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const struct tag_data_definition* DataDefinition;
		#endif

		// How many [t_type] objects are actually in the definition.
		size_t Count() const
		{
			return Size / sizeof(t_type);
		}

		// Sizeof(t_type)
		size_t SizeOf() const
		{
			return sizeof(t_type);
		}

		tag_data* to_tag_data()
		{
			return CAST_PTR(tag_data*, &this->Size);
		}

		const tag_data* to_tag_data() const
		{
			return CAST_PTR(const tag_data*, &this->Size);
		}

		t_type* operator[](
			int32 index)
		{
			return &this->Definitions[index];
		}

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
			return Definitions + Count();
		}

		iterator end()
		{
			return Definitions + Count();
		}

		const_iterator const_end() const
		{
			return Definitions + Count();
		}

		const_iterator const_end()
		{
			return Definitions + Count();
		}

		bool empty() const
		{
			return Size == 0;
		}

		size_t size() const
		{
			return CAST(size_t, Count());
		}

		#if PLATFORM_IS_EDITOR
		void resize(
			size_t new_size = 0)
		{
			blam::tag_data_resize(this, new_size);
		}
		#endif
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT(sizeof(TagData<byte>) == 0x14);
	#else
	BOOST_STATIC_ASSERT(sizeof(TagData<byte>) == 0x8);
	#endif
}
