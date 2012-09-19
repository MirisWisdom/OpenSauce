/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/CmdLineSettings.hpp"

#define CMDLINE_ARG(name, type)				c_cmd_line_argument<##type##> cmd_line_param_##name
#define CMDLINE_SWITCH(name)				c_cmd_line_switch cmd_line_param_##name
#define CMDLINE_INIT_PARAM(name, parameter) CMDLINE_GET_PARAM(##name##).Ctor(##parameter##)

namespace Yelo
{
	namespace Settings
	{
		CMDLINE_ARG(path, cstring);

		#if PLATFORM_IS_DEDI
			CMDLINE_ARG(http_root, cstring);
			CMDLINE_ARG(http_threads, int32);
		#else
			CMDLINE_ARG(mp_version, cstring);
			CMDLINE_SWITCH(no_os_gfx);
		#endif

		static c_cmd_line_parameter* g_cmd_line_params[] =
		{
			&CMDLINE_GET_PARAM(path),

#if PLATFORM_IS_DEDI
			&CMDLINE_GET_PARAM(http_root),
			&CMDLINE_GET_PARAM(http_threads),
#else
			&CMDLINE_GET_PARAM(mp_version),
			&CMDLINE_GET_PARAM(no_os_gfx),
#endif
		};

		void ReadCmdLineSettings()
		{
			CMDLINE_INIT_PARAM(path, "-path");

#if PLATFORM_IS_DEDI
			CMDLINE_INIT_PARAM(http_root, "-http_root");
			CMDLINE_INIT_PARAM(http_threads, "-http_threads");
#else
			CMDLINE_INIT_PARAM(mp_version, "-mp_version");
			CMDLINE_INIT_PARAM(no_os_gfx, "-no_os_gfx");
#endif

			for(int i = 0; i < NUMBEROF(g_cmd_line_params); i++)
				g_cmd_line_params[i]->GetParameter();
		}
	};
};