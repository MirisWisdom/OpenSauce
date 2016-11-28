/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#if PLATFORM_TYPE != PLATFORM_TOOL
#include <blamlib/Halo1/guerilla/tags_interface.hpp>
#endif

namespace Yelo
{
	namespace TagGroups
	{
		// Don't use me directly unless there's not an existing tag function for getting a certain property
		tag_instance_data_t&	TagInstances();


		void Initialize();


		// if true, trying to load a 'mode' (.model) tag will instead result in a 'mod2' (.gbxmodel) tag with the same name being loaded
		void tag_groups_set_model_upgrade_hack(BOOL hack_enabled);
	};
};