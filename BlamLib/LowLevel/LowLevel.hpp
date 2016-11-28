/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

namespace LowLevel {

	using namespace System;

	public mcpp_enum HResult : mcpp_uint
	{
		Success = S_OK,

		Unexpected =		mcpp_cast_to(mcpp_uint, E_UNEXPECTED),
		NotImplemented =	mcpp_cast_to(mcpp_uint, E_NOTIMPL),
		OutOfMemory =		mcpp_cast_to(mcpp_uint, E_OUTOFMEMORY),
		InvalidArg =		mcpp_cast_to(mcpp_uint, E_INVALIDARG),
		NoInterface =		mcpp_cast_to(mcpp_uint, E_NOINTERFACE),
		InvalidPointer =	mcpp_cast_to(mcpp_uint, E_POINTER),
		InvalidHandle =		mcpp_cast_to(mcpp_uint, E_HANDLE),
		Abort =				mcpp_cast_to(mcpp_uint, E_ABORT),
		Fail =				mcpp_cast_to(mcpp_uint, E_FAIL),
		AccessDenied =		mcpp_cast_to(mcpp_uint, E_ACCESSDENIED),

		DataPending =		mcpp_cast_to(mcpp_uint, E_PENDING),
	};

	public mcpp_class StructBitConverter abstract sealed
	{
	mcpp_public

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static array<mcpp_byte>^ GetBytes(mcpp_ref(T) value_ref);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static void GetBytes(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_ref(T) value_ref);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static T ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index);

		mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
			static void ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_out(T) value_ref);
	};

	public mcpp_class ByteSwap abstract sealed
	{
	mcpp_public
		static mcpp_real SingleFromUInt32(mcpp_uint val);
		static mcpp_uint SingleToUInt32(mcpp_real real_number);

		/// <summary>
		/// Swaps a single-precision number and returns the value
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static mcpp_real SwapSingle(mcpp_real dword);

		/// <summary>
		/// Swaps a single-precision number by reference
		/// </summary>
		/// <param name="dword"></param>
		static void SwapSingle(mcpp_ref(mcpp_real) dword);
	};
}