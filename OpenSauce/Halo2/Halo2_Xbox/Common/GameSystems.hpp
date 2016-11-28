/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Main
	{
		struct s_project_component
		{
			// Called from Main on startup
			proc_initialize				Initialize;
			// Called from Main on shutdown
			proc_dispose				Dispose;
		};

		//	[out_components] - Returns a list of components, or NULL if no components exist
		//	returns: Number of components in [out_components] minus 1, or just 0 if no components exist
		int32 GetProjectComponents(s_project_component*& out_components);
	};
};