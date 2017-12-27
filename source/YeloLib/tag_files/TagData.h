#pragma once

namespace Yelo
{
	struct tag_data;
	struct tag_data_definition;

	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template <typename t_type>
	struct TagData
	{
		typedef t_type* iterator_t;
		typedef const t_type* const_iterator_t;
		typedef t_type value_type_t;
		typedef t_type& reference_t;
		typedef const t_type& const_reference_t;
		typedef t_type* pointer_t;
		typedef const t_type* const_pointer_t;

		int32 total_size;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		long_flags flags;
		int32 stream_position;
		#endif
		union
		{
			void* address;
			t_type* definitions;
		};
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const tag_data_definition* data_definition;
		#endif

		size_t count() const
		{
			return total_size / sizeof(t_type);
		}

		static size_t size_of_type()
		{
			return sizeof(t_type);
		}

		tag_data* to_tag_data()
		{
			return reinterpret_cast<tag_data*>(&this->total_size);
		}

		const tag_data* to_tag_data() const
		{
			return reinterpret_cast<const tag_data*>(&this->total_size);
		}

		t_type* operator[](
			const int32 index)
		{
			return &this->definitions[index];
		}

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
			return definitions + count();
		}

		iterator_t end()
		{
			return definitions + count();
		}

		const_iterator_t const_end() const
		{
			return definitions + count();
		}

		const_iterator_t const_end()
		{
			return definitions + count();
		}

		bool empty() const
		{
			return total_size == 0;
		}

		size_t size() const
		{
			return static_cast<size_t>(count());
		}

		#if PLATFORM_IS_EDITOR
		void resize(
			const size_t new_size = 0)
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
