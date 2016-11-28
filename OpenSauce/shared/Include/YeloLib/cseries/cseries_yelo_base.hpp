/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <cseries/MacrosCpp.hpp>

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/debug_memory_yelo.hpp>
#include <YeloLib/cseries/errors_yelo.hpp>
#include <YeloLib/cseries/handle_abi.hpp>

#include <boost/integer/static_log2.hpp>

//#include <errno.h>
extern const errno_t k_errnone;

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_uuid_buffer_size = 16,

			k_kilo = 1024,
			k_mega = k_kilo * k_kilo,
		};
	};

	namespace Flags
	{
		enum {
			k_alignment_8bit,
			k_alignment_16bit,
			k_alignment_32bit,
			k_alignment_64bit,
			k_alignment_128bit,
			k_alignment_256bit,
			k_alignment_512bit,
			k_alignment_1024bit,
			k_alignment_2048bit,
			k_alignment_4096bit,
		};
	};

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
	// System Initialize for bsp load function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_initialize_for_new_bsp)();
	// System Dispose for bsp unload function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_dispose_from_old_bsp)();
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
		
		template<typename T, const size_t k_offset>
		T GetData()					{ return *(	CAST_PTR(T*,		&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset>
		T GetData() const			{ return *(	CAST_PTR(const T*,	&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset>
		T* GetDataPtr()				{ return	CAST_PTR(T*,		&m_data[k_offset]); }
		template<typename T, const size_t k_offset>
		const T* GetDataPtr() const	{ return	CAST_PTR(const T*,	&m_data[k_offset]); }

		// Usage - "struct s_some_object : TStructImpl(0x40) {};"
		#define TStructImpl(size) public TStruct< size >

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-value getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
		#define TStructGetImpl(type, name, offset)										\
			type Get##name()					{ return GetData<type, offset>(); }		\
			type Get##name() const			{ return GetData<type, offset>(); }			\
			BOOST_STATIC_ASSERT( ( offset + sizeof( type )) <= k_size );
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Implement a by-address getter. </summary>
		///
		/// <param name="type">  	Getter's return type. </param>
		/// <param name="name">  	Getter's method name. </param>
		/// <param name="offset">	Field offset within the struct to treat as the get result. </param>
		#define TStructGetPtrImpl(type, name, offset)									\
			type* Get##name()				{ return GetDataPtr<type, offset>(); }		\
			type const* Get##name() const	{ return GetDataPtr<type, offset>(); }		\
			/*   ^ use const here, instead of before the type, in case [type] is defined as something like "int32*" */	\
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


	// TODO: refactor these to PascalCase

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether an ASCII string is NULL or begins with a null terminator </summary>
	inline bool is_null_or_empty( cstring const str) { return str == nullptr || str[0] ==  '\0'; }
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether an ASCII string buffer begins with a null terminator </summary>
	template<size_t kLength> inline
	bool is_null_or_empty(const char (&array)[kLength])
	{
		return array[0] == '\0';
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string is NULL or begins with a null terminator </summary>
	inline bool is_null_or_empty(wcstring const str) { return str == nullptr || str[0] == L'\0'; }
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Tests whether a wide string buffer begins with a null terminator </summary>
	template<size_t kLength> inline
	bool is_null_or_empty(const wchar_t (&array)[kLength])
	{
		return array[0] == L'\0';
	}

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
	/// <summary>	Determines whether the end of the string instance matches the specified suffix string. </summary>
	///
	/// <param name="str">   	The string in question. </param>
	/// <param name="suffix">	The string to compare to the substring at the end of this instance. </param>
	///
	/// <returns>	true if suffix matches the end of the instance; otherwise, false. </returns>
	bool EndsWith(const std::string& str, const std::string& suffix);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// 	Allows us to interpret a machine word (in this case, 32-bits) as various sorts of types. Either as a pointer,
	/// 	a value-array, or just vanilla values.
	/// 	It's a shitty struct name, but fuck, this has been around almost as-is since...2006 or earlier.
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

		// hope you know what you're doing if you use this!
		bool IsNull() const { return pointer == nullptr; }

		// default equality logic is to compare the pointer (32) bits.
		// could be problematic for value types <32bits like bool, where the upper bits aren't zeroed.
		// at which point you should be doing value.boolean == ... anyway.
		OVERRIDE_OPERATOR_MATH_BOOL(TTypeHolder, pointer, == );
		OVERRIDE_OPERATOR_MATH_BOOL(TTypeHolder, pointer, != );
	};
	typedef TTypeHolder<void> TypeHolder;
	BOOST_STATIC_ASSERT( sizeof(TypeHolder) == 0x4 );

	extern const TypeHolder k_null_as_type_holder;  ///< nullptr represented as a TypeHolder value
	extern const TypeHolder k_none_as_type_holder;  ///< NONE represented as TypeHolder value

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the COM interface reference isn't NULL, releases it and NULL it. </summary>
	///
	/// <tparam name="TInterface">	COM interface type. </tparam>
	/// <param name="obj">	[in,out] If non-null, the COM object. </param>
	template<typename TInterface> inline
	void safe_release(TInterface*& obj) // TODO: refactor to SafeRelease
	{
		if(obj != nullptr)
		{
			obj->Release();
			obj = nullptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Class for automatically releasing a COM object when it goes out of scope. </summary>
	///
	/// <tparam name="TInterface">	COM interface type. </tparam>
	template<typename TInterface>
	class c_auto_release final
	{
		TInterface* m_target_object;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="target_object">	[in] If non-null, the target object. </param>
		c_auto_release(TInterface* target_object)
			: m_target_object(target_object)
		{ }

		/// <summary>	Default constructor. </summary>
		c_auto_release()
			: m_target_object(nullptr)
		{ }

		/// <summary>	Destructor. </summary>
		~c_auto_release()
		{
			if(m_target_object)
			{
				m_target_object->Release();
			}
			m_target_object = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Assignment operator. Sets the raw value to the provided value. </summary>
		///
		/// <param name="value">	The value to set. </param>
		///
		/// <returns>	The current value. </returns>
		TInterface*& operator=(const TInterface*& value)
		{
			m_target_object = value;

			return m_target_object;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Member reference operator. </summary>
		///
		/// <returns>	The referenced member. </returns>
		TInterface** operator&()
		{
			return &m_target_object;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Member dereference operator. </summary>
		///
		/// <returns>	The dereferenced object. </returns>
		TInterface*& operator->()
		{
			return m_target_object;
		}

		/// <summary>	Support for casting the object to the templated type. </summary>
		operator TInterface*&()
		{
			return m_target_object;
		}
	};

	/// <summary>	Class for executing a function at the end of a scope. </summary>
	class c_scope_end_execute
	{
		/// <summary>	Defines an alias representing the end execute function. </summary>
		typedef std::function<void()> end_execute_func_t;

		const end_execute_func_t m_function;

	public:
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="function">	The function to run. </param>
		c_scope_end_execute(const end_execute_func_t& function)
			: m_function(function)
		{ }

		/// <summary>	Destructor. </summary>
		~c_scope_end_execute()
		{
			m_function();
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the object pointer reference isn't NULL, deletes it and NULL it. </summary>
	///
	/// <param name="obj">	[in,out] The object to delete. NULL on return </param>
	template<typename T> inline
	void SafeDelete(T*& obj)
	{
		if(obj != nullptr)
		{
			delete obj;
			obj = nullptr;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	If the object pointer reference isn't NULL, deletes it and NULL it. </summary>
	///
	/// <param name="obj">	[in,out] The object to delete. NULL on return </param>
	template<typename T> inline
	void SafeDeleteArray(T*& obj)
	{
		if(obj != nullptr)
		{
			delete[] obj;
			obj = nullptr;
		}
	}

	// Returns INVALID_HANDLE_VALUE as nullptr, else returns [h]
	// Should be used in conjunction with std::unique_ptr and winapi_handle_closer
	inline HANDLE ToSafeHandle(HANDLE h)
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
	/// <summary>	Tests whether an array is bitwise zero. </summary>
	///
	/// <param name="array">	Array to test. </param>
	/// <param name="index"> 	Element index in array to start testing at. </param>
	/// 
	///  <returns>	true if the array elements are equal to bitwise zero, false if one or more aren't. </returns>
	template<typename T, size_t kLength> inline
	bool ArrayIsZero (T (&array)[kLength], size_t index = 0)
	{
		T zero = T();
		for (size_t x = index; x < kLength; x++)
			if (memcmp(&array[x], &zero, sizeof(zero)) != 0)
				return false;

		return true;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Clear an array to be bitwise zero. </summary>
	///
	/// <param name="array">	Array to zero. </param>
	/// <param name="index"> 	Element index in array to start zeroing at. </param>
	template<typename T, size_t kLength> inline
	void ArrayZero   (T (&array)[kLength], size_t index = 0)
	{
		memset(&array[index], 0, sizeof(T) * (kLength - index));
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="dst_index">	Element index in dst to start the copy at. </param>
	/// <param name="src">			Source array. </param>
	/// <param name="src_index">	Element index in src to start the copy from. </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template<typename T, size_t kDstLength, size_t kSrcLength> inline
	bool ArrayCopy   (T (&dst)[kDstLength], size_t dst_index, const T (&src)[kSrcLength], size_t src_index, size_t count = kSrcLength)
	{
		return memcpy_s(&dst[dst_index], sizeof(T) * (kDstLength - dst_index),
						&src[src_index], sizeof(T) * (count - src_index)) == k_errnone;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst"> 		Destination array. </param>
	/// <param name="src"> 		Source array </param>
	/// <param name="count">	The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template<typename T, size_t kDstLength, size_t kSrcLength> inline
	bool ArrayCopy   (T (&dst)[kDstLength], const T (&src)[kSrcLength], size_t count = kSrcLength)
	{
		return ArrayCopy(dst, 0, src, 0, count);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="dst_index">	Element index in dst to start the copy at. </param>
	/// <param name="src">			Pointer to the first source element.  </param>
	/// <param name="src_index">	Element index in src to start the copy from. </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template<typename T, size_t kDstLength> inline
	bool ArrayCopyPtr(T (&dst)[kDstLength], size_t dst_index, const T* src, size_t src_index, size_t count)
	{
		return memcpy_s(&dst[dst_index], sizeof(T) * (kDstLength - dst_index),
						&src[src_index], sizeof(T) *  count) == k_errnone;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Copy a range of elements from one array to another. </summary>
	///
	/// <param name="dst">			Destination array.</param>
	/// <param name="src">			Pointer to the first source element.  </param>
	/// <param name="count">		The number of elements to copy from src to dst. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	template<typename T, size_t kDstLength> inline
	bool ArrayCopyPtr(T (&dst)[kDstLength], const T* src, size_t count)
	{
		return ArrayCopyPtr(dst, 0, src, 0, count);
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
