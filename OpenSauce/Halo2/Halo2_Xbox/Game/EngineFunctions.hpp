/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_data_array;
		struct s_data_array_iterator;
	};

	namespace Engine
	{
		namespace AI
		{
			// Erase an actor
			// If [ai_index] is null, then it erases ALL actors,
			// else it only erases the actor associated with [ai_index]
			void Erase(datum_index ai_index = datum_index::null);
		};

		namespace Cheats
		{
			// Toggle inf. ammo (rounds and energy) on or off
			void InfiniteAmmo(bool is_on);
			// Toggle inf. grenades (frag and plasma) on or off
			void InfiniteGrenades(bool is_on);
		};

		namespace Cinematic
		{
			void LightingSetPrimaryLight(angle horiz, angle vert, const real_rgb_color* color);
			void LightingSetSecondaryLight(angle horiz, angle vert, const real_rgb_color* color);

			void FadeIn(int16 time, const real_rgb_color* color);
			void FadeOut(int16 time, const real_rgb_color* color);

			void Start();
			void Stop();
			void SkipStartInternal();
			void SkipStopInternal();
			void ShowLetterbox(bool show);
			void ShowLetterboxImmediate(bool show);

			void SetFilterBitmap(datum_index bitmap_index);

			void RenderLightsEnableCinematicShadow(real arg_value = 3.0f);

			void ScreenEffectStart(bool start_effect = true);
			void ScreenEffectStop();

			void LightmapShadow(bool enable_shadow = true);
			void AmbienceDetails(bool enable_details = true);
		};

		namespace Game
		{
			void GameRate(real one, real two, real three);

			// Tell the engine to revert to last checkpoint
			void Revert();
			// Tell the engine to save the current state
			void Save();
			// Tell the engine that the player won
			void Won();

			// Play a sound tag index with a given volume
			void PlaySound(datum_index sound_index, real volume);
		};

		namespace Interface
		{
			DWORD XInputGetState(HANDLE device, void* state);

			void HudCinematicFade(real init_opcaity, real final_opcaity);

			// Add a text message to a player's hud
			void HudMessagePrint(wcstring string, int32 player_index = 0);
			// Add a item message to a player's hud
			void HudAddItemMessage(wcstring string, int32 player_index = 0);

			// Set the hud timer's set
			void HudTimerSet(short minutes, int16 seconds);
			// Set the hud timer's warning time threshold
			// At this time and below, it will start flashing the hud timer text
			void HudTimerSetWarningTime(int16 minutes, int16 seconds);
			// Set the hud timer's position
			void HudTimerSetPosition(int16 x, int16 y, _enum hud_corner);
			// Pause the hud timer
			void HudTimerPause(bool pause_timer);

			// Set the current hud state text for a player (default is first)
			// with a given uptime and optional sound index to play
			void HudSetStateText(
				wcstring state_text, 
				int32 player_index = 0, 
				int32 up_time = NONE, 
				datum_index optional_sound = datum_index::null);
		};

		namespace Memory
		{
			// Get a reference to the next valid datum data in a s_data_array
			// See: Memory/DataShared.hpp and Memory/MemoryInterface.cpp
			void* DataIteratorNext(Yelo::Memory::s_data_array_iterator* iter);

			// Get the next (valid) datum's absolute index, relative to [cursor]
			// [cursor] - an absolute index
			int32 DataNextIndex(const Yelo::Memory::s_data_array* array, int32 cursor);
		};

		namespace Networking
		{
		};

		namespace Players
		{
			// Finds the local player's datum index in the players array
			datum_index LocalGetPlayerIndex(int32 local_player_index = 0);

			// Gets a player index from a unit object index
			datum_index PlayerFromUnitIndex(datum_index unit_index);
		};

		namespace Objects
		{
			// Returns true if [object_index] IS a player or CONTAINS
			// a player somewhere down its current attachment hierarchy
			bool IsOrContainsPlayer(datum_index object_index);

			// Hide or stop hiding [object_index]
			void Hide(datum_index object_index, bool hide_it = true);
			// Set the position of [object_index], while optionally set its forward vector and up angles
			void SetPosition(datum_index object_index, const real_vector3d* pos, const real_vector3d* forward, const real_euler_angles3d* up);

			// Make [unit_index] play its weapon lowering animation with a given time frame
			void UnitLowerWeapon(datum_index unit_index, int32 duration_seconds); // really a short
			// Make [unit_index] play its weapon raising animation with a given time frame
			void UnitRaiseWeapon(datum_index unit_index, int32 duration_seconds); // really a short
		};
	};
};