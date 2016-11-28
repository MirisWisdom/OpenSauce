/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace ValueConversion 
	{
#pragma region From String
		bool FromString(cstring string, bool& output);
		bool FromString(cstring string, byte& output);
		bool FromString(cstring string, sbyte& output);
		bool FromString(cstring string, uint16& output);
		bool FromString(cstring string, int16& output);
		bool FromString(cstring string, uint32& output);
		bool FromString(cstring string, int32& output);
		bool FromString(cstring string, uint64& output);
		bool FromString(cstring string, int64& output);
		bool FromString(cstring string, real& output);
		bool FromString(cstring string, std::string& output);
		bool FromString(cstring string, point2d& output);
		bool FromString(cstring string, rectangle2d& output);
		bool FromString(cstring string, real_vector2d& output);
		bool FromString(cstring string, real_vector3d& output);
		bool FromString(cstring string, real_point2d& output);
		bool FromString(cstring string, real_point3d& output);
		bool FromString(cstring string, real_quaternion& output);
		bool FromString(cstring string, rgb_color& output);
		bool FromString(cstring string, argb_color& output);
		bool FromString(cstring string, real_rgb_color& output);
		bool FromString(cstring string, real_argb_color& output);
#pragma endregion
	};
};