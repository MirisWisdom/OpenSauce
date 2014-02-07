/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/sky/c_sky_manager.hpp>

namespace Yelo
{
	namespace Render { namespace Sky
	{
		/// <summary>	Resets the sky manager. </summary>
		void c_sky_manager::Ctor()
		{
			// reset to initial values
			m_sky_datum = datum_index::null;

			m_scenario_sky_refs = nullptr;
			m_sky_override_refs = nullptr;
		}

		/// <summary>	Resets the sky manager. </summary>
		void c_sky_manager::Dtor()
		{
			// reset to initial values
			m_sky_datum = datum_index::null;

			m_scenario_sky_refs = nullptr;
			m_sky_override_refs = nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the sky managers gamestate memory pointer. </summary>
		///
		/// <param name="gamestate">	[in] Pointer to the sky managers gamestate memory. </param>
		void c_sky_manager::SetGamestate(byte* gamestate)
		{
			m_sky_index_gamestate = gamestate;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the sky managers scenario and override tag reference block pointers. </summary>
		///
		/// <param name="scenario"> 	[in] Pointer to the scenarios sky tag references block. </param>
		/// <param name="overrides">	Pointer to sky override tag reference blocks. </param>
		void c_sky_manager::SetSkyBlocks(TagBlock<tag_reference>* scenario, const TagBlock<TagGroups::s_scenario_information_sky>* overrides)
		{
			m_scenario_sky_refs = scenario;
			m_sky_override_refs = overrides;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets sky index. </summary>
		///
		/// <param name="sky_index">	Zero-based index of the sky. </param>
		void c_sky_manager::SetSkyIndex(byte sky_index)
		{
			if (sky_index == 0xFF)
			{
				return;
			}

			YELO_ASSERT_DISPLAY(m_sky_index_gamestate, "sky manager gamestate pointer not set");

			m_sky_index = sky_index;

			if(m_sky_index_gamestate[sky_index] != 0xFF)
			{
				YELO_ASSERT_DISPLAY(m_sky_override_refs, "sky override block pointer not set in sky manager");
				YELO_ASSERT_DISPLAY(m_sky_index_gamestate[sky_index] < (*m_sky_override_refs).Count, "invalid sky override index in sky manager");

				// If the override index in the gamestate is not 0xFF use the override sky
				const tag_reference& sky_ref = (*m_sky_override_refs)[m_sky_index_gamestate[sky_index]].sky;
				m_sky_datum = sky_ref.tag_index;
			}
			else
			{
				YELO_ASSERT_DISPLAY(m_scenario_sky_refs, "scenario sky block pointer not set in sky manager");

				// Ignore invalid sky index as this can occur on map change
				if (sky_index >= (*m_scenario_sky_refs).Count)
				{
					return;
				}

				// If the override index in the gamestate is 0xFF use the stock sky
				tag_reference& sky_ref = (*m_scenario_sky_refs)[sky_index];
				m_sky_datum = sky_ref.tag_index;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a sky index to be overriden with another sky index. </summary>
		///
		/// <param name="scenario_sky_index">	Zero-based index of the scenario sky. </param>
		/// <param name="override_sky_index">	Zero-based index of the override sky. </param>
		void c_sky_manager::SetSkyOverride(byte scenario_sky_index, byte override_sky_index)
		{
			// change the override index in the gamestate
			m_sky_index_gamestate[scenario_sky_index] = override_sky_index;

			// update the current sky datum if the current sky index was changed
			if ((byte)m_sky_index == scenario_sky_index)
			{
				SetSkyIndex(m_sky_index);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the current sky to use by reference. </summary>
		///
		/// <param name="sky">	[out] Datum index reference the sky datum is set to. </param>
		void c_sky_manager::GetCurrentSkyDatum(datum_index& sky)
		{
			sky = m_sky_datum;
		}
	};};
};