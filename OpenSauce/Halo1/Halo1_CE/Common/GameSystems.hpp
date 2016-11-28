/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Common/GameSystemDefinitions.hpp"

namespace Yelo
{
	namespace Main
	{
		/*
		  Yes, there's a reason why we return component_count minus 1.
		  For most components, there's a dispose/shutdown pattern that can be ran.
		  So we can easily do for loops that are:
			'x <= count' for initialize
			'x >= 0' for dispose
		  In both cases 'x' will be a valid index into the components array.
		  Then since we use NONE for component arrays that don't exist, the for loops will never run
		  (thus the arrays will never get dereferenced. they're null in said conditions after all)
		*/

		// [out_components] - Returns a list of components, or NULL if no components exist
		// returns: Number of components in [out_components] minus 1, or NONE if no components exist in this build
		int32 GetProjectComponents(s_project_component*& out_components);
		// [out_components] - Returns a list of components, or NULL if no components exist
		// returns: Number of components in [out_components] minus 1, or NONE if no components exist in this build
		int32 GetProjectComponents(s_project_map_component*& out_components);
		// [out_components] - Returns a list of components, or NULL if no components exist
		// returns: Number of components in [out_components] minus 1, or NONE if no components exist in this build
		int32 GetProjectComponents(s_project_bsp_component*& out_components);
		// [out_components] - Returns a list of components, or NULL if no components exist
		// returns: Number of components in [out_components] minus 1, or NONE if no components exist in this build
		int32 GetProjectComponents(s_project_game_state_component*& out_components);


#if PLATFORM_IS_USER
		// [out_components] - Returns a list of components, or NULL if no components exist
		// returns: Number of components in [out_components] minus 1, or NONE if no components exist in this build
		int32 GetDXComponents(s_dx_component*& out_components);
#endif

		void InsertHooks();
	};
};