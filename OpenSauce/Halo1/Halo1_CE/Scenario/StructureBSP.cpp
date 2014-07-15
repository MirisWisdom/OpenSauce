/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Scenario/StructureBSP.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>
#include <YeloLib/Halo1/saved_games/game_state_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Game/GameState.hpp"
#include "Common/GameSystemDefinitions.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/Lightmaps.hpp"
#include "Rasterizer/Sky.hpp"
#include "Scenario/Scenario.hpp"
#include "Scenario/ScenarioInfo.hpp"

namespace Yelo
{
	namespace StructureBSP
	{
		using namespace TagGroups;

		struct s_bsp_modifier_globals
		{
			struct
			{
				/// <summary>	Flag for whether bsp modifier are used in the scenario. </summary>
				bool m_system_enabled;
				PAD24;
			}m_flags;

			struct s_bsp_modifier_state
			{
				byte m_lightmap_set;
				byte m_sky_set;
			};

			/// <summary>	Gamestate memory for storing the current variant index of each zone. </summary>
			s_bsp_modifier_state* m_bsp_modifier_gamestate;
		};
		/// <summary>	Contains all globals variables for the bsp modifiers. </summary>
		static s_bsp_modifier_globals g_bsp_modifier_globals;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Checks whether the map has bsp modifiers defined. </summary>
		///
		/// <remarks>
		/// 	Validation checks here are not exhaustive, checks should be done on map build to ensure data in the map is
		/// 	correct.
		/// </remarks>
		///
		/// <returns>	Returns true if the scenario has bsp modifiers set up. </returns>
		static bool VerifyVariantSetup()
		{
			if(!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			return (Scenario::ScenarioInfo::ScenarioInfo()->bsps.Count > 0);
		}

		/// <summary>	Resets the gamestate. </summary>
		static void ResetGamestate()
		{
			memset(g_bsp_modifier_globals.m_bsp_modifier_gamestate, 0, Enums::k_maximum_structure_bsps_per_scenario_upgrade * sizeof(s_bsp_modifier_globals::s_bsp_modifier_state));
		}

		/// <summary>	Initializes for new game state. </summary>
		void InitializeForNewGameState()
		{
			g_bsp_modifier_globals.m_bsp_modifier_gamestate = GameState::GameStateMalloc<s_bsp_modifier_globals::s_bsp_modifier_state>(true
				, Enums::k_maximum_structure_bsps_per_scenario_upgrade);

			YELO_ASSERT_DISPLAY(g_bsp_modifier_globals.m_bsp_modifier_gamestate, "Failed to allocate bsp modifier gamestate memory");

			ResetGamestate();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a bsp's lightmap set. </summary>
		///
		/// <param name="bsp_info_index">	Zero-based index of the bsp info block. </param>
		/// <param name="set_index">	 	Zero-based index of the sky set. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool SetLightmapSet(const byte bsp_info_index, const byte set_index)
		{
			if(!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			auto& bsp_list = Scenario::ScenarioInfo::ScenarioInfo()->bsps;
			if((bsp_list.Count == 0) || (bsp_info_index >= bsp_list.Count))
			{
				return false;
			}
			
			auto& lightmap_sets = bsp_list[bsp_info_index].lightmap_sets;
			if(set_index >= lightmap_sets.Count)
			{
				return false;
			}
			
			// If the edited bsp is the current bsp, set the lightmaps
			auto& lightmap_set = lightmap_sets[set_index];
			if(Scenario::ScenarioInfo::BSPInfoIndex() == bsp_info_index)
			{
				Render::Lightmaps::SetLightmaps(lightmap_set.standard_lightmap
					, lightmap_set.directional_lightmap_1
					, lightmap_set.directional_lightmap_2
					, lightmap_set.directional_lightmap_3);
			}

			// Store the bsps current lightmap sequence
			g_bsp_modifier_globals.m_bsp_modifier_gamestate[bsp_info_index].m_lightmap_set = set_index;

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a bsp's lightmap set. </summary>
		///
		/// <param name="bsp_info_index">	Zero-based index of the bsp info block. </param>
		/// <param name="set_name">		 	Name of the lightmap set. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool SetLightmapSet(const byte bsp_info_index, cstring set_name)
		{
			if(!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			auto& bsp_list = Scenario::ScenarioInfo::ScenarioInfo()->bsps;
			if((bsp_list.Count == 0) || (bsp_info_index >= bsp_list.Count))
			{
				return false;
			}

			// Find the sky set by name
			auto& lightmap_sets = bsp_list[bsp_info_index].lightmap_sets;

			for(byte set_index = 0; set_index < lightmap_sets.Count; ++set_index)
			{
				if(strcmp(lightmap_sets[set_index].name, set_name) == 0)
				{
					return SetLightmapSet(bsp_info_index, set_index);
				}
			}

			return false;
		}

		/// <summary>	Sets the standard lightmap. </summary>
		static void SetStandardLightmap()
		{
			auto& bsp_tag = *Scenario::StructureBsp();

			Render::Lightmaps::SetLightmaps(bsp_tag.lightmap_bitmaps.tag_index);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a bsp's sky set. </summary>
		///
		/// <param name="bsp_info_index">	Zero-based index of the bsp info block. </param>
		/// <param name="set_index">	 	Zero-based index of the sky set. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool SetSkySet(const byte bsp_info_index, const byte set_index)
		{
			if(!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			auto& bsp_list = Scenario::ScenarioInfo::ScenarioInfo()->bsps;
			if((bsp_list.Count == 0) || (bsp_info_index >= bsp_list.Count))
			{
				return false;
			}
			
			auto& sky_sets = bsp_list[bsp_info_index].sky_sets;
			if(set_index >= sky_sets.Count)
			{
				return false;
			}

			// If the edited bsp is the current bsp, set the skies
			if(Scenario::ScenarioInfo::BSPInfoIndex() == bsp_info_index)
			{
				Render::Sky::Reset();
				
				auto& sky_set = sky_sets[set_index];

				for(auto sky : sky_set.skies)
				{
					Render::Sky::SetSkyIndex((byte)sky.sky_index, sky.sky.tag_index);
				}
			}

			// Store the bsps current sky set
			g_bsp_modifier_globals.m_bsp_modifier_gamestate[bsp_info_index].m_sky_set = set_index;

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a bsp's sky set. </summary>
		///
		/// <param name="bsp_info_index">	Zero-based index of the bsp info block. </param>
		/// <param name="set_name">		 	Name of the sky set. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool SetSkySet(const byte bsp_info_index, cstring set_name)
		{
			if(!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			auto& bsp_list = Scenario::ScenarioInfo::ScenarioInfo()->bsps;
			if((bsp_list.Count == 0) || (bsp_info_index >= bsp_list.Count))
			{
				return false;
			}

			// Find the sky set by name
			auto& sky_sets = bsp_list[bsp_info_index].sky_sets;

			for(byte set_index = 0; set_index < sky_sets.Count; ++set_index)
			{
				if(strcmp(sky_sets[set_index].name, set_name) == 0)
				{
					return SetSkySet(bsp_info_index, set_index);
				}
			}
			
			return false;
		}

		static void SetToCurrentGameState()
		{
			const byte bsp_info_index = Scenario::ScenarioInfo::BSPInfoIndex();
			auto gamestate = g_bsp_modifier_globals.m_bsp_modifier_gamestate[Scenario::ScenarioInfo::BSPInfoIndex()];

			SetLightmapSet(bsp_info_index, gamestate.m_lightmap_set);
			SetSkySet(bsp_info_index, gamestate.m_sky_set);
		}

#pragma region Scripting
		static void* API_FUNC scripting_structure_bsp_set_lightmap_set_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 bsp_index;
				PAD16;
				cstring set_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.boolean = SetLightmapSet((byte)args->bsp_index, args->set_name);

			return result.pointer;
		}

		static void* API_FUNC scripting_structure_bsp_set_sky_set_evaluate(void** arguments)
		{
			struct s_arguments {
				int16 bsp_index;
				PAD16;
				cstring set_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result; result.pointer = nullptr;

			result.boolean = SetSkySet((byte)args->bsp_index, args->set_name);

			return result.pointer;
		}

		static void InitializeScripting()
		{
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_set_lightmap_set, 
				scripting_structure_bsp_set_lightmap_set_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_set_sky_set, 
				scripting_structure_bsp_set_sky_set_evaluate);
		}
#pragma endregion Scripting

		/// <summary>	Initializes the bsp modifier script functions. </summary>
		void Initialize()
		{
			InitializeScripting();
		}

		/// <summary>	Unused. </summary>
		void Dispose()
		{ }

		/// <summary>	Initializes for new map. </summary>
		void InitializeForNewMap()
		{
			g_bsp_modifier_globals.m_flags.m_system_enabled = VerifyVariantSetup();

			if(g_bsp_modifier_globals.m_flags.m_system_enabled)
			{
				ResetGamestate();

				// InitializeForNewMap is called after the first InitializeForBSPLoad so re-call here to set up for the first BSP
				InitializeForNewBSP();
			}
		}

		/// <summary>	Initializes for new bsp. </summary>
		void InitializeForNewBSP()
		{
			Render::Sky::Reset();
			SetStandardLightmap();

			if (!g_bsp_modifier_globals.m_flags.m_system_enabled || (Scenario::ScenarioInfo::BSPInfoIndex() == NONE))
			{
				return;
			}

			SetToCurrentGameState();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handles the game state life cycle described by life_state. </summary>
		///
		/// <param name="life_state">	State of the gamestate. </param>
		void HandleGameStateLifeCycle(_enum life_state)
		{
			switch(life_state)
			{
			case Enums::_project_game_state_component_life_cycle_before_save:
				break;
			case Enums::_project_game_state_component_life_cycle_before_load:
				// reset the gamestate to default values
				ResetGamestate();
				break;
			case Enums::_project_game_state_component_life_cycle_after_load:
				SetToCurrentGameState();
				break;
			}
		}
	};
};
#endif