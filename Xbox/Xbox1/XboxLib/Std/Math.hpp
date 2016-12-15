/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

#define M_PI       3.1415927f

namespace XboxLib
{
	namespace Math
	{
		inline int abs(int j) { return j<0 ? -j : j; }

		struct _RealConstants
		{
			const float Zero;	// 0.0f
			const float One;	// 1.0f
			const float Ninety;	// 90.0f
			const float Pi;		// float(M_PI)
			const float RadDegs;// # of degrees in one radian
			const float Deg3;	// 3 degrees in radians
			const float Deg90;	// 90 degrees in radians
			const float Deg360;	// 360 degrees in radians
			const float Ten;	// 10.0f
		};

		extern _RealConstants RealConstants;

		double __stdcall acos(double x);
		float __stdcall acosf(float x);

		double __stdcall asin(double x);
		float __stdcall asinf(float x);

		double __stdcall atan(double x);
		float __stdcall atanf(float x);
		double __stdcall atan2(double x, double y);

		double __stdcall ceil(double x);
		float __stdcall ceilf(float x);

		double __stdcall cos(double x);
		float __stdcall cosf(float x);

		double __stdcall cosh(double x);
		float __stdcall coshf(float x);

		double __stdcall exp(double x);
		float __stdcall expf(float x);

		double __stdcall fabs(double x);
		float __stdcall fabsf(float x);

		double __stdcall floor(double x);
		float __stdcall floorf(float x);

		double __stdcall fmod(double x, double y);
		double __stdcall frexp(double x, int *n);
		long __stdcall ftol(double x);
		double __stdcall ldexp(double x, int n);

		double __stdcall log(double x);
		float __stdcall logf(float x);

		double __stdcall log10(double x);
		float __stdcall log10f(float x);

		double __stdcall modf(double x, double *y);
		double __stdcall pow(double x, double y);

		double __stdcall sin(double x);
		float __stdcall sinf(float x);

		double __stdcall sinh(double x);

		double __stdcall sqrt(double x);
		float __stdcall sqrtf(float x);

		double __stdcall tan(double x);
		float __stdcall tanf(float x);

		double __stdcall tanh(double x);
	};
};