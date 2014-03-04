/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_manager.hpp>

#include <blamlib/Halo1/rasterizer/dx9/rasterizer_dx9.hpp>

namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns true if the lightmap set has certain lightmap types. </summary>
		///
		/// <param name="flag">	The lightmap type. </param>
		///
		/// <returns>	true if the requested type is available, false if not. </returns>
		bool c_lightmap_manager::HasLightmaps(const Flags::render_lightmaps_flags flag)
		{
			return (m_available_lightmaps & flag) == flag;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns true if the manager has set certain lightmaps on the render device. </summary>
		///
		/// <param name="flag">	The lightmap type. </param>
		///
		/// <returns>	true if the lightmap type was sent to the device, false if not. </returns>
		bool c_lightmap_manager::UsingLightmaps(const Flags::render_lightmaps_flags flag)
		{
			return (m_used_lightmaps & flag) == flag;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Changes the active lightmap datums. </summary>
		///
		/// <param name="standard">			Teh standard lightmaps datum. </param>
		/// <param name="directional_1">	The first directional lightmaps datum. </param>
		/// <param name="directional_2">	The second directional lightmaps datum. </param>
		/// <param name="directional_3">	The third directional lightmaps datum. </param>
		void c_lightmap_manager::SetLightmapDatums(datum_index standard
			, datum_index directional_1
			, datum_index directional_2
			, datum_index directional_3)
		{
			m_available_lightmaps = Flags::_render_lightmaps_flags_none;

			// Update the current lightmap bitmap datums
			m_current_lightmaps.standard = standard;

			m_current_lightmaps.directional[0] = directional_1;
			m_current_lightmaps.directional[1] = directional_2;
			m_current_lightmaps.directional[2] = directional_3;

			if (m_current_lightmaps.standard != datum_index::null)
			{
				m_available_lightmaps = (Flags::render_lightmaps_flags)(m_available_lightmaps | Flags::_render_lightmaps_flags_standard);
			}
			
			// We shouldn't have a case where only the first DLM is present, map should not compile without all 3
			if (m_current_lightmaps.directional[0] != datum_index::null)
			{
				m_available_lightmaps = (Flags::render_lightmaps_flags)(m_available_lightmaps | Flags::_render_lightmaps_flags_directional);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap samplers. </summary>
		///
		/// <param name="device">		  	The current render device. </param>
		/// <param name="lightmap_index"> 	Index of the lightmap. </param>
		/// <param name="get_bitmap_data">	Function for getting a bitmap data block. </param>
		void c_lightmap_manager::SetLightmapSamplers(LPDIRECT3DDEVICE9 device
			, int32 lightmap_index
			, t_get_bitmap_data_func get_bitmap_data)
		{
			m_used_lightmaps = Flags::_render_lightmaps_flags_none;

			auto set_sampler = 
				[&](datum_index bitmap_datum, int32 sampler)
				{
					auto bitmap_data = get_bitmap_data(bitmap_datum, lightmap_index);
					blam::rasterizer_set_texture_bitmap_data((_enum)sampler, bitmap_data);
					
					device->SetSamplerState(sampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
					device->SetSamplerState(sampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
					device->SetSamplerState(sampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
				};

			if(HasLightmaps(Flags::_render_lightmaps_flags_directional))
			{
				// The map does have directional lightmaps so use those
				set_sampler(m_current_lightmaps.directional[0], 2);
				set_sampler(m_current_lightmaps.directional[1], 4);
				set_sampler(m_current_lightmaps.directional[2], 5);

				m_used_lightmaps = Flags::_render_lightmaps_flags_directional;
			}
			else if(HasLightmaps(Flags::_render_lightmaps_flags_standard))
			{
				// The map does not have directional lightmaps, so use the standard lightmaps
				set_sampler(m_current_lightmaps.standard, 2);
						
				m_used_lightmaps = Flags::_render_lightmaps_flags_standard;
			}
		}
	};};
};