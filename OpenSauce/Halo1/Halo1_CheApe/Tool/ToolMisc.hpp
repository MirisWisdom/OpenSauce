/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_ID == PLATFORM_TOOL

namespace Yelo
{
	namespace Tool
	{
		void PLATFORM_API scenario_compile_scripts(void** arguments);

		void PLATFORM_API scenario_cleanse_of_yelo_data(void** arguments);
	};
};
#endif