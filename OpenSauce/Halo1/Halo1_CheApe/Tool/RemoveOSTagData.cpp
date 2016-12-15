/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/RemoveOSTagData.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/units/vehicle_definitions.hpp>
#include <blamlib/Halo1/units/biped_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/effects/contrail_definitions.hpp>
#include <blamlib/Halo1/effects/particle_definitions.hpp>
#include <blamlib/Halo1/effects/particle_system_definitions.hpp>
#include <blamlib/Halo1/effects/weather_particle_system_definitions.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

#include <YeloLib/cseries/value_conversion.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

#include "Tool/Console.hpp"
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Tool
	{
		static std::vector<std::string> g_warning_strings;

		static void AddWarning(cstring warning, const datum_index tag_index)
		{
			std::string warning_string(warning);
			
			TagGroups::group_tag_to_string tag_group_string;
			tag_group_string.group = blam::tag_get_group_tag(tag_index);

			warning_string.append(" [");
			warning_string.append(blam::tag_get_name(tag_index));
			warning_string.append(" (");
			warning_string.append(tag_group_string.Terminate().TagSwap().str);
			warning_string.append(")]");

			g_warning_strings.push_back(warning_string);
		}
	};
};

#include "Tool/RemoveOSTagData.inl"

namespace Yelo
{
	namespace Tool
	{
		struct s_tag_loading_type
		{
			cstring m_tag_type;
			datum_index (*m_tag_load)(cstring, long_flags);
		};

		static s_tag_loading_type g_data_loading_types[] = 
		{
			{ "scenario",					[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::scenario>(location, flags); } },
			{ "unit",						[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_unit_definition>(location, flags); } },
			{ "biped",						[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_biped_definition>(location, flags); } },
			{ "vehicle",					[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_vehicle_definition>(location, flags); } },
			{ "shader_model",				[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_shader_model_definition>(location, flags); } },
			{ "shader_environment",			[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_shader_environment_definition>(location, flags); } },
			{ "contrail",					[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_contrail_definition>(location, flags); } },
			{ "particle",					[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_particle_definition>(location, flags); } },
			{ "particle_system",			[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_particle_system_definition>(location, flags); } },
			{ "weather_particle_system",	[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::s_weather_particle_system_definition>(location, flags); } },
			{ "model_animations",			[](cstring location, long_flags flags) { return blam::tag_load<TagGroups::model_animation_graph>(location, flags); } }
		};

		static std::map<const tag, void (*)(const datum_index)> g_data_removal_functions =
		{
			{ TagGroups::scenario::k_group_tag, &RemoveOpenSauceData_Scenario },
			{ TagGroups::s_unit_definition::k_group_tag, &RemoveOpenSauceData_Unit },
			{ TagGroups::s_vehicle_definition::k_group_tag, &RemoveOpenSauceData_Vehicle },
			{ TagGroups::s_biped_definition::k_group_tag, &RemoveOpenSauceData_Biped },
			{ TagGroups::s_shader_model_definition::k_group_tag, &RemoveOpenSauceData_ShaderModel },
			{ TagGroups::s_shader_environment_definition::k_group_tag, &RemoveOpenSauceData_ShaderEnvironment },
			{ TagGroups::s_contrail_definition::k_group_tag, &RemoveOpenSauceData_Contrail },
			{ TagGroups::s_particle_definition::k_group_tag, &RemoveOpenSauceData_Particle },
			{ TagGroups::s_particle_system_definition::k_group_tag, &RemoveOpenSauceData_ParticleSystem },
			{ TagGroups::s_weather_particle_system_definition::k_group_tag, &RemoveOpenSauceData_WeatherParticleSystem },
			{ TagGroups::model_animation_graph::k_group_tag, &RemoveOpenSauceData_ModelAnimations }
		};

		static bool DisplayDestructiveWarning()
		{
			puts("");
			Console::ColorPrint(Enums::_console_color_lightyellow, "WARNING: This action is destructive! Your OpenSauce tag data will be lost.", true);

			return Console::EnterCommand("y;n", nullptr, "do you want to continue (y/n)?") == 0;
		}

		void PLATFORM_API remove_os_tag_data(char* arguments[])
		{
			struct s_arguments {
				cstring tag_name;
				cstring tag_type;
				cstring	recursive;
			}* args = CAST_PTR(s_arguments*, arguments);

			bool do_recursive;
			if(!ValueConversion::FromString(args->recursive, do_recursive))
			{
				Console::ColorPrintF(Enums::_console_color_white, "Failed to parse %s as a boolean value", args->recursive);
				Console::PrintNewLine();
				return;
			}

			// Check the tag type is valid
			auto found_type = std::find_if(std::begin(g_data_loading_types), std::end(g_data_loading_types),
				[&](const s_tag_loading_type& entry)
				{
					return strcmp(entry.m_tag_type, args->tag_type) == 0;
				}
			);

			if(found_type == std::end(g_data_loading_types))
			{
				Console::ColorPrintF(Enums::_console_color_lightyellow, "Unknown tag type, or tag contains no OpenSauce tag data: %s", args->tag_type);
				Console::PrintNewLine();
				return;
			}
			auto& removal_type = *found_type;

			// Confirm intention
			if(!DisplayDestructiveWarning())
			{
				Console::ColorPrint(Enums::_console_color_lightyellow, "Action cancelled", true);
				return;
			}

			// Load tag with resolved references or not
			Console::ColorPrint(Enums::_console_color_white, "Loading tags...");
			auto base_tag_index = removal_type.m_tag_load(args->tag_name, do_recursive ? 0 : FLAG(Flags::_tag_load_non_resolving_references_bit));
			if(base_tag_index.IsNull())
			{
				Console::PrintNewLine();
				Console::ColorPrintF(Enums::_console_color_red, "ERROR: failed to open tag for editing: %s", args->tag_name);
				Console::PrintNewLine();
				return;
			}
			Console::ColorPrint(Enums::_console_color_lightgreen, "done", true);
			Console::PrintNewLine();

			// Iterate loaded tags
			auto& tag_iterator = TagGroups::c_tag_iterator::all();
			for(auto tag_index : tag_iterator)
			{
				// Remove tag data and save
				auto tag_group = blam::tag_get_group_tag(tag_index);

				auto found_entry = g_data_removal_functions.find(tag_group);
				if(found_entry == g_data_removal_functions.end())
				{
					continue;
				}

				TagGroups::group_tag_to_string tag_group_string;
				tag_group_string.group = tag_group;
				Console::ColorPrintF(Enums::_console_color_white, "Removing data from %s (%s)...", blam::tag_get_name(tag_index), tag_group_string.Terminate().TagSwap().str);

				found_entry->second(tag_index);
				blam::tag_save(tag_index);

				Console::ColorPrint(Enums::_console_color_lightgreen, "done", true);
			}

			// Print any warnings that were raised
			if(g_warning_strings.size())
			{
				Console::PrintNewLine();
				Console::ColorPrintF(Enums::_console_color_lightyellow, "%i warnings have been raised:", g_warning_strings.size());
				Console::PrintNewLine();

				for(auto& warning : g_warning_strings)
				{
					Console::ColorPrint(Enums::_console_color_white, warning.c_str(), true);
				}
			}

			blam::tag_unload(base_tag_index);
		}
	};
};
#endif