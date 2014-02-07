/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/Halo1/open_sauce/settings/yelo_settings_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum render_lightmaps_flags : byte
		{
			_render_lightmaps_flags_none = 0,
			_render_lightmaps_flags_standard = 1,
			_render_lightmaps_flags_directional = 2,
		};
	};
	namespace Render { namespace Lightmaps
	{
		class c_lightmap_manager
		{
			Flags::render_lightmaps_flags m_flags;
			PAD24;

			struct s_lightmap_datums
			{
				datum_index standard;					//!< Datum index for the current standard lightmap.
				datum_index directional[3];				//!< Datum indices for the current directional lightmaps.
			}m_current_lightmaps;

		public:
			void SetLightmapSet(const TagGroups::s_scenario_information_bsp_lightmap_set* lightmap_set);

			bool HasLightmaps(const Flags::render_lightmaps_flags flag);

			void GetCurrentLightmapDatums(datum_index& standard);
			void GetCurrentLightmapDatums(datum_index& directional_1, datum_index& directional_2, datum_index& directional_3);
			void ClearLightmapDatums();
		};
	};};
};