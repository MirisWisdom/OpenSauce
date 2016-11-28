/*
	Kornner Studios: Shared Code

	See license\Shared for specific license information
*/
#pragma once

#pragma region Managed
#define __MCPP_CODE_START__		__pragma( managed(push, on) )
#define __MCPP_CODE_END__		__pragma( managed(pop) )

// Managed class
#define mcpp_class				ref class
// Managed struct
#define mcpp_struct				value struct
// Managed interface
// NOTE: FUCKING MICROSOFT. Can't use this shit, actually have to type out "interface class".
// For whatever reason, the compiler is using the 'interface' macro in ObjBase.h whenever
// we try to use this macro. However, if we just type out "interface class", it doesn't
// act like a fucking cock sucking $5 whore. Even tried undefining "interface". Nada. Still
// a fucking sleazy bitch. Fuck You.
//#define mcpp_interface			interface class
// Managed enum
#define mcpp_enum				enum struct
// Managed 'template' class
#define mcpp_template			generic
// Managed 'template' class constraint
#define mcpp_template_constraint(constraint, param, ...) constraint param : __VA_ARGS__
// Managed type info retrieving
#define mcpp_typeof(_type)		(_type::typeid)
// Managed type size calculation
#define mcpp_sizeof(_type)		System::Runtime::InteropServices::Marshal::SizeOf(_type)

// Declare a tracking reference to a [_type_decl] object
// Also acts as the C++ equivalent of "ref" parameter modifier
#define mcpp_ref(_type_decl)	_type_decl##%
// Declare a tracking reference to a [_type_decl] object
// C++ equivalent of "out" parameter modifier
#define mcpp_out(_type_decl)	[System::Runtime::InteropServices::Out] _type_decl##%
// Declare a param array of [_type_decl]
// C++ equivalent of "params" array parameter modifier
#define mcpp_params(_type_decl)	... array<_type_decl>^

#define mcpp_public				public:
#define mcpp_private			private:
#define mcpp_protected			protected:
#define mcpp_internal			internal:

// C++ equivlent of C# "readonly" member modifier
// Can be applied to both instance and static members
#define mcpp_readonly			initonly
// C++ equivlent of C# "const" member modifier
#define mcpp_const				literal

// Class Usage:
//	mcpp_class <type-name> mcpp_abstract
// Method Usage:
//	virtual <function-signature> mcpp_abstract
#define mcpp_abstract			abstract

// Managed override of a base function
#define mcpp_override			virtual
// Managed override of a base function by name
// Usage:
//	mcpp_override <function-signature> mcpp_override_explicit <type>::<overridden-function-name>[, ...]
#define mcpp_override_explicit	sealed =
// Managed override of a function name in the base definition
// Usage:
//	mcpp_override <function-signature> mcpp_override_name <type>::<overridden-function-name>[, ...]
#define mcpp_override_name		/*override*/ =
// Managed override of a function by hiding the base definition
// Usage:
//	mcpp_override <function-signature> mcpp_override_hide
#define mcpp_override_hide		new

// Usage:
//	virtual <function-signature> mcpp_implement_explicit <interface>::<function-name>
#define mcpp_implement_explicit	sealed =

// Managed operator for sealing of a function
// Class Usage:
//	mcpp_class <type-name> mcpp_sealed
// Method Usage:
//	mcpp_override <function-signature> mcpp_sealed
#define mcpp_sealed				sealed

// Managed allocation
#define mcpp_new				gcnew
// Managed NULL
#define mcpp_null				nullptr

// Managed reference
#define mcpp_using(...)			using namespace __VA_ARGS__
// C++ equivlent of C# "using(...)" code block
// Usage:
//	mcpp_using_start(System::IO::MemoryStream, s, mcpp_new mcpp_byte[512]);
//	{
//		...<code>...
//	}
//	mcpp_using_end();
#define mcpp_using_start(managed_typename, instance_name, ...) { \
	managed_typename instance_name(__VA_ARGS__)
#define mcpp_using_end() }

// C++ equivlent of C# "using(...)" code block
// Usage:
//	mcpp_using_start(IDisposable, s, mcpp_new System::IO::MemoryStream());
//	{
//		...<code>...
//	}
//	mcpp_using_end(s);
// Note: named "explicit" since we have to explicitly delete with 'mcpp_using_end_explicit'
#define mcpp_using_start_explicit(managed_typename, instance_name, ...) { \
	managed_typename^ instance_name = __VA_ARGS__
#define mcpp_using_end_explicit(instance_name) delete instance_name; instance_name = mcpp_null; }

// Try to cast [obj] to [type. If cast isn't possible, even with 
// user-defined conversions, InvalidCastException is thrown
#define mcpp_cast_to(type, obj)	(cli::safe_cast<type>( (obj) ))
// Cast Object reference [obj], to a [type] reference.
// If cast isn't possible, resulting handle is [mcpp_null]
// C++ equivlent of C# "as" operator
#define mcpp_cast_as(type, obj)	(dynamic_cast<type^>( (obj) ))
#define mcpp_unbox(type, obj)	(*reinterpret_cast<type^>( (obj) ))

#define mcpp_object				System::Object

#define mcpp_string				System::String
#define mcpp_bool				System::Boolean
#define mcpp_true				true
#define mcpp_false				false

#define mcpp_sbyte				System::SByte
#define mcpp_byte				System::Byte

#define mcpp_short				System::Int16
#define mcpp_ushort				System::UInt16

#define mcpp_int				System::Int32
#define mcpp_uint				System::UInt32

#define mcpp_long				System::Int64
#define mcpp_ulong				System::UInt64

#define mcpp_real				System::Single
#define mcpp_double				System::Double

#define mcpp_char				System::Byte
#define mcpp_wchar				System::Char
#pragma endregion


#pragma region Native
#define __CPP_CODE_START__		__pragma( managed(push, off) )
#define __CPP_CODE_END__		__pragma( managed(pop) )

// Native class
#define cpp_class				class
// Native struct
#define cpp_struct				struct
// Native interface
#define cpp_interface			__interface
// Native enum
#define cpp_enum				enum
// Native 'template' class
#define cpp_template			template
// Native type size calculation
#define cpp_sizeof(_type)		sizeof(_type)

#define cpp_public				public:
#define cpp_private				private:
#define cpp_protected			protected:

#define cpp_const				static const

// Class Usage:
//	cpp_class <type-name> cpp_abstract
// Method Usage:
//	virtual <function-signature> cpp_abstract
#define cpp_abstract			abstract

// Native override of a base function
#define cpp_override			virtual

// Managed operator for sealing of a function
// Class Usage:
//	cpp_class <type-name> cpp_sealed
// Method Usage:
//	cpp_override <function-signature> cpp_sealed
#define cpp_sealed				sealed

// Native allocation
#define cpp_new					new
// Native NULL
#define cpp_null				0

// Native reference
#define cpp_using(...)			using namespace __VA_ARGS__

// Native type casting
#define cpp_cast_to(type, obj)	(static_cast<type>(obj))
// Native pointer casting
#define cpp_cast_ptr(type, obj)	(reinterpret_cast<type>(obj))

#define cpp_bool				bool
#define cpp_true				true
#define cpp_false				false

#define cpp_sbyte				signed __int8
#define cpp_byte				unsigned __int8

#define cpp_short				signed __int16
#define cpp_ushort				unsigned __int16

#define cpp_int					signed __int32
#define cpp_uint				unsigned __int32

#define cpp_long				signed __int64
#define cpp_ulong				unsigned __int64

#define cpp_real				float
#define cpp_double				double

#define cpp_char				unsigned char
#define cpp_wchar				wchar_t
#pragma endregion


// Managed structure which holds a pointer to an unmanaged object
template<typename T> mcpp_class cpp_ptr
{
private:
	T* _ptr;

public:
	// Initialize with null internal data
	cpp_ptr() : _ptr(cpp_null)											{}

	// Initialize with an existing native object
	cpp_ptr(T* t) : _ptr(t)												{}

	// Initialize from an existing cpp_ptr.
	// Will detach [other]'s internal data
	cpp_ptr(mcpp_ref(cpp_ptr<T>) other) : _ptr(other.Detach())			{}

	// Initialize from an existing cpp_ptr
	template<typename T_Derived> 
	cpp_ptr(mcpp_ref(cpp_ptr<T_Derived>) other) : _ptr(other.Detach())	{}

	// Destructor
	~cpp_ptr()											{ this->!cpp_ptr(); }

	// Finalizer
	!cpp_ptr()											{ delete _ptr; }

	// Assignment from a pointer to a [T] instance
	mcpp_ref(cpp_ptr<T>) operator=(T* t)				{ Attach(t); return *this; }

	// Assignment from an existing cpp_ptr.
	// If [other] isn't the same as [this] object, [other] will detach its 
	// grasp of it's internal pointer and [this] will assume command.
	mcpp_ref(cpp_ptr<T>) operator=(mcpp_ref(cpp_ptr<T>) other)
	{
		if(this != %other) Attach(other.Detach());
		return *this;
	}

	// Assignment from an existing cpp_ptr which is actually a container to 
	// a type that inherits from [T].
	template<typename T_Derived> 
	mcpp_ref(cpp_ptr<T>) operator=(mcpp_ref(cpp_ptr<T_Derived>) other)
	{
		Attach(other.Detach());
		return *this;
	}

	// pointer-to-member convenience operator
	static T* operator->(mcpp_ref(cpp_ptr<T>) obj)		{ return obj._ptr; }

	// cast to pointer convenience operator
	static operator T*(mcpp_ref(cpp_ptr<T>) obj)		{ return obj._ptr; }

	// Stops pointing to the [T] object and returns said native object
	T* Detach()
	{
		T* t = _ptr;
		_ptr = cpp_null;
		return t;
	}

	// Attach this object
	void Attach(T* t)
	{
		if(t)
		{	
			// if [t] isn't actually the same object as [_ptr]...
			// Note: Doesn't have to mean they're the same object memory, 
			// [T} may have a user-defined '!=' operator. Do we really want this?
			if(_ptr != t)
			{
				delete _ptr;
				_ptr = t;
			}
		}
		else
		{
#ifdef DEBUG
			throw mcpp_new System::Exception("Attempting to Attach(...) a nullptr!");
#endif
		}		
	}

	// Stop referencing and delete the internal object memory 
	void Destroy()
	{
		delete _ptr;
		_ptr = cpp_null;
	}
};


#include <vcclr.h>
// Unmanaged structure that contains a pointer to a managed object
template<typename TManaged> cpp_struct mcpp_ptr : gcroot< TManaged* >
{
	explicit mcpp_ptr(TManaged* obj) : base_type(obj) {}

	~mcpp_ptr() { (*this)->Dispose(); }
};



#ifndef _WIN64
	#define MCPP_X64_PLATFORM_EXCEPTION() __noop
#else
	#define MCPP_X64_PLATFORM_EXCEPTION() throw mcpp_new System::PlatformNotSupportedException("Feature not supported on x64 platforms currently")
#endif


// TODO: CAN I REMOVE THIS BULLSHIT CAST IN 2010??
// http://connect.microsoft.com/VisualStudio/feedback/details/101910/c-cli-incorrectly-requires-safe-cast-to-assign-cli-array-to-ienumerable-t
// UPDATE: Nope.
#define MCPP_IMPLEMENT_GET_ENUMERATOR(TType, field_name)																				\
	virtual System::Collections::Generic::IEnumerator<TType>^ GetEnumerator()															\
	{ return mcpp_cast_as(System::Collections::Generic::IEnumerable<TType>, field_name)->GetEnumerator(); }								\
	virtual System::Collections::IEnumerator^ GetEnumeratorObj() mcpp_override_explicit System::Collections::IEnumerable::GetEnumerator	\
	{ return field_name->GetEnumerator(); }
// For when [TType] is a generic parameter
#define MCPP_IMPLEMENT_GET_ENUMERATOR_GENERIC(TType, field_name)																		\
	virtual System::Collections::Generic::IEnumerator<TType>^ GetEnumerator()															\
	{ return mcpp_cast_to(System::Collections::Generic::IEnumerable<TType>^, field_name)->GetEnumerator(); }							\
	virtual System::Collections::IEnumerator^ GetEnumeratorObj() mcpp_override_explicit System::Collections::IEnumerable::GetEnumerator	\
	{ return field_name->GetEnumerator(); }