/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__XNA_MATH_IMPL_CODE_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__XNA_MATH_IMPL_CODE_INCLUDE_ID, __XNA_MATH_IMPL_CODE_INCLUDE_ID, "XNA/MathImpl.UByte4N.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __XNA_MATH_IMPL_CODE_INCLUDE_ID
	#endif


	//////////////////////////////////////////////////////////////////////////
	// C++/clr header code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_HPP

		static LowLevel::Math::real_quaternion ConvertUByte4N(mcpp_uint value);

		static void ConvertUByte4N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_uint) value);



	//////////////////////////////////////////////////////////////////////////
	// C++/clr source code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_CPP

		LowLevel::Math::real_quaternion Math::ConvertUByte4N(mcpp_uint value)
		{
			LowLevel::Math::real_quaternion ret = LowLevel::Math::real_quaternion();

			pin_ptr<mcpp_real> output = &ret.Vector.I;
			::Internal::ConvertUByte4N(value, output);

			return ret;
		}

		void Math::ConvertUByte4N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_uint) value)
		{
			pin_ptr<mcpp_real> vec = &input.Vector.I;
			mcpp_uint v;
			::Internal::StoreUByte4N(v, vec);
			value = v;
		}



	//////////////////////////////////////////////////////////////////////////
	// Native C++ code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CPP

		static void ConvertUByte4N(cpp_uint value, cpp_real* output)
		{
			XMUBYTEN4 xm_type; xm_type.v = value;

			XMVECTOR vec = ::XMLoadUByteN4(&xm_type);

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			_mm_storeu_ps(output, vec);
#else
			output[0] = vec.v[0];	output[1] = vec.v[1];	output[2] = vec.v[2];
			output[3] = vec.v[3];
#endif
		}

		static void StoreUByte4N(cpp_uint& value, cpp_real* input)
		{
			XMVECTOR vec;
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			vec = _mm_set_ps(input[0], input[1], input[2], input[3]);
#else
			vec.v[0] = input[0];	vec.v[1] = input[1];	vec.v[2] = input[2];
			vec.v[3] = input[3];
#endif

			XMUBYTEN4 xm_type;
			::XMStoreUByteN4(&xm_type, vec);
			value = xm_type.v;
		}

#else
#endif