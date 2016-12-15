/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		// NOTE: command line arguments are not populated in guerilla nor tool.exe by default
		// So basically only use this in sapien builds, unless we've fixed this limitation
		bool PLATFORM_API shell_get_command_line_argument(cstring param, _Out_opt_ cstring* value = nullptr);
	};
};