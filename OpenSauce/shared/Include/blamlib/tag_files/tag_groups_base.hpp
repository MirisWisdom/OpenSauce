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
		};
	};

// Halo1's editor allocates 256 characters for all tag_reference names, even if they're empty
// Halo2's uses a managed constant pool for tag names so we don't want to allow write-access
#if PLATFORM_IS_EDITOR && CHEAPE_PLATFORM == CHEAPE_PLATFORM_HALO1
	typedef char* tag_reference_name_t;
#else
	typedef const char* tag_reference_name_t;
#endif

	struct tag_reference
	{
		// group tag identifier for this reference
		tag group_tag;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_t name;
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
	// Clear the values of a tag reference so that it references no tag
	void tag_reference_clear(tag_reference& reference);

	void tag_reference_set(tag_reference& reference, tag group_tag, cstring name);
	template<typename T>
	void tag_reference_set(tag_reference& reference, cstring name)
	{
		return tag_reference_set(reference, T::k_group_tag, name);
	}


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

		void* get_element(int32 element);
		void delete_element(int32 element);
		int32 add_element();
		bool resize(int32 element_count);

#if PLATFORM_IS_EDITOR
		size_t get_element_size() const;
#endif
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0xC );
	#define pad_tag_block PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0x8 );
	#define pad_tag_block PAD32 PAD32
#endif
	// Get the address of a block element which exists at [element]
	void* tag_block_get_element(tag_block* block, int32 element);
	const void* tag_block_get_element(const tag_block* block, int32 element);
	// Add a new block element and return the index which 
	// represents the newly added element
	int32 tag_block_add_element(tag_block* block);
	// Resize the block to a new count of elements, returning the 
	// success result of the operation
	bool tag_block_resize(tag_block* block, int32 element_count);
	// Delete the block element at [element]
	void tag_block_delete_element(tag_block* block, int32 element);


	struct tag_data
	{
		// byte count of this data blob
		int32 size;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// unknown
		long_flags flags;
		// offset relative to the start of owner tag group?
		int32 data_offset;
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
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x14 );
	#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x8 );
	#define pad_tag_data PAD32 PAD32
#endif


	cstring tag_get_name(datum_index tag_index);

	bool tag_is_read_only(datum_index tag_index);

	// Get the tag definition's address by it's expected group tag and 
	// it's tag handle [tag_index]
	void* tag_get(tag group_tag, datum_index tag_index);
	template<typename T>
	T* tag_get(datum_index tag_index)
	{
		return CAST_PTR(T*, tag_get(T::k_group_tag, tag_index));
	}

	datum_index tag_new(tag group_name, cstring name);
	template<typename T>
	datum_index tag_new(cstring name)
	{
		return tag_new(T::k_group_tag, name);
	}

	// Load a tag definition into memory.
	// Returns the tag handle of the loaded tag definition
	datum_index tag_load(tag group_tag, cstring name, long_flags file_flags);
	template<typename T>
	datum_index tag_load(cstring name, long_flags file_flags)
	{
		return tag_load(T::k_group_tag, name, file_flags);
	}

	// Unload a tag definition from memory.
	// [tag_index] will resolve to an invalid index after this returns.
	void tag_unload(datum_index tag_index);

	bool tag_save(datum_index tag_index);


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

			void Terminate();
			void TagSwap();
		};
	};
};