/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__XNA_MATH_IMPL_CODE_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__XNA_MATH_IMPL_CODE_INCLUDE_ID, __XNA_MATH_IMPL_CODE_INCLUDE_ID, "XNA/MathImpl.Short2N.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __XNA_MATH_IMPL_CODE_INCLUDE_ID
	#endif


	//////////////////////////////////////////////////////////////////////////
	// C++/clr header code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_HPP

		static LowLevel::Math::real_quaternion ConvertShort2N(mcpp_short value_x, mcpp_short value_y);

		static void ConvertShort2N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_short) value_x, mcpp_out(mcpp_short) value_y);



	//////////////////////////////////////////////////////////////////////////
	// C++/clr source code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CLR_CPP

		LowLevel::Math::real_quaternion Math::ConvertShort2N(mcpp_short value_x, mcpp_short value_y)
		{
			LowLevel::Math::real_quaternion ret = LowLevel::Math::real_quaternion();

			pin_ptr<mcpp_real> output = &ret.Vector.I;
			::Internal::ConvertShort2N(value_x, value_y, output);

			return ret;
		}

		void Math::ConvertShort2N(LowLevel::Math::real_quaternion input, mcpp_out(mcpp_short) value_x, mcpp_out(mcpp_short) value_y)
		{
			pin_ptr<mcpp_real> vec = &input.Vector.I;
			mcpp_short v_x, v_y;
			::Internal::StoreShort2N(v_x, v_y, vec);
			value_x = v_x;	value_y = v_y;
		}



	//////////////////////////////////////////////////////////////////////////
	// Native C++ code
	#elif BOOST_PP_ITERATION() == __XNA_MATH_IMPL_CODE_CPP

		static void ConvertShort2N(mcpp_short value_x, mcpp_short value_y, cpp_real* output)
		{
			XMSHORTN2 xm_type; xm_type.x = value_x; xm_type.y = value_y;

			XMVECTOR vec = ::XMLoadShortN2(&xm_type);

#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			_mm_storeu_ps(output, vec);
#else
			output[0] = vec.v[0];	output[1] = vec.v[1];
#endif
		}

		static void StoreShort2N(cpp_short& value_x, cpp_short& value_y, cpp_real* input)
		{
			XMVECTOR vec;
#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			vec = _mm_set_ps(input[0], input[1], 0, 0);
#else
			vec.v[0] = input[0];	vec.v[1] = input[1];
#endif

			XMSHORTN2 xm_type;
			::XMStoreShortN2(&xm_type, vec);
			value_x = xm_type.x;	value_y = xm_type.y;
		}

#else
#endif