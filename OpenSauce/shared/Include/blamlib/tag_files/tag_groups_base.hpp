/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_max_tag_name_length = 255,

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

		API_INLINE operator datum_index() const { return tag_index; }

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
		template<typename T> inline
		void tag_reference_set(tag_reference& reference, cstring name)
		{
			return tag_reference_set(reference, T::k_group_tag, name);
		}
	};


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
		template<typename T> inline T* Elements() { return CAST_PTR(T*, address); }

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
			inline s_iterator(tag_block& block, size_t element_size, size_t element_index = 0)
				: m_address(CAST_PTR(byte*, block.address) + (element_size * element_index))
				, m_element_index(element_index)
				, m_element_size(element_size)
			{
			}
			inline bool operator!=(const s_iterator& other) const
			{
				return m_address != other.m_address;
			}
			inline s_iterator& operator++()
			{
				m_address += m_element_size;
				++m_element_index;
				return *this;
			}
			inline s_iterator_result operator*() const
			{
				return s_iterator_result(m_address, m_element_index);
			}
		};
		inline s_iterator	begin()	{ return s_iterator(*this, this->get_element_size()); }
		inline s_iterator	end()	{ return s_iterator(*this, this->get_element_size(), CAST(size_t, this->count)); }
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
		template<typename T> inline T* Elements() { return CAST_PTR(T*, address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		inline byte* Bytes() { return CAST_PTR(byte*, address); }

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

		bool PLATFORM_API tag_read_only(datum_index tag_index);

		// Get the tag definition's address by it's expected group tag and 
		// it's tag handle [tag_index]
		void* PLATFORM_API tag_get(tag group_tag, datum_index tag_index);
		template<typename T> inline
		T* tag_get(datum_index tag_index)
		{
			return CAST_PTR(T*, tag_get(T::k_group_tag, tag_index));
		}

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

			inline cstring ToString()
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