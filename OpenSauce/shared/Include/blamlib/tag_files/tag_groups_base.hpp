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

			k_tag_block_format_buffer_size = 512,

			k_maximum_tags_per_tag_chain = 4,
			k_maximum_children_per_tag = 16,
		};
	};

// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
// Halo2's uses a managed constant pool for tag names so we don't want to allow write-access
#if PLATFORM_IS_EDITOR && CHEAPE_PLATFORM == CHEAPE_PLATFORM_HALO1
	typedef char* tag_reference_name_reference;
#else
	typedef const char* tag_reference_name_reference;
#endif

	struct tag_reference
	{
		enum {
			k_debug_data_size = sizeof(tag_reference_name_reference) + sizeof(int32),
		};

		// group tag identifier for this reference
		tag group_tag;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference name;
		// length of the reference name
		int32 name_length;
#endif
		// datum index of this reference in the tag index
		datum_index tag_index;

		operator datum_index() const { return tag_index; }

		void clear();

		void set(tag group_tag, cstring name);
		template<typename T>
		void set(cstring name)
		{
			this->set(T::k_group_tag, name);
		}
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x10 );
	#define pad_tag_reference PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x8 );
	#define pad_tag_reference PAD32 PAD32
#endif
	namespace blam
	{
		// Clear the values of a tag reference so that it references no tag
		void PLATFORM_API tag_reference_clear(tag_reference& reference);

		void PLATFORM_API tag_reference_set(tag_reference& reference, tag group_tag, cstring name);
		template<typename T>
		void tag_reference_set(tag_reference& reference, cstring name)
		{
			return tag_reference_set(reference, T::k_group_tag, name);
		}

		datum_index PLATFORM_API tag_reference_try_and_get(const tag_reference* reference);

		bool PLATFORM_API tag_reference_resolve(_Inout_ tag_reference* reference);

		// non-standard overload of the above resolve()
		bool tag_reference_resolve(_Inout_ tag_reference& reference, tag expected_group_tag);
		template<typename T>
		bool tag_reference_resolve(_Inout_ tag_reference& reference)
		{
			return tag_reference_resolve(reference, T::k_group_tag);
		}
	};


	struct tag_block
	{
		enum {
			k_debug_data_size = sizeof(struct tag_block_definition*),
		};

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
		template<typename T>
		T* Elements() { return CAST_PTR(T*, address); }

		void* get_element(int32 element_index);
		void delete_element(int32 element_index);
		int32 add_element();
		bool resize(int32 element_count);

		void* add_and_get_element();

#if PLATFORM_IS_EDITOR
		size_t get_element_size() const;

		struct s_iterator_result
		{
			void* address;
			int32 index;

			s_iterator_result(void* ptr, int32 i) : address(ptr), index(i) {}
		};
		// NOTE: Design assumes there's no concurrent element changing (adding or removing)
		struct s_iterator
		{
			byte* m_address;
			int32 m_element_index;
			size_t m_element_size;
		public:
			s_iterator(tag_block& block, size_t element_size, size_t element_index = 0)
				: m_address(CAST_PTR(byte*, block.address) + (element_size * element_index))
				, m_element_index(element_index)
				, m_element_size(element_size)
			{
			}
			bool operator!=(const s_iterator& other) const
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
		s_iterator	begin()	{ return s_iterator(*this, this->get_element_size()); }
		s_iterator	end()	{ return s_iterator(*this, this->get_element_size(), CAST(size_t, this->count)); }
#endif
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0xC );
	#define pad_tag_block PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0x8 );
	#define pad_tag_block PAD32 PAD32
#endif
	namespace blam
	{
		// Get the address of a block element which exists at [element_index]
		void* PLATFORM_API tag_block_get_element(tag_block* block, int32 element_index);
		const void* PLATFORM_API tag_block_get_element(const tag_block* block, int32 element_index);
		// Add a new block element and return the index which 
		// represents the newly added element
		int32 PLATFORM_API tag_block_add_element(tag_block* block);
		// Resize the block to a new count of elements, returning the 
		// success result of the operation
		bool PLATFORM_API tag_block_resize(tag_block* block, int32 element_count);
		// Delete the block element at [element_index]
		void PLATFORM_API tag_block_delete_element(tag_block* block, int32 element_index);

		void* tag_block_add_and_get_element(tag_block* block);
	};


	struct tag_data
	{
		enum {
			k_debug_data_size = sizeof(long_flags) + sizeof(int32) +
				sizeof(struct tag_block_definition*),
		};

		// byte count of this data blob
		int32 size;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// unknown
		long_flags flags;
		// offset in the source tag file (relative to the start of the definition bytes)
		int32 stream_position;
#endif
		// data blob bytes pointer
		void* address;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer of this data blob instance
		const struct tag_data_definition* definition;
#endif

		// Returns a [T] pointer that is the same as [address].
		// Just makes coding a little more cleaner
		template<typename T>
		T* Elements() { return CAST_PTR(T*, address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		byte* Bytes() { return CAST_PTR(byte*, address); }

		bool resize(int32 new_size = 0);
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x14 );
	#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x8 );
	#define pad_tag_data PAD32 PAD32
#endif
	namespace blam
	{
		bool PLATFORM_API tag_data_resize(tag_data* data, int32 new_size);

		void* PLATFORM_API tag_data_get_pointer(tag_data& data, int32 offset, int32 size);
		template<typename T> inline
		T* tag_data_get_pointer(tag_data& data, int32 offset, int32 index = 0)
		{
			return CAST_PTR(T*, tag_data_get_pointer(data, 
				offset + (sizeof(T) * index), 
				sizeof(T)) );
		}
	};


	namespace blam
	{
		tag PLATFORM_API tag_get_group_tag(datum_index tag_index);

		tag_block* PLATFORM_API tag_get_root_block(datum_index tag_index);

		void PLATFORM_API tag_orphan(datum_index tag_index);

		datum_index PLATFORM_API tag_loaded(tag group_tag, cstring name);
		template<typename T> inline
		datum_index tag_loaded(cstring name)
		{
			return tag_loaded(T::k_group_tag, name);
		}

		cstring PLATFORM_API tag_get_name(datum_index tag_index);
		inline cstring tag_try_get_name(datum_index tag_index)
		{
			return tag_index.IsNull()
				? "<unspecified tag>"
				: tag_get_name(tag_index);
		}

		bool PLATFORM_API tag_read_only(datum_index tag_index);

#if PLATFORM_USES_CACHE_FILES
	#define TAG_GET_RETURN_MODIFIER const
#else
	#define TAG_GET_RETURN_MODIFIER
#endif
		// Get the tag definition's address by it's expected group tag and 
		// it's tag handle [tag_index]
		TAG_GET_RETURN_MODIFIER void* PLATFORM_API tag_get(tag group_tag, datum_index tag_index);
		template<typename T> inline
		TAG_GET_RETURN_MODIFIER T* tag_get(datum_index tag_index)
		{
			return CAST_PTR(TAG_GET_RETURN_MODIFIER T*, tag_get(T::k_group_tag, tag_index));
		}
#undef TAG_GET_RETURN_MODIFIER

		datum_index PLATFORM_API tag_new(tag group_name, cstring name);
		template<typename T> inline
		datum_index tag_new(cstring name)
		{
			return tag_new(T::k_group_tag, name);
		}

		// Load a tag definition into memory.
		// Returns the tag handle of the loaded tag definition
		datum_index PLATFORM_API tag_load(tag group_tag, cstring name, long_flags file_flags);
		template<typename T> inline
		datum_index tag_load(cstring name, long_flags file_flags)
		{
			return tag_load(T::k_group_tag, name, file_flags);
		}

		datum_index PLATFORM_API tag_reload(tag group_tag, cstring name);
		template<typename T> inline
		datum_index tag_reload(cstring name)
		{
			return tag_reload(T::k_group_tag, name);
		}

		void PLATFORM_API tag_load_children(datum_index tag_index);

		// Unload a tag definition from memory.
		// [tag_index] will resolve to an invalid index after this returns.
		void PLATFORM_API tag_unload(datum_index tag_index);

		bool PLATFORM_API tag_save(datum_index tag_index);
	};


	namespace TagGroups
	{
		// just an endian swap
		void TagSwap(tag& x);

		tag string_to_group_tag(cstring name);

		// Returns true if the tag is an instance of the group_tag or is a child group of it.
		// Returns false if not, or tag_index is invalid.
		bool TagIsInstanceOf(datum_index tag_index, tag group_tag);
		template<typename T> inline
		bool TagIsInstanceOf(datum_index tag_index)
		{
			return TagIsInstanceOf(tag_index, T::k_group_tag);
		}

		// Returns the tag as non-const. Are you sure you want to be writing to tags at runtime?
		template<typename T> inline
		T* TagGetForModify(datum_index tag_index)
		{
			return CAST_QUAL(T*, blam::tag_get<T>(tag_index));
		}

		// 'Unsafe' in that it returns the tag as non-const and doesn't do any bounds checking
		// Useful when you're using tag_iterator and known you're getting some good input
		template<typename T> inline
		T* TagGetUnsafe(datum_index tag_index)
		{
			extern void* TagGetUnsafeImpl(datum_index tag_index);

			return CAST_PTR(T*, TagGetUnsafeImpl(tag_index));
		}

		// Union hack to use a group tag as a string
		union group_tag_to_string
		{
			struct {
				tag group;
				PAD8; // null terminator
			};
			char str[sizeof(tag)+1];

			group_tag_to_string& Terminate();
			group_tag_to_string& TagSwap();

			cstring ToString()
			{
				return Terminate().TagSwap().str;
			}
		};

		struct s_tag_iterator {
			Memory::s_data_iterator instances_iterator;
			tag group_tag_filter;
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_iterator) == 0x14 );
	};
};