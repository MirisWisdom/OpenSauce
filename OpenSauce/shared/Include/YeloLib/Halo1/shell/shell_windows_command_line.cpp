/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#if PLATFORM_IS_EDITOR
	#include "Engine/EngineFunctions.hpp"
#else
	#include "Game/EngineFunctions.hpp"
#endif

#define CMDLINE_ARG(name, type)				c_cmd_line_argument<##type##> CMDLINE_PARAM_NAME(name)
#define CMDLINE_SWITCH(name)				c_cmd_line_switch CMDLINE_PARAM_NAME(name)
#define CMDLINE_INIT_PARAM(name, parameter) CMDLINE_GET_PARAM( name ).Ctor( parameter )

namespace Yelo
{
	namespace Settings
	{
		//////////////////////////////////////////////////////////////////////////
		// Actually define the params here
#define CMDLINE_DEFINE_ARG(name, type)		CMDLINE_ARG(name, type);
#define CMDLINE_DEFINE_SWITCH(name)			CMDLINE_SWITCH(name);
#include "Common/YeloSettings.CmdLineSettings.inl"

		static c_cmd_line_parameter* g_cmd_line_params[] =
		{
#define CMDLINE_DEFINE_ARG(name, type)		&CMDLINE_GET_PARAM(name),
#define CMDLINE_DEFINE_SWITCH(name)			&CMDLINE_GET_PARAM(name),
#include "Common/YeloSettings.CmdLineSettings.inl"
			NULL
		};

		void ReadCmdLineSettings()
		{
#define CMDLINE_DEFINE_ARG(name, type)		CMDLINE_INIT_PARAM(name, BOOST_PP_CAT("-", #name));
#define CMDLINE_DEFINE_SWITCH(name)			CMDLINE_INIT_PARAM(name, BOOST_PP_CAT("-", #name));
#include "Common/YeloSettings.CmdLineSettings.inl"

			for(int i = 0; i < NUMBEROF(g_cmd_line_params)-1; i++)
				g_cmd_line_params[i]->GetParameter();
		}


		void c_cmd_line_parameter::Ctor(cstring parameter)
		{
			m_parameter_set = false;
			m_parameter = parameter;
		}

		bool c_cmd_line_parameter::ParameterSet() const
		{
			return m_parameter_set;
		}

		void c_cmd_line_parameter::GetParameter()
		{
			cstring value = NULL;
			m_parameter_set = Engine::GetCmdLineParameter(m_parameter, &value);

			// if the parameter has been set, parse the parameters value
			if(m_parameter_set && value)
				m_parameter_set = ParseValue(value);
		}
	};
};