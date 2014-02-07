/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_manager.hpp>

namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the active lightmap set. </summary>
		///
		/// <param name="lightmap_set">	Pointer to the lightmap set to change to. </param>
		void c_lightmap_manager::SetLightmapSet(const TagGroups::s_scenario_information_bsp_lightmap_set* lightmap_set)
		{
			m_flags = Flags::_render_lightmaps_flags_none;

			if (!lightmap_set)
			{
				return;
			}

			// Update the current lightmap bitmap datums
			m_current_lightmaps.standard = lightmap_set->standard_lightmap.tag_index;

			m_current_lightmaps.directional[0] = lightmap_set->directional_lightmap_1.tag_index;
			m_current_lightmaps.directional[1] = lightmap_set->directional_lightmap_2.tag_index;
			m_current_lightmaps.directional[2] = lightmap_set->directional_lightmap_3.tag_index;

			// We shouldn't have a case where only the first DLM is present, map should not compile without all 3
			if (m_current_lightmaps.standard != datum_index::null)
			{
				m_flags = (Flags::render_lightmaps_flags)(m_flags | Flags::_render_lightmaps_flags_standard);
			}
			if (m_current_lightmaps.directional[0] != datum_index::null)
			{
				m_flags = (Flags::render_lightmaps_flags)(m_flags | Flags::_render_lightmaps_flags_directional);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns true if the lightmap set has certain lightmap types. </summary>
		///
		/// <param name="flag">	The lightmap type. </param>
		///
		/// <returns>	true if if has the requested type, false if not. </returns>
		bool c_lightmap_manager::HasLightmaps(const Flags::render_lightmaps_flags flag)
		{
			return (m_flags & flag) == flag;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the standard lightmap datum by reference. </summary>
		///
		/// <param name="standard">	[out] Datum index reference the standard lightmap datum is set to. </param>
		void c_lightmap_manager::GetCurrentLightmapDatums(datum_index& standard)
		{
			standard = m_current_lightmaps.standard;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns the directonal lightmap datums by reference. </summary>
		///
		/// <param name="directional_1">	[in,out] Datum index reference the directional lightmap 1 datum is set to. </param>
		/// <param name="directional_2">	[in,out] Datum index reference the directional lightmap 2 datum is set to. </param>
		/// <param name="directional_3">	[in,out] Datum index reference the directional lightmap 3 datum is set to. </param>
		void c_lightmap_manager::GetCurrentLightmapDatums(datum_index& directional_1, datum_index& directional_2, datum_index& directional_3)
		{
			directional_1 = m_current_lightmaps.directional[0];
			directional_2 = m_current_lightmaps.directional[1];
			directional_3 = m_current_lightmaps.directional[2];
		}

		void c_lightmap_manager::ClearLightmapDatums()
		{
			// Reset to initial values
			m_flags = Flags::_render_lightmaps_flags_none;

			m_current_lightmaps.standard = datum_index::null;
			m_current_lightmaps.directional[0] = datum_index::null;
			m_current_lightmaps.directional[1] = datum_index::null;
			m_current_lightmaps.directional[2] = datum_index::null;
		}
	};};
};