/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_sequence_controller.hpp>

namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Calls the set lightmap set callback to change the lightmap set in the lightmap manager. </summary>
		///
		/// <param name="index">	Index of the lightmap set to change to. </param>
		void c_lightmap_sequence_controller::ChangeLightmapSet(const int16 index)
		{
			YELO_ASSERT_DISPLAY(m_set_lightmap_set_func, "No lightmap change callback function set");
			YELO_ASSERT_DISPLAY(m_current_lightmap.bsps, "Scenario bsp info's pointer not set");

			if (index == NONE)
			{
				m_set_lightmap_set_func(nullptr);
			}
			else
			{
				const TagGroups::s_scenario_information_bsp bsp = (*m_current_lightmap.bsps)[m_current_lightmap.bsp_info_index];
				m_set_lightmap_set_func(&bsp.lightmap_sets[index]);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Performs logic whenever a sequence instance is updated. </summary>
		///
		/// <remarks>	Currently unused, potential for lightmap blending and such here. </remarks>
		///
		/// <param name="instance">	The sequence instance being updated. </param>
		/// <param name="progress">	The progress on the instances current sequence block. </param>
		void c_lightmap_sequence_controller::OnTransitionUpdate(const int16 instance, const real progress)
		{
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the current lightmap set to the set from the current sequence block. </summary>
		///
		/// <param name="instance">	The sequence instance being changed. </param>
		/// <param name="index">   	The sequence index that has been changed to. </param>
		void c_lightmap_sequence_controller::OnSequenceChanged(int16 instance, byte index)
		{
			// Only change the current lightmap set if the instance is the active one
			if (!instance == m_current_lightmap.bsp_info_index)
			{
				return;
			}

			const TagGroups::s_scenario_information_bsp bsp = (*m_current_lightmap.bsps)[m_current_lightmap.bsp_info_index];
			m_current_lightmap.sequence = &bsp.lightmap_sequences[index];

			YELO_ASSERT_DISPLAY(m_current_lightmap.sequence->lightmap_set_1 != NONE, "A lightmap set's first lightmap index is NONE");
			ChangeLightmapSet(m_current_lightmap.sequence->lightmap_set_1);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns a sequence block pointer from the requested bsp info. </summary>
		///
		/// <param name="instance">	The sequence instance the request is for. </param>
		/// <param name="index">   	Index of the sequence block being requested. </param>
		///
		/// <returns>	Returns a pointer to the requested sequence block. </returns>
		const TagGroups::s_transition_sequence_block* c_lightmap_sequence_controller::GetSequenceBlock(int16 instance, byte index)
		{
			const TagGroups::s_scenario_information_bsp bsp = (*m_current_lightmap.bsps)[instance];
			return &bsp.lightmap_sequences[index];
		}

		/// <summary>	Resets the sequence controller. </summary>
		void c_lightmap_sequence_controller::Ctor()
		{
			c_transition_sequence_controller::Ctor();

			m_current_lightmap.bsp_info_index = NONE;
			m_current_lightmap.bsps = nullptr;
			m_current_lightmap.set = nullptr;
			m_current_lightmap.sequence = nullptr;
		}

		/// <summary>	Resets the sequence controller. </summary>
		void c_lightmap_sequence_controller::Dtor()
		{
			c_transition_sequence_controller::Dtor();

			DisposeFromOldBSP();
			DisposeFromOldMap();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the sequence controllers pointer to the maps BSP info tag block. </summary>
		///
		/// <param name="bsps">	Pointer to the maps BSP info tag block. </param>
		void c_lightmap_sequence_controller::InitializeNewMap(const TagBlock<const TagGroups::s_scenario_information_bsp>* bsps)
		{
			m_current_lightmap.bsps = bsps;
		}

		void c_lightmap_sequence_controller::DisposeFromOldMap()
		{
			m_current_lightmap.bsp_info_index = NONE;
			m_current_lightmap.bsps = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the current BSP info index and changes to the associated lightmap set. </summary>
		///
		/// <param name="bsp_info_index">	The BSP info index to switch to. </param>
		void c_lightmap_sequence_controller::InitializeNewBSP(int16 bsp_info_index)
		{
			m_current_lightmap.bsp_info_index = bsp_info_index;

			// change the current sky set index to that stored in the gamestate
			if (m_current_lightmap.bsp_info_index == NONE)
			{
				ChangeLightmapSet(NONE);
			}
			else
			{
				ChangeLightmapSet(m_gamestate.sequence_instance_states[bsp_info_index].m_index);
			}
		}

		void c_lightmap_sequence_controller::DisposeFromOldBSP()
		{
			m_current_lightmap.set = nullptr;
			m_current_lightmap.sequence = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the set lightmap set callback function. </summary>
		///
		/// <param name="set_func">	The lightmap set function. </param>
		void c_lightmap_sequence_controller::SetLightmapSetCallback(t_set_lightmap_set_func set_func)
		{
			m_set_lightmap_set_func = set_func;
		}
	}; };
};