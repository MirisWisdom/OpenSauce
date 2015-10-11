/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
    struct real_vector2d;
    struct real_vector3d;

    typedef float angle;

    ASSERT_SIZE(angle, 0x4);

    typedef float real_fraction;

    ASSERT_SIZE(real_fraction, 0x4);

    struct real_point2d
    {
        real x;
        real y;

        friend bool operator==(const real_point2d& lhs, const real_point2d& rhs)
        {
            return lhs.x == rhs.x
                && lhs.y == rhs.y;
        }

        friend bool operator!=(const real_point2d& lhs, const real_point2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS(real_vector2d);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);
#endif
    };

    ASSERT_SIZE(real_point2d, 0x8);

    struct real_point3d
    {
        real x;
        real y;
        real z;

        friend bool operator==(const real_point3d& lhs, const real_point3d& rhs)
        {
            return lhs.x == rhs.x
                && lhs.y == rhs.y
                && lhs.z == rhs.z;
        }

        friend bool operator!=(const real_point3d& lhs, const real_point3d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS(real_point2d);

        OVERRIDE_OPERATOR_CAST_THIS(real_vector3d);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);
#endif
    };

    ASSERT_SIZE(real_point3d, 0xC);

    struct real_vector2d
    {
        real i;
        real j;

        friend bool operator==(const real_vector2d& lhs, const real_vector2d& rhs)
        {
            return lhs.i == rhs.i
                && lhs.j == rhs.j;
        }

        friend bool operator!=(const real_vector2d& lhs, const real_vector2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS_REF(real_point2d);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);
#endif

        real Magnitude() const;

        API_INLINE void Normalize()
        {
            auto len = this->Magnitude();
            this->i /= len;
            this->j /= len;
        }

        API_INLINE void Inverse()
        {
            this->i = -this->i;
            this->j = -this->j;
        }

        API_INLINE void Set(real _i, real _j)
        {
            this->i = _i;
            this->j = _j;
        }

        API_INLINE real_vector2d& operator +=(const real_vector2d& v)
        {
            this->i += v.i;
            this->j += v.j;
            return *this;
        }

        API_INLINE real_vector2d& operator -=(const real_vector2d& v)
        {
            this->i -= v.i;
            this->j -= v.j;
            return *this;
        }

        API_INLINE real_vector2d& operator *=(real scalar)
        {
            this->i *= scalar;
            this->j *= scalar;
            return *this;
        }

        API_INLINE real_vector2d& operator /=(real scalar)
        {
            this->i /= scalar;
            this->j /= scalar;
            return *this;
        }

        API_INLINE real_vector2d operator -() const ///< Conjugate
        {
            return real_vector2d { -this->i, -this->j };
        }

        API_INLINE real_vector2d operator +(const real_vector2d& rh) const ///< Addition
        {
            return real_vector2d { this->i + rh.i, this->j + rh.j };
        }

        API_INLINE real_vector2d operator +(const real_point2d& rh) const ///< Addition
        {
            return real_vector2d { this->i + rh.x, this->j + rh.y };
        }

        API_INLINE real_vector2d operator -(const real_vector2d& rh) const ///< Subtraction
        {
            return real_vector2d { this->i - rh.i, this->j - rh.j };
        }

        API_INLINE real_vector2d operator -(const real_point2d& rh) const ///< Subtraction
        {
            return real_vector2d { this->i - rh.x, this->j - rh.y };
        }

        API_INLINE real operator *(const real_vector2d& rh) const ///< Dot Product
        {
            return this->i * rh.i + this->j * rh.j;
        }

        API_INLINE real operator *(const real_point2d& rh) const ///< Dot Product
        {
            return this->i * rh.x + this->j * rh.y;
        }

        API_INLINE real_vector2d operator *(real rh) const ///< Scalar Multiplication
        {
            return real_vector2d { this->i * rh, this->j * rh };
        }

        API_INLINE real_vector2d operator /(real rh) const ///< Scalar Division
        {
            return real_vector2d { this->i / rh, this->j / rh };
        }

        static void DotProduct2D(real_vector2d* a, real_vector2d* b, real& out_value);
    };

    ASSERT_SIZE(real_vector2d, 0x8);

    struct real_vector3d
    {
        real i;
        real j;
        real k;

        friend bool operator==(const real_vector3d& lhs, const real_vector3d& rhs)
        {
            return lhs.i == rhs.i
                && lhs.j == rhs.j
                && lhs.k == rhs.k;
        }

        friend bool operator!=(const real_vector3d& lhs, const real_vector3d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS(real_vector2d);

        OVERRIDE_OPERATOR_CAST_THIS_REF(real_point3d);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);
#endif

        real Magnitude() const;

        API_INLINE void Normalize()
        {
            auto len = this->Magnitude();
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
        {
            this->i = _i;
            this->j = _j;
            this->k = _k;
        }

        API_INLINE real_vector3d& operator +=(const real_vector3d& v)
        {
            this->i += v.i;
            this->j += v.j;
            this->k += v.k;
            return *this;
        }

        API_INLINE real_vector3d& operator -=(const real_vector3d& v)
        {
            this->i -= v.i;
            this->j -= v.j;
            this->k -= v.k;
            return *this;
        }

        API_INLINE real_vector3d& operator *=(real scalar)
        {
            this->i *= scalar;
            this->j *= scalar;
            this->k *= scalar;
            return *this;
        }

        API_INLINE real_vector3d& operator /=(real scalar)
        {
            this->i /= scalar;
            this->j /= scalar;
            this->k /= scalar;
            return *this;
        }

        API_INLINE real_vector3d operator -() const ///< Conjugate
        {
            return real_vector3d { -this->i, -this->j, -this->k };
        }

        API_INLINE real_vector3d operator +(const real_vector3d& rh) const ///< Addition
        {
            return real_vector3d { this->i + rh.i, this->j + rh.j, this->k + rh.k };
        }

        API_INLINE real_vector3d operator +(const real_point3d& rh) const ///< Addition
        {
            return real_vector3d { this->i + rh.x, this->j + rh.y, this->k + rh.z };
        }

        API_INLINE real_vector3d operator -(const real_vector3d& rh) const ///< Subtraction
        {
            return real_vector3d { this->i - rh.i, this->j - rh.j, this->k - rh.k };
        }

        API_INLINE real_vector3d operator -(const real_point3d& rh) const ///< Subtraction
        {
            return real_vector3d { this->i - rh.x, this->j - rh.y, this->k - rh.z };
        }

        API_INLINE real_vector3d operator ^(const real_vector3d& rh) const ///< Cross Product
        {
            return real_vector3d {
                this->j * rh.k - this->k * rh.j,
                -this->i * rh.k + this->k * rh.i,
                this->i * rh.j - this->j * rh.i
                };
        }

        API_INLINE real_vector3d operator ^(const real_point3d& rh) const ///< Cross Product
        {
            return real_vector3d {
                this->j * rh.z - this->k * rh.y,
                -this->i * rh.z + this->k * rh.x,
                this->i * rh.y - this->j * rh.x
                };
        }

        API_INLINE real operator *(const real_vector3d& rh) const ///< Dot Product
        {
            return this->i * rh.i + this->j * rh.j + this->k * rh.k;
        }

        API_INLINE real operator *(const real_point3d& rh) const ///< Dot Product
        {
            return this->i * rh.x + this->j * rh.y + this->k * rh.z;
        }

        API_INLINE real_vector3d operator *(real rh) const ///< Scalar Multiplication
        {
            return real_vector3d { this->i * rh, this->j * rh, this->k * rh };
        }

        API_INLINE real_vector3d operator /(real rh) const ///< Scalar Division
        {
            return real_vector3d { this->i / rh, this->j / rh, this->k / rh };
        }
    };

    ASSERT_SIZE(real_vector3d, 0xC);

    struct real_quaternion
    {
        real i;
        real j;
        real k;
        real w;

        friend bool operator==(const real_quaternion& lhs, const real_quaternion& rhs)
        {
            return lhs.i == rhs.i
                && lhs.j == rhs.j
                && lhs.k == rhs.k
                && lhs.w == rhs.w;
        }

        friend bool operator!=(const real_quaternion& lhs, const real_quaternion& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR4);

        OVERRIDE_OPERATOR_CAST_THIS(D3DXQUATERNION);
#endif

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

        void Compress64bit(uint16 out[4]) const;
        void Decompress64bit(int16 in[4]);
        void FromAngles(struct real_euler_angles3d* angles);
        void Lerp(real_quaternion* q1, real_quaternion* q2, real interp);
        void SLerp(real_quaternion* q1, real_quaternion* q2, real interp);
        void ToAxisAngle(real_point3d* axis, real& angle);
        void Normalize();
    };

    ASSERT_SIZE(real_quaternion, 0x10);

    struct real_euler_angles2d
    {
        angle yaw;
        angle pitch;

        friend bool operator==(const real_euler_angles2d& lhs, const real_euler_angles2d& rhs)
        {
            return lhs.yaw == rhs.yaw
                && lhs.pitch == rhs.pitch;
        }

        friend bool operator!=(const real_euler_angles2d& lhs, const real_euler_angles2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
    };

    ASSERT_SIZE(real_euler_angles2d, 0x8);

    struct real_euler_angles3d
    {
        angle yaw;
        angle pitch;
        angle roll;

        friend bool operator==(const real_euler_angles3d& lhs, const real_euler_angles3d& rhs)
        {
            return lhs.yaw == rhs.yaw
                && lhs.pitch == rhs.pitch
                && lhs.roll == rhs.roll;
        }

        friend bool operator!=(const real_euler_angles3d& lhs, const real_euler_angles3d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS(real_euler_angles2d);
    };

    ASSERT_SIZE(real_euler_angles3d, 0xC);

    struct real_plane2d
    {
        real i;
        real j;
        real d;

        friend bool operator==(const real_plane2d& lhs, const real_plane2d& rhs)
        {
            return lhs.i == rhs.i
                && lhs.j == rhs.j
                && lhs.d == rhs.d;
        }

        friend bool operator!=(const real_plane2d& lhs, const real_plane2d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS_REF(real_vector2d);

        API_INLINE void Normalize();
    };

    ASSERT_SIZE(real_plane2d, 0xC);

    struct real_plane3d
    {
        real i;
        real j;
        real k;
        real d;

        friend bool operator==(const real_plane3d& lhs, const real_plane3d& rhs)
        {
            return lhs.i == rhs.i
                && lhs.j == rhs.j
                && lhs.k == rhs.k
                && lhs.d == rhs.d;
        }

        friend bool operator!=(const real_plane3d& lhs, const real_plane3d& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);

        OVERRIDE_OPERATOR_CAST_THIS(real_plane2d);

        OVERRIDE_OPERATOR_CAST_THIS_REF(real_vector3d);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
        OVERRIDE_OPERATOR_CAST_THIS(D3DXPLANE);
#endif

        API_INLINE void Normalize();
    };

    ASSERT_SIZE(real_plane3d, 0x10);

    struct real_rectangle2d
    {
        real x0, x1; // top, bottom
        real y0, y1; // left, right

        friend bool operator==(const real_rectangle2d& lhs, const real_rectangle2d& rhs)
        {
            return lhs.x0 == rhs.x0
                && lhs.x1 == rhs.x1
                && lhs.y0 == rhs.y0
                && lhs.y1 == rhs.y1;
        }

        friend bool operator!=(const real_rectangle2d& lhs, const real_rectangle2d& rhs)
        {
            return !(lhs == rhs);
        }
    };

    ASSERT_SIZE(real_rectangle2d, 0x10);

    struct real_rectangle3d
    {
        real x0, x1;
        real y0, y1;
        real z0, z1;

        friend bool operator==(const real_rectangle3d& lhs, const real_rectangle3d& rhs)
        {
            return lhs.x0 == rhs.x0
                && lhs.x1 == rhs.x1
                && lhs.y0 == rhs.y0
                && lhs.y1 == rhs.y1
                && lhs.z0 == rhs.z0
                && lhs.z1 == rhs.z1;
        }

        friend bool operator!=(const real_rectangle3d& lhs, const real_rectangle3d& rhs)
        {
            return !(lhs == rhs);
        }
    };

    ASSERT_SIZE(real_rectangle3d, 0x18);

    struct real_orientation3d
    {
        real_quaternion rotation;
        real_point3d translation;
        real scale;

        friend bool operator==(const real_orientation3d& lhs, const real_orientation3d& rhs)
        {
            return lhs.rotation == rhs.rotation
                && lhs.translation == rhs.translation
                && lhs.scale == rhs.scale;
        }

        friend bool operator!=(const real_orientation3d& lhs, const real_orientation3d& rhs)
        {
            return !(lhs == rhs);
        }
    };

    ASSERT_SIZE(real_orientation3d, 0x20);

    struct angle_bounds
    {
        angle lower;
        angle upper;

        friend bool operator==(const angle_bounds& lhs, const angle_bounds& rhs)
        {
            return lhs.lower == rhs.lower
                && lhs.upper == rhs.upper;
        }

        friend bool operator!=(const angle_bounds& lhs, const angle_bounds& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
    };

    ASSERT_SIZE(angle_bounds, 0x8);

    struct real_bounds
    {
        real lower;
        real upper;

        friend bool operator==(const real_bounds& lhs, const real_bounds& rhs)
        {
            return lhs.lower == rhs.lower
                && lhs.upper == rhs.upper;
        }

        friend bool operator!=(const real_bounds& lhs, const real_bounds& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
    };

    ASSERT_SIZE(real_bounds, 0x8);

    struct real_fraction_bounds
    {
        real lower;
        real upper;

        friend bool operator==(const real_fraction_bounds& lhs, const real_fraction_bounds& rhs)
        {
            return lhs.lower == rhs.lower
                && lhs.upper == rhs.upper;
        }

        friend bool operator!=(const real_fraction_bounds& lhs, const real_fraction_bounds& rhs)
        {
            return !(lhs == rhs);
        }

        OVERRIDE_OPERATOR_CAST_THIS(float);
    };

    ASSERT_SIZE(real_fraction_bounds, 0x8);

#define pad_angle                   PAD32
#define pad_real_fraction           PAD32
#define pad_real_point2d            PAD32 PAD32
#define pad_real_point3d            PAD32 PAD32 PAD32
#define pad_real_vector2d           PAD32 PAD32
#define pad_real_vector3d           PAD32 PAD32 PAD32
#define pad_real_quaternion         PAD32 PAD32 PAD32 PAD32
#define pad_real_euler_angles2d     PAD32 PAD32
#define pad_real_euler_angles3d     PAD32 PAD32 PAD32
#define pad_real_plane2d            PAD32 PAD32 PAD32
#define pad_real_plane3d            PAD32 PAD32 PAD32 PAD32
#define pad_real_rectangle2d        PAD32 PAD32 PAD32 PAD32
#define pad_real_rectangle3d        PAD32 PAD32 PAD32 PAD32 PAD32 PAD32
#define pad_real_orientation3d      PAD_TYPE(real_quaternion) PAD_TYPE(real_point3d) PAD32
#define pad_angle_bounds            PAD32 PAD32
#define pad_real_bounds             PAD32 PAD32
#define pad_real_fraction_bounds    PAD32 PAD32
};
