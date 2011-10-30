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
#include "Game/EngineFunctions.hpp"

#include "Game/Console.hpp"
#include "Game/GameState.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Objects/Objects.hpp"

// comment the following to just use the parameters as-is 
// instead of copying to the stack. I believe there was some 
// concurrency issues originally which caused me to use local 
// buffers instead...
#define ENGINE_FUNCTIONS_USE_LOCAL
enum { k_engine_function_string_buffer_size = 512, };

namespace Yelo
{
	namespace Engine
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_ENGINE_FUNCTIONS
#include "Memory/_EngineLayout.inl"

		void GatherException(void* data, int32 arg_0, int32 arg_4, int32 arg_8)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GATHER_EXCEPTION);

			GET_PTR(gather_exception_data) = data;

			__asm {
				push	arg_8
				push	arg_4
#if PLATFORM_IS_DEDI
				mov		ecx, arg_0
#else
				push	arg_0
#endif
				call	TEMP_CALL_ADDR
#if PLATFORM_IS_DEDI
				add		esp, 4 * 2
#else
				add		esp, 4 * 3
#endif
			}
		}

		IDirect3DBaseTexture9** TextureCacheRequestTexture(Yelo::TagGroups::s_bitmap_data* bitmap, 
			bool add_to_cache, bool block_thread)
		{
#if !PLATFORM_IS_DEDI
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TEXTURE_CACHE_TEXTURE_REQUEST);

			__asm {
				xor		eax, eax
				mov		al, add_to_cache
				push	eax
				mov		al, block_thread
				push	eax
				mov		eax, bitmap
				call	TEMP_CALL_ADDR
				add		esp, 4 * 2
			}
#else
			return NULL;
#endif
		}

		bool SoundCacheRequestSound(Yelo::TagGroups::s_sound_permutation* sound_perm, 
			bool add_to_cache, bool block_thread, bool unknown2)
		{
#if !PLATFORM_IS_DEDI
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SOUND_CACHE_SOUND_REQUEST);

			__asm {
				push	edi
				push	ebx

				xor		eax, eax
				mov		al, unknown2
				push	eax
				mov		al, add_to_cache
				push	eax
				xor		ebx, ebx
				mov		bl, block_thread
				mov		edi, sound_perm
				call	TEMP_CALL_ADDR
				add		esp, 4 * 2

				pop		ebx
				pop		edi
			}
#else
			return false;
#endif
		}

		void MapListAddMap(cstring map_name, int32 map_index)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MAP_LIST_ADD_MAP);

			__asm {
				push	map_index
				mov		eax, map_name
				call	TEMP_CALL_ADDR
				add		esp, 4 * 1
			}
		}
		void RasterizerAddResolution(uint32 width, uint32 height, uint32 refresh_rate)
		{
#if !PLATFORM_IS_DEDI
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(RESOLUTION_LIST_ADD_RESOLUTION);

			_asm{
				mov		ecx, width
				mov		eax, height
				mov		edx, refresh_rate
				push	edx
				push	ecx
				call	TEMP_CALL_ADDR
				add		esp, 8
			}
#endif
		}

		int16 API_FUNC_NAKED AnimationPickRandomPermutation(bool animation_update_kind_affects_game_state, datum_index animation_graph_index, int32 animation_index)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(ANIMATION_PICK_RANDOM_PERMUTATION);

			NAKED_FUNC_START()
				push	ecx
				movzx	ecx, animation_update_kind_affects_game_state

				mov		edx, animation_index
				mov		eax, animation_graph_index
				push	ecx
				call	TEMP_CALL_ADDR
				add		esp, 4 * 1

				pop		ecx
			NAKED_FUNC_END(3)
		}

		void API_FUNC_NAKED GenerateMD5(const char* data, const DWORD data_length, const char* output)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GENERATE_MD5);

			NAKED_FUNC_START()
				push	output
				push	data_length
				push	data
				call	TEMP_CALL_ADDR
			NAKED_FUNC_END_CDECL(3);
		}

		namespace Console
		{
			void ProcessCommand(cstring command)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);
				strcpy_s(local, command);
#else
				cstring local = command;
#endif

				__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		edi, local
#else
					mov		edi, local
#endif
					push	0
					call	TEMP_CALL_ADDR
					add		esp, 4
				}
			}

			void ProcessRemoteCommand(int32 machine_index, cstring command)
			{
				Yelo::Console::TerminalGlobals()->rcon_machine_index = machine_index;
				ProcessCommand(command);
				Yelo::Console::TerminalGlobals()->rcon_machine_index = NONE;
			}

			void TerminalPrint(cstring msg)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_TERMINAL_PRINTF);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);
				strcpy_s(local, msg);
#else
				cstring local = msg;
#endif

				__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		ebx, local
#else
					mov		ebx, local
#endif
					push	ebx
					xor		al, al
					call	TEMP_CALL_ADDR
					add		esp, 4
				}
			}

			void TerminalPrintF(cstring format, ...)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_TERMINAL_PRINTF);

				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);

				va_list args;
				va_start(args, format);
				vsprintf_s(local, format, args);
				va_end(args);

				__asm {
					lea		ebx, local
					push	ebx
					xor		al, al
					call	TEMP_CALL_ADDR
					add		esp, 4
				}
			}

			void PrintF(cstring msg)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PRINTF);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);
				strcpy_s(local, msg);
#else
				cstring local = msg;
#endif

				__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		ebx, local
#else
					mov		ebx, local
#endif
					push	ebx
					push	0
					call	TEMP_CALL_ADDR
					add		esp, 8
				}
			}

			void Warning(cstring msg)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_WARNING);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);
				strcpy_s(local, msg);
#else
				cstring local = msg;
#endif

				__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		ebx, local
#else
					mov		ebx, local
#endif
					push	ebx
					call	TEMP_CALL_ADDR
					add		esp, 4
				}
			}
		};

		namespace Cheats
		{
			API_FUNC_NAKED void AllWeapons()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ALL_WEAPONS);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED void SpawnWarthog()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_SPAWN_WARTHOG);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED void TeleportToCamera()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_TELEPORT_TO_CAMERA);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED void ActiveCamoflage()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED void ActiveCamoflageLocalPlayer()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED datum_index PlayerIndex()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CHEAT_PLAYER_INDEX);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
			}

			API_FUNC_NAKED void DirectorSaveCamera()
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DIRECTOR_SAVE_CAMERA);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
#endif
			}

			API_FUNC_NAKED void DirectorLoadCamera()
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DIRECTOR_LOAD_CAMERA);

				__asm {
					call	TEMP_CALL_ADDR
					retn
				}
#endif
			}
		};

		namespace Game
		{
			API_FUNC_NAKED void SwitchBsp(int16 index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SCENARIO_SWITCH_STRUCTURE_BSP);

				NAKED_FUNC_START()
					xor		esi, esi
					mov		si, index
					call	TEMP_CALL_ADDR
				NAKED_FUNC_END(1)
			}

			void PlayVideo(cstring bink)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(BINK_PLAYBACK_START);

	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				char local[k_engine_function_string_buffer_size];
				memset(local, 0, k_engine_function_string_buffer_size);
				strcpy_s(local, bink);
	#else
				cstring local = bink;
	#endif

				__asm {
	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		eax, local
	#else
					mov		eax, local
	#endif
					push	eax
					call	TEMP_CALL_ADDR
					add		esp, 4
				}
#endif
			}

			API_FUNC_NAKED void RasterizerMessage(wcstring msg, uint32 flags)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_ENGINE_RASTERIZE_MESSAGE);

				NAKED_FUNC_START()
					push	flags
					push	msg
					call	TEMP_CALL_ADDR
				NAKED_FUNC_END_CDECL(2)
#endif
			}

			bool TeamIsEnemy(long_enum team, long_enum team_to_test)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_TEAM_IS_ENEMY);

				__asm {
					mov		ecx, team_to_test
					mov		edx, team
					call	TEMP_CALL_ADDR
				}
			}

			void PlayMultiplayerSound(_enum multiplayer_sound_index)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAME_ENGINE_PLAY_MULTIPLAYER_SOUND);

				__asm {
					push	0
					movsx	esi, multiplayer_sound_index
					or		edi, NONE
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
#endif
			}
		};

		namespace HS
		{
			void ObjectListAdd(datum_index object_list, datum_index object)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_LIST_ADD);

				if(object_list.IsNull()) return;

				__asm {
					push	object
					mov		eax, object_list
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}
		};

		namespace Input
		{
			bool KeyIsDown(int16 key)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(INPUT_KEY_IS_DOWN);

				__asm {

					mov		cx, key
					call	TEMP_CALL_ADDR

				}
#else
				return false;
#endif
			}
		};

		namespace Interface
		{
			void KeystoneChatLogAddString(wcstring str)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(KEYSTONE_CHAT_LOG_ADD_STRING);

	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				wchar_t local[64];
				memset(local, 0, sizeof(local));
				wcscpy_s(local, str);
	#else
				wcstring local = str;
	#endif

				__asm {
	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		eax, local
	#else
					mov		eax, local
	#endif
					push	eax
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
#endif
			}

			void HudPrintMessage(wcstring str)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_PRINT_MESSAGE);

	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				wchar_t local[64];
				memset(local, 0, sizeof(local));
				wcscpy_s(local, str);
	#else
				wcstring local = str;
	#endif

				__asm {
	#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		eax, local
	#else
					mov		eax, local
	#endif
					push	eax
					mov		eax, 0 // player index
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}

			wcstring HudGetItemMessage(int32 message_index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(HUD_GET_ITEM_MESSAGE);

				__asm {
					mov		edx, message_index
					call	TEMP_CALL_ADDR
				}
			}

			void MainmenuLoad()
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MAINMENU_LOAD);

				GameState::MainGlobals()->map.main_menu_scenario_load = true;

				__asm {
					call	TEMP_CALL_ADDR
				}
			}
		};

		namespace Math
		{
			double PeriodicFunctionEvaluate(Enums::periodic_function function_type, double input)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PERIODIC_FUNCTION_EVALUATE);

				__asm {
					fld		input
					sub		esp, 4 * 2			// allocate space for the 'input' parameter
					fstp	qword ptr [esp]		// store the input on the stack
					movzx	eax, function_type
					call	TEMP_CALL_ADDR
					add		esp, 4 * 2			// deallocate. double type consumes two DWORDs of stack
				}
			}

			float TransitionFunctionEvaluate(Enums::transition_function function_type, float input)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TRANSITION_FUNCTION_EVALUATE);

				__asm {
					push	input
					movzx	ecx, function_type
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}
		};

		namespace Memory
		{
			datum_index DatumNewAtIndex(Yelo::Memory::s_data_array* data, datum_index index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEW_AT_INDEX);

				if(data == NULL || index.index == NONE) return datum_index::null;

				__asm {
					mov		edx, data
					mov		eax, index
					call	TEMP_CALL_ADDR
				}
			}

			datum_index DatumNew(Yelo::Memory::s_data_array* data)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEW);

				if(data == NULL) return datum_index::null;

				__asm {
					mov		edx, data
					call	TEMP_CALL_ADDR
				}
			}

			void DatumDelete(Yelo::Memory::s_data_array* data, datum_index datum)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_DELETE);

				if(data == NULL || datum.index == NONE) return;

				__asm {
					mov		edx, datum
					mov		eax, data
					call	TEMP_CALL_ADDR
				}
			}

			void* DataIteratorNext(void* iterator)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATA_ITERATOR_NEXT);

				if(iterator == NULL) return NULL;

				__asm {
					push	edi

					mov		edi, iterator
					call	TEMP_CALL_ADDR

					pop		edi
				}
			}

			datum_index DatumNextIndex(Yelo::Memory::s_data_array* data, datum_index base)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_NEXT_INDEX);

				if(data == NULL || base.index == NONE) return datum_index::null;

				__asm {
					push	edi
					push	esi

					mov		edi, data
					mov		esi, base
					call	TEMP_CALL_ADDR

					pop		esi
					pop		edi
				}
			}

			void* DatumGet(Yelo::Memory::s_data_array* data, datum_index datum)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_GET);

				if(data == NULL || datum.index == NONE) return NULL;

				__asm {
					push	esi

					mov		esi, data
					mov		edx, datum
					call	TEMP_CALL_ADDR

					pop		esi
				}
			}

			void DatumInitialize(Yelo::Memory::s_data_array* data, void* buffer)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(DATUM_INITIALIZE);

				if (data == NULL || buffer == NULL) return;

				__asm {
					push	esi

					mov		edx, data
					mov		esi, buffer
					call	TEMP_CALL_ADDR

					pop		esi
				}
			}
		};

		namespace Networking
		{
			void EncodeObjectDeletionMessage(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_ENCODE_OBJECT_DELETION_MESSAGE);

				if(obj.IsNull()) return;

				__asm {
					push	edi

					mov		edi, obj
					call	TEMP_CALL_ADDR

					pop		edi
				}
			}

			void EncodeHudChatNetworkData(int32 player_number, _enum chat_type, wcstring msg)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(ENCODE_HUD_CHAT_NETWORK_DATA);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
				wchar_t local[255];
				memset(local, 0, sizeof(local));
				wcscpy_s(local, msg);
#else
				cstring local = msg;
#endif

				__asm {
					movsx	eax, chat_type
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
					lea		edx, local
#else
					mov		edx, local
#endif
					push	player_number // really a byte, but you can't push a byte!
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}

			datum_index TranslateObject(datum_index network_object)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_TRANSLATED_INDEX_TRANSLATE);

				if(network_object.IsNull()) return datum_index::null;

				__asm {
					push	esi

					mov		ecx, network_object
					mov		esi, [GET_DATA_PTR(MESSAGE_DELTA_FIELD_OBJECT_INDEX_PARAMETERS)]
					call	TEMP_CALL_ADDR

					pop		esi
				}
			}

			datum_index TranslatePlayer(datum_index network_player)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(MDP_TRANSLATED_INDEX_TRANSLATE);

				if(network_player.IsNull()) return datum_index::null;

				__asm {
					push	esi

					mov		ecx, network_player
					mov		esi, [GET_DATA_PTR(MESSAGE_DELTA_FIELD_PLAYER_INDEX_PARAMETERS)]
					call	TEMP_CALL_ADDR

					pop		esi
				}
			}
		};

		namespace Objects
		{
			void PlacementDataNew(Yelo::Objects::s_object_placement_data& data, datum_index object_definition_index, datum_index object_index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_PLACEMENT_DATA_NEW);

				if(object_definition_index.IsNull()) return;

				__asm {
					push	object_index
					push	object_definition_index
					mov		eax, data
					call	TEMP_CALL_ADDR
					add		esp, 4 * 2
				}
			}

			datum_index New(Yelo::Objects::s_object_placement_data& data)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_NEW);

				__asm {
					mov		ecx, data
					call	TEMP_CALL_ADDR
				}
			}

			datum_index NewWithRole(Yelo::Objects::s_object_placement_data& data, long_enum /*Enums::networked_datum*/ role)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_NEW_WITH_ROLE);

				__asm {
					push	role
					push	data
					call	TEMP_CALL_ADDR
					add		esp, 4 * 2
				}
			}

			void StartInterpolation(datum_index obj_datum)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_START_INTERPOLATION);

				if(obj_datum.IsNull()) return;

				__asm {
					mov		eax, obj_datum
					mov		edx, 30 * 2 // interpolate over 60 frames, or 2 secs
					call	TEMP_CALL_ADDR
				}
			}

			void Reset(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RESET);

				if(obj.IsNull()) return;

				__asm {
					push	ebx

					mov		ebx, obj
					call	TEMP_CALL_ADDR

					pop		ebx
				}
			}

			void ReconnectToMap(datum_index obj, s_scenario_location* location_reference)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RECONNECT_TO_MAP);

				if(obj.IsNull()) return;

				__asm {
					push	location_reference
					push	obj
					call	TEMP_CALL_ADDR
					add		esp, 4 * 2
				}
			}

			void DisconnectFromMap(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DISCONNECT_FROM_MAP);

				if(obj.IsNull()) return;

				__asm {
					push	obj
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}

			void GetOrigin(datum_index obj, real_point3d* out_origin)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_ORIGIN);

				if(obj.IsNull()) return;

				__asm {
					mov		eax, out_origin
					mov		ecx, obj
					call	TEMP_CALL_ADDR
				}
			}

			void GetOrientation(datum_index obj, real_vector3d* out_forward, real_vector3d* out_up)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_ORIENTATION);

				if(obj.IsNull()) return;

				__asm {
					mov		ecx, obj
					mov		eax, out_forward
					push	out_up
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}

			s_scenario_location* GetLocation(datum_index obj, s_scenario_location* out_location)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_GET_LOCATION);

				if(obj.IsNull()) return out_location;

				__asm {
					push	edi

					mov		edi, obj
					mov		eax, out_location
					call	TEMP_CALL_ADDR

					pop		edi
				}
			}

			void SetPosition(datum_index obj, real_point3d* new_pos, real_vector3d* new_forward, real_vector3d* new_up)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_POSITION);

				if(obj.IsNull()) return;

				__asm {
					push	edi

					mov		edi, new_pos
					push	new_up
					push	new_forward
					push	obj
					call	TEMP_CALL_ADDR
					add		esp, 4 * 3

					pop		edi
				}
			}

			void SetPositionNetwork(datum_index obj, real_point3d* new_pos)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_SET_POSITION_NETWORK);

				if(obj.IsNull()) return;

				__asm {
					push	edi
					push	esi

					mov		edi, obj
					mov		esi, new_pos
					call	TEMP_CALL_ADDR

					pop		esi
					pop		edi
				}
			}

			bool RestoreBody(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_RESTORE_BODY);

				if(obj.IsNull()) return false;

				__asm {
					mov		eax, obj
					call	TEMP_CALL_ADDR
				}
			}

			void DepleteBody(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DEPLETE_BODY);

				if(obj.IsNull()) return;

				__asm {
					mov		eax, obj
					call	TEMP_CALL_ADDR
				}
			}

			void DepleteShield(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DEPLETE_SHIELD);

				if(obj.IsNull()) return;

				__asm {
					push	edi

					mov		edi, obj
					call	TEMP_CALL_ADDR

					pop		edi
				}
			}

			void DoubleChargeShield(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(OBJECT_DOUBLE_CHARGE_SHIELD);

				if(obj.IsNull()) return;

				__asm {
					mov		eax, obj
					call	TEMP_CALL_ADDR
				}
			}

			void GetCameraPosition(datum_index unit, real_point3d* out_position)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

				if(unit.IsNull()) return;

				__asm {
					push	edi

					mov		edi, out_position
					mov		ecx, unit
					call	TEMP_CALL_ADDR

					pop		edi
				}
#else
				
#endif
			}

			void DefinitionPredict(datum_index obj)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_GET_CAMERA_POSITION);

				if(obj.IsNull()) return;

				__asm {
					mov		eax, obj
					call	TEMP_CALL_ADDR
				}
			}

			void OrientToCutsceneFlag(datum_index unit, int32 cutscene_flag_index, bool set_facing, bool i_dont_know)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNIT_ORIENT_TO_FLAG);

				if(unit.IsNull()) return;

				__asm {
					movzx	eax, set_facing
					push	eax
					movzx	eax, i_dont_know
					push	eax
					mov		eax, cutscene_flag_index
					push	unit
					call	TEMP_CALL_ADDR
					add		esp, 4 * 3
				}
			}
		};

		namespace Players
		{
			datum_index IndexFromUnitIndex(datum_index unit_datum)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_INDEX_FROM_UNIT_INDEX);

				if(unit_datum.IsNull()) return datum_index::null;

				__asm {
					push	unit_datum
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
			}

			bool Teleport(datum_index player, const real_point3d& position, datum_index source_unit_index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PLAYER_TELEPORT);

				if(player.IsNull()) return false;

				__asm {
					push	position
					push	source_unit_index
					push	player
					call	TEMP_CALL_ADDR
					add		esp, 4 * 3
				}
			}


			static void player_screen_effect(datum_index player, uint32 func)
			{
#if !PLATFORM_IS_DEDI
				__asm {
					mov		edx, player
					mov		eax, func
					call	eax
				}
#endif
			}

			void ScreenEffectOvershield(datum_index player)
			{
#if !PLATFORM_IS_DEDI
				if(player.IsNull()) return;
				player_screen_effect(player, GET_FUNC_PTR(PLAYER_OVER_SHIELD_SCREEN_EFFECT));
#endif
			}

			void ScreenEffectCamo(datum_index player)
			{
#if !PLATFORM_IS_DEDI
				if(player.IsNull()) return;
				player_screen_effect(player, GET_FUNC_PTR(PLAYER_CAMO_SCREEN_EFFECT));
#endif
			}

			void ScreenEffectHealth(datum_index player)
			{
#if !PLATFORM_IS_DEDI
				if(player.IsNull()) return;
				player_screen_effect(player, GET_FUNC_PTR(PLAYER_HEALTH_PACK_SCREEN_EFFECT));
#endif
			}

			datum_index FindClosestPlayerIndex(datum_index player)
			{
#if !PLATFORM_IS_DEDI
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(FIND_CLOSEST_PLAYER_INDEX);

				if(player.IsNull()) return datum_index::null;

				__asm {
					push	player
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1
				}
#else
				return datum_index::null;
#endif
			}
		};

		namespace Scenario
		{
			bool TriggerVolumeTestPoint(int32 trigger_volume_index, const real_point3d& point)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(SCENARIO_TRIGGER_VOLUME_TEST_POINT);

				__asm {
					mov		ecx, point
					mov		eax, trigger_volume_index
					call	TEMP_CALL_ADDR
				}
			}

			bool TriggerVolumeTestObject(int32 trigger_volume_index, datum_index object)
			{
				if(object.IsNull())
					return false;

				Yelo::Objects::s_object_data* obj = (*Yelo::Objects::ObjectHeader())[object]->_object;

				return TriggerVolumeTestPoint(trigger_volume_index, obj->GetNetworkDatumData()->position);
			}
		};

		namespace TagGroups
		{
			void PredictResources(TagBlock<Yelo::TagGroups::predicted_resource>& resources_block)
			{
#if !PLATFORM_IS_DEDI // dedi doesn't need to predict resources since it doesn't render bitmaps or play sounds
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES);

				__asm {
					push	esi

					mov		esi, resources_block
					call	TEMP_CALL_ADDR

					pop		esi
				}
#endif
			}

			datum_index TagLoaded(tag group_tag, cstring name)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(TAG_LOADED);

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
					call	TEMP_CALL_ADDR
					add		esp, 4 * 1

					pop		edi
				}
			}

			wcstring UnicodeStringListGetString(datum_index ustr, int32 index)
			{
				static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(UNICODE_STRING_LIST_GET_STRING);

				__asm {
					mov		edx, index
					mov		ecx, ustr
					call	TEMP_CALL_ADDR
				}
			}
		};
	};
};