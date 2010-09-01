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

#if PLATFORM_ID == PLATFORM_TOOL
#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <Blam/Halo1/project_yellow_shared_definitions.hpp>
#include <Blam/Halo1/BlamDataFileUpgrades.hpp>
#include <TagGroups/Halo1/scenario_definitions.hpp>

#include "CheApeInterface.hpp" // for Memory functions
#include "Common/YeloSettings.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"
#include "Tool/ToolDefinitions.hpp"
#include "Tool/Tool_AnimationFixups.hpp"

#include "Tool/ImportLightmapUV.hpp"
#include "Tool/ShaderPostprocess.hpp"

namespace Yelo
{
	namespace Tool
	{
		enum {
			k_number_of_tool_import_classes = 0x15,
		};

		#include "Tool/BuildCacheFile.inl"
		#include "Tool/BuildViewMap.inl" // OS_tool build-view-map "0screenshot00.tga" "view_state.bin" "world_points.txt"
		#include "Tool/CompileScripts.inl"
		#include "Tool/ImportShaderPostprocess.inl"
		#include "Tool/Help.inl"


//////////////////////////////////////////////////////////////////////////
// Import classes

		static const s_import_class yelo_import_classes[] = {
			#include "Tool/ImportClassDefinitions.inl"
		};

		int __cdecl s_import_class_compare(void *, const void* lhs, const void* rhs)
		{
			return strcmp(CAST_PTR(const s_import_class*,lhs)->name, CAST_PTR(const s_import_class*,rhs)->name);
		}
		void ImportClassesInitialize()
		{
			// Tool's original import classes
			static const s_import_class* tool_import_classes = CAST_PTR(s_import_class*, 0x6B01B0);
			// The new import class list which is made up of tool's
			// and [yelo_import_classes]
			static s_import_class import_classes[k_number_of_tool_import_classes + NUMBEROF(yelo_import_classes)];

			// copy official import classes
			memcpy_s(&import_classes[0], sizeof(import_classes),
				tool_import_classes, sizeof(s_import_class) * k_number_of_tool_import_classes);
			// copy yelo import classes
			memcpy_s(&import_classes[k_number_of_tool_import_classes], sizeof(yelo_import_classes),
				&yelo_import_classes[0], sizeof(yelo_import_classes));
			// Now I know my ABCs
			qsort_s(import_classes, NUMBEROF(import_classes), sizeof(s_import_class), s_import_class_compare, NULL);


			static s_import_class** import_classes_references[] = {
				CAST_PTR(s_import_class**, 0x414E6C),
				CAST_PTR(s_import_class**, 0x414EAB),
			};
			static void** import_classes_references_to_usage[] = {
				CAST_PTR(void**, 0x41541D),
			};
			static byte* import_classes_count[] = {
				CAST_PTR(byte*, 0x414E82),
				CAST_PTR(byte*, 0x415412),
			};

			// update references to the import class definitions
			for(int32 x = 0; x < NUMBEROF(import_classes_references); x++)			*import_classes_references[x] = &import_classes[0];
			// update references to the import class definition usages
			for(int32 x = 0; x < NUMBEROF(import_classes_references_to_usage); x++)	*import_classes_references_to_usage[x] = &import_classes[0].usage;
			// update code which contain the import class definitions count
			for(int32 x = 0; x < NUMBEROF(import_classes_count); x++)				*import_classes_count[x] = NUMBEROF(import_classes);
		}

//////////////////////////////////////////////////////////////////////////

		void Initialize()
		{
			c_animation_fixups::Initialize();

			Yelo::DataFiles::SharedInitialize();
			ImportClassesInitialize();
		}

		void Dispose()
		{
			Yelo::DataFiles::SharedDispose();

			c_animation_fixups::Dispose();
		}
	};
};

#endif