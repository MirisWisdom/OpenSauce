/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	#include <d3dx9math.h>
#endif

//#include <errno.h>
extern const errno_t k_errnone;

namespace Yelo
{
	namespace Enums
	{
		enum {
			// character count in a [tag_string] type
			k_tag_string_length = 31,
			// character count in a [long_string] type
			k_long_string_length = 255,

			k_string_id_length = 127,

			k_string_64_length = 63,
			k_string_128_length = 127,
			k_string_256_length = 255,
		};
	};


	// System Initialize function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_initialize)();
	// System Dispose function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_dispose)();
	// System Initialize for new map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_initialize_for_new_map)();
	// System Dispose from old map function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_dispose_from_old_map)();
	// System Update function pointer
	typedef void (API_FUNC* proc_update)(float);
	// System Update function pointer
	// Note: We can use this in engine definitions as well since it takes no parameters
	typedef void (API_FUNC* proc_update_no_args)();


	/// Template class for defining an interface for blocks of data whose memory layout is not entirely mapped out
	template<const size_t k_size> struct TStruct {

	protected:
		unsigned char m_data[k_size];
		
		template<typename T, const size_t k_offset> API_INLINE T GetData()					{ return *( CAST_PTR(T*,		&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> API_INLINE T GetData() const			{ return *( CAST_PTR(const T*,	&m_data[k_offset]) ); }
		template<typename T, const size_t k_offset> API_INLINE T* GetDataPtr()				{ return	CAST_PTR(T*,		&m_data[k_offset]); }
		template<typename T, const size_t k_offset> API_INLINE const T* GetDataPtr() const	{ return	CAST_PTR(const T*,	&m_data[k_offset]); }

		// Usage - "struct some_object : TStructImpl(0x40) {};"
		#define TStructImpl(size) public TStruct< size >

		// Implement a by-value getter
		#define TStructGetImpl(type, name, offset)															\
			API_INLINE type Get##name()					{ return GetData<type, offset>(); }					\
			API_INLINE type Get##name() const			{ return GetData<type, offset>(); }
		// Implement a by-address getter
		#define TStructGetPtrImpl(type, name, offset)														\
			API_INLINE type* Get##name()				{ return GetDataPtr<type, offset>(); }				\
			API_INLINE type const* Get##name() const	{ return GetDataPtr<type, offset>(); }
			//              ^ use const here, instead of before the type, in case [type] is defined as something like "int32*"

		// Implement a by-value getter for fake TStruct sub-classes
		#define TStructSubGetImpl(type, name, offset)		TStructGetImpl(type, name, offset - DATA_OFFSET)
		// Implement a by-address getter for fake TStruct sub-classes
		#define TStructSubGetPtrImpl(type, name, offset)	TStructGetPtrImpl(type, name, offset - DATA_OFFSET)
	};


#pragma region Numeral types

#define pad_bool PAD8

	// # from 0 to 255
	typedef unsigned char byte;
	// # from -128 to 127
	typedef signed char sbyte;
#define pad_byte PAD8

	// # from 0 to 65,535
	typedef unsigned short uint16;
	// # from -32,768 to 32,767
	typedef signed short int16;
#define pad_int16 PAD16

	// # from 0 to 4,294,967,295
	typedef unsigned long uint32;
#define pad_uint32 PAD32
	// # from -2147483648 to 2147483647
	typedef signed long int32;
#define pad_int32 PAD32

	// # from 0 to 18,446,744,073,709,551,615
	typedef unsigned __int64 uint64;
	// # from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
	typedef signed __int64 int64;
#define pad_int64 PAD32

	// a floating-point # from 1.175494351e-38F to 3.402823466e+38F
	typedef float real;
#define pad_real PAD32

	// min value for a [sbyte]
	extern const sbyte K_SBYTE_MIN;
	// max value for a [sbyte]
	extern const sbyte K_SBYTE_MAX;
	// min value for a [byte]
	extern const byte K_BYTE_MIN;
	// max value for a [byte]
	extern const byte K_BYTE_MAX;
	// min value for a [int16]
	extern const int16 K_INT16_MIN;
	// max value for a [int16]
	extern const int16 K_INT16_MAX;
	// min value for a [uint16]
	extern const uint16 K_UINT16_MIN;
	// max value for a [uint16]
	extern const uint16 K_UINT16_MAX;
	// min value for a [int32]
	extern const int32 K_INT32_MIN;
	// max value for a [int32]
	extern const int32 K_INT32_MAX;
	// min value for a [uint32]
	extern const uint32 K_UINT32_MIN;
	// max value for a [uint32]
	extern const uint32 K_UINT32_MAX;
	// min value for a [int64]
	extern const int64 K_INT64_MIN;
	// max value for a [int64]
	extern const int64 K_INT64_MAX;
	// min value for a [uint64]
	extern const uint64 K_UINT64_MIN;
	// max value for a [uint64]
	extern const uint64 K_UINT64_MAX;
	// min value for a [real]
	extern const real K_REAL_MIN;
	// max value for a [real]
	extern const real K_REAL_MAX;
#pragma endregion


#pragma region String types / utils
	// Constant '\0' terminated ascii string
	typedef const char* cstring;
	// '\0\0' terminated unicode string
	typedef wchar_t* wstring;
	// Constant '\0\0' terminated unicode string
	typedef const wchar_t* wcstring;

	// null value for a [string] or [cstring]
#define CSTRING_NULL	"\0"
	// null value for a [wstring] or [wcstring]
#define WSTRING_NULL	L"\0"

	API_INLINE bool is_null_or_empty(const char* const str) { return str == NULL || str[0] == '\0'; }
	API_INLINE bool is_null_or_empty(const wchar_t* const str) { return str == NULL || str[0] == L'\0'; }

	// Takes [wide] and converts it to an ascii string, to be held in [string]. If [wide_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [string] if successful
	char* wstring_to_string(char* string, int32 string_length, wcstring wide, int32 wide_length = -1);

	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide);

	// Takes [string] and converts it to an unicode string, to be held in [wide]. If [string_length] is not -1, the string
	// is assumed to be null terminated
	// Returns [wide] if successful
	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length = -1);

	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii);

	// 32 character ASCII string, with null terminator
	typedef char tag_string[Yelo::Enums::k_tag_string_length+1];
#define pad_tag_string PAD32 PAD32 PAD32 PAD32 PAD32 PAD32 PAD32 PAD32
	// 256 character ASCII string, with null terminator
	typedef char long_string[Yelo::Enums::k_long_string_length+1];
#define pad_long_string PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128 PAD128

	typedef char string_id_value[Yelo::Enums::k_string_id_length+1];
	typedef uint32 string_id;
#define pad_string_id PAD32

	// 128 character ASCII string, with null terminator
	typedef char string128[Yelo::Enums::k_string_128_length+1];
	// 256 character ASCII string, with null terminator
	typedef char string256[Yelo::Enums::k_string_256_length+1];

	// 64 character UNICODE string (128 bytes), with null terminator
	typedef wchar_t wstring64[Yelo::Enums::k_string_64_length+1];
	// 256 character UNICODE string (512 bytes), with null terminator
	typedef wchar_t wstring256[Yelo::Enums::k_string_256_length+1];

#pragma endregion

#pragma region tag type definitions
	// a floating number in degrees
	typedef float angle;
#define pad_angle PAD32

	// a tag group id
	typedef unsigned long tag;

	// an enumerated value in a 1 byte range
	typedef unsigned char byte_enum;
#define pad_byte_enum PAD8

	// an enumerated value in a 2 byte range
	typedef unsigned short _enum;
#define pad_enum PAD16

	// an enumerated value in a 4 byte range (not an officially used type in halo 1 tags, at least not up front anyway)
	typedef unsigned long long_enum;
#define pad_long_enum PAD32

	// bit flags in a 1 byte range
	typedef unsigned char byte_flags;
#define pad_byte_flags PAD8

	// bit flags in a 2 byte range
	typedef unsigned short word_flags;
#define pad_word_flags PAD16

	// bit flags in a 4 byte range
	typedef unsigned long long_flags;
#define pad_long_flags PAD32


	// 2 shorts representing 'x' and 'y'
	struct point2d
	{
		int16 x;
		int16 y;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};
#define pad_point2d PAD16 PAD16

	// 4 shorts, Top, Left, Bottom, Right
	struct rectangle2d
	{
		// Top
		int16 top;

		// Left
		int16 left;

		// Bottom
		int16 bottom;

		// Right
		int16 right;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};
#define pad_rectangle2d PAD16 PAD16 PAD16 PAD16


	struct argb_color;
	// 0 - 255, represents a color
	struct rgb_color
	{
		PAD8;
		byte red;
		byte green;
		byte blue;

		OVERRIDE_OPERATOR_CAST_THIS(argb_color);
		OVERRIDE_OPERATOR_CAST_THIS(uint32);
	};
#define pad_rgb_color PAD32

	// 0 - 255, represents a color
	struct argb_color
	{
		byte alpha;
		byte red;
		byte green;
		byte blue;

		OVERRIDE_OPERATOR_CAST_THIS(rgb_color);
		OVERRIDE_OPERATOR_CAST_THIS(uint32);
	};
#define pad_argb_color PAD32

	// bounding in short integer values
	struct short_bounds
	{
		int16 lower;
		int16 upper;

		OVERRIDE_OPERATOR_CAST_THIS(int16);
	};

#pragma endregion


#pragma region 'real' type based definitions
	// a fraction represented in a floating-point number
	typedef float real_fraction;

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
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);
#endif
	};
	#define pad_real_point2d PAD32 PAD32

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
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);
#endif
	};
	#define pad_real_point3d PAD32 PAD32 PAD32

	// A 2d real vector
	/*union*/ struct real_vector2d
	{
		// X-Component
		real i;

		// Y-Component
		real j;

		OVERRIDE_OPERATOR_CAST_THIS(float);
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR2);
#endif

		real Magnitude() const;

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

		static void DotProduct2D(real_vector2d* a, real_vector2d* b, real& out_value);
	};
	#define pad_real_vector2d PAD32 PAD32

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
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR3);
#endif

		real Magnitude() const;

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
	#define pad_real_vector3d PAD32 PAD32 PAD32

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
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXVECTOR4);
		OVERRIDE_OPERATOR_CAST_THIS(D3DXQUATERNION);
#endif

		void Compress64bit(uint16 out[4]) const;
		void Decompress64bit(int16 in[4]);

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
		void ToAxisAngle(real_point3d* axis, real& angle);
		void Normalize();
	};
	#define pad_real_quaternion PAD32 PAD32 PAD32 PAD32

	// real Euler angles in 2d space
	/*union*/ struct real_euler_angles2d
	{
		// Up, Down offset degrees
		angle yaw;

		// Side to Side offset degrees
		angle pitch;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};
	#define pad_real_euler_angles2d PAD32 PAD32

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
	#define pad_real_euler_angles3d PAD32 PAD32 PAD32

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

		API_INLINE void Normalize();
	};
	#define pad_real_plane2d PAD32 PAD32 PAD32

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
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		OVERRIDE_OPERATOR_CAST_THIS(D3DXPLANE);
#endif

		API_INLINE void Normalize();
	};
	#define pad_real_plane3d PAD32 PAD32 PAD32 PAD32

	struct real_matrix3x3
	{
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
	};
	#define pad_real_matrix3x3	PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d)

	struct real_matrix4x3
	{
		real Scale;
		real_vector3d Forward;
		real_vector3d Left;
		real_vector3d Up;
		real_point3d Position;

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
		void ConvertTo4x4(D3DMATRIX& d3dmat) const
		{
			d3dmat._11 = Forward.i;		d3dmat._12 = Forward.j;		d3dmat._13 = Forward.k;		d3dmat._14 = 0.0f;
			d3dmat._21 = Left.i;		d3dmat._22 = Left.j;		d3dmat._23 = Left.k;		d3dmat._24 = 0.0f;
			d3dmat._31 = Up.i;			d3dmat._32 = Up.j;			d3dmat._33 = Up.k;			d3dmat._34 = 0.0f;
			d3dmat._41 = Position.x;	d3dmat._42 = Position.y;	d3dmat._43 = Position.z;	d3dmat._44 = Scale;
		}
#endif
	};
	#define pad_real_matrix4x3	PAD32 \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_vector3d) \
								PAD_TYPE(real_point3d)

	struct real_matrix3x4
	{
		float m[4][3];
	};

	/*union*/ struct real_rectangle2d
	{
		real x0, x1;
		real y0, y1;
	};
	#define pad_real_rectangle2d	PAD32 PAD32 \
									PAD32 PAD32

	/*union*/ struct real_rectangle3d
	{
		real x0, x1;
		real y0, y1;
		real z0, z1;
	};
	#define pad_real_rectangle3d	PAD32 PAD32 \
									PAD32 PAD32 \
									PAD32 PAD32

	struct real_orientation3d
	{
		real_quaternion rotation;
		real_point3d translation;
		real scale;
	};
	#define pad_real_orientation3d	PAD_TYPE(real_quaternion)	\
									PAD_TYPE(real_point3d)		\
									PAD32

	// represents a color, range: 0 - 1
	struct real_rgb_color
	{
		real red;
		real green;
		real blue;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};
	#define pad_real_rgb_color PAD32 PAD32 PAD32

	// represents a color, range: 0 - 1
	struct real_argb_color
	{
		real alpha;
		real red;
		real green;
		real blue;

		OVERRIDE_OPERATOR_CAST_THIS(float);
		OVERRIDE_OPERATOR_CAST_THIS_BY_FIELD(real_rgb_color, red);
	};
	#define pad_real_argb_color PAD32 PAD32 PAD32 PAD32

	// bounding in real angle values
	struct angle_bounds
	{
		angle lower;
		angle upper;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};

	// bounding in real values
	struct real_bounds
	{
		real lower;
		real upper;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};
	#define pad_real_bounds PAD32 PAD32

	// bounding in real values
	struct real_fraction_bounds
	{
		real lower;
		real upper;

		OVERRIDE_OPERATOR_CAST_THIS(float);
	};

#pragma endregion


	// Handle to data allocated by the engine's data-array construct
	struct datum_index
	{
		typedef int16 index_t;
		typedef int16 salt_t;

		static const datum_index null;

		union {
			uint32 handle;
			struct {
				// absolute index
				// Note: This is actually suppose to be a unsigned short but it's not 
				// until Halo 3 where data starts breaking past the 0x7FFF cap, so for 
				// Halo 1 and Halo 2 projects this should still hold safe.
				// Maybe I'll update the code to uint16 one day...
				index_t index;
				// salted index, gives the handle a certain uniqueness
				salt_t salt;
			};
		};

		API_INLINE bool IsNull() const { return null == handle; }

		static datum_index Create(index_t index, salt_t salt)
		{
			datum_index result; result.handle = (CAST(uint32,salt) << 16) | index;
			return result;
		}
		// [header] should be a pointer to the start of a datum instance in a data array
		static datum_index Create(index_t index, void* header)
		{
			if(header == NULL) return null;

			return Create(index, *CAST_PTR(salt_t*, header));
		}

		OVERRIDE_OPERATOR_CAST_THIS(uint32);
		OVERRIDE_OPERATOR(=, datum_index&, uint32 arg) { this->handle = arg; return *this; }
		OVERRIDE_OPERATOR_MATH_BOOL(datum_index, handle, ==);
		OVERRIDE_OPERATOR_MATH_BOOL(datum_index, handle, !=);
	private:
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, handle, ==);
		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, handle, !=);
	}; BOOST_STATIC_ASSERT( sizeof(datum_index) == 0x4 );
#define pad_datum_index PAD16 PAD16
#define DATUM_INDEX_TO_IDENTIFIER(datum)		(datum & 0xFFFF0000)
#define DATUM_INDEX_TO_ABSOLUTE_INDEX(datum)	(datum & 0x0000FFFF)


	// The engine uses this for things (objects, ai, etc) which get dis/connected to the scenario's bsp
	struct s_scenario_location
	{
		int32 leaf_index;
		int16 cluster_index;
		int16 structure_bsp;
	}; BOOST_STATIC_ASSERT( sizeof(s_scenario_location) == 0x8 );


	// Hacked up structure.
	// Allows us to treat a machine word (in this case, 32-bits) as various sorts 
	// of types. Either as a pointer, a value-array or just vanilla values.
	template<typename T> struct TTypeHolder
	{
		union { // t_type_interface_union
			T* pointer;

			union {// t_type_interface_pointers
				bool* boolean;
				char* character;
				Yelo::byte* byte;
				Yelo::sbyte* sbyte;
				Yelo::uint16* uint16;
				Yelo::int16* int16;
				Yelo::uint32* uint32;
				Yelo::int32* int32;
				Yelo::real* real;

				datum_index* datum;

				Yelo::cstring ascii;
				Yelo::wstring unicode;
			}ptr;

			union {// t_type_interface_arrays
				bool boolean[1];
				char character[1];
				Yelo::byte byte[1];
				Yelo::sbyte sbyte[1];
				Yelo::uint16 uint16[1];
				Yelo::int16 int16[1];
				Yelo::uint32 uint32[1];
				Yelo::int32 int32[1];
				Yelo::real real[1];

				datum_index datum[1];

				Yelo::cstring ascii[1];
				Yelo::wstring unicode[1];
			}array;

			bool boolean;
			char character;
			Yelo::byte byte;
			Yelo::sbyte sbyte;
			Yelo::uint16 uint16;
			Yelo::int16 int16;
			Yelo::uint32 uint32;
			Yelo::int32 int32;
			Yelo::real real;

			datum_index datum;
		};
	};
	typedef TTypeHolder<void> TypeHolder;
	BOOST_STATIC_ASSERT( sizeof(TypeHolder) == 0x4 );

	// If the COM interface reference isn't null, releases it and nulls it
	template<typename t_interface> void safe_release(t_interface*& obj)
	{
		if(obj != NULL)
		{
			obj->Release();
			obj = NULL;
		}
	}
};