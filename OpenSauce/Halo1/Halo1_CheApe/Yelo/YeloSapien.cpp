/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_SAPIEN
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include "CheApeInterface.hpp" // for Memory functions
#include "Common/YeloSettings.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

namespace Yelo
{
	namespace Sapien
	{
		namespace AddressOf
		{
			// Addresses which check a counter against the max tag limit
			static uint16* MaxTagsCheck[] = {
				CAST_PTR(uint16*, 0x4FC9AB),
			};
		};

		void Initialize()
		{
			// Set Sapien's affinity mask if there is one specified
			const Settings::s_profile& k_profile = Settings::Get().active_profile;
			if(!k_profile.IsIgnored() && k_profile.has_affinity_mask)
			{
				SetProcessAffinityMask(GetCurrentProcess(), k_profile.affinity_mask);
			}

			for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
				*AddressOf::MaxTagsCheck[x] = Konstants::MaximumTagCountUpgrade();

			TagGroups::ScenarioYeloLoadHookInitialize();
		}

		void Dispose()
		{
			for(int32 x = 0; x < NUMBEROF(AddressOf::MaxTagsCheck); x++)
				*AddressOf::MaxTagsCheck[x] = Enums::k_maximum_tag_count;

			TagGroups::ScenarioYeloLoadHookDispose();
		}
	};
};
#endif