/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#include <d3dx9math.h>

//#include <errno.h>
extern const errno_t k_errnone;

#include <Common/BaseBlamTypes.hpp>

namespace Yelo
{
#pragma region 'real' type based definitions
	struct real_vector2d;
	struct real_vector3d;

	// a point in real 2d space.
	/*union*/ struct real_point2d
	{
		// X-Axis
		real x;

		// Y-Axis
		real y;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(real_vector2d);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);
	};

	// a point in real 3d space
	/*union*/ struct real_point3d
	{
		// X-Axis
		real x;

		// Y-Axis
		real y;

		// Z-Axis
		real z;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(real_point2d);
		OVERRIDE_OPERATOR_CAST_THIS(real_vector3d);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);
	};

	// A 2d real vector
	/*union*/ struct real_vector2d
	{
		// X-Component
		real i;

		// Y-Component
		real j;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);

		API_INLINE real Magnitude() const
		{ return (real)sqrt( this->i*this->i + this->j*this->j ); }

		API_INLINE void Normalize()
		{
			real len = this->Magnitude();
			this->i /= len;
			this->j /= len;
		}

		API_INLINE void Inverse()
		{ this->i = -this->i; this->j = -this->j; }

		API_INLINE void Set(real _i, real _j)
		{ this->i = _i; this->j = _j; }

		API_INLINE real_vector2d& operator += (const real_vector2d& v)
		{
			this->i += v.i;		this->j += v.j;		return *this;
		}

		API_INLINE real_vector2d& operator -= (const real_vector2d& v)
		{
			this->i -= v.i;		this->j -= v.j;		return *this;
		}

		API_INLINE real_vector2d& operator *= (real scalar)
		{
			this->i *= scalar;		this->j *= scalar;		return *this;
		}

		API_INLINE real_vector2d& operator /= (real scalar)
		{
			this->i /= scalar;		this->j /= scalar;		return *this;
		}

		API_INLINE real_vector2d operator - () const ///< Conjugate
		{
			real_vector2d value; value.Set(-this->i, -this->j);
			return value;
		}

		API_INLINE real_vector2d operator + (const real_vector2d& rh) const ///< Addition
		{
			real_vector2d value; value.Set(this->i+rh.i, this->j+rh.j);
			return value;
		}

		API_INLINE real_vector2d operator + (const real_point2d& rh) const ///< Addition
		{
			real_vector2d value; value.Set(this->i+rh.x, this->j+rh.y);
			return value;
		}


		API_INLINE real_vector2d operator - (const real_vector2d& rh) const ///< Subtraction
		{
			real_vector2d value; value.Set(this->i-rh.i, this->j-rh.j);
			return value;
		}

		API_INLINE real_vector2d operator - (const real_point2d& rh) const ///< Subtraction
		{
			real_vector2d value; value.Set(this->i-rh.x, this->j-rh.y);
			return value;
		}


		API_INLINE real operator * (const real_vector2d& rh) const ///< Dot Product
		{ return this->i*rh.i + this->j*rh.j; }

		API_INLINE real operator * (const real_point2d& rh) const ///< Dot Product
		{ return this->i*rh.x + this->j*rh.y; }


		API_INLINE real_vector2d operator * (real rh) const ///< Scalar Multiplication
		{
			real_vector2d value; value.Set(this->i*rh, this->j*rh);
			return value;
		}

		API_INLINE real_vector2d operator / (real rh) const ///< Scalar Division
		{
			real_vector2d value; value.Set(this->i/rh, this->j/rh);
			return value;
		}
	};

	// a 3d real vector
	/*union*/ struct real_vector3d
	{
		// X-Component
		real i;

		// Y-Component
		real j;

		// Z-Component
		real k;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(real_vector2d);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);

		API_INLINE real Magnitude() const
		{ return (real)sqrt( this->i*this->i + this->j*this->j + this->k*this->k ); }

		API_INLINE void Normalize()
		{
			real len = this->Magnitude();
			this->i /= len;
			this->j /= len;
			this->k /= len;
		}

		API_INLINE void Inverse()
		{
			this->i = -this->i;
			this->j = -this->j;
			this->k = -this->k;
		}

		API_INLINE void Set(real _i, real _j, real _k)
		{ this->i = _i; this->j = _j; this->k = _k; }

		API_INLINE real_vector3d& operator += (const real_vector3d& v)
		{
			this->i += v.i;
			this->j += v.j;
			this->k += v.k;
			return *this;
		}

		API_INLINE real_vector3d& operator -= (const real_vector3d& v)
		{
			this->i -= v.i;
			this->j -= v.j;
			this->k -= v.k;
			return *this;
		}

		API_INLINE real_vector3d& operator *= (real scalar)
		{
			this->i *= scalar;
			this->j *= scalar;
			this->k *= scalar;
			return *this;
		}

		API_INLINE real_vector3d& operator /= (real scalar)
		{
			this->i /= scalar;
			this->j /= scalar;
			this->k /= scalar;
			return *this;
		}

		API_INLINE real_vector3d operator - () const ///< Conjugate
		{
			real_vector3d value; 
			value.Set(-this->i, -this->j, -this->k);
			return value;
		}

		API_INLINE real_vector3d operator + (const real_vector3d& rh) const ///< Addition
		{
			real_vector3d value; value.Set(this->i+rh.i, this->j+rh.j, this->k+rh.k);
			return value;
		}

		API_INLINE real_vector3d operator + (const real_point3d& rh) const ///< Addition
		{
			real_vector3d value; value.Set(this->i+rh.x, this->j+rh.y, this->k+rh.z);
			return value;
		}


		API_INLINE real_vector3d operator - (const real_vector3d& rh) const ///< Subtraction
		{
			real_vector3d value; value.Set(this->i-rh.i, this->j-rh.j, this->k-rh.k);
			return value;
		}

		API_INLINE real_vector3d operator - (const real_point3d& rh) const ///< Subtraction
		{
			real_vector3d value; value.Set(this->i-rh.x, this->j-rh.y, this->k-rh.z);
			return value;
		}


		API_INLINE real_vector3d operator ^ (const real_vector3d& rh) const ///< Cross Product
		{
			real_vector3d value; value.Set(
				this->j*rh.k - this->k*rh.j,
				-this->i*rh.k + this->k*rh.i,
				this->i*rh.j - this->j*rh.i
				);
			return value;
		}

		API_INLINE real_vector3d operator ^ (const real_point3d& rh) const ///< Cross Product
		{
			real_vector3d value; value.Set(
				this->j*rh.z - this->k*rh.y,
				-this->i*rh.z + this->k*rh.x,
				this->i*rh.y - this->j*rh.x
				);
			return value;
		}


		API_INLINE real operator * (const real_vector3d& rh) const ///< Dot Product
		{ return this->i*rh.i + this->j*rh.j + this->k*rh.k; }

		API_INLINE real operator * (const real_point3d& rh) const ///< Dot Product
		{ return this->i*rh.x + this->j*rh.y + this->k*rh.z; }


		API_INLINE real_vector3d operator * (real rh) const ///< Scalar Multiplication
		{
			real_vector3d value; value.Set(this->i*rh, this->j*rh, this->k*rh);
			return value;
		}

		API_INLINE real_vector3d operator / (real rh) const ///< Scalar Division
		{
			real_vector3d value; value.Set(this->i/rh, this->j/rh, this->k/rh);
			return value;
		}
	};

	// a 3d real vector with ?
	struct real_quaternion
	{
		// X-Component
		real i;

		// Y-Component
		real j;

		// Z-Component
		real k;

		// ?
		real w;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR4);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXQUATERNION);

		static void Compress64bit(real_quaternion* in, uint16 out[4]);
		static void Decompress64bit(short in[4], real_quaternion* out);

		API_INLINE void Inverse()
		{
			this->i = -this->i;
			this->j = -this->j;
			this->k = -this->k;
			this->w = -this->w;
		}

		API_INLINE void Conjugate()
		{
			this->i = -this->i;
			this->j = -this->j;
			this->k = -this->k;
		}

		void FromAngles(struct real_euler_angles3d* angles);
		void Lerp(real_quaternion* q1, real_quaternion* q2, real interp);
		void SLerp(real_quaternion* q1, real_quaternion* q2, real interp);
		void ToAxisAngle(real_point3d* axis, real* angle);
		API_INLINE void Normalize()
		{
			real mag = (real)sqrt(
				this->i*this->i + 
				this->j*this->j + 
				this->k*this->k + 
				this->w*this->w	);

			this->i /= mag;
			this->j /= mag;
			this->k /= mag;
			this->w /= mag;
		}
	};

	// real Euler angles in 2d space
	/*union*/ struct real_euler_angles2d
	{
		// Up, Down offset degrees
		angle yaw;

		// Side to Side offset degrees
		angle pitch;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};

	// real Euler angles in 3d space
	/*union*/ struct real_euler_angles3d
	{
		// Up, Down offset degrees
		angle yaw;

		// Side to Side offset degrees
		angle pitch;

		// Left to Right offset degrees
		angle roll;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(real_euler_angles2d);
	};

	// 
	struct real_plane2d
	{
		// X-Component of plane's normal
		real i;

		// Y-Component of plane's normal
		real j;

		// Distance the plane is from the origin
		real d;

		OVERRIDE_OPERATOR_CAST_THIS(float);

		API_INLINE void Normalize()
		{
			real mag = (real)sqrt(
				this->i*this->i +
				this->j*this->j
				);

			this->i /= mag;
			this->j /= mag;
			this->d /= mag;
		}
	};

	// 
	struct real_plane3d
	{
		// X-Component of plane's normal
		real i;

		// Y-Component of plane's normal
		real j;

		// Z-Component of plane's normal
		real k;

		// Distance the plane is from the origin
		real d;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS(real_plane2d);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXPLANE);

		API_INLINE void Normalize()
		{
			real mag = (real)sqrt(
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

	struct real_matrix3x3
	{
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
	};

	struct real_matrix4x3
	{
		real Scale;
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
		real_point3d Position;

		void ConvertTo4x4(D3DMATRIX& d3dmat) const
		{
			d3dmat._11 = Forward.i;		d3dmat._12 = Forward.j;		d3dmat._13 = Forward.k;		d3dmat._14 = 0.0f;
			d3dmat._21 = Left.i;		d3dmat._22 = Left.j;		d3dmat._23 = Left.k;		d3dmat._24 = 0.0f;
			d3dmat._31 = Up.i;			d3dmat._32 = Up.j;			d3dmat._33 = Up.k;			d3dmat._34 = 0.0f;
			d3dmat._41 = Position.x;	d3dmat._42 = Position.y;	d3dmat._43 = Position.z;	d3dmat._44 = Scale;
		}
	};

	struct real_matrix3x4
	{
		float m[4][3];
	};

	struct real_orientation3d
	{
		real_quaternion rotation;
		real_point3d translation;
		real scale;
	};

#pragma endregion

};

// Begin a naked function's assembly block (pointless to use this unless your function takes arguments)
#define NAKED_FUNC_START() __asm	\
	{								\
		__asm push	ebp				\
		__asm mov	ebp, esp

// End a naked function's assembly block (use with matching NAKED_FUNC_START).
// Implemented for __stdcall functions
#define NAKED_FUNC_END(arg_count)	\
		__asm pop	ebp				\
		__asm retn	(arg_count * 4) \
	}

// For usage after calling cdecl functions in assembly code.
// In the case were our assembly code is just interfacing 
// with an outside function.
#define NAKED_FUNC_END_CDECL(arg_count)		\
		__asm add	esp, (arg_count * 4)	\
	NAKED_FUNC_END(arg_count)