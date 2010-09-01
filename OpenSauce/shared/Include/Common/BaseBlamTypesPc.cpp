/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Common/Precompile.hpp>
#include <Common/BaseBlamTypesPc.hpp>

const errno_t k_errnone = 0;

namespace Yelo
{
	char* wstring_to_string(char* string, int32 string_length, wcstring wide, int32 wide_length)
	{
		if( !WideCharToMultiByte(CP_ACP, 0, wide, wide_length, string, string_length, NULL, NULL) )
			return NULL;
		else
			return string;
	}

	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length)
	{
		if( !MultiByteToWideChar(CP_ACP, 0, string, string_length, wide, wide_length) )
			return NULL;
		else
			return wide;
	}


	const real k_rad_to_deg = 57.2957795f;

	void real_quaternion::Compress64bit(real_quaternion* in, uint16 out[4])
	{
		out[0] = (uint16)(in->i * 32767.0f);
		out[1] = (uint16)(in->j * 32767.0f);
		out[2] = (uint16)(in->k * 32767.0f);
		out[3] = (uint16)(in->w * 32767.0f);
	}

	void real_quaternion::Decompress64bit(short in[4], real_quaternion* out)
	{
		out->i = (real)(in[0] / 32767.0f);
		out->j = (real)(in[1] / 32767.0f);
		out->k = (real)(in[2] / 32767.0f);
		out->w = (real)(in[3] / 32767.0f);
	}

	void real_quaternion::FromAngles(real_euler_angles3d* angles)
	{
		real angle;
		double sr, sp, sy, cr, cp, cy;

		angle = angles->roll*0.5f;
		sy = sin(angle);
		cy = cos( angle );
		angle = angles->pitch*0.5f;
		sp = sin( angle );
		cp = cos( angle );
		angle = angles->yaw*0.5f;
		sr = sin( angle );
		cr = cos( angle );

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

		if( abs(1.0f + cosOmega) < 1.0e-6f )
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
			this->k = start.k + interp * (q2->j - start.k);
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
				double omega = acos(cosom);
				double sinom = sin(omega);
				sclq1 = sin((1.0-interp)*omega )/sinom;
				sclq2 = sin(interp*omega)/sinom;
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

			sclq1 = sin(( 1.0f-interp )*0.5f*M_PI );
			sclq2 = sin( interp*0.5f*M_PI );
			this->i = ( real )( sclq1*q1->i+sclq2*this->i );
			this->j = ( real )( sclq1*q1->j+sclq2*this->j );
			this->k = ( real )( sclq1*q1->k+sclq2*this->k );
		}
	}

	void real_quaternion::ToAxisAngle(real_point3d* axis, real* angle)
	{
		// The quaternion representing the rotation is
		//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

		double SqrLength = 
			this->i*this->i + 
			this->j*this->j + 
			this->k*this->k;
		if(SqrLength > 0.0f)
		{
			*angle = (real)( k_rad_to_deg * 2.0f * acos(this->w) );
			real fInvLength = (real)( 1.0f / sqrt(SqrLength) );
			axis->x = this->i*fInvLength;
			axis->y = this->j*fInvLength;
			axis->z = this->k*fInvLength;
		}
		else
		{
			// angle is 0 (mod 2*pi), so any axis will do
			*angle = 0.0f;
			axis->x = 1.0f;
			axis->y = 0.0f;
			axis->z = 0.0f;
		}
	}
};