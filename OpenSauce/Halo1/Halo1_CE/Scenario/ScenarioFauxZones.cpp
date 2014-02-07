/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Scenario/ScenarioFauxZones.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include "Game/GameState.hpp"
#include "Common/GameSystemDefinitions.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/Lightmaps.hpp"
#include "Rasterizer/Sky.hpp"
#include "Scenario/Scenario.hpp"
#include "Scenario/ScenarioInfo.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace ScenarioFauxZones
	{
		using namespace TagGroups;

		struct s_faux_zone_globals
		{
			struct
			{
				/// <summary>	Flag for whether faux faux zones are set up in the scenario. </summary>
				bool m_system_enabled;
				PAD24;
			}m_flags;

			byte m_active_faux_zone;
			PAD24;

			/// <summary>	Mapping array to associate the BSP info index with the zone index. </summary>
			byte m_bsp_zone_map[Enums::k_maximum_structure_bsps_per_scenario_upgrade];
			/// <summary>	Gamestate memory for storing the current variant index of each zone. </summary>
			byte* m_zone_variant_gamestate;
		};
		/// <summary>	Contains all globals variables for the faux zone system. </summary>
		static s_faux_zone_globals g_faux_zone_globals;
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Checks whether the map has faux zones defined. </summary>
		///
		/// <remarks>
		/// 	Validation checks here are not exhaustive, checks should be done on map build to ensure data in the map is
		/// 	correct.
		/// </remarks>
		///
		/// <returns>	Returns true if the scenario has faux zones set up. </returns>
		static bool VerifyFauxZoneSetup()
		{
			// TODO: use build time flags
			// Test for yellow tag and scenario info block
			if (!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			// Test whether the scenario info has any faux zones defined
			if (!Scenario::ScenarioInfo::ScenarioInfo()->faux_zones.Count)
			{
				return false;
			}

			return true;
		}

#include "Scenario/ScenarioFauxZones.Scripting.inl"

		/// <summary>	Sets up all scripting functions associated with scenario faux zones. </summary>
		static void InitializeScripting()
		{
			Scripting::InitializeScriptFunction(Enums::_hs_function_scenario_faux_zones_reset, 
				scripting_scenario_faux_zones_reset_evaluate);

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_current_switch_variant, 
				scripting_scenario_faux_zone_current_switch_variant_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_variant, 
				scripting_scenario_faux_zone_switch_variant_evaluate);

			// Depreceated
			Scripting::NullifyScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_sky);
		}

		/// <summary>	Resets the faux zone gamestate. </summary>
		static void ResetGamestate()
		{
			// TODO: check result
			memset(g_faux_zone_globals.m_zone_variant_gamestate, 0, Enums::k_maximum_structure_bsps_per_scenario_upgrade);
		}

		/// <summary>	Allocate gamestate memory for storing the zone variant index data. </summary>
		void InitializeForNewGameState()
		{
			byte* gamestate = GameState::GameStateMalloc<byte>(true, Enums::k_maximum_structure_bsps_per_scenario_upgrade);

			YELO_ASSERT_DISPLAY(gamestate, "Failed to allocate zone variant index gamestate memory");
			g_faux_zone_globals.m_zone_variant_gamestate = gamestate;

			// Reset the gamestate so all zones use variant 0
			// TODO: check result
			memset(g_faux_zone_globals.m_zone_variant_gamestate, 0, Enums::k_maximum_structure_bsps_per_scenario_upgrade);
		}

		/// <summary>	Called on map load to create a mapping between bsp info's and their associated faux zones. </summary>
		static void BuildZoneMap()
		{
			// TODO: could this be done at compile time and stored in the map?
			// TODO: check result
			// Reset the zone map
			memset(&g_faux_zone_globals.m_bsp_zone_map, 0xFF, sizeof(g_faux_zone_globals.m_bsp_zone_map));

			// Map the bsp zone indices to the bsp info indices
			for(int i = 0; i < Scenario::ScenarioInfo::ScenarioInfo()->faux_zones.Count; i++)
			{
				const s_scenario_faux_zone& zone = Scenario::ScenarioInfo::ScenarioInfo()->faux_zones[i];
				g_faux_zone_globals.m_bsp_zone_map[zone.bsp_info_index] = i;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the specified zones variant. </summary>
		///
		/// <param name="zone_index">   	Zone to change. </param>
		/// <param name="variant_index">	Zero-Variant to switch to. </param>
		static void SetFauxZoneVariant(byte zone_index, byte variant_index)
		{
			// If the zone is not valid reset the lightmap sequence index and sky overrides
			if(zone_index == 0xFF)
			{
				Render::Lightmaps::SetLightmapSequence(0);
				Render::Sky::ResetSkyOverrides();
				return;
			}

			// Get the zones variant
			const s_scenario_faux_zone& zone = Scenario::ScenarioInfo::ScenarioInfo()->faux_zones[zone_index];
			const s_scenario_faux_zone_variant& variant = zone.variants[variant_index];

			// If the edited zone is the current zone, set the lightmaps and skies for the variant
			byte active_zone_index = g_faux_zone_globals.m_bsp_zone_map[Scenario::ScenarioInfo::BSPInfoIndex()];
			if(active_zone_index == zone_index)
			{
				Render::Lightmaps::SetLightmapSequence((byte)variant.lightmap_sequence_index);

				Render::Sky::ResetSkyOverrides();

				for (int i = 0; i < variant.sky_overrides.Count; i++)
				{
					Render::Sky::SetSkyOverride((byte)variant.sky_overrides[i].sky_index, (byte)variant.sky_overrides[i].sky_reference);
				}
			}

			// Store the zones current variant
			g_faux_zone_globals.m_zone_variant_gamestate[zone_index] = variant_index;
		}

		/// <summary>	Initializes the Faux zone script functions. </summary>
		void Initialize()
		{
			InitializeScripting();
		}

		/// <summary>	Unused. </summary>
		void Dispose()
		{
		}

		/// <summary>	Sets up the faux zone system for a new map. </summary>
		void InitializeForNewMap()
		{
			g_faux_zone_globals.m_flags.m_system_enabled = VerifyFauxZoneSetup();

			if(g_faux_zone_globals.m_flags.m_system_enabled)
			{
				ResetGamestate();
				BuildZoneMap();

				// InitializeForNewMap is called after the first InitializeForBSPLoad so re-call here to set up for the first BSP
				InitializeForNewBSP();
			}
		}

		/// <summary>	Initialize for faux zone associated with the current BSP. </summary>
		void InitializeForNewBSP()
		{
			if (!g_faux_zone_globals.m_flags.m_system_enabled || (Scenario::ScenarioInfo::BSPInfoIndex() == NONE))
			{
				return;
			}

			g_faux_zone_globals.m_active_faux_zone = g_faux_zone_globals.m_bsp_zone_map[Scenario::ScenarioInfo::BSPInfoIndex()];

			// Set the zones variant to that which is stored in the gamestate
			SetFauxZoneVariant(g_faux_zone_globals.m_active_faux_zone, g_faux_zone_globals.m_zone_variant_gamestate[g_faux_zone_globals.m_active_faux_zone]);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Handles the game state life cycle described by life_state. </summary>
		///
		/// <param name="life_state">	State of the gamestate. </param>
		void HandleGameStateLifeCycle(_enum life_state)
		{
			//TODO: is this even needed?
			switch(life_state)
			{
			case Enums::_project_game_state_component_life_cycle_before_save:
				break;
			case Enums::_project_game_state_component_life_cycle_before_load:
				// reset the gamestate to default values
				Reset();
				break;
			case Enums::_project_game_state_component_life_cycle_after_load:
				break;
			}
		}

		/// <summary>	Resets the gamestate so that all zones use variant 0 and reinitializes for the current BSP. </summary>
		void Reset()
		{
			ResetGamestate();
			InitializeForNewBSP();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a faux zone by name. </summary>
		///
		/// <param name="name">	The zone name to search for. </param>
		///
		/// <returns>	The zone index if found, otherwise -1. </returns>
		static byte GetZone(cstring name)
		{
			const TAG_TBLOCK(& zones, s_scenario_faux_zone) = _global_yelo->scenario[0].faux_zones;

			for(int x = 0; x < zones.Count; x++)
			{
				if (_stricmp(name, zones[x].name) != 0)
				{
					continue;
				}
				return x;
			}
			return NONE;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a zone variant by name. </summary>
		///
		/// <param name="zone">	The zone to search in. </param>
		/// <param name="name">	The zone variant name to search for. </param>
		///
		/// <returns>	The zone variant index if found, otherwise -1. </returns>
		static byte GetZoneVariant(const s_scenario_faux_zone& zone, cstring name)
		{
			for(int x = 0; x < zone.variants.Count; x++)
			{
				if (_stricmp(name, zone.variants[x].name) != 0)
				{
					continue;
				}
				return x;
			}
			return NONE;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the variant of the current sky zone. </summary>
		///
		/// <param name="variant_name">	Variant to change the current sky zone to. </param>
		///
		/// <returns>	Returns true of the variant was set successfully. </returns>
		bool SwitchCurrentZoneVariant(cstring variant_name)
		{
			if (!g_faux_zone_globals.m_flags.m_system_enabled)
			{
				return false;
			}

			// Get the current sky zone
			const byte zone_index = g_faux_zone_globals.m_bsp_zone_map[Scenario::ScenarioInfo::BSPInfoIndex()];
			if (zone_index == 0xFF)
			{
				return false;
			}
			const s_scenario_faux_zone& zone = Scenario::ScenarioInfo::ScenarioInfo()->faux_zones[zone_index];

			// Find a matching variant
			const byte variant_index = GetZoneVariant(zone, variant_name);
			if (variant_index == 0xFF)
			{
				return false;
			}

			SetFauxZoneVariant(zone_index, variant_index);
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the variant of a specific BSP zone. </summary>
		///
		/// <param name="zone_name">   	Zone to change the variant of. </param>
		/// <param name="variant_name">	Variant to change the zone to. </param>
		///
		/// <returns>	Returns true of the variant was set successfully. </returns>
		bool SwitchZoneVariant(cstring zone_name, cstring variant_name)
		{
			if (!g_faux_zone_globals.m_flags.m_system_enabled)
			{
				return false;
			}

			// Find a matching bsp zone
			const byte zone_index = GetZone(zone_name);
			if (zone_index == 0xFF)
			{
				return false;
			}
			const s_scenario_faux_zone& zone = Scenario::ScenarioInfo::ScenarioInfo()->faux_zones[zone_index];

			// Find a matching variant
			const byte variant_index = GetZoneVariant(zone, variant_name);
			if (variant_index == 0xFF)
			{
				return false;
			}

			SetFauxZoneVariant(zone_index, variant_index);
			return true;
		}
	};
};
#endif