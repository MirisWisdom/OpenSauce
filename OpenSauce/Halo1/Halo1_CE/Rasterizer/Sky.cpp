/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Sky.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/Halo1/render/sky/c_sky_manager.hpp>

#include "Common/GameSystemDefinitions.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Scenario/Scenario.hpp"
#include "Scenario/ScenarioInfo.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SKY
#include "Memory/_EngineLayout.inl"

	namespace Render { namespace Sky
	{
		struct s_sky_globals
		{
			struct
			{
				/// <summary>	Flag for whether the sky switching system is available. </summary>
				bool system_enabled;
				PAD24;
			}m_flags;

			/// <summary>	The last scenario sky index used. </summary>
			int16						m_previous_sky_index;
			/// <summary>	The current scenario sky index in use. </summary>
			int16						m_current_sky_index;

			/// <summary>	Sky manager that maintains the current sky tag index. </summary>
			c_sky_manager				m_sky_manager;
			/// <summary>	Gamestate memory pointer for the current override indices. </summary>
			byte*						m_sky_index_gamestate;
		};
		/// <summary>	Global variables for sky management. </summary>
		static s_sky_globals g_sky_globals;

		/// <summary>	Hooks the sky datum index for atmospheric fog. </summary>
		static API_FUNC_NAKED void AtmosphericFog_Hook()
		{
			static const uint32 RETN_ADDRESS = GET_FUNC_PTR(FOG_ATMOSPHERIC_SKY_DATUM_RETN);
			static datum_index SKY_TAG;

			// If the sky switching system is available, use the sky from the sky manager
			// Otherwise revert to the stock implementation
			_asm
			{
				cmp		g_sky_globals.m_flags.system_enabled, 0
				jz		stock_tag
				pushad
			};

			g_sky_globals.m_sky_manager.GetCurrentSkyDatum(SKY_TAG);

			_asm
			{
				popad
				mov		eax, SKY_TAG
				jmp		complete
			stock_tag:
				shl		ecx, 4
				mov		eax, [ecx+eax+0Ch]
			complete:
				jmp		RETN_ADDRESS
			}
		}

		/// <summary>	Hooks the sky datum index for planar fog. </summary>
		static API_FUNC_NAKED void PlanarFog_Hook()
		{
			static const uint32 RETN_ADDRESS = GET_FUNC_PTR(FOG_PLANAR_SKY_DATUM_RETN);
			static datum_index SKY_TAG;

			// If the sky switching system is available, use the sky from the sky manager
			// Otherwise revert to the stock implementation
			_asm
			{
				cmp		g_sky_globals.m_flags.system_enabled, 0
				jz		stock_tag
				pushad
			};

			g_sky_globals.m_sky_manager.GetCurrentSkyDatum(SKY_TAG);

			_asm
			{
				popad
				mov		ecx, SKY_TAG
				jmp		complete
			stock_tag:
				mov		ecx, [edx+34h]
				mov		ecx, [ecx+0Ch]
			complete:
				jmp		RETN_ADDRESS
			}
		}

		/// <summary>	Hooks the sky datum index for rendering the sky. </summary>
		static API_FUNC_NAKED void RenderSky_Hook()
		{
			static const uint32 RETN_ADDRESS = GET_FUNC_PTR(RENDER_SKY_SKY_DATUM_RETN);
			static datum_index SKY_TAG;

			// If the sky switching system is available, use the sky from the sky manager
			// Otherwise revert to the stock implementation
			_asm
			{
				cmp		g_sky_globals.m_flags.system_enabled, 0
				jz		stock_tag
				pushad
			};

			g_sky_globals.m_sky_manager.GetCurrentSkyDatum(SKY_TAG);

			_asm
			{
				popad
				mov		eax, SKY_TAG
				jmp		complete
			stock_tag:
				shl		ecx, 4
				mov		eax, [ecx+eax+0Ch]
			complete:
				jmp		RETN_ADDRESS
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Checks that the loaded map has a scenario info and at least one sky info for sky switching. </summary>
		///
		/// <remarks>
		/// 	Scenario validation for sky switching is not exhaustive in the runtime, specific tests should be run when the
		/// 	map is built to ensure that the data at the runtime can be trusted.
		/// </remarks>
		///
		/// <returns>	Returns true if the map has the required information for sky switching. </returns>
		static bool VerifyScenarioInfo()
		{
			//TODO: use flags set at build time
			// See whether the map has a scenario info block
			if (!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			return true;
		}

		/// <summary>	Resets the sky managers gamestate to NONE. </summary>
		static void ResetGamestate()
		{
			// TODO: Check result
			memset(g_sky_globals.m_sky_index_gamestate, 0xFF, Enums::k_maximum_skies_per_scenario);
		}

		/// <summary>	Creates sky tag datum hooks and initializes the manager and controller. </summary>
		void Initialize()
		{
			Memory::WriteRelativeJmp(&AtmosphericFog_Hook, GET_FUNC_VPTR(FOG_ATMOSPHERIC_SKY_DATUM_HOOK), true);
			Memory::WriteRelativeJmp(&PlanarFog_Hook, GET_FUNC_VPTR(FOG_PLANAR_SKY_DATUM_HOOK), true);
			Memory::WriteRelativeJmp(&RenderSky_Hook, GET_FUNC_VPTR(RENDER_SKY_SKY_DATUM_HOOK), true);
		
			// Reset the sky manager
			g_sky_globals.m_sky_manager.Ctor();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Runs the destructor on the manager.
		/// </summary>
		void Dispose()
		{
			// Destruct the sky manager
			g_sky_globals.m_sky_manager.Dtor();
		}

		/// <summary>	Allocate gamestate memory for storing the map index override data. </summary>
		void InitializeForNewGameState()
		{
			byte* gamestate = GameState::GameStateMalloc<byte>(true, Enums::k_maximum_skies_per_scenario);
			YELO_ASSERT_DISPLAY(gamestate, "Failed to allocate sky index gamestate memory");

			g_sky_globals.m_sky_index_gamestate = gamestate;
			ResetGamestate();

			g_sky_globals.m_sky_manager.SetGamestate(g_sky_globals.m_sky_index_gamestate);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handles the game state life cycle described by life_state. </summary>
		///
		/// <param name="life_state">	State of the gamestate. </param>
		void HandleGameStateLifeCycle(_enum life_state)
		{
			if (!g_sky_globals.m_flags.system_enabled)
			{
				return;
			}

			switch(life_state)
			{
			case Enums::_project_game_state_component_life_cycle_before_save:
				break;
			case Enums::_project_game_state_component_life_cycle_before_load:
				break;
			case Enums::_project_game_state_component_life_cycle_after_load:
				// Force the sky manager to be refreshed after the gamestate is loaded
				g_sky_globals.m_current_sky_index = NONE;
				break;
			default:
				YELO_ASSERT_DISPLAY(false, "Invalid game state life cycle stage");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the active scenario sky index in the sky manager. </summary>
		///
		/// <remarks>	This function gets called each time the current sky index gets updated by the game. </remarks>
		void InitializeForRenderSky()
		{
			if (!g_sky_globals.m_flags.system_enabled)
			{
				return;
			}

			g_sky_globals.m_previous_sky_index = g_sky_globals.m_current_sky_index;

			// If the current sky index has not changed since the last call, return
			g_sky_globals.m_current_sky_index = Scenario::SkyIndex();
			if (g_sky_globals.m_current_sky_index == g_sky_globals.m_previous_sky_index)
			{
				return;
			}

			// Switch the manager to the current sky
			g_sky_globals.m_sky_manager.SetSkyIndex((byte)Scenario::SkyIndex());
		}

		/// <summary>	Resets the sky manager and sets it up for a new map. </summary>
		void InitializeForNewMap()
		{
			g_sky_globals.m_previous_sky_index = NONE;
			g_sky_globals.m_current_sky_index = NONE;

			// Reset the sky manager
			g_sky_globals.m_sky_manager.Ctor();

			// See whether the map has the required tag data
			g_sky_globals.m_flags.system_enabled = VerifyScenarioInfo();

			if(g_sky_globals.m_flags.system_enabled)
			{
				g_sky_globals.m_sky_manager.SetSkyBlocks(&Scenario::Scenario()->skies, &Scenario::ScenarioInfo::ScenarioInfo()->skies);
				ResetSkyOverrides(false);
			}
		}

		/// <summary>	Resets the manager if in use. </summary>
		void DisposeFromOldMap()
		{
			ResetGamestate();

			if (!g_sky_globals.m_flags.system_enabled)
			{
				return;
			}

			// Reset the sky manager
			g_sky_globals.m_sky_manager.Dtor();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resets the sky gamestate and reinitializes the manager for the current sky. </summary>
		///
		/// <param name="update_sky">	Flag for whether to reinitialize for the current sky. </param>
		void ResetSkyOverrides(bool update_sky)
		{
			// Reset the sky managers gamestate
			ResetGamestate();

			if (update_sky)
			{
				g_sky_globals.m_sky_manager.SetSkyIndex((byte)Scenario::SkyIndex());
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets up a sky override. </summary>
		///
		/// <param name="sky_index">	 	Index of the sky to override. </param>
		/// <param name="override_index">	Index of the replacement sky. </param>
		void SetSkyOverride(byte sky_index, byte override_index)
		{
			g_sky_globals.m_sky_manager.SetSkyOverride(sky_index, override_index);
		}
	};};
};
#endif