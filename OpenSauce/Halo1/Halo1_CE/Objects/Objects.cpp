/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#include "Objects/Objects.hpp"

#include <TagGroups/Halo1/object_definitions.hpp>
#include <TagGroups/Halo1/device_definitions.hpp>
#include <TagGroups/Halo1/item_definitions.hpp>
#include <TagGroups/Halo1/unit_definitions.hpp>

#include <TagGroups/Halo1/model_definitions.hpp>
#include <TagGroups/Halo1/damage_effect_definitions.hpp>
#include "TagGroups/project_yellow_definitions.hpp"

#include "Game/EngineFunctions.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Memory/MemoryInterface.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_OBJECTS
#include "Memory/_EngineLayout.inl"

		t_widget_data* Widgets()													DPTR_IMP_GET(widgets);
		t_flag_data* Flags()														DPTR_IMP_GET(flags);
		t_antenna_data* Antennas()													DPTR_IMP_GET(antennas);
		t_glow_data* Glow()															DPTR_IMP_GET(glow);
		t_glow_particles_data* GlowParticles()										DPTR_IMP_GET(glow_particles);
		t_light_volumes_data* LightVolumes()										DPTR_IMP_GET(light_volumes);
		t_lightnings_data* Lightnings()												DPTR_IMP_GET(lightnings);


		t_cached_object_render_states_data* CachedObjectRenderStates()				DPTR_IMP_GET(cached_object_render_states);
		s_unit_globals_data* UnitGlobals()											DPTR_IMP_GET(unit_globals);
		t_device_groups_data* DeviceGroups()										DPTR_IMP_GET(device_groups);
		t_object_header_data* ObjectHeader()										DPTR_IMP_GET(object_header);
		s_objects_pool_data* ObjectsPool()											DPTR_IMP_GET(objects_pool);
		s_object_globals_data* ObjectGlobals()										DPTR_IMP_GET(object_globals);
		s_object_name_list_data* ObjectNameList()									DPTR_IMP_GET(object_name_list);


		collideable_object_data* CollideableObject()								DPTR_IMP_GET(collideable_object);
		t_cluster_collideable_object_reference_data* ClusterCollideableObjectReference()		DPTR_IMP_GET(cluster_collideable_object_reference);
		t_collideable_object_cluster_reference_data* CollideableObjectClusterReference()		DPTR_IMP_GET(collideable_object_cluster_reference);

		noncollideable_object_data* NoncollideableObject()							DPTR_IMP_GET(noncollideable_object);
		t_cluster_noncollideable_object_reference_data* ClusterNoncollideableObjectReference()	DPTR_IMP_GET(cluster_noncollideable_object_reference);
		t_noncollideable_object_cluster_reference_data* NoncollideableObjectClusterReference()	DPTR_IMP_GET(noncollideable_object_cluster_reference);


#include "Objects/Objects.Damage.inl"
#include "Objects/Objects.Scripting.inl"
		void InitializeScripting()
		{
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_objects_distance_to_object, 
				scripting_objects_distance_to_object_evaluate);

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_get_real, 
				scripting_object_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_set_real, 
				scripting_object_data_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_get_real, 
				scripting_weapon_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_set_real, 
				scripting_weapon_data_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_trigger_set_real, 
				scripting_weapon_data_trigger_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_object, 
				scripting_unit_data_get_object_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_integer, 
				scripting_unit_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_set_integer, 
				scripting_unit_data_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_real, 
				scripting_unit_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_set_real, 
				scripting_unit_data_set_integer_evaluate);
		}
		void Initialize()
		{
			//Memory::WriteRelativeCall(&Objects::Update, GET_FUNC_VPTR(OBJECTS_UPDATE_HOOK), false);

			ToggleMultiTeamVehicles();

			Weapon::Initialize();
			Vehicle::Initialize();

			InitializeScripting();
		}

		void Dispose()
		{
			Vehicle::Dispose();
			Weapon::Dispose();
		}

		void PLATFORM_API Update()
		{
			static uint32 OBJECTS_GARBAGE_COLLECTION = GET_FUNC_PTR(OBJECTS_GARBAGE_COLLECTION);
			_asm {
				call	OBJECTS_GARBAGE_COLLECTION
			}

			// Do custom code here:
		}

#if !PLATFORM_IS_DEDI
		void LoadSettings(TiXmlElement* objects_element)
		{
			TiXmlElement* weapons_element = NULL,
						* vehicles_element = NULL
				;

			if(objects_element != NULL)
			{
				weapons_element = objects_element->FirstChildElement("weaponViews");
				vehicles_element = objects_element->FirstChildElement("vehicleViews");
			}

			Weapon::LoadSettings(weapons_element);
			Vehicle::LoadSettings(vehicles_element);
		}

		void SaveSettings(TiXmlElement* objects_element)
		{
			TiXmlElement* weapons_element = NULL,
						* vehicles_element = NULL
				;

			weapons_element = new TiXmlElement("weaponViews");
				objects_element->LinkEndChild(weapons_element);
			vehicles_element = new TiXmlElement("vehicleViews");
				objects_element->LinkEndChild(vehicles_element);

			Weapon::SaveSettings(weapons_element);
			Vehicle::SaveSettings(vehicles_element);
		}
#endif

		bool ToggleMultiTeamVehicles()
		{
			// mov bl, 1
			// jmp
			static byte enable_code[] = {0xB3, 0x01, 0xEB};
			// test al, al
			// jz
			static byte disable_code[] = {0x84, 0xC0, 0x74};

			static bool enabled = false;

			Memory::WriteMemory(GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_MOD), (enabled ? disable_code : enable_code), sizeof(enable_code));
			return enabled = !enabled;
		}

		datum_index GetUltimateObject(datum_index obj)
		{
			if(!obj.IsNull())
			{
				s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
				s_object_data* current_obj = object_header_datums[obj.index]._object;

				datum_index parent_index;
				while( !(parent_index = *current_obj->GetParentObjectIndex()).IsNull() )
				{
					current_obj = object_header_datums[parent_index.index]._object;
				}
			}

			return obj;
		}

		datum_index GetNextObjectN(datum_index obj, int16 n)
		{
			if(!obj.IsNull())
			{
				s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
				s_object_data* current_obj = object_header_datums[obj.index]._object;

				do
				{
					obj = *current_obj->GetNextObjectIndex();
					current_obj = object_header_datums[obj.index]._object;
				}while(n--);
			}

			return obj;
		}

		s_cache_tag_instance const* GetObjectDefinition(datum_index obj)
		{
			if(!obj.IsNull())
			{
				s_object_data* obj_data = (*Objects::ObjectHeader())[obj]->_object;

				return &TagGroups::Instances()[ obj_data->GetTagDefinition()->index ];
			}

			return NULL;
		}

		real GetObjectDistanceFromPoint(datum_index obj, const real_vector3d& dest_point)
		{
			real dist = -1.0f;

			if(!obj.IsNull())
			{
				real_vector3d object_origin;
				Engine::Objects::GetOrigin(obj, CAST_PTR(real_point3d*, &object_origin));

				real_vector3d relative_pos = object_origin - dest_point;
				dist = relative_pos.Magnitude();
			}

			return dist;
		}
	};
};

#include "Common/YeloSettings.hpp"
#include "Game/Players.hpp"
#include "Interface/Controls.hpp"
#include "Interface/GameUI.hpp"
#include "Interface/TextBlock.hpp"

#include "Objects/Objects.Weapon.inl"
#include "Objects/Objects.Vehicle.inl"