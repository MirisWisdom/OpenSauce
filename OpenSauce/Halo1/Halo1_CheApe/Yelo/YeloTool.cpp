/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_TOOL
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>
#include <blamlib/Halo1/objects/object_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/tool/tool.hpp>

#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Common/YeloSettings.hpp"
#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"
#include "Tool/Tool_AnimationFixups.hpp"

#include "Tool/ImportLightmapUV.hpp"
#include "Tool/ShaderPostprocess.hpp"
#include "Tool/BuildCppDefinition.hpp"
#include "Tool/RuntimeCacheView.hpp"
#include "Tool/ToolMisc.hpp"

namespace Yelo
{
	namespace Tool
	{
		enum {
			k_number_of_tool_import_classes = 0x15,
		};

		#include "Tool/BuildCacheFile.inl"
		#include "Tool/Help.inl"


//////////////////////////////////////////////////////////////////////////
// Import classes

		static const s_import_class yelo_import_classes[] = {
			#include "Tool/ImportClassDefinitions.inl"
		};

		int __cdecl s_import_class_compare(void*, const void* lhs, const void* rhs)
		{
			return strcmp(CAST_PTR(const s_import_class*,lhs)->name, CAST_PTR(const s_import_class*,rhs)->name);
		}
		int __cdecl s_import_class_search_by_name(void*, const void* key, const void* element)
		{
			return strcmp(CAST_PTR(const char*,key), CAST_PTR(const s_import_class*,element)->name);
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

			// Modify build-cache-file to use our own implementation
			{
				s_import_class* bcf_definition = CAST_PTR(s_import_class*, 
					bsearch_s("build-cache-file", import_classes, NUMBEROF(import_classes), sizeof(s_import_class), s_import_class_search_by_name, NULL));

				if(bcf_definition != NULL)
					bcf_definition->import_proc = build_cache_file_for_scenario_stock_override;
				else
					YELO_WARN("CheApe: failed to override build-cache-file :s");
			}
		}

//////////////////////////////////////////////////////////////////////////

		void IntializeBeforeTagGroupsInitalize()
		{
		}

		// patch the shader-type-names list with the shader types gbx added
		static void FixupShaderTypeNamesList()
		{
			static cstring const k_fixed_shader_type_names[] = {
				"screen",
				"effect",
				"decal",

				"environment",
				"model",
				"transparent generic",
				"transparent chicago",			// gbx forgot to add this one
				"transparent chicago extended",	// gbx forgot to add this one
				"transparent water",
				"transparent glass",
				"transparent meter",
				"transparent plasma",
			};

			*CAST_PTR(cstring const**, 0x44F2B0+3) = k_fixed_shader_type_names;
		}
		void Initialize()
		{
			c_animation_fixups::Initialize();

			Yelo::DataFiles::SharedInitialize();
			ImportClassesInitialize();
			FixupShaderTypeNamesList();
		}

		void Dispose()
		{
			Yelo::DataFiles::SharedDispose();

			c_animation_fixups::Dispose();
		}
	};
};

#endif