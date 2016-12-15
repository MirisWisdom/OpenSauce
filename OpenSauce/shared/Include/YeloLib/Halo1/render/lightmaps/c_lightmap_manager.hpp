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
	namespace Render { namespace Lightmaps
	{
		class c_lightmap_manager final
		{
		public:
			enum lightmap_type_flags : word_flags
			{
				_lightmap_type_flags_standard_bit = 1,
				_lightmap_type_flags_directional_bit = 2,
			};

		private:
			lightmap_type_flags m_available_lightmaps;

			struct s_lightmap_tag_indices
			{
				datum_index standard_tag_index;					//!< Tag index for the current standard lightmap.
				datum_index directional_tag_indices[3];			//!< Tag indices for the current directional lightmaps.
			}m_current_lightmaps;

		public:
			typedef TagGroups::s_bitmap_data* (API_FUNC *proc_get_bitmap_data)(const datum_index, const int32);

			bool HasLightmaps(const lightmap_type_flags flag) const;

			void SetLightmapDatums(const datum_index standard_tag_index
				, const datum_index directional_1_tag_index
				, const datum_index directional_2_tag_index
				, const datum_index directional_3_tag_index);
			void SetLightmapSamplers(LPDIRECT3DDEVICE9 device
				, const int32 lightmap_index
				, const bool use_directional
				, const proc_get_bitmap_data get_bitmap_data);

			datum_index GetStandardLightmapTagIndex();
		};
	};};
};