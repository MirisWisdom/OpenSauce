/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
	typedef char* t_tag_reference_name;
#else
	typedef const char* t_tag_reference_name;
#endif

	struct string_list
	{
		int32 count;
		cstring* strings;
	}; BOOST_STATIC_ASSERT( sizeof(string_list) == 0x8 );


	struct tag_reference
	{
		// group tag identifier for this reference
		tag group_tag;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		t_tag_reference_name name;
		// length of the reference name
		int32 name_length;
#endif
		// datum index of this reference in the tag index
		datum_index tag_index;

		API_INLINE operator datum_index() const { return tag_index; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x10 );
	#define pad_tag_reference PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_reference) == 0x8 );
	#define pad_tag_reference PAD32 PAD32
#endif


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
		template<typename T> API_INLINE T* Items() { return CAST_PTR(T*, address); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0xC );
	#define pad_tag_block PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_block) == 0x8 );
	#define pad_tag_block PAD32 PAD32
#endif


	struct tag_data
	{
		// byte count of this data blob
		int32 size;
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// unknown
		PAD32;
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
		template<typename T> API_INLINE T* Items() { return CAST_PTR(T*, address); }

		// Returns byte pointer that is the same as [address]
		// Just makes coding a little more cleaner
		API_INLINE byte* Bytes() { return CAST_PTR(byte*, address); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x14 );
	#define pad_tag_data PAD32 PAD32 PAD32 PAD32 PAD32
#else
	BOOST_STATIC_ASSERT( sizeof(tag_data) == 0x8 );
	#define pad_tag_data PAD32 PAD32
#endif


	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template<tag K_TAG> class TagReference
	{
	public:
		// group tag identifier for this reference
		tag GroupTag;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		t_tag_reference_name Name;
		// length of the reference name
		int32 NameLength;
#endif

		// datum index of this reference in the tag index
		datum_index TagIndex;

		inline operator datum_index() { return TagIndex; }
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, TagIndex, ==);

		// Returns whether or not this reference has a valid group reference
		inline bool IsValid() const { return GroupTag == K_TAG; }

		// Sets this reference to null
		inline void Reset() { GroupTag = K_TAG; TagIndex.handle = datum_index.Null; }

		inline tag Tag() const { return K_TAG; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagReference<NULL_HANDLE>) == 0x10 );
#else
	BOOST_STATIC_ASSERT( sizeof(TagReference<NULL_HANDLE>) == 0x8 );
#endif
	template<>
	inline bool TagReference<NULL_HANDLE>::IsValid() const { return true; }


	// Template'd tag block for allowing more robust code.
	template<typename T> class TagBlock
	{
	public:
		typedef T*			iterator;
		typedef const T*	const_iterator;
		typedef T			value_type;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T*			pointer;
		typedef const T*	const_pointer;

		// Element count for this block
		int32 Count;

		// Anonymous union for allowing less code for converting, and less 
		// "#pragma warning" code entries
	public: union {
			// Pointer to the first tag block definition element
			void* Address;

			// Pointer to a specific block of object(s) of type T
			T* Definitions;
		};

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer for this block instance
		const struct tag_block_definition* BlockDefinition;
#endif

	public:
		// Using the class's template 'T' parameter, calculates the total
		// size, in bytes, the elements assume in memory
		__forceinline uint32 SizeOf() const { return sizeof(T) * Count; }

		__forceinline tag_block* to_tag_block() { return CAST_PTR(tag_block*, &this->Count); }

		// Sets this object to equal that of a anonymous tag block object. 
		API_INLINE  TagBlock<T>& Copy(const tag_block& block)
		{
			this->Count = block.count;
			this->Address = block.address;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
			this->BlockDefinition = block.definition;
#endif

			return this;
		}

		// Indexer for getting a definition reference via the definition's index in the block
		API_INLINE T& operator [](int32 index)				{ return this->Definitions[index]; }
		// Indexer for getting a (const) definition reference via the definition's index in the block
		API_INLINE const T& operator [](int32 index) const	{ return this->Definitions[index]; }


		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		inline const_iterator	begin() const		{ return Definitions; }
		inline iterator			begin()				{ return Definitions; }
		inline const_iterator	const_begin() const	{ return Definitions; }
		inline const_iterator	const_begin()		{ return Definitions; }
		inline const_iterator	end() const			{ return Definitions + Count; }
		inline iterator			end()				{ return Definitions + Count; }
		inline const_iterator	const_end() const	{ return Definitions + Count; }
		inline const_iterator	const_end()			{ return Definitions + Count; }

		inline bool empty() const { return Count == 0; }
		inline size_t size() const { return CAST(size_t, Count); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagBlock<byte>) == 0xC );
#else
	BOOST_STATIC_ASSERT( sizeof(TagBlock<byte>) == 0x8 );
#endif


	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template<typename T> class TagData
	{
	public:
		// Size, in bytes, of the data blob
		int32 Size;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		PAD32;
		PAD32;
#endif

	public: union {
			// Address of the data blob
			void* Address;

			T* Definitions;
		};

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const struct tag_data_definition* DataDefinition;
#endif

		// How many [T] objects are actually in the definition.
		__forceinline uint32 Count() const { return Size / sizeof(T); }

		// Sizeof(T)
		__forceinline uint32 SizeOf() const { return sizeof(T); }

		API_INLINE T* operator [](int32 index) { return &this->Definitions[index]; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x14 );
#else
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x8 );
#endif
};


// [arg2] = documentation
#define TAG_FLAG8(name, ...)				unsigned char name##_bit : 1

// [arg2] = documentation
#define TAG_FLAG16(name, ...)				unsigned short name##_bit : 1

// [arg2] = documentation
#define TAG_FLAG(name, ...)					unsigned long name##_bit : 1

// [arg2] = enum type
// [arg3] = documentation
#define TAG_ENUM(name, ...)					Yelo::_enum name

#define TAG_BLOCK(name, block_type)			Yelo::tag_block name
// Used for blocks which need to allow non-const access
#define TAG_TBLOCK_(name, block_type)		Yelo::TagBlock<block_type> name
#if !PLATFORM_IS_EDITOR
	#define TAG_TBLOCK(name, block_type)	Yelo::TagBlock<const block_type> name
#else
	#define TAG_TBLOCK(name, block_type)	Yelo::TagBlock<block_type> name
#endif

// [arg2] = units
// [arg3] = documentation
#define TAG_FIELD(type, name, ...)			type name

#define TAG_ARRAY(type, name, count)		type name[count]

#define TAG_PAD(type, count)				Yelo::byte BOOST_JOIN(pad, __COUNTER__)[sizeof(type)*(count)]