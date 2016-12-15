/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template<tag K_TAG>
	struct TagReference
	{
		// group tag identifier for this reference
		tag GroupTag;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference Name;
		// length of the reference name
		int32 NameLength;
#endif

		// datum index of this reference in the tag index
		datum_index TagIndex;

		operator datum_index() { return TagIndex; }
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, TagIndex, ==);

		// Returns whether or not this reference has a valid group reference
		bool IsValid() const { return GroupTag == K_TAG; }

		// Sets this reference to null
		void Reset() { GroupTag = K_TAG; TagIndex = datum_index::null; }

		tag Tag() const { return K_TAG; }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagReference<NONE>) == 0x10 );
#else
	BOOST_STATIC_ASSERT( sizeof(TagReference<NONE>) == 0x8 );
#endif
	template<>
	inline bool TagReference<NONE>::IsValid() const { return true; }


	// Template'd tag block for allowing more robust code.
	template<typename T>
	struct TagBlock
	{
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
		union {
			// Pointer to the first tag block definition element
			void* Address;

			// Pointer to a specific block of object(s) of type T
			T* Definitions;
		};

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// definition pointer for this block instance
		const struct tag_block_definition* BlockDefinition;
#endif

		// Using the class's template 'T' parameter, calculates the total
		// size, in bytes, the elements assume in memory
		size_t SizeOf() const { return sizeof(T) * Count; }

		tag_block* to_tag_block()				{ return CAST_PTR(tag_block*, &this->Count); }
		const tag_block* to_tag_block() const	{ return CAST_PTR(const tag_block*, &this->Count); }

		// Sets this object to equal that of a anonymous tag block object. 
		TagBlock<T>& Copy(const tag_block& block)
		{
			this->Count = block.count;
			this->Address = block.address;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
			this->BlockDefinition = block.definition;
#endif

			return this;
		}

		// Indexer for getting a definition reference via the definition's index in the block
		T& operator[](int32 index)
		{
#if PLATFORM_IS_EDITOR
			return *blam::tag_block_get_element(*this, index);
#else
			YELO_ASSERT(index >= 0 && index < Count);
			return this->Definitions[index];
#endif
		}
		// Indexer for getting a (const) definition reference via the definition's index in the block
		const T& operator[](int32 index) const
		{
#if PLATFORM_IS_EDITOR
			return *blam::tag_block_get_element(*this, index);
#else
			YELO_ASSERT(index >= 0 && index < Count);
			return this->Definitions[index];
#endif
		}

#if PLATFORM_IS_EDITOR
		T* get_element(int32 element)			{ return blam::tag_block_get_element(*this, index); }
		void delete_element(int32 element)		{ blam::tag_block_delete_element(*this, element); }
		bool delete_all_elements()				{ return blam::tag_block_delete_all_elements(*this); }
		int32 add_element()						{ return blam::tag_block_add_element(*this); }
		bool resize(int32 element_count)		{ return blam::tag_block_resize(*this, element_count); }
		T* add_and_get_element()				{ return blam::tag_block_add_and_get_element(*this); }
#endif


		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		const_iterator	begin() const		{ return Definitions; }
		iterator		begin()				{ return Definitions; }
		const_iterator	const_begin() const	{ return Definitions; }
		const_iterator	const_begin()		{ return Definitions; }
		const_iterator	end() const			{ return Definitions + Count; }
		iterator		end()				{ return Definitions + Count; }
		const_iterator	const_end() const	{ return Definitions + Count; }
		const_iterator	const_end()			{ return Definitions + Count; }

		bool empty() const { return Count == 0; }
		size_t size() const { return CAST(size_t, Count); }
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagBlock<byte>) == 0xC );
#else
	BOOST_STATIC_ASSERT( sizeof(TagBlock<byte>) == 0x8 );
#endif
	namespace blam
	{
#if PLATFORM_IS_EDITOR
		template<typename T>
		T* tag_block_get_element(TagBlock<T>& block, int32 element)
		{
			return CAST_PTR(T*, tag_block_get_element(block.to_tag_block(), element));
		}
		template<typename T>
		const T* tag_block_get_element(const TagBlock<T>& block, int32 element)
		{
			return CAST_PTR(const T*, tag_block_get_element(block.to_tag_block(), element));
		}
		template<typename T>
		int32 tag_block_add_element(TagBlock<T>& block)
		{
			return tag_block_add_element(block.to_tag_block());
		}
		template<typename T>
		bool tag_block_resize(TagBlock<T>& block, int32 element_count)
		{
			return tag_block_resize(block.to_tag_block(), element_count);
		}
		template<typename T>
		void tag_block_delete_element(TagBlock<T>& block, int32 element)
		{
			tag_block_delete_element(block.to_tag_block(), element);
		}

		template<typename T>
		T* tag_block_add_and_get_element(TagBlock<T>& block)
		{
			return CAST_PTR(T*, tag_block_add_and_get_element(block.to_tag_block()));
		}
#endif
	};


	// Template'd tag_data for more robust code dealing with known
	// sub-structures.
	template<typename T>
	struct TagData
	{
		typedef T*			iterator;
		typedef const T*	const_iterator;
		typedef T			value_type;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T*			pointer;
		typedef const T*	const_pointer;

		// Size, in bytes, of the data blob
		int32 Size;

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		long_flags Flags;
		int32 StreamPosition;
#endif

		union {
			// Address of the data blob
			void* Address;

			T* Definitions;
		};

#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		const struct tag_data_definition* DataDefinition;
#endif

		// How many [T] objects are actually in the definition.
		size_t Count() const { return Size / sizeof(T); }

		// Sizeof(T)
		size_t SizeOf() const { return sizeof(T); }

		tag_data* to_tag_data()				{ return CAST_PTR(tag_data*, &this->Size); }
		const tag_data* to_tag_data() const	{ return CAST_PTR(const tag_data*, &this->Size); }

		T* operator[](int32 index) { return &this->Definitions[index]; }

		//////////////////////////////////////////////////////////////////////////
		// STL-like APIs
		const_iterator	begin() const		{ return Definitions; }
		iterator		begin()				{ return Definitions; }
		const_iterator	const_begin() const	{ return Definitions; }
		const_iterator	const_begin()		{ return Definitions; }
		const_iterator	end() const			{ return Definitions + Count(); }
		iterator		end()				{ return Definitions + Count(); }
		const_iterator	const_end() const	{ return Definitions + Count(); }
		const_iterator	const_end()			{ return Definitions + Count(); }

		bool empty() const { return Size == 0; }
		size_t size() const { return CAST(size_t, Count()); }


#if PLATFORM_IS_EDITOR
		void resize(size_t new_size = 0)	{ blam::tag_data_resize(this, new_size); }
#endif
	};
#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x14 );
#else
	BOOST_STATIC_ASSERT( sizeof(TagData<byte>) == 0x8 );
#endif
	namespace blam
	{
#if PLATFORM_IS_EDITOR
		template<typename T>
		bool tag_data_resize(TagData<T>& data, size_t new_size = 0)
		{
			return tag_data_resize(data.to_tag_data(), new_size);
		}
#endif
	};
};


#include <YeloLib/tag_files/tag_groups_markup.hpp>