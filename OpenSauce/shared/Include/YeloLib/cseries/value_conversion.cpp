/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/value_conversion.hpp>

namespace Yelo
{
	namespace ValueConversion
	{
#pragma region From String
		template<typename ... Arguments>
		static bool FromStringImpl(cstring format, int member_count, cstring input, Arguments ... args)
		{
			return member_count == sscanf_s(input, format, args ...);
		}

		bool FromString(cstring string, bool& output)
		{
			auto std_string = std::string(string);

			if((std_string == "True") || (std_string == "true") || (std_string == "on") || (std_string == "1"))
			{
				output = true;
				return true;
			}
			else if((std_string == "False") || (std_string == "false") || (std_string == "off") || (std_string == "0"))
			{
				output = false;
				return true;
			}

			return false;
		}

		bool FromString(cstring string, byte& output)
		{
			uint32 value;
			auto result = FromStringImpl("%u", 1, string, &value);
			output = (byte)value;

			return result;
		}

		bool FromString(cstring string, sbyte& output)
		{
			int32 value;
			auto result = FromStringImpl("%d", 1, string, &value);
			output = (sbyte)value;

			return result;
		}

		bool FromString(cstring string, uint16& output)
		{
			uint32 value;
			auto result = FromStringImpl("%u", 1, string, &value);
			output = (uint16)value;

			return result;
		}

		bool FromString(cstring string, int16& output)
		{
			int32 value;
			auto result = FromStringImpl("%d", 1, string, &value);
			output = (int16)value;

			return result;
		}

		bool FromString(cstring string, uint32& output)
		{
			return FromStringImpl("%u", 1, string, &output);
		}

		bool FromString(cstring string, int32& output)
		{
			return FromStringImpl("%d", 1, string, &output);
		}

		bool FromString(cstring string, uint64& output)
		{
			return FromStringImpl("%lu", 1, string, &output);
		}

		bool FromString(cstring string, int64& output)
		{
			return FromStringImpl("%ld", 1, string, &output);
		}

		bool FromString(cstring string, real& output)
		{
			return FromStringImpl("%f", 1, string, &output);
		}

		bool FromString(cstring string, std::string& output)
		{
			output.assign(string);
			return true;
		}

		bool FromString(cstring string, point2d& output)
		{
			int32 x, y;
			auto result = FromStringImpl("%d %d", 2, string, &x, &y);
			output.x = (int16)x;
			output.y = (int16)y;

			return result;
		}

		bool FromString(cstring string, rectangle2d& output)
		{
			int32 top, left, bottom, right;
			auto result = FromStringImpl("%d %d %d %d", 4, string, &top, &left, &bottom, &right);
			output.top = (int16)top;
			output.left = (int16)left;
			output.bottom = (int16)bottom;
			output.right = (int16)right;

			return result;
		}

		bool FromString(cstring string, real_vector2d& output)
		{
			return FromStringImpl("%f %f", 2, string, &output.i, &output.j);
		}

		bool FromString(cstring string, real_vector3d& output)
		{
			return FromStringImpl("%f %f %f", 3, string, &output.i, &output.j, &output.k);
		}

		bool FromString(cstring string, real_point2d& output)
		{
			return FromStringImpl("%f %f", 2, string, &output.x, &output.y);
		}

		bool FromString(cstring string, real_point3d& output)
		{
			return FromStringImpl("%f %f %f", 3, string, &output.x, &output.y, &output.z);
		}

		bool FromString(cstring string, real_quaternion& output)
		{
			return FromStringImpl("%f %f %f %f", 4, string, &output.i, &output.j, &output.k, &output.w);
		}

		bool FromString(cstring string, rgb_color& output)
		{
			uint32 red, green, blue;
			auto result = FromStringImpl("%u %u %u", 3, string, &red, &green, &blue);
			output.red = (byte)red;
			output.green = (byte)green;
			output.blue =(byte) blue;

			return result;
		}

		bool FromString(cstring string, argb_color& output)
		{
			uint32 red, green, blue, alpha;
			auto result = FromStringImpl("%u %u %u %u", 4, string, &red, &green, &blue, &alpha);
			output.red = (byte)red;
			output.green = (byte)green;
			output.blue = (byte)blue;
			output.alpha = (byte)alpha;

			return result;
		}

		bool FromString(cstring string, real_rgb_color& output)
		{
			return FromStringImpl("%f %f %f", 3, string, &output.red, &output.green, &output.blue);
		}

		bool FromString(cstring string, real_argb_color& output)
		{
			return FromStringImpl("%f %f %f %f", 4, string, &output.red, &output.green, &output.blue, &output.alpha);
		}
#pragma endregion
	};
};