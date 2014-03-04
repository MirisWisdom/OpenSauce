/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
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
			Flags::render_lightmaps_flags m_available_lightmaps;
			Flags::render_lightmaps_flags m_used_lightmaps;
			PAD16;

			struct s_lightmap_datums
			{
				datum_index standard;					//!< Datum index for the current standard lightmap.
				datum_index directional[3];				//!< Datum indices for the current directional lightmaps.
			}m_current_lightmaps;

		public:
			typedef TagGroups::s_bitmap_data* (*t_get_bitmap_data_func)(datum_index, int32);

			bool HasLightmaps(const Flags::render_lightmaps_flags flag);
			bool UsingLightmaps(const Flags::render_lightmaps_flags flag);

			void SetLightmapDatums(datum_index standard
				, datum_index directional_1
				, datum_index directional_2
				, datum_index directional_3);
			void SetLightmapSamplers(LPDIRECT3DDEVICE9 device
				, int32 lightmap_index
				, t_get_bitmap_data_func get_bitmap_data);
		};
	};};
};