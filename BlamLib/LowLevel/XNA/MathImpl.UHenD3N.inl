/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__XNA_MATH_IMPL_CODE_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__XNA_MATH_IMPL_CODE_INCLUDE_ID, __XNA_MATH_IMPL_CODE_INCLUDE_ID, "XNA/MathImpl.UHenD3N.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __XNA_MATH_IMPL_CODE_INCLUDE_ID
	#endif


	//////////////////////////////////////////////////////////////////////////
	// C++/clr header code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_HPP

		static LowLevel::Math::real_quaternion ConvertUHenD3N(mcpp_uint value);

		static void ConvertUHenD3N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_uint) value);



	//////////////////////////////////////////////////////////////////////////
	// C++/clr source code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_CPP

		LowLevel::Math::real_quaternion Math::ConvertUHenD3N(mcpp_uint value)
		{
			LowLevel::Math::real_quaternion ret = LowLevel::Math::real_quaternion();

			pin_ptr<mcpp_real> output = &ret.Vector.I;
			::Internal::ConvertUHenD3N(value, output);

			return ret;
		}

		void Math::ConvertUHenD3N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_uint) value)
		{
			pin_ptr<mcpp_real> vec = &input.Vector.I;
			mcpp_uint v;
			::Internal::StoreUHenD3N(v, vec);
			value = v;
		}



	//////////////////////////////////////////////////////////////////////////
	// Native C++ code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CPP

		static void ConvertUHenD3N(cpp_uint value, cpp_real* output)
		{
			XMUHENDN3 xm_type; xm_type.v = value;

			XMVECTOR vec = ::XMLoadUHenDN3(&xm_type);

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			_mm_storeu_ps(output, vec);
#else
			output[0] = vec.v[0];	output[1] = vec.v[1];	output[2] = vec.v[2];
#endif
		}

		static void StoreUHenD3N(cpp_uint& value, cpp_real* input)
		{
			XMVECTOR vec;
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			vec = _mm_set_ps(input[0], input[1], input[2], 0);
#else
			vec.v[0] = input[0];	vec.v[1] = input[1];	vec.v[2] = input[2];
#endif

			XMUHENDN3 xm_type;
			::XMStoreUHenDN3(&xm_type, vec);
			value = xm_type.v;
		}

#else
#endif