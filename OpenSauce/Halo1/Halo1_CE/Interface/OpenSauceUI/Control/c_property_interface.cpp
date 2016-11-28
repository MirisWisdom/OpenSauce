/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Control/c_property_interface.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		c_property_interface::c_property_interface(const property_get_func_t& get
			, const property_set_func_t& set
			, const property_set_by_string_func_t& set_string)
			: m_get(get)
			, m_set(set)
			, m_set_string(set_string)
		{ }

		void c_property_interface::Set(i_control& control, const char* value)
		{
			if(m_set_string)
			{
				m_set_string(control, value);
			}
		}

		void c_property_interface::Set(i_control& control, const s_interface_value& value)
		{
			if(m_set)
			{
				m_set(control, value);
			}
		}

		bool c_property_interface::Get(i_control& control, s_interface_value& output_value) const
		{
			if(m_get)
			{
				m_get(control, output_value);
				return true;
			}

			return false;
		}
	};};};
};
#endif