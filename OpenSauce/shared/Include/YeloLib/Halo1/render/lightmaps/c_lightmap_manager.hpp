/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
    namespace Render
    {
        class i_render_device;

        namespace Lightmaps
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
                    datum_index standard_tag_index;
                    datum_index directional_tag_indices[3];
                } m_current_lightmaps;

            public:
                bool HasLightmaps(const lightmap_type_flags flag) const;

                void SetLightmapDatums(const datum_index standard_tag_index
                    , const datum_index directional_1_tag_index
                    , const datum_index directional_2_tag_index
                    , const datum_index directional_3_tag_index);

                void SetLightmapSamplers(i_render_device& device
                    , const int32 lightmap_index
                    , const bool use_directional);

                datum_index GetStandardLightmapTagIndex() const;
            };
        };
    };
};
