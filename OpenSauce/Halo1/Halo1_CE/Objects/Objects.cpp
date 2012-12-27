/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Objects.hpp"

#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/models/collision_model_definitions.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>
#include <blamlib/Halo1/objects/damage.hpp>

#include "TagGroups/project_yellow_definitions.hpp"

#include "Objects/ObjectFieldDefinitions.hpp"
#include "Objects/Equipment.hpp"
#include "Objects/Units.hpp"

#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Game/GameStateRuntimeData.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/Networking.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_OBJECTS
#include "Memory/_EngineLayout.inl"

		s_object_type_definition** ObjectTypeDefinitions()							PTR_IMP_GET(object_type_definitions);
		s_widget_type_definition*  WidgetTypeDefinitions()							PTR_IMP_GET(widget_type_definitions);

		t_widget_data* Widgets()													DPTR_IMP_GET(widgets);
		t_flag_data* Flags()														DPTR_IMP_GET(flags);
		t_antenna_data* Antennas()													DPTR_IMP_GET(antennas);
		t_glow_data* Glow()															DPTR_IMP_GET(glow);
		t_glow_particles_data* GlowParticles()										DPTR_IMP_GET(glow_particles);
		t_light_volumes_data* LightVolumes()										DPTR_IMP_GET(light_volumes);
		t_lightnings_data* Lightnings()												DPTR_IMP_GET(lightnings);


		Render::cached_object_render_states_data_t* CachedObjectRenderStates()		DPTR_IMP_GET(cached_object_render_states);
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


#include <YeloLib/Halo1/render/render_objects_upgrades.inl>
#include "Objects/Objects.Damage.inl"
#include "Objects/Objects.Scripting.inl"
#include "Objects/Objects.UnitInfections.inl"

		void InitializeScripting()
		{
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_objects_distance_to_object, 
				scripting_objects_distance_to_object_evaluate);

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_get_real, 
				scripting_object_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_set_real, 
				scripting_object_data_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_set_vector, 
				scripting_object_data_set_vector_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_object_data_save_vector, 
				scripting_object_data_save_vector_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_get_real, 
				scripting_weapon_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_set_real, 
				scripting_weapon_data_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_trigger_set_real, 
				scripting_weapon_data_trigger_set_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_object, 
				scripting_unit_data_get_object_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_integer, 
				scripting_unit_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_set_integer, 
				scripting_unit_data_set_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_get_real, 
				scripting_unit_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_unit_data_set_real, 
				scripting_unit_data_set_real_evaluate);
		}
		void Initialize()
		{
#if !PLATFORM_DISABLE_UNUSED_CODE
			Memory::WriteRelativeCall(&Objects::Update, GET_FUNC_VPTR(OBJECTS_UPDATE_HOOK), false);
#endif

			render_objects_mods::Initialize();

			UnitInfections::Initialize();
			Vehicle::Initialize();
			Weapon::Initialize();
			Equipment::Initialize();
			Units::Initialize();

			InitializeScripting();
			InitializeObjectFieldDefinitions();
		}

		void Dispose()
		{
			Units::Dispose();

			Weapon::Dispose();
			Vehicle::Dispose();
			UnitInfections::Dispose();
		}

		static void UseBipedJumpPenalty(bool use_fix)
		{
			const size_t k_game_globals_player_stun_offset = 
				FIELD_OFFSET(TagGroups::s_game_globals_player_information, stun);
			const size_t k_game_globals_player_stun_turning_penalty_offset = 
				k_game_globals_player_stun_offset + 
				FIELD_OFFSET(TagGroups::s_game_globals_player_information::_stun, turning_penalty);
			const size_t k_game_globals_player_stun_jumping_penalty_offset = 
				k_game_globals_player_stun_offset + 
				FIELD_OFFSET(TagGroups::s_game_globals_player_information::_stun, jumping_penalty);

			BOOST_STATIC_ASSERT( k_game_globals_player_stun_turning_penalty_offset == 0x84 );
			BOOST_STATIC_ASSERT( k_game_globals_player_stun_jumping_penalty_offset == 0x88 );
			*CAST_PTR(size_t*, GET_FUNC_VPTR(BIPED_JUMP_MOD_STUN_PENALTY_FIELD_REF)) = 
				use_fix ? k_game_globals_player_stun_jumping_penalty_offset : 
					k_game_globals_player_stun_turning_penalty_offset; // stock code uses turning penalty for whatever reason
		}
		void InitializeForNewMap()
		{
			bool mtv_disabled = TagGroups::_global_yelo->gameplay.flags.prohibit_multiteam_vehicles_bit;
			MultiTeamVehiclesSet(!mtv_disabled);

			bool use_jump_penalty_fix = TagGroups::_global_yelo_globals->flags.force_game_to_use_stun_jumping_penalty_bit;
			UseBipedJumpPenalty(use_jump_penalty_fix);

			Units::InitializeForNewMap();
		}

		void DisposeFromOldMap()
		{
			Units::DisposeFromOldMap();
		}

		void PLATFORM_API Update()
		{
			static uint32 OBJECTS_GARBAGE_COLLECTION = GET_FUNC_PTR(OBJECTS_GARBAGE_COLLECTION);
			_asm {
				call	OBJECTS_GARBAGE_COLLECTION
			}

			// Do custom code here:
		}

		void InitializeForYeloGameState(bool enabled)
		{
			size_t* omp_allocation_size = GET_PTR2(object_memory_pool_allocation_size);
//			*omp_allocation_size = Enums::k_object_memory_pool_allocation_size;
//			if(enabled) *omp_allocation_size += Enums::k_game_state_allocation_size_object_memory_pool_upgrade;

			Units::InitializeForYeloGameState(enabled);
		}


		void LoadSettings(TiXmlElement* objects_element)
		{
			// TODO: store this setting somewhere!
			if(objects_element != NULL && Settings::ParseBoolean(objects_element->Attribute("disableVehicleRemapper")))
				VehicleRemapperEnable(false);

#if !PLATFORM_IS_DEDI
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
#endif
		}

		void SaveSettings(TiXmlElement* objects_element)
		{
			// TODO:
			if(false)
				objects_element->SetAttribute("disableVehicleRemapper", 
					Settings::BooleanToString(true));

#if !PLATFORM_IS_DEDI
			TiXmlElement* weapons_element = NULL,
						* vehicles_element = NULL
				;

			weapons_element = new TiXmlElement("weaponViews");
				objects_element->LinkEndChild(weapons_element);
			vehicles_element = new TiXmlElement("vehicleViews");
				objects_element->LinkEndChild(vehicles_element);

			Weapon::SaveSettings(weapons_element);
			Vehicle::SaveSettings(vehicles_element);
#endif
		}

		void MultiTeamVehiclesSet(bool enabled)
		{
			// jmp
			static const byte k_enable_code[] = {0xEB};
			// jz
			static const byte k_disable_code[] = {0x74};

			Memory::WriteMemory(GET_FUNC_VPTR(UNIT_CAN_ENTER_SEAT_MOD), (enabled ? k_enable_code : k_disable_code), sizeof(k_enable_code));
		}
		void VehicleRemapperEnable(bool enabled)
		{
			// jnz eip+2+10
			static const byte k_enable_code[] = {0x75, 0x0A};
			// nop, nop
			static const byte k_disable_code[] = {Enums::_x86_opcode_nop, Enums::_x86_opcode_nop};

			Memory::WriteMemory(GET_FUNC_VPTR(OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP), (enabled ? k_enable_code : k_disable_code), sizeof(k_enable_code));
		}


		void PlacementDataNewAndCopy(s_object_placement_data& data, datum_index src_object_index, 
			datum_index tag_index_override, datum_index owner_object_index)
		{
			s_object_data* src_object = (*Objects::ObjectHeader())[src_object_index]->_object;

			if(tag_index_override.IsNull())
				tag_index_override = src_object->definition_index;

			Engine::Objects::PlacementDataNew(data, tag_index_override, owner_object_index);
			src_object->network.CopyToPlacementData(data);
		}

		datum_index GetUltimateObject(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
				s_object_data* current_obj = object_header_datums[object_index.index]._object;

				datum_index parent_index;
				while( !(parent_index = current_obj->parent_object_index).IsNull() )
				{
					current_obj = object_header_datums[parent_index.index]._object;
				}
			}

			return object_index;
		}

		datum_index GetNextObjectN(datum_index object_index, int32 n)
		{
			if(!object_index.IsNull())
			{
				s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
				s_object_data* current_obj = object_header_datums[object_index.index]._object;

				do
				{
					object_index = current_obj->next_object_index;
					current_obj = object_header_datums[object_index.index]._object;
				}while(n--);
			}

			return object_index;
		}

		TagGroups::s_object_definition const* GetObjectDefinition(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_data* object = (*Objects::ObjectHeader())[object_index]->_object;

				return TagGroups::TagGet<TagGroups::s_object_definition>(object->definition_index);
			}

			return NULL;
		}

		TagGroups::model_animation_graph const* GetObjectAnimations(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_data* object = (*Objects::ObjectHeader())[object_index]->_object;
				datum_index tag_index = object->animation.definition_index;

				return TagGroups::TagGet<TagGroups::model_animation_graph>(tag_index);
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

		typedef void (API_FUNC* proc_object_action_perfomer)(datum_index object_index);
		static void PerformActionOnChildrenByType(datum_index parent, long_flags object_type_mask,
			proc_object_action_perfomer action_performer)
		{
			s_object_header_datum* object_header_datums = *Objects::ObjectHeader();
			s_object_data* parent_object = object_header_datums[parent.index]._object;
			s_object_data* child_object;

			for(datum_index child_index = parent_object->first_object_index;
				!child_index.IsNull();
				child_index = child_object->next_object_index)
			{
				child_object = object_header_datums[child_index.index]._object;

				if(child_object->VerifyType(object_type_mask))
					action_performer(child_index);
			}
		}
		void DeleteChildrenByType(datum_index parent, long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, Engine::Objects::Delete);
		}
		void DetachChildrenByType(datum_index parent, long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, Engine::Objects::Detach);
		}

		size_t PredictMemoryPoolUsage(Enums::object_type type, int32 node_count, bool include_yelo_upgrades)
		{
			size_t total_headers_size = sizeof(Memory::s_memory_pool_block);
			size_t total_node_memory_size = (sizeof(real_orientation3d) * 2) + sizeof(real_matrix4x3);
			total_node_memory_size *= node_count;

			size_t object_type_size = 0;
			switch(type)
			{
			case Enums::_object_type_biped: object_type_size = Enums::k_object_size_biped;
			case Enums::_object_type_vehicle: object_type_size = Enums::k_object_size_vehicle;

			case Enums::_object_type_weapon: object_type_size = Enums::k_object_size_weapon;
			case Enums::_object_type_equipment: object_type_size = Enums::k_object_size_equipment;
			case Enums::_object_type_garbage: object_type_size = Enums::k_object_size_garbage;

			case Enums::_object_type_projectile: object_type_size = Enums::k_object_size_projectile;
			case Enums::_object_type_scenery: object_type_size = Enums::k_object_size_scenery;

			case Enums::_object_type_machine: object_type_size = Enums::k_object_size_machine;
			case Enums::_object_type_control: object_type_size = Enums::k_object_size_control;
			case Enums::_object_type_light_fixture: object_type_size = Enums::k_object_size_light_fixture;

			case Enums::_object_type_placeholder: object_type_size = Enums::k_object_size_placeholder;
			case Enums::_object_type_sound_scenery: object_type_size = Enums::k_object_size_sound_scenery;

			default: return 0;
			}

			return object_type_size + total_node_memory_size + total_headers_size;
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