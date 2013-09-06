/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/EngineFunctions.hpp"

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/memory/data.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#include "Game/Console.hpp"
#include "Game/GameState.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Networking/MDP.hpp"
#include "Objects/Objects.hpp"

// comment the following to just use the parameters as-is 
// instead of copying to the stack. I believe there was some 
// concurrency issues originally which caused me to use local 
// buffers instead...
#define ENGINE_FUNCTIONS_USE_LOCAL
enum { k_engine_function_string_buffer_size = 512, };

namespace Yelo
{
	namespace Physics
	{
		struct s_collision_result;
	};

	namespace TagGroups
	{
		struct s_bitmap_data;

		struct s_sound_permutation;

		struct predicted_resource;
	};

	namespace Engine
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

		#include "Game/EngineFunctions._Misc.inl"

		namespace AI
		{
			void API_FUNC_NAKED Delete(datum_index actor_index, bool is_dead)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(ACTOR_DELETE);

				API_FUNC_NAKED_START()
					movzx	eax, is_dead
					push	eax
					mov		ebx, actor_index
					call	FUNCTION
					add		esp, 4 * 1
				API_FUNC_NAKED_END(2)
			}

			void API_FUNC_NAKED AttachFree(datum_index unit_index, datum_index actor_variant_definition)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(AI_SCRIPTING_ATTACH_FREE);

				API_FUNC_NAKED_START()
					mov		eax, actor_variant_definition
					push	unit_index
					call	FUNCTION
					add		esp, 4 * 1
				API_FUNC_NAKED_END(2)
			}
		};

		namespace Cache
		{
			API_FUNC_NAKED bool FileReadRequest(/*datum_index tag_index,*/
				uint32 offset_, uint32 size_, void* buffer, const Yelo::Cache::s_cache_file_request_params& params, 
				bool block, Enums::cache_file_request_source source)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(CACHE_FILE_READ_REQUEST);

				API_FUNC_NAKED_START()
					push	esi
					xor		eax, eax

					movzx	eax, source
					push	eax
					movzx	eax, block
					push	eax
					mov		esi, params
					push	buffer
					push	size_
					push	offset_
					call	FUNCTION
					add		esp, 4 * 5

					pop		esi
					API_FUNC_NAKED_END(6); // not including the unused tag_index
			}

			cstring GetMapExtension()
			{
				return GET_PTR2(MAP_LIST_MAP_EXTENSION);
			}

			void MapListAddMap(cstring map_name, cstring extension, int32 map_index)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(MAP_LIST_ADD_MAP);
				static cstring* MAP_LIST_EXTENSION_REF = GET_PTR2(MAP_LIST_MAP_EXTENSION_REF);
				static cstring MAP_LIST_EXTENSION_STOCK = GET_PTR2(MAP_LIST_MAP_EXTENSION);

				*MAP_LIST_EXTENSION_REF = extension;
				__asm {
					push	map_index
					mov		eax, map_name
					call	FUNCTION
					add		esp, 4 * 1
				}
				*MAP_LIST_EXTENSION_REF = MAP_LIST_EXTENSION_STOCK;
			}

			API_FUNC_NAKED int GetMapEntryIndexFromName(const char* name)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(MAP_ENTRY_INDEX_FROM_NAME);

				API_FUNC_NAKED_START()
					mov		eax, name
					call	FUNCTION
				API_FUNC_NAKED_END(1);
			}
		};

		namespace Cheats
		{
			#include "Game/EngineFunctions.Cheats.inl"
		};

		namespace Console
		{
			#include "Game/EngineFunctions.Console.inl"
		};

		namespace Effects
		{
			void API_FUNC_NAKED NewOnObjectMarker(datum_index effect_definition_index, datum_index object_index, cstring marker_name)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(EFFECT_NEW_ON_OBJECT_MARKER);

				API_FUNC_NAKED_START()
					push	esi
					push	edi

					mov		ecx, marker_name
					push	ecx
					mov		esi, object_index
					mov		edi, effect_definition_index
					call	FUNCTION
					add		esp, 4 * 1

					pop		edi
					pop		esi
				API_FUNC_NAKED_END(3)
			}
		};

		namespace Game
		{
			#include "Game/EngineFunctions.Game.inl"
		};

		namespace HS
		{
			void ObjectListAdd(datum_index object_list, datum_index object_index)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(OBJECT_LIST_ADD);

				if(object_list.IsNull()) return;

				__asm {
					push	object_index
					mov		eax, object_list
					call	FUNCTION
					add		esp, 4 * 1
				}
			}
		};

		namespace Input
		{
			bool KeyIsDown(_enum key)
			{
#if !PLATFORM_IS_DEDI
				static const uintptr_t FUNCTION = GET_FUNC_PTR(INPUT_KEY_IS_DOWN);

				__asm {
					mov		cx, key
					call	FUNCTION
				}
#else
				return false;
#endif
			}
		};

		namespace Interface
		{
			#include "Game/EngineFunctions.Interface.inl"
		};

		namespace Math
		{
			float PeriodicFunctionEvaluate(Enums::periodic_function function_type, double input)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

				__asm {
					fld		input
					sub		esp, 4 * 2			// allocate space for the 'input' parameter
					fstp	qword ptr [esp]		// store the input on the stack
					movzx	eax, function_type
					call	FUNCTION
					add		esp, 4 * 2			// deallocate. double type consumes two DWORDs of stack
				}
			}

			float TransitionFunctionEvaluate(Enums::transition_function function_type, float input)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

				__asm {
					push	input
					movzx	ecx, function_type
					call	FUNCTION
					add		esp, 4 * 1
				}
			}
		};

		namespace Memory
		{
			#include "Game/EngineFunctions.Memory.inl"
		};

		namespace Networking
		{
			#include "Game/EngineFunctions.Networking.inl"
		};

		namespace Objects
		{
			#include "Game/EngineFunctions.Objects.inl"
		};

		namespace Physics
		{
			API_FUNC_NAKED bool PLATFORM_API CollisionTestVector(long_flags flags, real_point3d& location, real_vector3d& vector, 
				datum_index object_index, Yelo::Physics::s_collision_result& collision)
			{
				static const uintptr_t JMP_ADDR = GET_FUNC_PTR(COLLISION_TEST_VECTOR);

				__asm jmp	JMP_ADDR
			}
		};

		namespace Players
		{
			#include "Game/EngineFunctions.Players.inl"
		};

		namespace Scenario
		{
			bool TriggerVolumeTestPoint(int32 trigger_volume_index, const real_point3d& point)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(SCENARIO_TRIGGER_VOLUME_TEST_POINT);

				__asm {
					mov		ecx, point
					mov		eax, trigger_volume_index
					call	FUNCTION
				}
			}

			bool TriggerVolumeTestObject(int32 trigger_volume_index, datum_index object)
			{
				if(object.IsNull())
					return false;

				Yelo::Objects::s_object_data* obj = Yelo::Objects::ObjectHeader()[object]->_object;

				return TriggerVolumeTestPoint(trigger_volume_index, obj->center);
			}
		};

		namespace TagGroups
		{
			void PredictResources(TagBlock<Yelo::TagGroups::predicted_resource>& resources_block)
			{
#if !PLATFORM_IS_DEDI // dedi doesn't need to predict resources since it doesn't render bitmaps or play sounds
				static const uintptr_t FUNCTION = GET_FUNC_PTR(PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES);

				__asm {
					push	esi

					mov		esi, resources_block
					call	FUNCTION

					pop		esi
				}
#endif
			}

			datum_index TagLoaded(tag group_tag, cstring name)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOADED);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[256];
				memset(local, 0, sizeof(local));
				strcpy_s(local, name);
#else
				cstring local = name;
#endif

				__asm {
					push	edi

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		eax, local
#else
					mov		eax, local
#endif
					push	eax
					mov		edi, group_tag
					call	FUNCTION
					add		esp, 4 * 1

					pop		edi
				}
			}

			wcstring UnicodeStringListGetString(datum_index ustr, int32 index)
			{
				static const uintptr_t FUNCTION = GET_FUNC_PTR(UNICODE_STRING_LIST_GET_STRING);

				__asm {
					mov		edx, index
					mov		ecx, ustr
					call	FUNCTION
				}
			}
		};
	};

	//////////////////////////////////////////////////////////////////////////
	// ai
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// actors.c
		void PLATFORM_API actor_delete(datum_index actor_index, bool is_dead)
		{
			Engine::AI::Delete(actor_index, is_dead);
		}
		//////////////////////////////////////////////////////////////////////////
		// ai_script.c
		void PLATFORM_API ai_scripting_attach_free(datum_index unit_index, datum_index actor_variant_definition_index)
		{
			Engine::AI::AttachFree(unit_index, actor_variant_definition_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// cache
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// cache_files_windows.c
		bool PLATFORM_API cache_file_read_request(/*datum_index tag_index,*/
			uint32 offset, uint32 size, void* buffer, const Cache::s_cache_file_request_params& params, 
			bool block, Enums::cache_file_request_source source)
		{
			return Engine::Cache::FileReadRequest(offset, size, buffer, params, block, source);
		}
		//////////////////////////////////////////////////////////////////////////
		// pc_sound_cache.c
		bool PLATFORM_API sound_cache_sound_request(TagGroups::s_sound_permutation* sound_perm, 
			bool block_thread, bool load, bool reference)
		{
			return Engine::SoundCacheRequestSound(sound_perm, block_thread, load, reference);
		}
		//////////////////////////////////////////////////////////////////////////
		// pc_texture_cache.c
		IDirect3DBaseTexture9** PLATFORM_API texture_cache_bitmap_get_hardware_format(TagGroups::s_bitmap_data* bitmap, 
			// block
			bool block_thread, bool load)
		{
			return Engine::TextureCacheBitmapGetHardwareFormat(bitmap, block_thread, load);
		}
		//////////////////////////////////////////////////////////////////////////
		// physical_memory_map.c
		void predicted_resources_precache(TagBlock<TagGroups::predicted_resource>& resources)
		{
			return Engine::TagGroups::PredictResources(resources);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// camera
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// director.c
		void PLATFORM_API director_save_camera()
		{
			Engine::Cheats::DirectorSaveCamera();
		}
		void PLATFORM_API director_load_camera()
		{
			Engine::Cheats::DirectorLoadCamera();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// game
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// cheats.c
		void PLATFORM_API cheat_all_weapons()
		{
			Engine::Cheats::AllWeapons();
		}
		void PLATFORM_API cheat_spawn_warthog()
		{
			Engine::Cheats::SpawnWarthog();
		}
		void PLATFORM_API cheat_teleport_to_camera()
		{
			Engine::Cheats::TeleportToCamera();
		}
		void PLATFORM_API cheat_active_camouflage()
		{
			Engine::Cheats::ActiveCamoflage();
		}
		void PLATFORM_API cheat_active_camouflage_local_player()
		{
			Engine::Cheats::ActiveCamoflageLocalPlayer();
		}
		datum_index PLATFORM_API cheat_local_player()
		{
			return Engine::Cheats::PlayerIndex();
		}
		//////////////////////////////////////////////////////////////////////////
		// game_allegiance.c
		bool PLATFORM_API game_team_is_enemy(long_enum team, long_enum team_to_test)
		{
			return Engine::Game::TeamIsEnemy(team, team_to_test);
		}
		//////////////////////////////////////////////////////////////////////////
		// game_engine.c
		datum_index PLATFORM_API find_closest_player_index(datum_index player_index)
		{
			return Engine::Players::FindClosestPlayerIndex(player_index);
		}


		void PLATFORM_API game_engine_rasterize_message(wcstring message, real alpha)
		{
			Engine::Game::RasterizeMessage(message, alpha);
		}
		//////////////////////////////////////////////////////////////////////////
		// game_engine_multiplayer_sounds.c
		void PLATFORM_API game_engine_play_multiplayer_sound(_enum multiplayer_sound_index)
		{
			// TODO
			//Engine::Game::PlayMultiplayerSound(multiplayer_sound_index);
		}
		//////////////////////////////////////////////////////////////////////////
		// players.c
		void PLATFORM_API player_examine_nearby_vehicle(datum_index player_index, datum_index vehicle_index)
		{
			Engine::Players::PlayerExamineNearbyVehicle(player_index, vehicle_index);
		}
		void PLATFORM_API player_set_action_result(datum_index player_index, datum_index action_object_index, int32 action_result, int32 action_seat_index)
		{
			Engine::Players::PlayerSetActionResult(player_index, action_object_index, action_result, action_seat_index);
		}
		void PLATFORM_API player_set_action_result_to_network(datum_index player_index, datum_index action_object_index, int32 action_result_type, int32 action_result, int32 action_seat_index, datum_index next_weapon_index)
		{
			Engine::Players::PlayerSetActionResultToNetwork(player_index, action_object_index, action_result_type, action_result, action_seat_index, next_weapon_index);
		}


		datum_index PLATFORM_API player_index_from_unit_index(datum_index unit_index)
		{
			return Engine::Players::IndexFromUnitIndex(unit_index);
		}
		bool PLATFORM_API player_teleport(datum_index player_index, datum_index source_unit_index, const real_point3d& position)
		{
			return Engine::Players::Teleport(player_index, position, source_unit_index);
		}
		void PLATFORM_API player_over_shield_screen_effect(datum_index player_index)
		{
			Engine::Players::ScreenEffectOvershield(player_index);
		}
		void PLATFORM_API player_active_camo_screen_effect(datum_index player_index)
		{
			Engine::Players::ScreenEffectCamo(player_index);
		}
		void PLATFORM_API player_health_pack_screen_effect(datum_index player_index)
		{
			Engine::Players::ScreenEffectHealth(player_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// hs
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// hs_library_external.c
		void PLATFORM_API hs_object_orient(datum_index object_index, int32 cutscene_flag_index, bool teleport, bool set_facing)
		{
			Engine::Objects::ObjectOrient(object_index, cutscene_flag_index, teleport, set_facing);
		}


		void PLATFORM_API hs_effect_new_from_object_marker(datum_index effect_definition_index, datum_index object_index, cstring marker_name)
		{
			Engine::Effects::NewOnObjectMarker(effect_definition_index, object_index, marker_name);
		}
		//////////////////////////////////////////////////////////////////////////
		// object_lists.c
		void PLATFORM_API object_list_add(datum_index object_list_index, datum_index object_index)
		{
			Engine::HS::ObjectListAdd(object_list_index, object_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// input
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// input_windows.c
		bool PLATFORM_API input_key_is_down(_enum key_code)
		{
			return Engine::Input::KeyIsDown(key_code);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// interface
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// hud.c
		wcstring PLATFORM_API hud_get_item_string(int16 reference_index)
		{
			return Engine::Interface::HudGetItemMessage(reference_index);
		}
		//////////////////////////////////////////////////////////////////////////
		// hud_chat.c
		void PLATFORM_API hud_chat_to_network(int32 player_number, long_enum chat_type, wcstring text)
		{
			Engine::Networking::EncodeHudChatNetworkData(player_number, chat_type, text);
		}
		void PLATFORM_API hud_chat_display_message(wcstring message)
		{
			Engine::Interface::KeystoneChatLogAddString(message);
		}
		//////////////////////////////////////////////////////////////////////////
		// hud_draw.c
		//////////////////////////////////////////////////////////////////////////
		// hud_messaging.c
		void PLATFORM_API hud_print_message(int16 local_player_index, wcstring message)
		{
			// TODO local player index
			Engine::Interface::HudPrintMessage(message);
		}
		//////////////////////////////////////////////////////////////////////////
		// ui_video_screen.c
		void PLATFORM_API ui_video_screen_add_resolution(uint32 width, uint32 height, uint32 refresh_rate)
		{
			Engine::RasterizerAddResolution(width, height, refresh_rate);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// main
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// main.c
		bool PLATFORM_API main_connect(cstring address, cstring password)
		{
			return Engine::Networking::ConnectToServer(address, password);
		}
		void PLATFORM_API main_menu_load()
		{
			// TODO: refactor this
			Engine::Interface::MainmenuLoad();
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// math
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// periodic_functions.c
		real PLATFORM_API periodic_function_evaluate(Enums::periodic_function function_type, real input)
		{
			return Engine::Math::PeriodicFunctionEvaluate(function_type, input);
		}
		real PLATFORM_API transition_function_evaluate(Enums::transition_function function_type, real input)
		{
			return Engine::Math::TransitionFunctionEvaluate(function_type, input);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// memory
	namespace blam
	{
		using namespace Yelo::Memory;

		//////////////////////////////////////////////////////////////////////////
		// data.c
		s_data_array* PLATFORM_API data_new(cstring name, int32 maximum_count, size_t datum_size)
		{
			return Engine::Memory::DataNew(name, maximum_count, datum_size);
		}
		void PLATFORM_API data_dispose(s_data_array* data)
		{
			if(data != nullptr)
			{
				data_verify(data);
				CAST_PTR(Yelo::Memory::s_data_array*, GlobalFree(data));
			}
		}
		void PLATFORM_API data_delete_all(s_data_array* data)
		{
			Engine::Memory::DataDeleteAll(data);
		}
		datum_index PLATFORM_API data_next_index(s_data_array* data, datum_index cursor)
		{
			return Engine::Memory::DataNextIndex(data, cursor);
		}
		void* PLATFORM_API data_iterator_next(s_data_iterator& iterator)
		{
			return Engine::Memory::DataIteratorNext(&iterator);
		}
		datum_index PLATFORM_API datum_new_at_index(s_data_array* data, datum_index index)
		{
			return Engine::Memory::DatumNewAtIndex(data, index);
		}
		datum_index PLATFORM_API datum_new(s_data_array* data)
		{
			return Engine::Memory::DatumNew(data);
		}
		void PLATFORM_API datum_delete(s_data_array* data, datum_index index)
		{
			return Engine::Memory::DatumDelete(data, index);
		}
		void* PLATFORM_API datum_try_and_get(s_data_array* data, datum_index index)
		{
			return Engine::Memory::DatumTryAndGet(data, index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// models
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// model_animations.c
		int16 PLATFORM_API animation_choose_random_permutation_internal(long_enum render_or_affects_game_state, datum_index animation_graph_index, int32 animation_index)
		{
			return Engine::AnimationPickRandomPermutation(render_or_affects_game_state, animation_graph_index, animation_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// objects
	namespace blam
	{
		using namespace Yelo::Objects;

		//////////////////////////////////////////////////////////////////////////
		// damage.c
		bool PLATFORM_API object_restore_body(datum_index object_index)
		{
			return Engine::Objects::RestoreBody(object_index);
		}
		void PLATFORM_API object_deplete_body(datum_index object_index)
		{
			Engine::Objects::DepleteBody(object_index);
		}
		void PLATFORM_API object_deplete_shield(datum_index object_index)
		{
			Engine::Objects::DepleteShield(object_index);
		}
		void PLATFORM_API object_double_charge_shield(datum_index object_index)
		{
			Engine::Objects::DoubleChargeShield(object_index);
		}
		void PLATFORM_API area_of_effect_cause_damage(s_damage_data& data, datum_index )
		{
			// TODO
		}
		void PLATFORM_API object_cause_damage(s_damage_data& data, datum_index damaged_object_index, 
			int16 node_index, int16 region_index, int16 damage_materials_element_index, 
			real_vector3d* normal)
		{
			Engine::Objects::ObjectCauseDamage(data, damaged_object_index, 
				node_index, region_index, damage_materials_element_index, 
				normal);
		}
		//////////////////////////////////////////////////////////////////////////
		// index_resolution.c
		int32 PLATFORM_API index_resolution_table_translate(MessageDeltas::s_index_resolution_table& table, datum_index local_index)
		{
			// TODO: get rid of these guards
#ifndef YELO_NO_NETWORK
			return Engine::Networking::TranslateIndex(table, local_index);
#else
			return 0;
#endif
		}
		//////////////////////////////////////////////////////////////////////////
		// objects.c
		void PLATFORM_API object_reset(datum_index object_index)
		{
			Engine::Objects::Reset(object_index);
		}
		void PLATFORM_API object_set_position(datum_index object_index, 
			__in_opt real_point3d* new_position, __in_opt real_vector3d* new_forward, __in_opt real_vector3d* new_up)
		{
			Engine::Objects::SetPosition(object_index, new_position, new_forward, new_up);
		}
		void PLATFORM_API object_set_position_network(datum_index object_index, 
			real_point3d* new_position)
		{
			Engine::Objects::SetPositionNetwork(object_index, new_position);
		}
		void PLATFORM_API object_translate(datum_index object_index, 
			const real_point3d& new_position, __in_opt const s_scenario_location* new_location)
		{
			// TODO
		}
		void PLATFORM_API object_placement_data_new(s_object_placement_data& data, datum_index object_definition_index, datum_index owner_object_index)
		{
			Engine::Objects::PlacementDataNew(data, object_definition_index, owner_object_index);
		}
		datum_index PLATFORM_API object_new(s_object_placement_data& data)
		{
			return Engine::Objects::New(data);
		}
		datum_index PLATFORM_API object_new_with_role(s_object_placement_data& data, Enums::networked_datum role)
		{
			return Engine::Objects::NewWithRole(data, role);
		}
		void PLATFORM_API object_delete_to_network(datum_index object_index)
		{
			Engine::Networking::EncodeObjectDeletionMessage(object_index);
		}
		void PLATFORM_API object_delete(datum_index object_index)
		{
			Engine::Objects::Delete(object_index);
		}
		void PLATFORM_API object_reconnect_to_map(datum_index object_index, __in_opt s_scenario_location* location_reference)
		{
			Engine::Objects::ReconnectToMap(object_index, location_reference);
		}
		void PLATFORM_API object_disconnect_from_map(datum_index object_index)
		{
			Engine::Objects::DisconnectFromMap(object_index);
		}
		void PLATFORM_API object_attach_to_marker(datum_index target_object_index, cstring target_marker_name, datum_index object_index, cstring marker_name)
		{
			Engine::Objects::Attach(target_object_index, target_marker_name, object_index, marker_name);
		}
		void PLATFORM_API object_detach(datum_index object_index)
		{
			Engine::Objects::Detach(object_index);
		}
		real_point3d& PLATFORM_API object_get_origin(datum_index object_index, __out real_point3d& return_origin)
		{
			return Engine::Objects::GetOrigin(object_index, return_origin);
		}
		void PLATFORM_API object_get_orientation(datum_index object_index, 
			__out_opt real_vector3d* return_forward, __out_opt real_vector3d* return_up)
		{
			Engine::Objects::GetOrientation(object_index, return_forward, return_up);
		}
		s_scenario_location& PLATFORM_API object_get_location(datum_index object_index, __out s_scenario_location& return_location)
		{
			return Engine::Objects::GetLocation(object_index, return_location);
		}
		void PLATFORM_API object_start_interpolation(datum_index object_index, int32 interpolation_frame_count)
		{
			Engine::Objects::StartInterpolation(object_index, interpolation_frame_count);
		}
		s_object_data* PLATFORM_API object_iterator_next(s_object_iterator& iter)
		{
			return Engine::Objects::IteratorNext(iter);
		}
		int16 PLATFORM_API objects_in_sphere(Flags::objects_find_flags find_flags, long_flags object_type_flags, 
			const s_scenario_location& location, const real_point3d& center, real radius, 
			datum_index object_indices[], int16 maximum_object_indices)
		{
			return Engine::Objects::FindInSphere(find_flags, object_type_flags,
				location, center, radius, object_indices, maximum_object_indices);
		}
		void PLATFORM_API object_definition_predict(datum_index object_index)
		{
			Engine::Objects::DefinitionPredict(object_index);
		}
		bool PLATFORM_API object_header_block_allocate(datum_index object_index, size_t block_reference_offset, size_t size)
		{
			return Engine::Objects::HeaderBlockAllocate(object_index, block_reference_offset, size);
		}
		void PLATFORM_API objects_scripting_set_scale(datum_index object_index, real scale, int32 ticks)
		{
			Engine::Objects::SetScale(object_index, scale, ticks);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// physics
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// collisions.c
		bool PLATFORM_API collision_test_vector(long_flags flags, real_point3d& location, real_vector3d& vector, 
			datum_index object_index, Physics::s_collision_result& collision)
		{
			return Engine::Physics::CollisionTestVector(flags, location, vector, object_index, collision);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// rasterizer
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// dx9/rasterizer_dx9.c
		bool PLATFORM_API rasterizer_set_texture_bitmap_data(_enum stage, TagGroups::s_bitmap_data* bitmap)
		{
			return Engine::SetTextureSamplerStage(bitmap, stage);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// scenario
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// scenario.c
		bool PLATFORM_API scenario_switch_structure_bsp(int16 bsp_index)
		{
			return Engine::Game::SwitchBsp(bsp_index);
		}
		bool PLATFORM_API scenario_trigger_volume_test_point(int32 trigger_volume_index, const real_point3d& point)
		{
			return Engine::Scenario::TriggerVolumeTestPoint(trigger_volume_index, point);
		}
		bool PLATFORM_API scenario_trigger_volume_test_object(int32 trigger_volume_index, datum_index object_index)
		{
			return Engine::Scenario::TriggerVolumeTestObject(trigger_volume_index, object_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// tag_files
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// tag_groups.c
		datum_index PLATFORM_API tag_loaded(tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(TAG_LOADED);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
			char local[256];
			memset(local, 0, sizeof(local));
			strcpy_s(local, name);
#else
			cstring local = name;
#endif

			__asm {
				push	edi

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				lea		eax, local
#else
				mov		eax, local
#endif
				push	eax
				mov		edi, group_tag
				call	FUNCTION
				add		esp, 4 * 1

				pop		edi
			}
		}
		void PLATFORM_API tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter)
		{
			memset(&iter, 0, sizeof(iter));
			iter.group_tag_filter = group_tag_filter;
		}
		datum_index PLATFORM_API tag_iterator_next(TagGroups::s_tag_iterator& iter)
		{
			static const uintptr_t FUNCTION = Engine::GET_FUNC_PTR(TAG_ITERATOR_NEXT);

			__asm {
				push	esi

				mov		esi, iter
				call	FUNCTION

				pop		esi
			}
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// text
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// text_group.c
		wcstring unicode_string_list_get_string(datum_index unicode_string_list_definition_index, int32 reference_index)
		{
			return Engine::TagGroups::UnicodeStringListGetString(unicode_string_list_definition_index, reference_index);
		}
	};
	//////////////////////////////////////////////////////////////////////////
	// units
	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// units.c
		void PLATFORM_API unit_set_animation(datum_index unit_index, datum_index animation_graph_index, int32 animation_index)
		{
			Engine::Objects::UnitSetAnimation(unit_index, animation_graph_index, animation_index);
		}


		int16 PLATFORM_API unit_find_nearby_seat(datum_index unit_index, datum_index target_unit_index, __out int16& parent_seat_index)
		{
			return Engine::Objects::UnitFindNearbySeat(unit_index, target_unit_index, parent_seat_index);
		}
		bool PLATFORM_API unit_can_enter_seat(datum_index unit_index, datum_index target_unit_index, int16 seat_index, 
			__out datum_index* unit_in_seat_index)
		{
			return Engine::Objects::UnitCanEnterSeat(unit_index, target_unit_index, seat_index, unit_in_seat_index);
		}
		bool PLATFORM_API unit_enter_seat(datum_index unit_index, datum_index target_unit_index, int32 seat_index)
		{
			return Engine::Objects::UnitEnterSeat(unit_index, target_unit_index, seat_index);
		}
		void PLATFORM_API unit_get_camera_position(datum_index unit_index, __out real_point3d* return_position)
		{
			Engine::Objects::UnitGetCameraPosition(unit_index, return_position);
		}
		void PLATFORM_API unit_open(datum_index unit_index)
		{
			Engine::Objects::UnitOpen(unit_index);
		}
		void PLATFORM_API unit_close(datum_index unit_index)
		{
			Engine::Objects::UnitClose(unit_index);
		}
		void PLATFORM_API unit_scripting_exit_vehicle(datum_index unit_index)
		{
			Engine::Objects::UnitExitVehicle(unit_index);
		}
		void PLATFORM_API unit_exit_seat_end(datum_index unit_index, 
			bool no_network_message, bool can_run_on_client_side, bool unknown)
		{
			Engine::Objects::UnitExitSeatEnd(unit_index, no_network_message, can_run_on_client_side, unknown);
		}
		int16 unit_get_custom_animation_time(datum_index unit_index)
		{
			return Engine::Objects::UnitGetCustomAnimationTime(unit_index);
		}
	};
};