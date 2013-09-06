/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Scenario/ScenarioFauxZones.hpp"

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include "Game/GameState.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Scenario/Scenario.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace ScenarioFauxZones
	{
		using namespace TagGroups;

		struct s_scenario_faux_zone_globals
		{
			bool initialized; PAD24;

			// Original tag index of the scenario skies block
			std::array<datum_index, Enums::k_maximum_skies_per_scenario>					original_skies;
			std::array<datum_index, Enums::k_maximum_structure_bsps_per_scenario_upgrade>	original_lightmap_bitmaps;

			void ChangeLightmap(structure_bsp* bsp, datum_index lightmap_bitmap_index)
			{
				bsp->lightmap_bitmaps.tag_index = lightmap_bitmap_index;
				// TODO: force the object lighting to update
			}

			void InitializeOriginalSkies( const TAG_TBLOCK_(& skies, tag_reference) )
			{
				for(int x = 0; x < skies.Count && x < (int)original_skies.size(); x++)
					original_skies[x] = skies[x].tag_index;
			}
			void RestoreOriginalSkies( TAG_TBLOCK_(& skies, tag_reference) )
			{
				for(int x = 0; x < skies.Count && x < (int)original_skies.size(); x++)
					if(!original_skies[x].IsNull())
						skies[x].tag_index = original_skies[x];
			}
			// [skies] should be the scenario's skies block
			bool ReplaceScenarioSky( TAG_TBLOCK_(& skies, tag_reference), 
				datum_index original_sky_index, datum_index new_sky_index)
			{
				for(int x = 0; x < skies.Count && x < (int)original_skies.size(); x++)
				{
					if(original_sky_index == original_skies[x])
					{
						skies[x].tag_index = new_sky_index;
						return true;
					}
				}

				return false;
			}

			void InitializeOriginalLightmaps( const TAG_TBLOCK(& bsps, scenario_structure_bsp_reference) )
			{
				for(int x = 0; x < bsps.Count && x < (int)original_lightmap_bitmaps.size(); x++)
				{
					const auto* bsp = TagGet<structure_bsp>(bsps[x].structure_bsp.tag_index);

					if(bsp != nullptr) // should never happen, but just to be safe
						original_lightmap_bitmaps[x] = bsp->lightmap_bitmaps.tag_index;
				}
			}
			void RestoreOriginalLightmaps( const TAG_TBLOCK(& bsps, scenario_structure_bsp_reference) )
			{
				for(int x = 0; x < bsps.Count && x < (int)original_lightmap_bitmaps.size(); x++)
				{
					if(original_lightmap_bitmaps[x].IsNull()) continue;

					auto* bsp = TagGetForModify<structure_bsp>(bsps[x].structure_bsp.tag_index);

					if(bsp != nullptr) // should never happen, but just to be safe
						ChangeLightmap(bsp, original_lightmap_bitmaps[x]);
				}
			}
			bool ReplaceBspLightmap(structure_bsp* bsp, datum_index new_lightmap_bitmap_index)
			{
				if(!new_lightmap_bitmap_index.IsNull())
				{
					ChangeLightmap(bsp, new_lightmap_bitmap_index);
					return true;
				}

				return false;
			}

			void Initialize(scenario* scnr)
			{
				initialized = true;

				InitializeOriginalSkies(scnr->skies);
				InitializeOriginalLightmaps(scnr->structure_bsps);
			}

			void Update()
			{
			}
		};
		static s_scenario_faux_zone_globals* scenario_faux_zone_globals;

#include "Scenario/ScenarioFauxZones.Scripting.inl"
		static void InitializeScripting()
		{
			Scripting::InitializeScriptFunction(Enums::_hs_function_scenario_faux_zones_reset, 
				scripting_scenario_faux_zones_reset_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_current_switch_variant, 
				scripting_scenario_faux_zone_current_switch_variant_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_variant, 
				scripting_scenario_faux_zone_switch_variant_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_sky, 
				scripting_scenario_faux_zone_switch_sky_evaluate);
		}
		void Initialize()
		{
			InitializeScripting();
		}
		void Dispose()
		{
		}

		void Update(real delta_time)
		{
			if(scenario_faux_zone_globals == nullptr) return;

			scenario_faux_zone_globals->Update();
		}

		void InitializeForNewGameState()
		{
			scenario_faux_zone_globals = GameState::GameStateMalloc<s_scenario_faux_zone_globals>();
		}

		void InitializeForNewMap()
		{
			if(scenario_faux_zone_globals == nullptr) return;

			if(GameState::MainGlobals()->map.reset_map)
			{
				ScenarioFauxZones::Reset();
			}
			else
				scenario_faux_zone_globals->Initialize(Scenario::Scenario());
		}

		static bool ScenarioFauxZoneSetSwitchVariantUpdateGameStateWithZoneSky(scenario* scnr,
			const s_project_yellow_scenario_information& info,
			const s_scenario_faux_zone_sky* zone_sky)
		{
			if( zone_sky == nullptr || 
				!zone_sky->IsValid() )
				return false;

			return scenario_faux_zone_globals->ReplaceScenarioSky(scnr->skies, 
				info.skies[zone_sky->scenario_sky_to_replace].tag_index,
				zone_sky->sky.tag_index);
		}

		static bool ScenarioFauxZoneSetSwitchVariantUpdateGameState(structure_bsp* bsp, scenario* scnr,
			const s_project_yellow_scenario_information& info,
			const s_scenario_faux_zone_set& zone_set,
			const s_scenario_faux_zone_set_variant& variant)
		{
			// it is assumed at this point that scenario_faux_zone_globals has been check for NULL

			const s_scenario_faux_zone_lightmap_set* lightmap_set = nullptr;
			if(variant.lightmap_index != NONE)
				lightmap_set = &zone_set.lightmaps[variant.lightmap_index];

			const s_scenario_faux_zone_sky* zone_sky = nullptr;
			if(variant.sky_index != NONE)
				zone_sky = &info.zone_skies[variant.sky_index];

			bool result = true;

			if(result && lightmap_set != nullptr)
				result &= scenario_faux_zone_globals->ReplaceBspLightmap(bsp, lightmap_set->definition.tag_index);

			if(result)
				result &= ScenarioFauxZoneSetSwitchVariantUpdateGameStateWithZoneSky(scnr, info, zone_sky);

			return result;
		}

		static bool ScenarioFauxZoneSetSwitchVariantByName(structure_bsp* bsp, scenario* scnr,
			const s_project_yellow_scenario_information& info,
			const s_scenario_faux_zone_set& zone_set,
			cstring variant_name)
		{
			const TAG_TBLOCK(& variants, s_scenario_faux_zone_set_variant) = zone_set.variants;
			for(const auto& variant : zone_set.variants)
			{
				if( _stricmp(variant.name, variant_name)==0 )
				{
					if(variant.HasGameStateChanges())
						return ScenarioFauxZoneSetSwitchVariantUpdateGameState(bsp, scnr, 
						info, zone_set, variant);

					return false;
				}
			}

			return false;
		}


		void Reset()
		{
			if(scenario_faux_zone_globals == nullptr) return;

			scenario_faux_zone_globals->RestoreOriginalSkies(Scenario::Scenario()->skies);
			scenario_faux_zone_globals->RestoreOriginalLightmaps(Scenario::Scenario()->structure_bsps);
		}
		bool SwitchCurrentZoneVariant(cstring variant_name)
		{
			if(scenario_faux_zone_globals != nullptr && _global_yelo->scenario.Count == 1)
			{
				structure_bsp* current_bsp = Scenario::StructureBsp();

				for(const auto& zone : _global_yelo->scenario[0].zones)
				{
					if(TagGet<structure_bsp>(zone.structure_bsp) == current_bsp)
					{
						return ScenarioFauxZoneSetSwitchVariantByName(current_bsp, Scenario::Scenario(),
							_global_yelo->scenario[0], zone,
							variant_name);
					}
				}
			}

			return false;
		}
		bool SwitchZoneVariant(cstring zone_name, cstring variant_name)
		{
			if(scenario_faux_zone_globals != nullptr && _global_yelo->scenario.Count == 1)
			{
				for(const auto& zone : _global_yelo->scenario[0].zones)
				{
					if(_stricmp(zone_name, zone.name) != 0) continue;

					auto* zone_bsp = TagGetForModify<structure_bsp>(zone.structure_bsp);

					if(zone_bsp != nullptr)
					{
						return ScenarioFauxZoneSetSwitchVariantByName(zone_bsp, Scenario::Scenario(),
							_global_yelo->scenario[0], zone,
							variant_name);
					}
				}
			}

			return false;
		}

		bool SwitchZoneSky(cstring zone_sky_name)
		{
			if(scenario_faux_zone_globals != nullptr && _global_yelo->scenario.Count == 1)
			{
				const TAG_TBLOCK(& zone_skies, s_scenario_faux_zone_sky) = _global_yelo->scenario[0].zone_skies;

				for(const auto& zone_sky : _global_yelo->scenario[0].zone_skies)
				{
					if(_stricmp(zone_sky_name, zone_sky.name) != 0) continue;

					return ScenarioFauxZoneSetSwitchVariantUpdateGameStateWithZoneSky(Scenario::Scenario(), 
						_global_yelo->scenario[0], &zone_sky);
				}
			}

			return false;
		}
	};
};