/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

#include <blamlib/Halo1/hs/hs_structures.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			// stock size
			k_total_scenario_hs_syntax_data = sizeof(Memory::s_data_array)  + 
				(sizeof(Scripting::hs_syntax_node) * Enums::k_maximum_hs_syntax_nodes_per_scenario),

			k_total_scenario_hs_syntax_data_upgrade = sizeof(Memory::s_data_array)  + 
				(sizeof(Scripting::hs_syntax_node) * Enums::k_maximum_hs_syntax_nodes_per_scenario_upgrade),
		};
	};

	namespace Scripting
	{
		const cstring k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";

		size_t GetTotalScenarioHsSyntaxData()		{ return Enums::k_total_scenario_hs_syntax_data; }
		size_t GetTotalScenarioHsSyntaxDataUpgrade(){ return Enums::k_total_scenario_hs_syntax_data_upgrade; }

		void UpdateTypeHolderFromPtrToData(TypeHolder& data, const Enums::hs_type type)
		{
			if(data.pointer != nullptr)
			{
				TypeHolder copy; copy.pointer = data.pointer;
				data.pointer = nullptr;
				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	data.byte = *copy.ptr.byte;		break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(script):
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(object_name):
				case HS_TYPE(short):data.uint16 = *copy.ptr.uint16;	break;
				case HS_TYPE(real):
				case HS_TYPE(ai):
				//////////////////////////////////////////////////////////////////////////
				// datum_index based types
				case HS_TYPE(object_list):
				case HS_TYPE(sound):
				case HS_TYPE(effect):
				case HS_TYPE(damage):
				case HS_TYPE(looping_sound):
				case HS_TYPE(animation_graph):
				case HS_TYPE(actor_variant):
				case HS_TYPE(damage_effect):
				case HS_TYPE(object_definition):
				case HS_TYPE(object):
				case HS_TYPE(unit):
				case HS_TYPE(vehicle):
				case HS_TYPE(weapon):
				case HS_TYPE(device):
				case HS_TYPE(scenery):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(long):	data.uint32 = *copy.ptr.uint32;	break;

				//////////////////////////////////////////////////////////////////////////
//				case HS_TYPE(string): data.ptr.ascii = copy.ptr.ascii; break;
				}
			}
			else // the pointer is null, so default to NONE
			{
				data.pointer = nullptr;

				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	data.byte = NONE;	break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(script):
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(object_name):
				case HS_TYPE(short):data.int16 = NONE;	break;
				case HS_TYPE(real): data.real = -1.0f;	break;
				case HS_TYPE(ai):
				//////////////////////////////////////////////////////////////////////////
				// datum_index based types
				case HS_TYPE(object_list):
				case HS_TYPE(sound):
				case HS_TYPE(effect):
				case HS_TYPE(damage):
				case HS_TYPE(looping_sound):
				case HS_TYPE(animation_graph):
				case HS_TYPE(actor_variant):
				case HS_TYPE(damage_effect):
				case HS_TYPE(object_definition):
				case HS_TYPE(object):
				case HS_TYPE(unit):
				case HS_TYPE(vehicle):
				case HS_TYPE(weapon):
				case HS_TYPE(device):
				case HS_TYPE(scenery):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(long):	data.int32 = NONE;	break;

				//////////////////////////////////////////////////////////////////////////
//				case HS_TYPE(string): data.ptr.ascii = nullptr; break;
				}
			}
		}
		void UpdateTypeHolderDataFromPtr(const TypeHolder& data, const Enums::hs_type type, void* ptr)
		{
			if(data.pointer != nullptr && ptr != nullptr)
			{
				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	*data.ptr.byte = *CAST_PTR(byte*, ptr);		break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(script):
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(object_name):
				case HS_TYPE(short):*data.ptr.uint16 = *CAST_PTR(uint16*, ptr);	break;
				case HS_TYPE(real):
				case HS_TYPE(ai):
				//////////////////////////////////////////////////////////////////////////
				// datum_index based types
				case HS_TYPE(object_list):
				case HS_TYPE(sound):
				case HS_TYPE(effect):
				case HS_TYPE(damage):
				case HS_TYPE(looping_sound):
				case HS_TYPE(animation_graph):
				case HS_TYPE(actor_variant):
				case HS_TYPE(damage_effect):
				case HS_TYPE(object_definition):
				case HS_TYPE(object):
				case HS_TYPE(unit):
				case HS_TYPE(vehicle):
				case HS_TYPE(weapon):
				case HS_TYPE(device):
				case HS_TYPE(scenery):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(long):	*data.ptr.uint32 = *CAST_PTR(uint32*, ptr);	break;

				//////////////////////////////////////////////////////////////////////////
//				case HS_TYPE(string): *data.ptr.ascii = CAST_PTR(cstring, ptr); break;
				}
			}
		}
	};
};