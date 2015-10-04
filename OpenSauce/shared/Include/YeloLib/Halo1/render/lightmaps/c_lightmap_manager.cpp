/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_manager.hpp>

#include <YeloLib/render/i_render_device.hpp>

namespace Yelo
{
    namespace Render
    {
        namespace Lightmaps
        {
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Returns true if the lightmap set has certain lightmap types. </summary>
            ///
            /// <param name="flag">	The lightmap type. </param>
            ///
            /// <returns>	true if the requested type is available, false if not. </returns>
            bool c_lightmap_manager::HasLightmaps(const lightmap_type_flags flag) const
            {
                return (m_available_lightmaps & flag) == flag;
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Changes the active lightmap tag indices. </summary>
            ///
            /// <param name="standard_tag_index">	  	The standard lightmaps tag index. </param>
            /// <param name="directional_1_tag_index ">	The first directional lightmaps tag index. </param>
            /// <param name="directional_2_tag_index">	The second directional lightmaps tag index. </param>
            /// <param name="directional_3_tag_index">	The third directional lightmaps tag index. </param>
            void c_lightmap_manager::SetLightmapDatums(const datum_index standard_tag_index
                , const datum_index directional_1_tag_index
                , const datum_index directional_2_tag_index
                , const datum_index directional_3_tag_index)
            {
                m_available_lightmaps = static_cast<lightmap_type_flags>(0);

                m_current_lightmaps.standard_tag_index = standard_tag_index;
                m_current_lightmaps.directional_tag_indices[0] = directional_1_tag_index;
                m_current_lightmaps.directional_tag_indices[1] = directional_2_tag_index;
                m_current_lightmaps.directional_tag_indices[2] = directional_3_tag_index;

                if (!m_current_lightmaps.standard_tag_index.IsNull())
                {
                    m_available_lightmaps = static_cast<lightmap_type_flags>(m_available_lightmaps | _lightmap_type_flags_standard_bit);
                }

                // We shouldn't have a case where only the first DLM is present, map should not compile without all 3
                if (!m_current_lightmaps.directional_tag_indices[0].IsNull())
                {
                    m_available_lightmaps = static_cast<lightmap_type_flags>(m_available_lightmaps | _lightmap_type_flags_directional_bit);
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Sets the lightmap samplers. </summary>
            ///
            /// <param name="device">		  	The current render device. </param>
            /// <param name="lightmap_index"> 	Index of the lightmap. </param>
            /// <param name="get_bitmap_data">	Function for getting a bitmap data block. </param>
            void c_lightmap_manager::SetLightmapSamplers(i_render_device& device
                , const int32 lightmap_index
                , const bool use_directional)
            {
                if (HasLightmaps(_lightmap_type_flags_directional_bit) && use_directional)
                {
                    device.SetTextureSampler(m_current_lightmaps.directional_tag_indices[0], lightmap_index, 2, D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
                    device.SetTextureSampler(m_current_lightmaps.directional_tag_indices[1], lightmap_index, 4, D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
                    device.SetTextureSampler(m_current_lightmaps.directional_tag_indices[2], lightmap_index, 5, D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
                }
                else
                {
                    device.SetTextureSampler(m_current_lightmaps.standard_tag_index, lightmap_index, 2, D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>	Gets standard lightmap tag index. </summary>
            ///
            /// <returns>	The standard lightmap tag index. </returns>
            datum_index c_lightmap_manager::GetStandardLightmapTagIndex() const
            {
                return m_current_lightmaps.standard_tag_index;
            }

#ifdef API_DEBUG

            class c_mock_render_device : public i_render_device
            {
            public:
                std::vector<datum_index> m_datum_indices;
                std::vector<uint32> m_sampler_indices;

                void SetTextureSampler(const datum_index bitmap, const uint32 index, const _enum sampler, const uint32 uv_mode, const uint32 sample_mode) override
                {
                    m_datum_indices.push_back(bitmap);
                    m_sampler_indices.push_back(sampler);
                }
            };

#define YELO_UNIT_TEST_LIGHTMAP_MANAGER(name) YELO_UNIT_TEST(name, Yelo.Render.Lightmaps.c_lightmap_manager)

            const datum_index g_any_datum_index = datum_index::Create(0x1111, 0x2222);
            
            YELO_UNIT_TEST_LIGHTMAP_MANAGER(SetLightmapSamplers_WithStandardLightmapOnly_SetsStandardLightmap)
            {
                c_mock_render_device device;
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, datum_index::null, datum_index::null, datum_index::null);
                manager.SetLightmapSamplers(device, 0, true);

                AssertThat(device.m_datum_indices, EqualsContainer(std::vector<datum_index>{ g_any_datum_index }));
                AssertThat(device.m_sampler_indices, EqualsContainer(std::vector<uint32>{ 2 }));
            }

            YELO_UNIT_TEST_LIGHTMAP_MANAGER(SetLightmapSamplers_WithDirectionalLightmaps_SetsDirectionalLightmaps)
            {
                c_mock_render_device device;
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, g_any_datum_index, g_any_datum_index, g_any_datum_index);
                manager.SetLightmapSamplers(device, 0, true);

                AssertThat(device.m_datum_indices, EqualsContainer(std::vector<datum_index>{ g_any_datum_index, g_any_datum_index, g_any_datum_index }));
                AssertThat(device.m_sampler_indices, EqualsContainer(std::vector<uint32>{ 2, 4, 5 }));
            }

            YELO_UNIT_TEST_LIGHTMAP_MANAGER(SetLightmapSamplers_WithDirectionalLightmapsDisabled_SetsStandardLightmap)
            {
                c_mock_render_device device;
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, g_any_datum_index, g_any_datum_index, g_any_datum_index);
                manager.SetLightmapSamplers(device, 0, false);

                AssertThat(device.m_datum_indices, EqualsContainer(std::vector<datum_index>{ g_any_datum_index }));
                AssertThat(device.m_sampler_indices, EqualsContainer(std::vector<uint32>{ 2 }));
            }

            YELO_UNIT_TEST_LIGHTMAP_MANAGER(HasLightmaps_WithStandardFlagAndStandardLightmaps_ReturnsTrue)
            {
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, datum_index::null, datum_index::null, datum_index::null);
                auto result = manager.HasLightmaps(c_lightmap_manager::lightmap_type_flags::_lightmap_type_flags_standard_bit);

                AssertThat(result, Equals(true));
            }

            YELO_UNIT_TEST_LIGHTMAP_MANAGER(HasLightmaps_WithDirectionalFlagAndDirectionalLightmaps_ReturnsTrue)
            {
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, g_any_datum_index, g_any_datum_index, g_any_datum_index);
                auto result = manager.HasLightmaps(c_lightmap_manager::lightmap_type_flags::_lightmap_type_flags_directional_bit);

                AssertThat(result, Equals(true));
            }

            YELO_UNIT_TEST_LIGHTMAP_MANAGER(HasLightmaps_WithDirectionalFlagAndStandardLightmaps_ReturnsFalse)
            {
                c_lightmap_manager manager;

                manager.SetLightmapDatums(g_any_datum_index, datum_index::null, datum_index::null, datum_index::null);
                auto result = manager.HasLightmaps(c_lightmap_manager::lightmap_type_flags::_lightmap_type_flags_directional_bit);

                AssertThat(result, Equals(false));
            }

#undef YELO_UNIT_TEST_LIGHTMAP_MANAGER
#endif
        };
    };
};
