/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Objects.hpp"

#include <blamlib/Halo1/ai/actors.hpp>
#include <blamlib/Halo1/ai/ai_script.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/hs/hs_library_external.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/models/collision_model_definitions.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>
#include <blamlib/Halo1/objects/damage.hpp>

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "TagGroups/project_yellow_definitions.hpp"

#include "Objects/ObjectFieldDefinitions.hpp"
#include "Objects/Equipment.hpp"
#include "Objects/Units.hpp"

#include "Game/Camera.hpp"
#include "Game/GameState.hpp"
#include "Game/GameStateRuntimeData.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/Networking.hpp"
#include "Scenario/Scenario.hpp"
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

		widget_data_t& Widgets()													DPTR_IMP_GET_BYREF(widgets);
		flag_data_t& Flags()														DPTR_IMP_GET_BYREF(flags);
		antenna_data_t& Antennas()													DPTR_IMP_GET_BYREF(antennas);
		glow_data_t& Glow()															DPTR_IMP_GET_BYREF(glow);
		glow_particles_data_t& GlowParticles()										DPTR_IMP_GET_BYREF(glow_particles);
		light_volumes_data_t& LightVolumes()										DPTR_IMP_GET_BYREF(light_volumes);
		lightnings_data_t& Lightnings()												DPTR_IMP_GET_BYREF(lightnings);


		Render::cached_object_render_states_data_t* CachedObjectRenderStates()		DPTR_IMP_GET(cached_object_render_states);
		s_unit_globals_data* UnitGlobals()											DPTR_IMP_GET(unit_globals);
		device_groups_data_t& DeviceGroups()										DPTR_IMP_GET_BYREF(device_groups);
		object_header_data_t& ObjectHeader()										DPTR_IMP_GET_BYREF(object_header);
		s_objects_pool_data* ObjectsPool()											DPTR_IMP_GET(objects_pool);
		s_object_globals_data* ObjectGlobals()										DPTR_IMP_GET(object_globals);
		s_object_name_list_data* ObjectNameList()									DPTR_IMP_GET(object_name_list);


		collideable_object_data* CollideableObject()								DPTR_IMP_GET(collideable_object);
		cluster_collideable_object_reference_data_t& ClusterCollideableObjectReference()		DPTR_IMP_GET_BYREF(cluster_collideable_object_reference);
		collideable_object_cluster_reference_data_t& CollideableObjectClusterReference()		DPTR_IMP_GET_BYREF(collideable_object_cluster_reference);

		noncollideable_object_data* NoncollideableObject()							DPTR_IMP_GET(noncollideable_object);
		cluster_noncollideable_object_reference_data_t& ClusterNoncollideableObjectReference()	DPTR_IMP_GET_BYREF(cluster_noncollideable_object_reference);
		noncollideable_object_cluster_reference_data_t& NoncollideableObjectClusterReference()	DPTR_IMP_GET_BYREF(noncollideable_object_cluster_reference);


		static struct s_object_yelo_globals
		{
			bool vehicle_remapper_disabled;
			PAD24;
		}g_object_yelo_globals;
#include <YeloLib/Halo1/render/render_objects_upgrades.inl>
#include "Objects/Objects.Damage.inl"
#include "Objects/Objects.Scripting.inl"
#include "Objects/Objects.UnitInfections.inl"

		static void InitializeScripting()
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
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_magazine_get_integer, 
				scripting_weapon_data_magazine_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_weapon_data_magazine_set_integer, 
				scripting_weapon_data_magazine_set_integer_evaluate);
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

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_vehicle_remapper_enabled, 
				scripting_vehicle_remapper_enabled_evaluate);
		}
		void Initialize()
		{
			Memory::WriteRelativeJmp(&Objects::Update, GET_FUNC_VPTR(OBJECTS_UPDATE_HOOK), false);

#if PLATFORM_IS_USER
			if(!CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
				render_objects_mods::Initialize();
#endif

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

		static void ObjectsUpdateIgnorePlayerPvs(bool use_fix)
		{
#if PLATFORM_IS_USER
			typedef Memory::s_memory_change_data<GET_FUNC_VPTR(OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP1), 1+1+sizeof(uintptr_t)> 
				nop1_t;
			typedef Memory::s_memory_change_data<GET_FUNC_VPTR(OBJECTS_UPDATE__OBJECT_IN_PLAYER_PVS_NOP2), 2> 
				nop2_t;

			if(use_fix)
			{
				nop1_t::MemoryApplyNopCodes();
				nop2_t::MemoryApplyNopCodes();
			}
			else
			{
				nop1_t::MemoryApplyUndo();
				nop2_t::MemoryApplyUndo();
			}
#endif
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
			bool objects_update_ignore_player_pvs = Scenario::Scenario()->type == Enums::_scenario_type_main_menu && 
				TagGroups::_global_yelo->flags.game_updates_ignore_player_pvs_hack_bit;
			ObjectsUpdateIgnorePlayerPvs(objects_update_ignore_player_pvs);

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
			static const uintptr_t OBJECTS_GARBAGE_COLLECTION = GET_FUNC_PTR(OBJECTS_GARBAGE_COLLECTION);
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
			if(objects_element != nullptr)
			{
				if(g_object_yelo_globals.vehicle_remapper_disabled = Settings::ParseBoolean(objects_element->Attribute("disableVehicleRemapper")))
					VehicleRemapperEnable(false);
			}

#if !PLATFORM_IS_DEDI
			TiXmlElement* weapons_element = nullptr,
						* vehicles_element = nullptr
				;

			if(objects_element != nullptr)
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
			objects_element->SetAttribute("disableVehicleRemapper", 
				BooleanToString(VehicleRemapperEnabled()==false));

#if !PLATFORM_IS_DEDI
			TiXmlElement* weapons_element = nullptr,
						* vehicles_element = nullptr
				;

			weapons_element = new TiXmlElement("weaponViews");
				objects_element->LinkEndChild(weapons_element);
			vehicles_element = new TiXmlElement("vehicleViews");
				objects_element->LinkEndChild(vehicles_element);

			Weapon::SaveSettings(weapons_element);
			Vehicle::SaveSettings(vehicles_element);
#endif
		}

		bool VehicleRemapperEnabled()
		{
			return g_object_yelo_globals.vehicle_remapper_disabled==false;
		}
		void VehicleRemapperEnable(bool enabled)
		{
			// jnz eip+2+10
			static const byte k_enable_code[] = { Enums::_x86_opcode_jnz_short, 0x0A };
			// nop, nop
			static const byte k_disable_code[] = {Enums::_x86_opcode_nop, Enums::_x86_opcode_nop};

			g_object_yelo_globals.vehicle_remapper_disabled = enabled==false;

			Memory::WriteMemory(GET_FUNC_VPTR(OBJECT_TYPES_PLACE_ALL_MOD_VEHI_REMAP), (enabled ? k_enable_code : k_disable_code), sizeof(k_enable_code));
		}


		s_object_data* IteratorNextAndVerifyType(s_object_iterator& iter, long_enum object_type)
		{
			ASSERT( TEST_FLAG(iter.type_mask, object_type), "Wrong object_type given to IteratorNext<T>" );

			return blam::object_iterator_next(iter);
		}

		void PlacementDataNewAndCopy(s_object_placement_data& data, datum_index src_object_index, 
			datum_index tag_index_override, datum_index owner_object_index)
		{
			s_object_data* src_object = Objects::ObjectHeader()[src_object_index]->_object;

			if(tag_index_override.IsNull())
				tag_index_override = src_object->definition_index;

			blam::object_placement_data_new(data, tag_index_override, owner_object_index);
			src_object->CopyToPlacementData(data);
		}

		datum_index GetUltimateObject(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_header_datum* object_header_datums = Objects::ObjectHeader().Datums();
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
				s_object_header_datum* object_header_datums = Objects::ObjectHeader().Datums();
				s_object_data* current_obj = object_header_datums[object_index.index]._object;

				for(object_index = current_obj->next_object_index;
					!object_index.IsNull() && n > 0;
					object_index = current_obj->next_object_index, --n)
					current_obj = object_header_datums[object_index.index]._object;
			}

			return n == 0 ? object_index : datum_index::null;
		}

		TagGroups::s_object_definition const* GetObjectDefinition(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_data* object = Objects::ObjectHeader()[object_index]->_object;

				return TagGroups::TagGet<TagGroups::s_object_definition>(object->definition_index);
			}

			return nullptr;
		}

		TagGroups::model_animation_graph const* GetObjectAnimations(datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				s_object_data* object = Objects::ObjectHeader()[object_index]->_object;
				datum_index tag_index = object->animation.definition_index;

				return TagGroups::TagGet<TagGroups::model_animation_graph>(tag_index);
			}

			return nullptr;
		}

		real GetObjectDistanceFromPoint(datum_index obj, const real_vector3d& dest_point)
		{
			real dist = -1.0f;

			if(!obj.IsNull())
			{
				real_vector3d object_origin;
				blam::object_get_origin(obj, object_origin);

				real_vector3d relative_pos = object_origin - dest_point;
				dist = relative_pos.Magnitude();
			}

			return dist;
		}

		typedef void (API_FUNC* proc_object_action_perfomer)(datum_index object_index);
		static void PerformActionOnChildrenByType(datum_index parent, long_flags object_type_mask,
			proc_object_action_perfomer action_performer)
		{
			s_object_header_datum* object_header_datums = Objects::ObjectHeader().Datums();
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

		static void API_FUNC object_delete_thunk(datum_index object_index)
		{
			blam::object_delete(object_index);
		}
		void DeleteChildrenByType(datum_index parent, long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, object_delete_thunk);
		}
		static void API_FUNC object_detach_thunk(datum_index object_index)
		{
			blam::object_detach(object_index);
		}
		void DetachChildrenByType(datum_index parent, long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, object_delete_thunk);
		}

		size_t PredictMemoryPoolUsage(Enums::object_type type, int32 node_count, bool include_yelo_upgrades)
		{
			size_t total_headers_size = sizeof(Memory::s_memory_pool_block);
			size_t total_node_memory_size = (sizeof(real_orientation3d) * 2) + sizeof(real_matrix4x3);
			total_node_memory_size *= node_count;

			size_t object_type_size = 0;
			switch(type)
			{
			case Enums::_object_type_biped:			object_type_size = Enums::k_object_size_biped; break;
			case Enums::_object_type_vehicle:		object_type_size = Enums::k_object_size_vehicle; break;

			case Enums::_object_type_weapon:		object_type_size = Enums::k_object_size_weapon; break;
			case Enums::_object_type_equipment:		object_type_size = Enums::k_object_size_equipment; break;
			case Enums::_object_type_garbage:		object_type_size = Enums::k_object_size_garbage; break;

			case Enums::_object_type_projectile:	object_type_size = Enums::k_object_size_projectile; break;
			case Enums::_object_type_scenery:		object_type_size = Enums::k_object_size_scenery; break;

			case Enums::_object_type_machine:		object_type_size = Enums::k_object_size_machine; break;
			case Enums::_object_type_control:		object_type_size = Enums::k_object_size_control; break;
			case Enums::_object_type_light_fixture:	object_type_size = Enums::k_object_size_light_fixture; break;

			case Enums::_object_type_placeholder:	object_type_size = Enums::k_object_size_placeholder; break;
			case Enums::_object_type_sound_scenery:	object_type_size = Enums::k_object_size_sound_scenery; break;

			default: return 0;
			}

			return object_type_size + total_node_memory_size + total_headers_size;
		}

		bool ObjectIsEnemy(datum_index object_index, datum_index object_index_to_test)
		{
			if (!object_index.IsNull() && !object_index_to_test.IsNull())
			{
				Objects::s_object_data* object = Objects::ObjectHeader()[object_index]->_object;
				Objects::s_object_data* object_to_test = Objects::ObjectHeader()[object_index_to_test]->_object;

				int16 object_team = object->owner_team;
				int16 object_to_test_team = object_to_test->owner_team;

				return blam::game_team_is_enemy(object_team, object_to_test_team);
			}
			return false;
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