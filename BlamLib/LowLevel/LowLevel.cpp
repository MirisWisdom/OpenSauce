/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#include "Precompile.hpp"

#include "Cryptography/XXTEA.hpp"
#include "Compression/LZMA.hpp"
#include "Math/Math.hpp"
#include "LowLevel.hpp"

#include <cseries/MacrosCpp.hpp>
#define BS_SWAP_U32(q)							\
	(											\
		( (((cpp_uint) (q)))>> 24) |			\
		( (((cpp_uint) (q)) >>  8)&0xFF00 ) |	\
		( (((cpp_uint) (q)) <<  8)&0xFF0000 ) |	\
		( (((cpp_uint) (q)) << 24)&0xFF000000 )	\
	)

mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
array<mcpp_byte>^ LowLevel::StructBitConverter::GetBytes(mcpp_ref(T) value_ref)
{
	// Create an array of bytes the same size as the input.
	array<mcpp_byte>^ bytes = mcpp_new array<mcpp_byte>(sizeof(T));

	// Assign the value to the buffer
	*reinterpret_cast< interior_ptr<T> >(&bytes[0]) = value_ref;

	return bytes;
}


mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
void LowLevel::StructBitConverter::GetBytes(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_ref(T) value_ref)
{
	if(buffer == mcpp_null) throw mcpp_new ArgumentNullException("buffer");

	if(	start_index >= buffer->Length || 
		start_index > (mcpp_int)(buffer->Length - sizeof(T))
		)
		throw mcpp_new ArgumentOutOfRangeException("start_index");

	*reinterpret_cast< interior_ptr<T> >(&buffer[start_index]) = value_ref;
}


mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
T LowLevel::StructBitConverter::ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index)
{
	if(buffer == mcpp_null) throw mcpp_new ArgumentNullException("buffer");

	if(	start_index >= buffer->Length || 
		start_index > (mcpp_int)(buffer->Length - sizeof(T))
		)
		throw mcpp_new ArgumentOutOfRangeException("start_index");

	return *reinterpret_cast< interior_ptr<T> >(&buffer[start_index]);
}

mcpp_template<typename T> mcpp_template_constraint(where, T, System::ValueType)
void LowLevel::StructBitConverter::ToValue(array<mcpp_byte>^ buffer, mcpp_int start_index, mcpp_out(T) value_ref)
{
	if(buffer == mcpp_null) throw mcpp_new ArgumentNullException("buffer");

	if(	start_index >= buffer->Length || 
		start_index > (mcpp_int)(buffer->Length - sizeof(T))
		)
		throw mcpp_new ArgumentOutOfRangeException("start_index");

	value_ref = *reinterpret_cast< interior_ptr<T> >(&buffer[start_index]);
}

//////////////////////////////////////////////////////////////////////////
// Single

mcpp_real LowLevel::ByteSwap::SingleFromUInt32(mcpp_uint val)
{
	mcpp_real* real_number = CAST_PTR(mcpp_real*, &val);

	return *real_number;
}
mcpp_uint LowLevel::ByteSwap::SingleToUInt32(mcpp_real real_number)
{
	mcpp_uint* val = CAST_PTR(mcpp_uint*, &real_number);

	return *val;
}

mcpp_real LowLevel::ByteSwap::SwapSingle(mcpp_real dword)
{
	mcpp_uint* val = CAST_PTR(mcpp_uint*, &dword);
	*val = BS_SWAP_U32(*val);

	return dword;
}

void LowLevel::ByteSwap::SwapSingle(mcpp_ref(mcpp_real) dword)
{
	mcpp_real local = dword;
	mcpp_uint* val = CAST_PTR(mcpp_uint*, &local);
	*val = BS_SWAP_U32(*val);

	dword = local;
}