/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/hs/hs_yelo.hpp>

namespace Yelo
{
	namespace Scripting
	{
		const cstring k_external_global_opensauce_override_name = "ai_debug_path_maximum_radius";

		void UpdateTypeHolderFromPtrToData(TypeHolder& data, const Enums::hs_type type)
		{
			if(data.pointer != NULL)
			{
				TypeHolder copy; copy.pointer = data.pointer;
				data.pointer = NULL;
				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	data.byte = *copy.ptr.byte;		break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				//case HS_TYPE(ai):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(short):data.uint16 = *copy.ptr.uint16;	break;
				case HS_TYPE(real):
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
					break;
				}
			}
			else // the pointer is null, so default to NONE
			{
				data.pointer = NULL;

				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	data.byte = NONE;	break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				//case HS_TYPE(ai):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(short):data.int16 = NONE;	break;
				case HS_TYPE(real): data.real = -1.0f;	break;
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
					break;
				}
			}
		}
		void UpdateTypeHolderDataFromPtr(const TypeHolder& data, const Enums::hs_type type, void* ptr)
		{
			if(data.pointer != NULL && ptr != NULL)
			{
				switch(type)
				{
				//case HS_TYPE(byte):
				case HS_TYPE(bool):	*data.ptr.byte = *CAST_PTR(byte*, ptr);		break;
				//////////////////////////////////////////////////////////////////////////
				// block index based types
				case HS_TYPE(trigger_volume):
				case HS_TYPE(cutscene_flag):
				case HS_TYPE(cutscene_camera_point):
				case HS_TYPE(cutscene_title):
				case HS_TYPE(cutscene_recording):
				case HS_TYPE(device_group):
				//case HS_TYPE(ai):
				case HS_TYPE(ai_command_list):
				case HS_TYPE(starting_profile):
				case HS_TYPE(conversation):
				//////////////////////////////////////////////////////////////////////////
				case HS_TYPE(short):*data.ptr.uint16 = *CAST_PTR(uint16*, ptr);	break;
				case HS_TYPE(real):
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
					break;
				}
			}
		}
	};
};