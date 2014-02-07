/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Lightmaps.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/rasterizer/dx9/rasterizer_dx9.hpp>
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_manager.hpp>
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_sequence_controller.hpp>

#include "Common/GameSystemDefinitions.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Scenario/Scenario.hpp"
#include "Scenario/ScenarioInfo.hpp"
#include "TagGroups/TagGroups.hpp"
#include "TagGroups/project_yellow_definitions.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "DX9/DX9.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_LIGHTMAPS
#include "Memory/_EngineLayout.inl"

	namespace Render { namespace Lightmaps
	{
		/// <summary>	Container for the lightmap globals. </summary>
		struct s_lightmap_globals
		{
			struct
			{
				/// <summary>	Flag for whether the lightmap handling system is available. </summary>
				bool system_enabled;
				/// <summary>	Flag for whether the current lightmap set uses directional lightmaps. </summary>
				bool using_directional_lightmaps;
				PAD16;
			}m_flags;

			/// <summary>	Index of the current lightmap being rendered. </summary>
			int32 m_lightmap_index;

			/// <summary>	Lightmap manager that maintains the current standard and directional lightmaps. </summary>
			c_lightmap_manager m_lightmap_manager;
			/// <summary>	Sequence controller that handles progressing through lightmap sequences over time. </summary>
			c_lightmap_sequence_controller m_lightmap_sequence_controller;
			/// <summary>	Pointer to the sequence controller's gamestate memory. </summary>
			Time::Sequence::s_sequence_instance_state* m_lightmap_sequence_gamestate;
		};
		/// <summary>	Contains all global variables of the lightmaps system. </summary>
		static s_lightmap_globals g_lightmap_globals;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Interop function so that the sequence controller can set the lightmap managers current lightmap set.
		/// </summary>
		///
		/// <param name="lightmap_set">	Pointer to the lightmap set to switch to. </param>
		static void SetLightmapSet(const TagGroups::s_scenario_information_bsp_lightmap_set* lightmap_set)
		{
			g_lightmap_globals.m_lightmap_manager.SetLightmapSet(lightmap_set);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Puts a texture from a bitmap tag into a specific texture sampler. </summary>
		///
		/// <param name="device">	   	[in] Pointer to the current render device. </param>
		/// <param name="bitmap_tag">  	Datum index of the bitmap tag containing the texture. </param>
		/// <param name="bitmap_index">	Index of the bitmap in the bitmap tag to set. </param>
		/// <param name="sampler">	   	The sampler to set. </param>
		static void SetTextureSampler(IDirect3DDevice9* device, datum_index bitmap_tag, int32 bitmap_index, int32 sampler)
		{
			TagGroups::s_bitmap_group* bitmap = nullptr;
			TagGroups::s_bitmap_data* bitmap_datum = nullptr;

			// Get the bitmap datum from the tag
			bitmap = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(bitmap_tag);
			bitmap_datum = CAST_PTR(TagGroups::s_bitmap_data*, &bitmap->bitmaps[bitmap_index]);

			// Set the sampler texture
			blam::rasterizer_set_texture_bitmap_data((_enum)sampler, bitmap_datum);

			// Set the sampler's sample type
			device->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets texture sampler 2 to the standard lightmap currently set in the lightmap manager. </summary>
		///
		/// <param name="lightmap_index">	Index of the current lightmap being rendered. </param>
		static void SetupStandardLightmap(int32 lightmap_index)
		{
			// Set the lightmap sampler for the standard lightmap
			datum_index lightmap;
			g_lightmap_globals.m_lightmap_manager.GetCurrentLightmapDatums(lightmap);

			SetTextureSampler(DX9::Direct3DDevice(), lightmap, lightmap_index, 2);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Sets texture samplers 2, 4 and 5 to the directional lightmaps currently set in the lightmap manager.
		/// </summary>
		///
		/// <param name="lightmap_index">	Index of the current lightmap being rendered. </param>
		static void SetupDirectionalLightmaps(int32 lightmap_index)
		{
			// set the 3 samplers for directional lightmaps
			datum_index lightmaps[3];
			g_lightmap_globals.m_lightmap_manager.GetCurrentLightmapDatums(lightmaps[0], lightmaps[1], lightmaps[2]);

			SetTextureSampler(DX9::Direct3DDevice(), lightmaps[0], lightmap_index, 2);
			SetTextureSampler(DX9::Direct3DDevice(), lightmaps[1], lightmap_index, 4);
			SetTextureSampler(DX9::Direct3DDevice(), lightmaps[2], lightmap_index, 5);
		}

		/// <summary>	Hook to get the current lightmap index being rendered. </summary>
		static API_FUNC_NAKED void Hook_BSPLightmapIndex()
		{
			static const uint32 RETN_ADDRESS = GET_FUNC_PTR(BSP_LIGHTMAP_INDEX_RETN);

			_asm
			{
				mov		g_lightmap_globals.m_lightmap_index, edx
				lea		ecx, [edx+edx*2]
				mov		edx, [eax+64h]

				jmp		RETN_ADDRESS
			};
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Override of the stock lightmap sampler function to handle lightmaps in OS and set directional lightmaps if
		/// 	applicable.
		/// </summary>
		///
		/// <param name="sampler_index">	Index of the texture sampler to set the texture to. </param>
		static void PLATFORM_API SetLightmapSampler(uint32 sampler_index)
		{
			static TagGroups::s_bitmap_data* original_datum = nullptr;

			//Get the bitmap datum from the esi register
			_asm mov	original_datum, esi;

			if(g_lightmap_globals.m_flags.system_enabled
				&& (g_lightmap_globals.m_lightmap_index != -1)
				&& g_lightmap_globals.m_lightmap_manager.HasLightmaps(Flags::_render_lightmaps_flags_standard))
			{
				if(g_lightmap_globals.m_lightmap_manager.HasLightmaps(Flags::_render_lightmaps_flags_directional) && Rasterizer::ShaderExtension::ExtensionsEnabled())
				{
					// The map does have directional lightmaps so use those
					SetupDirectionalLightmaps(g_lightmap_globals.m_lightmap_index);
					g_lightmap_globals.m_flags.using_directional_lightmaps = true;
				}
				else
				{
					// The map does not have directional lightmaps, so use the standard lightmaps
					SetupStandardLightmap(g_lightmap_globals.m_lightmap_index);
					g_lightmap_globals.m_flags.using_directional_lightmaps = false;
				}
			}
			else
			{
				// The system is disabled so revert to the stock implementation
				blam::rasterizer_set_texture_bitmap_data((_enum)sampler_index, original_datum);
				g_lightmap_globals.m_flags.using_directional_lightmaps = false;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Checks the scenario to see if it has a scenario info block with BSP info blocks that have lightmap sets and
		/// 	sequences.
		/// </summary>
		///
		/// <remarks>
		/// 	The checks done here are not exhaustive, checks should have been done at map build time to ensure the
		/// 	necessary data is present.
		/// </remarks>
		///
		/// <returns>	Returns true if the scenario has the required data for lightmap switching. </returns>
		static bool VerifyScenarioInfo()
		{
			//TODO: use flags set at compile time
			// See whether the map has a scenario info block
			if (!Scenario::ScenarioInfo::ScenarioInfo())
			{
				return false;
			}

			// See whether the map has a bsp info block
			if (!Scenario::ScenarioInfo::HasBSPInfo() || !Scenario::ScenarioInfo::BSPInfo())
			{
				return false;
			}

			// Check the bsp info block has at least one lightmap set and lightmap sequence
			if (Scenario::ScenarioInfo::BSPInfo()->lightmap_sets.Count == 0)
			{
				return false;
			}

			if (Scenario::ScenarioInfo::BSPInfo()->lightmap_sequences.Count == 0)
			{
				return false;
			}

			return true;
		}

		/// <summary>	Resets the lightmap sequence controllers gamestate so all bsp's use sequence 0. </summary>
		static void ResetGamestate()
		{ 
			// TODO: check result
			memset(g_lightmap_globals.m_lightmap_sequence_gamestate, 0, sizeof(Time::Sequence::s_sequence_instance_state) * Enums::k_maximum_structure_bsps_per_scenario_upgrade);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns true if directional lightmaps are in use. </summary>
		///
		/// <returns>	true if it directional lightmaps are in use, otherwise false. </returns>
		bool UsingDirectionalLightmaps()
		{
			return g_lightmap_globals.m_flags.using_directional_lightmaps;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Inserts hooks to override the stock lightmap management and initializes the lightmap manager and sequence
		/// 	controller.
		/// </summary>
		void Initialize()
		{
			// Hook lightmap set code
			Memory::WriteRelativeJmp(&Hook_BSPLightmapIndex,	GET_FUNC_VPTR(BSP_LIGHTMAP_INDEX_HOOK), true);
			Memory::WriteRelativeCall(&SetLightmapSampler,		GET_FUNC_VPTR(SET_LIGHTMAP_HANDLER_CALL), true);

			// Reset the lightmap manager and sequence controller
			g_lightmap_globals.m_lightmap_sequence_controller.SetLightmapSetCallback(&SetLightmapSet);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Resets the lightmap manager and sequence controller.
		/// </summary>
		void Dispose()
		{
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	If the lightmap system is enabled, updates the sequence controller. </summary>
		///
		/// <param name="delta">	Time in seconds since the last update. </param>
		void Update(real delta)
		{
			// Update the sequence controller
			if (g_lightmap_globals.m_flags.system_enabled)
			{
				g_lightmap_globals.m_lightmap_sequence_controller.Update(delta);
			}
		}

		/// <summary>	Allocates gamestate memory for the sequence controller. </summary>
		void InitializeForNewGameState()
		{
			Time::Sequence::s_sequence_instance_state* gamestate = GameState::GameStateMalloc<Time::Sequence::s_sequence_instance_state>(true, Enums::k_maximum_structure_bsps_per_scenario_upgrade);
			YELO_ASSERT_DISPLAY(gamestate, "Failed to allocate lightmap sequence gamestate memory");

			g_lightmap_globals.m_lightmap_sequence_gamestate = gamestate;
			ResetGamestate();

			g_lightmap_globals.m_lightmap_sequence_controller.SetGameStateAllocation(gamestate);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Runs any logic required for the individual gamestate load/save stages. </summary>
		///
		/// <param name="life_state">	The current gamestate lifecycle stage to handle. </param>
		void HandleGameStateLifeCycle(_enum life_state)
		{
			if (!g_lightmap_globals.m_flags.system_enabled)
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
				// Reset the sequence controller to values stored in the gamestate and reinitialize for the current bsp
				g_lightmap_globals.m_lightmap_sequence_controller.SetToGamestate((int16)TagGroups::_global_yelo->scenario[0].bsps.Count);
				InitializeForNewBSP();
				break;
			default:
				YELO_ASSERT_DISPLAY(false, "Invalid game state life cycle stage");
			}
		}

		/// <summary>	Resets the manager and sequence controller, setting them up for a new map if necessary. </summary>
		void InitializeForNewMap()
		{
			// Reset the lightmap manager and sequence controller
			g_lightmap_globals.m_lightmap_manager.ClearLightmapDatums();

			// See whether the map has the required tag data
			g_lightmap_globals.m_flags.system_enabled = VerifyScenarioInfo();

			if(g_lightmap_globals.m_flags.system_enabled)
			{
				g_lightmap_globals.m_lightmap_sequence_controller.InitializeNewMap(&TagGroups::_global_yelo->scenario[0].bsps);

				ResetGamestate();
				g_lightmap_globals.m_lightmap_sequence_controller.SetToGamestate((int16)TagGroups::_global_yelo->scenario[0].bsps.Count);

				// Initialize for the current BSP here, because InitializeForBSPLoad gets called before this on map load
				InitializeForNewBSP();
			}
			else
			{
				DisposeFromOldBSP();
			}
		}

		/// <summary>	Resets the lightmap manager and sequence controller. </summary>
		void DisposeFromOldMap()
		{
			g_lightmap_globals.m_flags.system_enabled = false;

			ResetGamestate();

			// Destruct the lightmap manager and sequence controller
			g_lightmap_globals.m_lightmap_manager.ClearLightmapDatums();
			g_lightmap_globals.m_lightmap_sequence_controller.DisposeFromOldMap();
		}

		/// <summary>	Sets the sequence controller's active bsp index to the current bsp. </summary>
		void InitializeForNewBSP()
		{
			if (!g_lightmap_globals.m_flags.system_enabled)
			{
				return;
			}

			// Get the BSP info for the loaded BSP
			if (Scenario::ScenarioInfo::BSPInfo())
			{
				g_lightmap_globals.m_lightmap_sequence_controller.InitializeNewBSP(Scenario::ScenarioInfo::BSPInfoIndex());
			}
			else
			{
				g_lightmap_globals.m_lightmap_sequence_controller.InitializeNewBSP(NONE);
			}
		}

		/// <summary>	Resets the manager and sequence controller, ready for the next BSP. </summary>
		void DisposeFromOldBSP()
		{
			if (!g_lightmap_globals.m_flags.system_enabled)
			{
				return;
			}

			// reset the lightmap manager and sequence controller
			g_lightmap_globals.m_lightmap_manager.ClearLightmapDatums();
			g_lightmap_globals.m_lightmap_sequence_controller.DisposeFromOldBSP();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the lightmap sequence index of the active BSP. </summary>
		///
		/// <param name="seq_index">	Sequence index to switch to. </param>
		void SetLightmapSequence(byte seq_index)
		{
			g_lightmap_globals.m_lightmap_sequence_controller.SetSequenceIndex(Scenario::ScenarioInfo::BSPInfoIndex(), seq_index);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the lightmap sequence index of a specific BSP. </summary>
		///
		/// <param name="bsp_index">	Bsp sequence index to change. </param>
		/// <param name="seq_index">	Sequence index to switch to. </param>
		void SetLightmapSequence(int16 bsp_index, byte seq_index)
		{
			g_lightmap_globals.m_lightmap_sequence_controller.SetSequenceIndex(bsp_index, seq_index);
		}
	};};
};
#endif