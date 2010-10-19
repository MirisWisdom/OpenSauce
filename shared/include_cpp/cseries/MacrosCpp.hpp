/*
    Kornner Studios: Shared Code

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
/*!
 * Code enriching (or obfuscating depending on how you look at things) interfaces
 */
#pragma once
#include <shell/Platform.hpp>


#pragma region cast macros
	/// Cast [value] to whatever
	#define CAST(type, value) (static_cast<type>(value))
	#define CAST_OP(type) static_cast<type>

	/// Cast the pointer [ptr] to a pointer of type [type]
	#define CAST_PTR(type, ptr) (reinterpret_cast<type>(ptr))
	#define CAST_PTR_OP(type) reinterpret_cast<type>

	/// Casts to [type] while removing any qualifiers of [value]
	/// [const], [volatile], and [__unaligned]
	#define CAST_QUAL(type, value) (const_cast<type>(value))
	#define CAST_QUAL_OP(type) const_cast<type>

#pragma endregion


#pragma region operator macros
	/// Implement an operator cast from the implementing class to [Type]
	#define OVERRIDE_OPERATOR_CAST_THIS(Type) \
		API_INLINE operator Type*() { return reinterpret_cast<Type*>(this); }
	/// Implement an operator cast from the implementing class to [Type] (by value)
	#define OVERRIDE_OPERATOR_CAST_THIS_(Type) \
		API_INLINE operator Type() { return *reinterpret_cast<Type*>(this); }

	/// Implement an operator cast from the implementing class to [Type] at [Field]'s offset in the class
	#define OVERRIDE_OPERATOR_CAST_THIS_BY_FIELD(Type, Field) \
		API_INLINE operator Type*() { return reinterpret_cast<Type*>(&this->Field); }

	/// Implement a operator override for [Sign] based on the parent type's [Field] member that results in a boolean operation
	#define OVERRIDE_OPERATOR_MATH_BOOL(Type, Field, Sign) \
		API_INLINE bool operator Sign(const Type& rhs) const { return this->Field Sign rhs.Field ; }
	#define OVERRIDE_OPERATOR_MATH_BOOL_TYPE(Type, Field, Sign) \
		API_INLINE bool operator Sign(const Type& rhs) const { return this->Field Sign rhs ; }

	/// Implement a operator override for [Sign] based on the parent type's [Field] member
	#define OVERRIDE_OPERATOR_MATH(ReturnType, Type, Field, Sign) \
		API_INLINE ReturnType operator Sign(const Type& rhs) { return this->Field Sign rhs.Field ; }
	#define OVERRIDE_OPERATOR_MATH_TYPE(ReturnType, Type, Field, Sign) \
		API_INLINE ReturnType operator Sign(const Type& rhs) { return this->Field Sign rhs ; }

	/// Implement an operator override for the parent type, allowing the user to specify some arguments and define its code following the macro block
	#define OVERRIDE_OPERATOR(Sign, ReturnType, ...) \
		ReturnType operator Sign(__VA_ARGS__)

#pragma endregion


#pragma region pad/unknown/unused macros
	// Pad a structure using a byte[] field named pad[num] by [count] bytes
	#define PAD(num, count) byte pad##num##[ count ]
	// Pad a structure using a byte[] field named pad_[name] by [count] bytes
	#define PAD_(name, count) byte pad_##name##[ count ]
	// Markup a field as unknown
	#define UNKNOWN(num) Unknown##num

	/// Add an anonymous 8-bit (1 byte) field to a structure.
	#define PAD8 unsigned char : 8;
	/// Add an anonymous 16-bit (2 byte) field to a structure.
	#define PAD16 unsigned short : 16;
	/// Add an anonymous 24-bit field to a structure.
	#define PAD24 unsigned char : 8; unsigned short : 16;
	/// Add an anonymous 32-bit (4 byte) field to a structure.
	#define PAD32 unsigned long : 32;
	/// Add an anonymous 48-bit field to a structure.
	#define PAD48 unsigned short : 16; unsigned long : 32;
	/// Add an anonymous 64-bit (8 byte) field to a structure.
	#define PAD64 unsigned __int64 : 64;
	/// Add an anonymous 128-bit (16 byte) field to a structure.
	#define PAD128 unsigned __int64 : 64; unsigned __int64 : 64;

	// Add a field to a structure that pads as if it were of type [type]
	#define PAD_TYPE(type) pad_##type
	// Add a field to a structure that markups what is thought to be a field of type [type]
	#define UNKNOWN_TYPE(type) pad_##type
	// Add a field to a structure that markups a unused field of type [type]
	#define UNUSED_TYPE(type) pad_##type

#pragma endregion


// Library's function convention
#define API_FUNC __stdcall

// Declare a function to be thread friendly
#define API_THREAD_SAFE __declspec( thread )

// Declare a definition/instance to be aligned to [alignment]
#define API_ALIGN(alignment) __declspec(align(alignment))

// Declare a function that is inlined into caller objects
#define API_INLINE inline

#define API_IMPORT __declspec(dllimport)
#define API_EXPORT __declspec(dllexport)

// Documentation purposes only, used to document the export index of a library function
#define API_EXPORTNUM(index)

#ifdef _DEBUG
	#define API_DEBUG

	// Runs code only if API_DEBUG is defined
	#define DebugOnly( ... ) __VA_ARGS__

	// Gets the current function's name
	#define DebugFuncName() __FUNCTION__
	// Gets the current function's name using c++ decoration
	#define DebugFuncNameCpp() __FUNCDNAME__
	// Gets the current function's signature
	#define DebugFuncSignature() __FUNCSIG__
#else

	// Runs code only if API_DEBUG is defined
	#define DebugOnly( ... )

	// Not used in RELEASE
	#define DebugFuncName() /*""*/
	// Not used in RELEASE
	#define DebugFuncNameCpp()/*""*/
	// Not used in RELEASE
	#define DebugFuncSignature() /*""*/
#endif


// Returns a value which is [value] padded out to be aligned on a [page_size] memory page
#define ALIGN(value, page_size)										\
	(																\
		( ((value) + ((page_size)-1)) / (page_size) ) * (page_size)	\
	)

// 64-bit safe as size_t is used for converting [value]
#define BIT_ALIGN(value, algn_bits)													\
	(																				\
		( CAST(size_t, (value) ) + ((1<<CAST(unsigned char, (algn_bits) ))-1) ) &	\
		( ~((1<<CAST(unsigned char, (algn_bits) ))-1) )								\
	)

#define BIT_ALIGN_ADDRESS(addr, algn_bits)												\
	(																					\
		( CAST_PTR(UINT_PTR, (addr) ) + ((1<<CAST(unsigned char, (algn_bits) ))-1) ) &	\
		( ~((1<<CAST(unsigned char, (algn_bits) ))-1) )									\
	)

// [mul] must be a power of two
#define NEXT_MULTIPLE_OF(mul, value) \
	( ((value) + ((mul)-1)) & (~((mul)-1)) )


// USE BOOST_STATIC_ASSERT INSTEAD!
// If [expression] equals false, a compile error will occur and be prompted
//#define COMPILE_ASSERT( expression ) \
//	typedef char ___C_ASSERT__[ (expression) ? 1 : -1 ]

/// Calculates the location in memory of a given field of class\struct [cls] from the
/// start of the class\struct.
// USE 'offsetof' MACRO INSTEAD! (stddef.h)
//#define FIELD_OFFSET(cls, field) ((unsigned long)((const char *)&(((cls *)0)->field)-(const char *)0))

#if defined(_WIN64)
	/// Size of a field in an class\struct
	#define FIELD_SIZEOF(cls, field) sizeof( CAST_PTR(const volatile cls*,0)->field )
#else
	/// Size of a field in an class\struct
	#define FIELD_SIZEOF(cls, field) sizeof( CAST_PTR(const volatile cls*,0)->field )
#endif


/// checks to see if [value] is in between [lower] and [upper]
#define IN_RANGE(value, lower, upper) ((value) >= (lower) && (value) <= (upper))
/// checks to see if [value] is in between 0 and [max]
#define IN_RANGE_ENUM(value, max) ((value) >= 0 && (value) < (max))

/// Compile-time constant calculate of a inplace array (can't be a pointer)
#define NUMBEROF(array) ( sizeof(array) / sizeof( (array)[0] ) )


// returns the amount of bits that make up [type]
#define BIT_COUNT(type) ( sizeof(type) * 8 )

#define BIT32(bit) (1L<<(bit))
#define TEST_BIT32(flags, bit) ( ((flags) & BIT32(bit)) !=0 )
#define SET_BIT32(flags, bit, value) ( (value) ? ( (flags) |= BIT32(bit) ) : ( flags&= ~BIT32(bit) ) )

#define BIT16(bit) (1<<(bit))
#define TEST_BIT16(flags, bit) ( ((flags) & BIT16(bit)) !=0 )
#define SET_BIT16(flags, bit, value) ((void)( (value) ? ( (flags) |= BIT16(bit) ) : ( flags &= ~BIT16(bit) )) )

#define TEST_BIT(obj, flag) ( (obj) & (flag) )
#define SET_BIT(obj, flag, value) ( (void)( (value) ? ( (obj)|=(flag) ) : ( obj &= ~(flag) )) )

#define MASK(count) ( (unsigned)(1 << (count)) - (unsigned)1 )

// checks to see if [value] has only the bits its allowed to have set
#define VALID_BITS(value, count) \
	( \
		( \
			(value) & ( ~(1<<(count)) ) \
		) == 0 \
	)

// How many 8 bit integers it takes to hold a bit vector with [size] bits
#define BIT_VECTOR_SIZE_IN_BYTES(size)			( ((size) + 7 ) >> 3 )
// How many 16 bit integers it takes to hold a bit vector with [size] bits
#define BIT_VECTOR_SIZE_IN_WORDS(size)			( ((size) + 15) >> 4 )
// How many 32 bit integers it takes to hold a bit vector with [size] bits
#define BIT_VECTOR_SIZE_IN_DWORDS(size)			( ((size) + 31) >> 5 )
// How many total bits are in the supplied bit vector size and type container
#define BIT_VECTOR_SIZE_IN_BITS(size, type)		( (size) * BIT_COUNT(type) )
#define BIT_VECTOR_TEST_FLAG8(vector, bit)			TEST_BIT32(CAST_PTR(unsigned char*,vector) [bit>>3], bit&7)
#define BIT_VECTOR_SET_FLAG8(vector, bit, value)	SET_BIT32 (CAST_PTR(unsigned char*,vector) [bit>>3], bit&7,  value)
#define BIT_VECTOR_TEST_FLAG16(vector, bit)			TEST_BIT32(CAST_PTR(unsigned short*,vector)[bit>>4], bit&15)
#define BIT_VECTOR_SET_FLAG16(vector, bit, value)	SET_BIT32 (CAST_PTR(unsigned short*,vector)[bit>>4], bit&15, value)
#define BIT_VECTOR_TEST_FLAG32(vector, bit)			TEST_BIT32(CAST_PTR(unsigned long*,vector) [bit>>5], bit&31)
#define BIT_VECTOR_SET_FLAG32(vector, bit, value)	SET_BIT32 (CAST_PTR(unsigned long*,vector) [bit>>5], bit&31, value)

// Extracts the bits at [bit_low] to [bit_hi] in [value] 
// and returns the result value
#define BIT_FIELD_EXTRACT_RANGE(value, bit_low, bit_hi)			((value >> bit_low) & ((2 << (bit_hi-bit_low))-1))
// Extracts the bits at [bit_offset] to [bit_offset+bit_count] 
// in [value] and returns the result
#define BIT_FIELD_EXTRACT_VALUE(value, bit_offset, bit_count)	BIT_FIELD_EXTRACT_RANGE(value, bit_offset, bit_offset + (bit_count-1))



#ifndef NULL
	// null object value
	#define	NULL 0
#endif
// null handle value
#define	NULL_HANDLE 0xFFFFFFFF
// sentinel value for indexers and other stuff
#define NONE -1


#if defined(_DEBUG)
	#define MACRO_ML_START	do{
	#define MACRO_ML_END	}while(false)
#else	// for release, use a more compiler friendly statment
	#define MACRO_ML_START	if(true){
	#define MACRO_ML_END	} else
#endif

// Use 'BOOST_JOIN' instead
//#define MACRO_JOIN_TOKEN3(a, b) a##b
//#define MACRO_JOIN_TOKEN2(a, b) MACRO_JOIN_TOKEN3(a, b)
//// Join two macro arguments, even if the argument is a macro as well
//#define MACRO_JOIN_TOKEN(a, b) MACRO_JOIN_TOKEN2(a, b)

#define API_BREAKPOINT() ( (*CAST_PTR(int*, 0)) = 0 )

// Turns a argument into a varg-able argument
// [arg] should be the argument before an ellipses argument in a function
#define VARG_FROM_ARG(arg) ((char*)(&arg+1))


// a la Nocturnal Initiative
#define __DOC_TODO_STRING2(x) #x
#define __DOC_TODO_STRING(x) __DOC_TODO_STRING2(x)
#define DOC_TODO(msg) __pragma( message(__FILE__ "(" __DOC_TODO_STRING(__LINE__) "): TODO: " msg) )




#ifndef _WIN64
	// Declare a function naked of all things
	#define API_FUNC_NAKED __declspec(naked)

	// Start the code to a naked function which takes arguments
	#define API_FUNC_NAKED_START() __asm	\
		{									\
			__asm push	ebp					\
			__asm mov	ebp, esp

	// End the code to a naked function which takes arguments 
	// and is also __stdcall
	#define API_FUNC_NAKED_END(arg_count)	\
			__asm mov	esp, ebp			\
			__asm pop	ebp					\
			__asm retn	(arg_count * 4)		\
		}

	// Start the code to a naked function with no args
	#define API_FUNC_NAKED_START_() __asm	\
		{

	// End the code to a naked function with no args
	#define API_FUNC_NAKED_END_()		\
			__asm retn					\
		}
#else
	#define API_FUNC_NAKED

	#define API_FUNC_NAKED_START()
	#define API_FUNC_NAKED_END(arg_count)

	#define API_FUNC_NAKED_START_()
	#define API_FUNC_NAKED_END_()
#endif