/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__XNA_MATH_IMPL_CODE_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__XNA_MATH_IMPL_CODE_INCLUDE_ID, __XNA_MATH_IMPL_CODE_INCLUDE_ID, "XNA/MathImpl.Halfs.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __XNA_MATH_IMPL_CODE_INCLUDE_ID
	#endif


	//////////////////////////////////////////////////////////////////////////
	// C++/clr header code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_HPP

		static mcpp_real ConvertHalfToSingle(mcpp_ushort value);
		static array<mcpp_real>^ ConvertHalfToSingleStream(array<mcpp_ushort>^ half_stream);

		static mcpp_ushort ConvertSingleToHalf(mcpp_real value);
		static array<mcpp_ushort>^ ConvertSingleToHalfStream(array<mcpp_real>^ single_stream);

		static array<mcpp_ushort>^ ConvertSingleToHalfArray(array<mcpp_real>^ single_array);
		static array<mcpp_real>^ ConvertHalfToSingleArray(array<mcpp_ushort>^ half_array);



	//////////////////////////////////////////////////////////////////////////
	// C++/clr source code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_CPP

		mcpp_real Math::ConvertHalfToSingle(mcpp_ushort value)		{ return ::XMConvertHalfToFloat(value); }

		array<mcpp_real>^ Math::ConvertHalfToSingleStream(array<mcpp_ushort>^ half_stream)
		{
			array<mcpp_real>^ single_stream = mcpp_new array<mcpp_real>(half_stream->Length);

			pin_ptr<cpp_real> singles = &single_stream[0];
			pin_ptr<const cpp_ushort> halfs = &half_stream[0];

			::Internal::ConvertHalfToSingleStream(singles, halfs, half_stream->Length);
			//::XMConvertHalfToFloatStream(singles, 0, halfs, 0, half_stream->Length);

			return single_stream;
		}

		mcpp_ushort Math::ConvertSingleToHalf(mcpp_real value)		{ return ::XMConvertFloatToHalf(value); }

		array<mcpp_ushort>^ Math::ConvertSingleToHalfStream(array<mcpp_real>^ single_stream)
		{
			array<mcpp_ushort>^ half_stream = mcpp_new array<mcpp_ushort>(single_stream->Length);

			pin_ptr<cpp_ushort> halfs = &half_stream[0];
			pin_ptr<const cpp_real> singles = &single_stream[0];

			::Internal::ConvertSingleToHalfStream(halfs, singles, single_stream->Length);
			//::XMConvertFloatToHalfStream(halfs, 0, singles, 0, single_stream->Length);

			return half_stream;
		}

		array<mcpp_ushort>^ Math::ConvertSingleToHalfArray(array<mcpp_real>^ single_array)
		{
			array<mcpp_ushort>^ half_array = mcpp_new array<mcpp_ushort>(single_array->Length);

			pin_ptr<cpp_ushort> halfs = &half_array[0];
			pin_ptr<const cpp_real> singles = &single_array[0];

			::Internal::ConvertSingleToHalfArray(halfs, singles, single_array->Length);
			//::D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(halfs), singles, single_array->Length);

			return half_array;
		}

		array<mcpp_real>^ Math::ConvertHalfToSingleArray(array<mcpp_ushort>^ half_array)
		{
			array<mcpp_real>^ single_array = mcpp_new array<mcpp_real>(half_array->Length);

			pin_ptr<cpp_real> singles = &single_array[0];
			pin_ptr<const cpp_ushort> halfs = &half_array[0];

			::Internal::ConvertHalfToSingleArray(singles, halfs, half_array->Length);
			//::D3DXFloat16To32Array(singles, reinterpret_cast<const D3DXFLOAT16*>(halfs), half_array->Length);

			return single_array;
		}



	//////////////////////////////////////////////////////////////////////////
	// Native C++ code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CPP

		static void ConvertHalfToSingleStream(cpp_real* output_stream, const cpp_ushort* input_stream, cpp_int count)
		{
			::XMConvertHalfToFloatStream(output_stream, 0, input_stream, 0, count);
		}

		static void ConvertSingleToHalfStream(cpp_ushort* output_stream, const cpp_real* input_stream, cpp_int count)
		{
			::XMConvertFloatToHalfStream(output_stream, 0, input_stream, 0, count);
		}

		static void ConvertSingleToHalfArray(cpp_ushort* output_stream, const cpp_real* input_stream, cpp_int count)
		{
			::D3DXFloat32To16Array(reinterpret_cast<D3DXFLOAT16*>(output_stream), input_stream, count);
		}

		static void ConvertHalfToSingleArray(cpp_real* output_stream, const cpp_ushort* input_stream, cpp_int count)
		{
			::D3DXFloat16To32Array(output_stream, reinterpret_cast<const D3DXFLOAT16*>(input_stream), count);
		}

#else
#endif