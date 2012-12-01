/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <cseries/MacrosCpp.hpp>

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/debug_memory_yelo.hpp>
#include <YeloLib/cseries/errors_yelo.hpp>

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


	/// Template class for defining an interface for blocks of data whose memory layout is not entirely mapped out
	template<const size_t K_SIZE> struct TStruct {
		static const size_t k_size = K_SIZE;

	protected:
		unsigned char m_data[K_SIZE];
		
		template<typename T, const size_t k_offset> API_INLINE T GetData()					{ return *( CAST_PTR(T*,		&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> API_INLINE T GetData() const			{ return *( CAST_PTR(const T*,	&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> API_INLINE T* GetDataPtr()				{ return	CAST_PTR(T*,		&m_data[k_offset]); }
		template<typename T, const size_t k_offset> API_INLINE const T* GetDataPtr() const	{ return	CAST_PTR(const T*,	&m_data[k_offset]); }

		// Usage - "struct some_object : TStructImpl(0x40) {};"
		#define TStructImpl(size) public TStruct< size >

		// Implement a by-value getter
		#define TStructGetImpl(type, name, offset)															\
			API_INLINE type Get##name()					{ return GetData<type, offset>(); }					\
			API_INLINE type Get##name() const			{ return GetData<type, offset>(); }					\
			BOOST_STATIC_ASSERT( ( offset + sizeof( type )) <= k_size );
		// Implement a by-address getter
		#define TStructGetPtrImpl(type, name, offset)														\
			API_INLINE type* Get##name()				{ return GetDataPtr<type, offset>(); }				\
			API_INLINE type const* Get##name() const	{ return GetDataPtr<type, offset>(); }				\
			/*              ^ use const here, instead of before the type, in case [type] is defined as something like "int32*" */	\
			BOOST_STATIC_ASSERT( ( offset + sizeof( type )) <= k_size );

		// Implement a by-value getter for fake TStruct sub-classes
		#define TStructSubGetImpl(type, name, offset)		TStructGetImpl(type, name, offset - DATA_OFFSET)
		// Implement a by-address getter for fake TStruct sub-classes
		#define TStructSubGetPtrImpl(type, name, offset)	TStructGetPtrImpl(type, name, offset - DATA_OFFSET)
	};


	// min value for a [sbyte]
	extern const sbyte K_SBYTE_MIN;
	// max value for a [sbyte]
	extern const sbyte K_SBYTE_MAX;
	// min value for a [byte]
	extern const byte K_BYTE_MIN;
	// max value for a [byte]
	extern const byte K_BYTE_MAX;
	// min value for a [int16]
	extern const int16 K_INT16_MIN;
	// max value for a [int16]
	extern const int16 K_INT16_MAX;
	// min value for a [uint16]
	extern const uint16 K_UINT16_MIN;
	// max value for a [uint16]
	extern const uint16 K_UINT16_MAX;
	// min value for a [int32]
	extern const int32 K_INT32_MIN;
	// max value for a [int32]
	extern const int32 K_INT32_MAX;
	// min value for a [uint32]
	extern const uint32 K_UINT32_MIN;
	// max value for a [uint32]
	extern const uint32 K_UINT32_MAX;
	// min value for a [int64]
	extern const int64 K_INT64_MIN;
	// max value for a [int64]
	extern const int64 K_INT64_MAX;
	// min value for a [uint64]
	extern const uint64 K_UINT64_MIN;
	// max value for a [uint64]
	extern const uint64 K_UINT64_MAX;
	// min value for a [real]
	extern const real K_REAL_MIN;
	// max value for a [real]
	extern const real K_REAL_MAX;


	inline bool is_null_or_empty(cstring const str) { return str == NULL || str[0] == '\0'; }
	inline bool is_null_or_empty(wcstring const str) { return str == NULL || str[0] == L'\0'; }

	// Takes [wide] and converts it to an ascii string, to be held in [string]. If [wide_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [string] if successful
	char* wstring_to_string(char* string, int32 string_length, wcstring wide, int32 wide_length = -1);

	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide);

	// Takes [string] and converts it to an unicode string, to be held in [wide]. If [string_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [wide] if successful
	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length = -1);

	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii);

	// Hacked up structure.
	// Allows us to treat a machine word (in this case, 32-bits) as various sorts 
	// of types. Either as a pointer, a value-array or just vanilla values.
	template<typename T> struct TTypeHolder
	{
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

	// If the COM interface reference isn't null, releases it and nulls it
	template<typename t_interface> void safe_release(t_interface*& obj)
	{
		if(obj != NULL)
		{
			obj->Release();
			obj = NULL;
		}
	}


#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	// Get the current time and format it into [time_str].
	void GetTimeStampString(_Out_ tag_string time_str);
	// Get the current time and format it into [time_str] using a file name friendly format.
	// The formatting output by this function is YYYY_MM_DD_hh_mm_ss.
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


#if defined(API_DEBUG) && defined(ASSERTS_ENABLED)

// Assert the condition is true. Run assertion logic when it's false
#define ASSERT(value, message) if(!(value)) Yelo::Assert(#value, message, __FILE__, __LINE__, __FUNCTION__)

namespace Yelo
{
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function);
};
#else
#define ASSERT(value, message) __noop;
#endif