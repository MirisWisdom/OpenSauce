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
#pragma once

#include "Memory/MemoryInterface.hpp"
#include "Memory/Data.hpp"
#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/bitmap_definitions.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_object_placement_data;
	};

	namespace TagGroups
	{
		struct bitmap_data;

		struct s_sound_permutation;
	};
};

namespace Yelo
{
	namespace Enums
	{
		enum multiplayer_sound : _enum
		{
			_multiplayer_sound_playball,
			_multiplayer_sound_game_over,
			_multiplayer_sound_one_min_to_win,
			_multiplayer_sound_30_secs_to_win,
			_multiplayer_sound_red_team_one_min_to_win,
			_multiplayer_sound_red_team_30_secs_to_win,
			_multiplayer_sound_blue_team_one_min_to_win,
			_multiplayer_sound_blue_team_30_secs_to_win,
			_multiplayer_sound_red_team_flag_taken,
			_multiplayer_sound_red_team_flag_returned,
			_multiplayer_sound_red_team_flag_captured,
			_multiplayer_sound_blue_team_flag_taken,
			_multiplayer_sound_blue_team_flag_returned,
			_multiplayer_sound_blue_team_flag_captured,
			_multiplayer_sound_double_kill,
			_multiplayer_sound_tripple_kill,
			_multiplayer_sound_killtacular,
			_multiplayer_sound_running_riot,
			_multiplayer_sound_killing_spree,
			_multiplayer_sound_oddball,
			_multiplayer_sound_race,
			_multiplayer_sound_slayer,
			_multiplayer_sound_ctf,
			_multiplayer_sound_warthog,
			_multiplayer_sound_ghost,
			_multiplayer_sound_scorpion,
			_multiplayer_sound_countdown_timer,
			_multiplayer_sound_teleporter_activate,
			_multiplayer_sound_flag_failure,
			_multiplayer_sound_countdown_for_respawn,
			_multiplayer_sound_hill_move,
			_multiplayer_sound_player_respawn,
			_multiplayer_sound_koth,
			_multiplayer_sound_odd_ball,
			_multiplayer_sound_team_race,
			_multiplayer_sound_team_slayer,
			_multiplayer_sound_team_koth,
			_multiplayer_sound_blue_team_ctf,
			_multiplayer_sound_red_team_ctf,
			_multiplayer_sound_contest,
			_multiplayer_sound_control,
			_multiplayer_sound_hill_occupied,
			_multiplayer_sound_countdown_timer_end,
			_multiplayer_sound_ting,
			_multiplayer_sound,

			_multiplayer_sound_custom1 = _multiplayer_sound,
			_multiplayer_sound_custom2,
			_multiplayer_sound_custom3,
			_multiplayer_sound_custom4,
			_multiplayer_sound_custom5,
			_multiplayer_sound_custom6,
			_multiplayer_sound_custom7,
			_multiplayer_sound_custom8,
			_multiplayer_sound_custom9,
			_multiplayer_sound_custom10,
			_multiplayer_sound_custom11,
			_multiplayer_sound_custom12,
			_multiplayer_sound_custom13,
			_multiplayer_sound_custom14,
			_multiplayer_sound_custom15,
			_multiplayer_sound_custom16, // default max s_game_globals_multiplayer_information::sounds instances = 60

			_multiplayer_sound_custom,
		};
	};

	namespace Engine
	{
		void GatherException(void* data = NULL, int32 arg_0 = 0x89, int32 arg_4 = 0x7E, int32 arg_8 = 1);

		// Setting [block_thread] to true causes the engine to immediately create the rasterizer (D3D) data for it
		IDirect3DBaseTexture9** TextureCacheRequestTexture(TagGroups::s_bitmap_data* bitmap, 
			bool add_to_cache = true, bool block_thread = false);

		bool SoundCacheRequestSound(TagGroups::s_sound_permutation* sound_perm, 
			bool add_to_cache = true, bool block_thread = false, bool unknown2 = false);

		void MapListAddMap(cstring map_name, int32 map_index = 0x13);

		namespace Console
		{
			// Process a fully qualified string of script functions
			void ProcessCommand(cstring command);

			// Prints [msg] to the console
			void TerminalPrint(cstring msg);
			// Formats and prints to the console
			void TerminalPrintF(cstring format, ...);

			void PrintF(cstring msg);

			// Prints [msg] to the console with red coloring
			void Warning(cstring msg);
		};

		namespace Cheats
		{
			// Spawn all weapons referenced in the game globals around the player
			void AllWeapons();

			// Spawn a warthog near the player
			void SpawnWarthog();

			// Teleport the player to the camera's active position
			void TeleportToCamera();

			// Activates camo on [CheatPlayerIndex()]
			void ActiveCamoflage();

			// Activates camo on the local player
			void ActiveCamoflageLocalPlayer();

			// Gets the player for cheating in this game
			datum_index PlayerIndex();

			// Saves the current camera location to "camera.txt"
			void DirectorSaveCamera();

			// Loads the camera location from "camera.txt" and places the camera there
			void DirectorLoadCamera();
		};

		namespace Game
		{
			// Switches the bsp being used to the bsp at [index] in the scenario structure bsps block
			void SwitchBsp(int16 index);

			// Plays the [bink] file
			void PlayVideo(cstring bink);

			// Prints [msg]
			void RasterizerMessage(wcstring msg, uint32 flags);

			// Checks to see if [team_to_test] is an enemy of [team]
			bool TeamIsEnemy(long_enum team, long_enum team_to_test);

			// Plays [multiplayer_sound_index] locally on the player's computer.
			void PlayMultiplayerSound(_enum multiplayer_sound_index);
		};

		namespace HS
		{
			// Add [object] to the [object_list]
			void ObjectListAdd(datum_index object_list, datum_index object);
		};

		namespace Input
		{
			bool KeyIsDown(int16 key);
		};

		namespace Interface
		{
			// Client builds only (dedi does nothing)
			// Add a string to the hud-chat view
			void KeystoneChatLogAddString(wcstring str);

			// Print [str] to the hud message queue.
			// IE, text you see when the game says "Picked up 30 rounds"
			void HudPrintMessage(wcstring str);

			// Get a item object's message string
			wcstring HudGetItemMessage(int32 message_index);
		};

		namespace Math
		{
			double PeriodicFunctionEvaluate(Enums::periodic_function function_type, double input);

			float TransitionFunctionEvaluate(Enums::transition_function function_type, float input);
		};

		namespace Memory
		{
			datum_index DatumNewAtIndex(Yelo::Memory::s_data_array* data, datum_index index);

			// creates a new element in [data] and returns the datum index
			datum_index DatumNew(Yelo::Memory::s_data_array* data);

			// Delete the data associated with the [datum] handle in [data]
			void DatumDelete(Yelo::Memory::s_data_array* data, datum_index datum);

			// Get a reference to the next valid datum data in a s_data_array
			// See: Memory/Data.hpp
			void* DataIteratorNext(void* iterator);

			datum_index DatumNextIndex(Yelo::Memory::s_data_array* data, datum_index base);

			// Get the data associated with [datum] from the [data] array
			void* DatumGet(Yelo::Memory::s_data_array* data, datum_index datum);

			void DatumInitialize(Yelo::Memory::s_data_array* data, void* buffer);
		};

		namespace Networking
		{
			// Sends a message_object_deletion packet for [obj], deleting it in the process
			void EncodeObjectDeletionMessage(datum_index obj);

			void EncodeHudChatNetworkData(int32 player_number, _enum chat_type, wcstring msg);

			// Translate a network_object_index to a local object_index
			datum_index TranslateObject(datum_index network_object);
			// Translate a network_player_index to a local player_index
			datum_index TranslatePlayer(datum_index network_player);
		};

		namespace Objects
		{
			void PlacementDataNew(Yelo::Objects::s_object_placement_data& data, datum_index object_definition_index, datum_index object_index = datum_index::null);

			datum_index New(Yelo::Objects::s_object_placement_data& data);

			datum_index NewWithRole(Yelo::Objects::s_object_placement_data& data, long_enum /*Enums::networked_datum*/ role);

			void StartInterpolation(datum_index obj_datum);

			void Reset(datum_index obj);

			void ReconnectToMap(datum_index obj, s_scenario_location* location_reference = NULL);

			void DisconnectFromMap(datum_index obj);

			// Get the origin of [obj]. Takes the parent object (if there is one) into account.
			void GetOrigin(datum_index obj, real_point3d* out_origin);

			// Get the orientation of [obj]. Takes the parent object (if there is one) into account.
			void GetOrientation(datum_index obj, real_vector3d* out_forward, real_vector3d* out_up);

			// Get the scenario location of [obj]
			s_scenario_location* GetLocation(datum_index obj, s_scenario_location* out_location);

			// precondition: obj is a valid object index, everything else can be null
			void SetPosition(datum_index obj, real_point3d* new_pos = NULL, real_vector3d* new_forward = NULL, real_vector3d* new_up = NULL);

			void SetPositionNetwork(datum_index obj, real_point3d* new_pos);

			bool RestoreBody(datum_index obj);

			void DepleteBody(datum_index obj);

			void DepleteShield(datum_index obj);

			void DoubleChargeShield(datum_index obj);

			// Calculates the viewing position based on the unit's 'head' marker if it has one. 
			// If not, it will estimate the position based on the object's origin.
			void GetCameraPosition(datum_index unit, real_point3d* out_position);

			// Loads the predicted resources defined in [obj]'s tag definition (if they're not already loaded)
			void DefinitionPredict(datum_index obj);

			void OrientToCutsceneFlag(datum_index unit, int32 cutscene_flag_index, bool set_facing = true, bool i_dont_know = true);
		};

		namespace Players
		{
			// Get the player datum_index from an active unit object (via it's object datum_index)
			datum_index IndexFromUnitIndex(datum_index unit_datum);

			bool Teleport(datum_index player, const real_point3d& position, datum_index source_unit_index = datum_index::null);

			void ScreenEffectOvershield(datum_index player);

			void ScreenEffectCamo(datum_index player);

			void ScreenEffectHealth(datum_index player);

			// Get the player datum_index of the closest player relative to player
			// represented by the [player] datum_index parameter
			datum_index FindClosestPlayerIndex(datum_index player);
		};

		namespace Scenario
		{
			// Tests to see if [point] exists inside [trigger_volume_index] (index to a definition in the current scenario)
			bool TriggerVolumeTestPoint(int32 trigger_volume_index, const real_point3d& point);

			// Tests to see if [object] is inside [trigger_volume_index] (index to a definition in the current scenario)
			bool TriggerVolumeTestObject(int32 trigger_volume_index, datum_index object);
		};

		namespace TagGroups
		{
			void PredictResources(TagBlock<Yelo::TagGroups::predicted_resource>& resources_block);

			// [name] - the tag definition name
			// [group_tag] - the group tag of the tag definition (ie, 'ustr')
			datum_index TagLoaded(cstring name, tag group_tag);

			// [ustr] - the index of the unicode string definition
			// [index] - the index of the string you wish to get
			wcstring UnicodeStringListGetString(datum_index ustr, int32 index);
		};
	};
};