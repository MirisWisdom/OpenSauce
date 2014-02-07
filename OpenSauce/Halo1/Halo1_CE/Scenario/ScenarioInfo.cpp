/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Scenario/ScenarioInfo.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Scenario/Scenario.hpp"
#include "Rasterizer/Sky.hpp"

namespace Yelo
{
	namespace Scenario { namespace ScenarioInfo
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_SCENARIO
#define __EL_INCLUDE_FILE_ID	__EL_SCENARIO_SCENARIOINFO
#include "Memory/_EngineLayout.inl"

		struct s_scenario_info_globals
		{
			const TagGroups::s_project_yellow_scenario_information*		m_scenario_info;	//!< Pointer to the maps scenario info block.
			int16														m_bsp_info_index;	//!< Index of the current BSP info block.
			PAD16;
			const TagGroups::s_scenario_information_bsp*				m_bsp_info;			//!< Pointer to the current BSP info block.
		};
		static s_scenario_info_globals g_scenario_info_globals;								//!< Contains the global variables for the scenario info system.

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scenario information block. </summary>
		///
		/// <returns>	Returns a pointer to the maps scenario info block. </returns>
		const TagGroups::s_project_yellow_scenario_information* ScenarioInfo()
		{
			return g_scenario_info_globals.m_scenario_info;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if this object has a bsp information block. </summary>
		///
		/// <returns>	Returns true if the scenario info has at least 1 BSP info block. </returns>
		const bool HasBSPInfo()
		{
			if (!g_scenario_info_globals.m_scenario_info)
			{
				return false;
			}
			else
			{
				return g_scenario_info_globals.m_scenario_info->bsps.Count >= 1;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bsp information index. </summary>
		///
		/// <returns>	Returns the index of the current BSP info block. </returns>
		const int16 BSPInfoIndex()
		{
			return g_scenario_info_globals.m_bsp_info_index;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Bsp information block pointer. </summary>
		///
		/// <returns>	Returns a pointer to the current BSP info block. </returns>
		const TagGroups::s_scenario_information_bsp* BSPInfo()
		{
			return g_scenario_info_globals.m_bsp_info;
		}

		/// <summary>	Sets up the scenario info system for a new map. </summary>
		void InitializeForNewMap()
		{
			// See whether the map has a project yellow tag and a scenario info block
			if(TagGroups::_global_yelo && (TagGroups::_global_yelo->scenario.Count == 1))
			{
				g_scenario_info_globals.m_scenario_info = &TagGroups::_global_yelo->scenario[0];
			}
			else
			{
				g_scenario_info_globals.m_scenario_info = nullptr;
			}

			// Initialize for the current BSP here as this gets called after InitializeForBSPLoad on map load
			InitializeForNewBSP();
		}

		/// <summary>	Resets the scenario info system. </summary>
		void DisposeFromOldMap()
		{
			DisposeFromOldBSP();

			g_scenario_info_globals.m_scenario_info = nullptr;
		}

		/// <summary>	Resets the BSP info variables, setting up for a new BSP if applicable. </summary>
		void InitializeForNewBSP()
		{
			// Reset bsp info pointer and index
			g_scenario_info_globals.m_bsp_info_index = NONE;
			g_scenario_info_globals.m_bsp_info = nullptr;

			if (!g_scenario_info_globals.m_scenario_info)
			{
				return;
			}

			// Get the tag index for the current bsp
			int16 bsp_index = Scenario::StructureBspIndex();
			if (bsp_index == NONE)
			{
				return;
			}

			datum_index current_bsp = Scenario::Scenario()->structure_bsps[bsp_index].structure_bsp.tag_index;

			// Find a bsp info for the current bsp
			for(int i = 0; i < g_scenario_info_globals.m_scenario_info->bsps.Count; i++)
			{
				const TagGroups::s_scenario_information_bsp& bsp_info = g_scenario_info_globals.m_scenario_info->bsps[i];

				if(bsp_info.bsp.tag_index == current_bsp)
				{
					// Set the bsp info pointer and index
					g_scenario_info_globals.m_bsp_info_index = i;
					g_scenario_info_globals.m_bsp_info = &bsp_info;
					break;
				}
			}
			YELO_ASSERT_DISPLAY(g_scenario_info_globals.m_bsp_info, "Switched to a BSP that does not have a BSP info");
		}

		/// <summary>	Resets the BSP info variables. </summary>
		void DisposeFromOldBSP()
		{
			g_scenario_info_globals.m_bsp_info_index = NONE;
			g_scenario_info_globals.m_bsp_info = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Resets the sky info variables, setting up for a new sky if applicable. </summary>
		///
		/// <remarks>
		/// 	This gets called multiple times so is not a "new" sky function. Only 1 system uses this so does not justify a
		/// 	seperate system component setup.
		/// </remarks>
		static void InitializeForRenderSky()
		{
#if !PLATFORM_IS_DEDI
			// Initialize sky dependent systems
			Render::Sky::InitializeForRenderSky();
#endif
		}

		/// <summary>	Inserts a hook for render sky initialization. </summary>
		void Initialize()
		{
			Memory::CreateHookRelativeCall(&InitializeForRenderSky, GET_FUNC_VPTR(INITIALIZE_RENDER_SKY_CALL), Enums::_x86_opcode_ret);
		}

		/// <summary>	Unused. </summary>
		void Dispose()
		{
		}
	};};
};