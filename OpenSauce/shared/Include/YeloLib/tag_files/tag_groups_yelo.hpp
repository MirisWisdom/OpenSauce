/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template<tag K_TAG> class TagReference
	{
	public:
		// group tag identifier for this reference
		tag GroupTag;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_t Name;
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
		inline size_t SizeOf() const { return sizeof(T) * Count; }

		inline tag_block* to_tag_block() { return CAST_PTR(tag_block*, &this->Count); }

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
		inline T& operator [](int32 index)				{ return this->Definitions[index]; }
		// Indexer for getting a (const) definition reference via the definition's index in the block
		inline const T& operator [](int32 index) const	{ return this->Definitions[index]; }


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
		inline size_t Count() const { return Size / sizeof(T); }

		// Sizeof(T)
		inline size_t SizeOf() const { return sizeof(T); }

		inline T* operator [](int32 index) { return &this->Definitions[index]; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x14 );
#else
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x8 );
#endif
};


#include <YeloLib/tag_files/tag_groups_markup.hpp>