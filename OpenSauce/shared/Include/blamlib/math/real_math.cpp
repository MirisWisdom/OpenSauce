/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	#define MATH_FLOAT_FUNC(func) (real)func
	#define MATH_DOUBLE_FUNC(func) func
	#define MATH_FLOAT_TO_LONG(...) ( __VA_ARGS__ )
#else
	#include <Std/Math.hpp> // XboxLib

	#define MATH_FLOAT_FUNC(func) XboxLib::Math::##func##f
	#define MATH_DOUBLE_FUNC(func) XboxLib::Math::##func
	#define MATH_FLOAT_TO_LONG(...) XboxLib::Math::ftol( __VA_ARGS__ )
#endif

namespace Yelo
{
	//////////////////////////////////////////////////////////////////////////
	// real_vector2d
	real real_vector2d::Magnitude() const
	{ return MATH_FLOAT_FUNC(sqrt)( this->i*this->i + this->j*this->j ); }

	API_FUNC_NAKED void real_vector2d::DotProduct2D(real_vector2d* a, real_vector2d* b, real& out_value)
	{
		API_FUNC_NAKED_START()
			push	ecx
			push	esi

			mov		ecx, a
			mov		esi, b
			fld		dword ptr [ecx]real_vector2d.i
			fmul	dword ptr [esi]real_vector2d.i

			fld		dword ptr [ecx]real_vector2d.j
			fmul	dword ptr [esi]real_vector2d.j

			fadd
			fstp	out_value

			pop		esi
			pop		ecx
		API_FUNC_NAKED_END(3)
	}
	//////////////////////////////////////////////////////////////////////////
	// real_vector3d
	real real_vector3d::Magnitude() const
	{ return MATH_FLOAT_FUNC(sqrt)( this->i*this->i + this->j*this->j + this->k*this->k ); }
	//////////////////////////////////////////////////////////////////////////
	// real_quaternion
	static const real k_rad_to_deg = 57.2957795f;

	void real_quaternion::Compress64bit(uint16 out[4]) const
	{
		out[0] = (uint16)MATH_FLOAT_TO_LONG(this->i * 32767.0f);
		out[1] = (uint16)MATH_FLOAT_TO_LONG(this->j * 32767.0f);
		out[2] = (uint16)MATH_FLOAT_TO_LONG(this->k * 32767.0f);
		out[3] = (uint16)MATH_FLOAT_TO_LONG(this->w * 32767.0f);
	}

	void real_quaternion::Decompress64bit(int16 in[4])
	{
		this->i = (real)(in[0] / 32767.0f);
		this->j = (real)(in[1] / 32767.0f);
		this->k = (real)(in[2] / 32767.0f);
		this->w = (real)(in[3] / 32767.0f);
	}

	void real_quaternion::FromAngles(real_euler_angles3d* angles)
	{
		real angle;
		double sr, sp, sy, cr, cp, cy;

		angle = angles->roll*0.5f;
		sy = MATH_DOUBLE_FUNC(sin)(angle);
		cy = MATH_DOUBLE_FUNC(cos)( angle );
		angle = angles->pitch*0.5f;
		sp = MATH_DOUBLE_FUNC(sin)( angle );
		cp = MATH_DOUBLE_FUNC(cos)( angle );
		angle = angles->yaw*0.5f;
		sr = MATH_DOUBLE_FUNC(sin)( angle );
		cr = MATH_DOUBLE_FUNC(cos)( angle );

		double crcp = cr*cp;
		double srsp = sr*sp;

		this->i = ( real )( sr*cp*cy-cr*sp*sy );
		this->j = ( real )( cr*sp*cy+sr*cp*sy );
		this->k = ( real )( crcp*sy-srsp*cy );
		this->w = ( real )( crcp*cy+srsp*sy );
	}

	void real_quaternion::Lerp(real_quaternion* q1, real_quaternion* q2, real interp)
	{
		real_quaternion start = *q1; // copy
		start.Conjugate();

		real cosOmega = 
			start.i * q2->i +
			start.j * q2->j +
			start.k * q2->k +
			start.w * q2->w;

		if(cosOmega < 0)
		{
			cosOmega *= -1;
			start.Inverse();
		}

		if( MATH_DOUBLE_FUNC(abs)( MATH_FLOAT_TO_LONG(1.0f + cosOmega) ) < 1.0e-6f )
		{
			real tx = interp*start.i;
			real ty = interp*start.j;
			real tz = interp*start.k;
			real tw = interp*start.w;

			this->i = start.i - tx - ty;
			this->j = start.j - ty + tx;
			this->k = start.k - tz - tw;
			this->w = start.w;
		}
		else
		{
			this->i = start.i + interp * (q2->i - start.i);
			this->j = start.j + interp * (q2->j - start.j);
			this->k = start.k + interp * (q2->k - start.k);
			this->w = start.w + interp * (q2->w - start.w);
		}
	}

	void real_quaternion::SLerp(real_quaternion* q1, real_quaternion* q2, real interp)
	{
		// Decide if one of the quaternions is backwards
		real a = 0, b = 0;

		a += ( q1->i-q2->i )*( q1->i-q2->i );
		b += ( q1->i+q2->i )*( q1->i+q2->i );

		a += ( q1->j-q2->j )*( q1->j-q2->j );
		b += ( q1->j+q2->j )*( q1->j+q2->j );

		a += ( q1->k-q2->k )*( q1->k-q2->k );
		b += ( q1->k+q2->k )*( q1->k+q2->k );

		a += ( q1->w-q2->w )*( q1->w-q2->w );
		b += ( q1->w+q2->w )*( q1->w+q2->w );

		if ( a > b )
			q2->Inverse();

		real cosom = 
			q1->i*q2->i +
			q1->j*q2->j +
			q1->k*q2->k +
			q1->w*q2->w;
		double sclq1, sclq2;

		if (( 1.0f+cosom ) > 0.00000001f )
		{
			if (( 1.0f-cosom ) > 0.00000001f )
			{
				double omega = MATH_DOUBLE_FUNC(acos)(cosom);
				double sinom = MATH_DOUBLE_FUNC(sin)(omega);
				sclq1 = MATH_DOUBLE_FUNC(sin)((1.0-interp)*omega )/sinom;
				sclq2 = MATH_DOUBLE_FUNC(sin)(interp*omega)/sinom;
			}
			else
			{
				sclq1 = 1.0f-interp;
				sclq2 = interp;
			}
			this->i = ( real )( sclq1*q1->i+sclq2*q2->i );
			this->j = ( real )( sclq1*q1->j+sclq2*q2->j );
			this->k = ( real )( sclq1*q1->k+sclq2*q2->k );
			this->w = ( real )( sclq1*q1->w+sclq2*q2->w );
		}
		else
		{
			this->i = -q1->j;
			this->j = q1->i;
			this->k = -q1->w;
			this->w = q1->k;

			sclq1 = MATH_DOUBLE_FUNC(sin)(( 1.0f-interp )*0.5f*M_PI );
			sclq2 = MATH_DOUBLE_FUNC(sin)( interp*0.5f*M_PI );
			this->i = ( real )( sclq1*q1->i+sclq2*this->i );
			this->j = ( real )( sclq1*q1->j+sclq2*this->j );
			this->k = ( real )( sclq1*q1->k+sclq2*this->k );
		}
	}

	void real_quaternion::ToAxisAngle(real_point3d* axis, real& angle)
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		double SqrLength = 
			this->i*this->i + 
			this->j*this->j + 
			this->k*this->k;
		if(SqrLength > 0.0)
		{
			angle = (real)( k_rad_to_deg * 2.0f * MATH_DOUBLE_FUNC(acos)(this->w) );
			real fInvLength = (real)( 1.0 / MATH_DOUBLE_FUNC(sqrt)(SqrLength) );
			axis->x = this->i*fInvLength;
			axis->y = this->j*fInvLength;
			axis->z = this->k*fInvLength;
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			angle = 0.0f;
			axis->x = 1.0f;
			axis->y = 0.0f;
			axis->z = 0.0f;
		}
	}

	void real_quaternion::Normalize()
	{
		real mag = MATH_FLOAT_FUNC(sqrt)(
			this->i*this->i + 
			this->j*this->j + 
			this->k*this->k + 
			this->w*this->w	);

		this->i /= mag;
		this->j /= mag;
		this->k /= mag;
		this->w /= mag;
	}
	//////////////////////////////////////////////////////////////////////////
	// real_plane2d
	void real_plane2d::Normalize()
	{
		real mag = MATH_FLOAT_FUNC(sqrt)(
			this->i*this->i +
			this->j*this->j
			);

		this->i /= mag;
		this->j /= mag;
		this->d /= mag;
	}
	//////////////////////////////////////////////////////////////////////////
	// real_plane3d
	void real_plane3d::Normalize()
	{
		real mag = MATH_FLOAT_FUNC(sqrt)(
			this->i*this->i +
			this->j*this->j +
			this->k*this->k
			);

		this->i /= mag;
		this->j /= mag;
		this->k /= mag;
		this->d /= mag;
	}
};