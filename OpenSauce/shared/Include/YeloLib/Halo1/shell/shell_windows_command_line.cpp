/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include <blamlib/Halo1/shell/shell.hpp>

#define CMDLINE_ARG(name, type)				c_cmd_line_argument<type>	CMDLINE_PARAM_NAME(name)
#define CMDLINE_SWITCH(name)				c_cmd_line_switch			CMDLINE_PARAM_NAME(name)
#define CMDLINE_INIT_PARAM(name, parameter) CMDLINE_GET_PARAM( name ).Initialize( parameter )

namespace Yelo
{
	namespace Settings
	{
		//////////////////////////////////////////////////////////////////////////
		// Actually define the params here
#define CMDLINE_DEFINE_ARG(name, type)		CMDLINE_ARG(name, type);
#define CMDLINE_DEFINE_SWITCH(name)			CMDLINE_SWITCH(name);
#include <YeloLib/Halo1/shell/shell_windows_command_line.inl>

		static c_cmd_line_parameter* g_cmd_line_params[] =
		{
#define CMDLINE_DEFINE_ARG(name, type)		&CMDLINE_GET_PARAM(name),
#define CMDLINE_DEFINE_SWITCH(name)			&CMDLINE_GET_PARAM(name),
#include <YeloLib/Halo1/shell/shell_windows_command_line.inl>
			nullptr
		};

		struct s_cmd_line_initializer
		{
			static void Run()
			{
#define CMDLINE_DEFINE_ARG(name, type)		CMDLINE_INIT_PARAM(name, BOOST_PP_CAT("-", #name));
#define CMDLINE_DEFINE_SWITCH(name)			CMDLINE_INIT_PARAM(name, BOOST_PP_CAT("-", #name));
#include <YeloLib/Halo1/shell/shell_windows_command_line.inl>

				// we don't use ranged for here because the last entry is NULL and we'd then have to check for null
				for(size_t i = 0; i < NUMBEROF(g_cmd_line_params)-1; i++)
					g_cmd_line_params[i]->GetParameter();
			}
		};
		void ReadCmdLineSettings()
		{
			s_cmd_line_initializer::Run();
		}


		void c_cmd_line_parameter::Initialize(cstring parameter)
		{
			m_parameter_set = false;
			m_name = parameter;
		}

		void c_cmd_line_parameter::GetParameter()
		{
			cstring value = nullptr;
			m_parameter_set = blam::shell_get_command_line_argument(m_name, &value);

			// if the parameter has been set, parse the parameters value
			if(m_parameter_set && value)
				m_parameter_set = ParseValue(value);
		}
	};
};

#undef CMDLINE_ARG
#undef CMDLINE_SWITCH
#undef CMDLINE_INIT_PARAM