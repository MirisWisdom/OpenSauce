/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <cseries/MacrosCpp.hpp>

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/debug_memory_yelo.hpp>
#include <YeloLib/cseries/errors_yelo.hpp>

#include <boost/integer/static_log2.hpp>

//#include <errno.h>
extern const errno_t k_errnone;

namespace Yelo
{
	// System Initialize function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_initialize)();
	// System Dispose function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_dispose)();
	// System Initialize for new map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_initialize_for_new_map)();
	// System Dispose from old map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_dispose_from_old_map)();
	// System Update function pointer
	typedef void (API_FUNC* proc_update)(float);
	// System Update function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_update_no_args)();


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Class for defining an interface for blocks of data whose memory layout is not entirely mapped out.
	/// </summary>
	///
	/// <typeparam name="K_SIZE">	Size of the memory block. </typeparam>
	template<const size_t K_SIZE>
	struct TStruct {
		enum { k_size = K_SIZE };

	protected:
		// NOTE: I would use std::array here, but I have yet to tested how well it plays with xbox modules (ie, Halo2_Xbox)
		byte m_data[K_SIZE];
		
		template<typename T, const size_t k_offset> inline
		T GetData()					{ return *(	CAST_PTR(T*,		&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> inline
		T GetData() const			{ return *(	CAST_PTR(const T*,	&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> inline
		T* GetDataPtr()				{ return	CAST_PTR(T*,		&m_data[k_offset]); }
		template<typename T, const size_t k_offset> inline
		const T* GetDataPtr() const	{ return	CAST_PTR(const T*,	&m_data[k_offset]); }

		// Usage - "struct s_some_object : TStructImpl(0x40) {};"
		#define TStructImpl(size) public TStruct< size >

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
		#define TStructGetImpl(type, name, offset)												\
			inline type Get##name()					{ return GetData<type, offset>(); }			\
			inline type Get##name() const			{ return GetData<type, offset>(); }			\
			BOOST_STATIC_ASSERT( ( offset + sizeof( type )) <= k_size );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
		#define TStructGetPtrImpl(type, name, offset)											\
			inline type* Get##name()				{ return GetDataPtr<type, offset>(); }		\
			inline type const* Get##name() const	{ return GetDataPtr<type, offset>(); }		\
			/*          ^ use const here, instead of before the type, in case [type] is defined as something like "int32*" */	\
			BOOST_STATIC_ASSERT( ( offset + sizeof( type )) <= k_size );

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		/// 
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
		#define TStructSubGetImpl(type, name, offset)		TStructGetImpl(type, name, offset - DATA_OFFSET)
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter for fake TStruct sub-classes. </summary>
		///
		/// <remarks>
		/// 	Requires an 'DATA_OFFSET' constant to be defined, representing the start of the fake sub-class.
		/// </remarks>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the fake sub-class to treat as the get result. </param>
		#define TStructSubGetPtrImpl(type, name, offset)	TStructGetPtrImpl(type, name, offset - DATA_OFFSET)
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Describes the number of bits needed in a bitfield for a whole number. </summary>
	///
	/// <typeparam name="max_value">	The inclusive max value of the bit field. </typeparam>
	template<boost::static_log2_argument_type max_value>
	struct bitfield_size { enum { 
		value = boost::static_log2<max_value>::value + 1
	}; };
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Describes the number of bits needed in a bitfield for an enum value. </summary>
	///
	/// <typeparam name="number_of_members">	The number of members in the enum represented by this bitfield. </typeparam>
	template<boost::static_log2_argument_type number_of_members>
	struct bitfield_enum_size { enum { 
		value = boost::static_log2<number_of_members>::value
	}; };


	extern const sbyte K_SBYTE_MIN;		/// <summary>	min value for a [sbyte]. </summary>
	extern const sbyte K_SBYTE_MAX;		/// <summary>	max value for a [sbyte]. </summary>
	extern const byte K_BYTE_MIN;		/// <summary>	min value for a [byte]. </summary>
	extern const byte K_BYTE_MAX;		/// <summary>	max value for a [byte]. </summary>
	extern const int16 K_INT16_MIN;		/// <summary>	min value for a [int16]. </summary>
	extern const int16 K_INT16_MAX;		/// <summary>	max value for a [int16]. </summary>
	extern const uint16 K_UINT16_MIN;	/// <summary>	min value for a [uint16]. </summary>
	extern const uint16 K_UINT16_MAX;	/// <summary>	max value for a [uint16]. </summary>
	extern const int32 K_INT32_MIN;		/// <summary>	min value for a [int32]. </summary>
	extern const int32 K_INT32_MAX;		/// <summary>	max value for a [int32]. </summary>
	extern const uint32 K_UINT32_MIN;	/// <summary>	min value for a [uint32]. </summary>
	extern const uint32 K_UINT32_MAX;	/// <summary>	max value for a [uint32]. </summary>
	extern const int64 K_INT64_MIN;		/// <summary>	min value for a [int64]. </summary>
	extern const int64 K_INT64_MAX;		/// <summary>	max value for a [int64]. </summary>
	extern const uint64 K_UINT64_MIN;	/// <summary>	min value for a [uint64]. </summary>
	extern const uint64 K_UINT64_MAX;	/// <summary>	max value for a [uint64]. </summary>
	extern const real K_REAL_MIN;		/// <summary>	min value for a [real]. </summary>
	extern const real K_REAL_MAX;		/// <summary>	max value for a [real]. </summary>


	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string is NULL or contains no characters </summary>
	inline bool is_null_or_empty( cstring const str) { return str == nullptr || str[0] ==  '\0'; }
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string is NULL or contains no characters </summary>
	inline bool is_null_or_empty(wcstring const str) { return str == nullptr || str[0] == L'\0'; }

	// Takes [wide] and converts it to an ascii string, to be held in [string]. If [wide_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [string] if successful
	// If NULL is returned, you can use GetLastError() for error information
	char* wstring_to_string(char* string, int32 string_length, wcstring wide, int32 wide_length = -1);

	// [string_length] includes the null terminator
	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide);

	// Takes [string] and converts it to an unicode string, to be held in [wide]. If [string_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [wide] if successful
	// If NULL is returned, you can use GetLastError() for error information
	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length = -1);

	// [string_length] includes the null terminator
	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Allows us to interpret a machine word (in this case, 32-bits) as various sorts of types. Either as a pointer,
	/// 	a value-array, or just vanilla values.
	/// </summary>
	///
	/// <typeparam name="T">	. </typeparam>
	template<typename T>
	struct TTypeHolder {
		union { // t_type_interface_union
			T* pointer;

			union {// t_type_interface_pointers
				bool* boolean;
				char* character;
				Yelo::byte* byte;
				Yelo::sbyte* sbyte;
				Yelo::uint16* uint16;
				Yelo::int16* int16;
				Yelo::uint32* uint32;
				Yelo::int32* int32;
				Yelo::real* real;

				datum_index* datum;

				Yelo::cstring ascii;
				Yelo::wstring unicode;
			}ptr;

			union {// t_type_interface_arrays
				bool boolean[1];
				char character[1];
				Yelo::byte byte[1];
				Yelo::sbyte sbyte[1];
				Yelo::uint16 uint16[1];
				Yelo::int16 int16[1];
				Yelo::uint32 uint32[1];
				Yelo::int32 int32[1];
				Yelo::real real[1];

				datum_index datum[1];

				Yelo::cstring ascii[1];
				Yelo::wstring unicode[1];
			}array;

			bool boolean;
			char character;
			Yelo::byte byte;
			Yelo::sbyte sbyte;
			Yelo::uint16 uint16;
			Yelo::int16 int16;
			Yelo::uint32 uint32;
			Yelo::int32 int32;
			Yelo::real real;

			datum_index datum;
		};
	};
	typedef TTypeHolder<void> TypeHolder;
	BOOST_STATIC_ASSERT( sizeof(TypeHolder) == 0x4 );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the COM interface reference isn't NULL, releases it and NULL it. </summary>
	///
	/// <typeparam name="typename TInterface">	COM interface type. </typeparam>
	/// <param name="obj">	[in,out] If non-null, the COM object. </param>
	template<typename TInterface> inline
	void safe_release(TInterface*& obj)
	{
		if(obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}

	// Returns INVALID_HANDLE_VALUE as nullptr, else returns [h]
	// Should be used in conjunction with std::unique_ptr and winapi_handle_closer
	inline HANDLE safe_handle(HANDLE h)
	{
		return h == INVALID_HANDLE_VALUE ? nullptr : h;
	}
	/// <summary>	Primarily a deleter for std::unique_ptr for use with WinAPI handles. </summary>
	struct winapi_handle_closer
	{
		void operator()(HANDLE h) const;
	};

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	/// <summary>	Primarily a deleter for std::unique_ptr for objects allocated by LocalAlloc. </summary>
	struct winapi_local_deleter
	{
		void operator()(HLOCAL h) const;
	};

	/// <summary>	Primarily a deleter for std::unique_ptr for use CRT's FILE. </summary>
	struct crt_file_closer
	{
		void operator()(FILE* h) const;
	};
#endif

	cstring BooleanToString(bool value);


#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	typedef int (__cdecl* proc_stdlib_compare)(void*, const void*, const void*);

	template<typename T, size_t k_array_size, typename TContext = void*> inline
	void Qsort(T (&_Base)[k_array_size],
		int (__cdecl* _PtFuncCompare)(TContext, const T*, const T*), TContext _Context = nullptr)
	{
		static_assert(sizeof(TContext) <= sizeof(void*), "Context type is incompatible with C runtime API");

		::qsort_s(_Base, k_array_size, sizeof(T), CAST_PTR(proc_stdlib_compare,_PtFuncCompare), 
			CAST_PTR(void*, _Context));
	}
	template<typename T, typename TContext = void*, typename TCompareParam = const T*> inline
	void Qsort(T* _Base, rsize_t _NumOfElements,
		int (__cdecl* _PtFuncCompare)(TContext, TCompareParam, TCompareParam),
		TContext _Context = nullptr)
	{
		static_assert(sizeof(TContext) <= sizeof(void*), "Context type is incompatible with C runtime API");

		::qsort_s(_Base, _NumOfElements, sizeof(T), CAST_PTR(proc_stdlib_compare,_PtFuncCompare), 
			CAST_PTR(void*, _Context));
	}

	template<typename TKey, typename T, size_t k_array_size, typename TContext = void*> inline
	T* Bsearch(const TKey* _Key, T (&_Base)[k_array_size],
		int (__cdecl* _PtFuncCompare)(TContext, const TKey*, const T*), TContext _Context = nullptr)
	{
		static_assert(sizeof(TContext) <= sizeof(void*), "Context type is incompatible with C runtime API");

		return CAST_PTR(T*,
			::bsearch_s(_Key, _Base, k_array_size, sizeof(T), CAST_PTR(proc_stdlib_compare, _PtFuncCompare), 
				CAST_PTR(void*, _Context)));
	}
	template<typename TKey, typename T, typename TContext = void*, typename TCompareParam = const T*> inline
	T* Bsearch(const TKey* _Key, T* _Base, rsize_t _NumOfElements,
		int (__cdecl* _PtFuncCompare)(TContext, const TKey*, TCompareParam), TContext _Context = nullptr)
	{
		static_assert(sizeof(TContext) <= sizeof(void*), "Context type is incompatible with C runtime API");

		return CAST_PTR(T*,
			::bsearch_s(_Key, _Base, _NumOfElements, sizeof(T), CAST_PTR(proc_stdlib_compare, _PtFuncCompare), 
				CAST_PTR(void*, _Context)));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the current time and format it into [time_str]. </summary>
	///
	/// <param name="time_str">	The resulting time string. </param>
	void GetTimeStampString(_Out_ tag_string time_str);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Get the current time and format it into [time_str] using a file name friendly format. </summary>
	///
	/// <remarks>	The formatting output by this function is YYYY_MM_DD_hh_mm_ss. </remarks>
	///
	/// <param name="time_str">	The resulting time string. </param>
	void GetTimeStampStringForFile(_Out_ tag_string time_str);

	// Displays a message to the user using the WinAPI
	// Use this when are probably about to get really messy...
	void PrepareToDropError(cstring text);
#endif
};

#if defined(_DEBUG)
	// Mostly just useful for debug checking something (eg, game memory) on startup or on the first pass
	#define DebugRunOnce(...)							\
	{	static volatile bool iran_once_##__COUNTER__;	\
		if( iran_once_##__COUNTER__ == false )			\
		{												\
			iran_once_##__COUNTER__ = true;				\
			__VA_ARGS__	;								\
	} }
#else
	#define DebugRunOnce(...) __noop
#endif

#define HRESULT_ERETURN(p) if(FAILED(p)) return E_FAIL
