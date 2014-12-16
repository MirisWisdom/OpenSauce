/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Control/s_interface_value.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
#pragma region String Handling
		void s_interface_value::DeleteString()
		{
			if(is_cstring && m_cstring && is_string_owner)
			{
				delete [] m_cstring;
				m_cstring = nullptr;
			}
			else if(is_wstring && m_wstring && is_string_owner)
			{
				delete [] m_wstring;
				m_wstring = nullptr;
			}

			is_cstring = false;
			is_wstring = false;
			is_string_owner = false;
		}

		void s_interface_value::SetString(const std::string& value)
		{
			DeleteString();

			auto length = strlen(value.c_str()) + 1;
			m_cstring = new char[length];
			m_cstring[0] = '\0';

			strcpy_s(m_cstring, length, value.c_str());
			is_cstring = true;
			is_string_owner = true;
		}

		void s_interface_value::SetString(const std::wstring& value)
		{
			DeleteString();

			auto length = wcslen(value.c_str()) + 1;
			m_wstring = new wchar_t[length];
			m_wstring[0] = L'\0';

			wcscpy_s(m_wstring, length, value.c_str());
			is_wstring = true;
			is_string_owner = true;
		}
#pragma endregion

		s_interface_value::s_interface_value()
			: is_cstring(false)
			, is_wstring(false)
			, is_string_owner(false)
		{
			ZeroMemory(&m_real_argb_color, sizeof(real_argb_color));
		}

		s_interface_value::~s_interface_value()
		{
			DeleteString();
		}

		s_interface_value::s_interface_value(void* data, const int32 size)
			: s_interface_value()
		{
			memcpy_s(m_data, sizeof(m_data), data, size);
		}

		s_interface_value::s_interface_value(bool value)
			: s_interface_value(&value, sizeof(bool)) { }
		s_interface_value::s_interface_value(byte value)
			: s_interface_value(&value, sizeof(byte)) { }
		s_interface_value::s_interface_value(sbyte value)
			: s_interface_value(&value, sizeof(sbyte)) { }
		s_interface_value::s_interface_value(uint16 value)
			: s_interface_value(&value, sizeof(uint16)) { }
		s_interface_value::s_interface_value(int16 value)
			: s_interface_value(&value, sizeof(int16)) { }
		s_interface_value::s_interface_value(uint32 value)
			: s_interface_value(&value, sizeof(uint32)) { }
		s_interface_value::s_interface_value(int32 value)
			: s_interface_value(&value, sizeof(int32)) { }
		s_interface_value::s_interface_value(uint64 value)
			: s_interface_value(&value, sizeof(uint64)) { }
		s_interface_value::s_interface_value(int64 value)
			: s_interface_value(&value, sizeof(int64)) { }
		s_interface_value::s_interface_value(real value)
			: s_interface_value(&value, sizeof(real)) { }
		s_interface_value::s_interface_value(cstring value)
			: s_interface_value(&value, sizeof(cstring)) { }
		s_interface_value::s_interface_value(wstring value)
			: s_interface_value(&value, sizeof(wstring)) { }
		s_interface_value::s_interface_value(point2d value)
			: s_interface_value(&value, sizeof(point2d)) { }
		s_interface_value::s_interface_value(rectangle2d value)
			: s_interface_value(&value, sizeof(rectangle2d)) { }
		s_interface_value::s_interface_value(real_vector2d value)
			: s_interface_value(&value, sizeof(real_vector2d)) { }
		s_interface_value::s_interface_value(real_vector3d value)
			: s_interface_value(&value, sizeof(real_vector3d)) { }
		s_interface_value::s_interface_value(real_point2d value)
			: s_interface_value(&value, sizeof(real_point2d)) { }
		s_interface_value::s_interface_value(real_point3d value)
			: s_interface_value(&value, sizeof(real_point3d)) { }
		s_interface_value::s_interface_value(real_quaternion value)
			: s_interface_value(&value, sizeof(real_quaternion)) { }
		s_interface_value::s_interface_value(rgb_color value)
			: s_interface_value(&value, sizeof(rgb_color)) { }
		s_interface_value::s_interface_value(argb_color value)
			: s_interface_value(&value, sizeof(argb_color)) { }
		s_interface_value::s_interface_value(real_rgb_color value)
			: s_interface_value(&value, sizeof(real_rgb_color)) { }
		s_interface_value::s_interface_value(real_argb_color value)
			: s_interface_value(&value, sizeof(real_argb_color)) { }
	};};};
};
#endif