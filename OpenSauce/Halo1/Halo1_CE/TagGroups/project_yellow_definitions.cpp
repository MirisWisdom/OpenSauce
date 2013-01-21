/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		const project_yellow_globals_cv* CvGlobals()
		{
			if(_global_yelo_globals->HasCvGlobals())
			{
				return TagGet<project_yellow_globals_cv>(_global_yelo_globals->cv_globals.tag_index);
			}

			return NULL;
		}
	};
};