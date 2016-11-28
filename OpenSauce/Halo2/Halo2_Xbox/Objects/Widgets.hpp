/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_widgets_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<s_widgets_datum, 64> t_widgets_data;
		t_widgets_data* _Widgets();

		struct s_antenna_datum : TStructImpl(700)
		{
		};
		typedef Memory::DataArray<s_antenna_datum, 12> t_antenna_data;
		t_antenna_data* _Antenna();

		struct s_cloth_datum : TStructImpl(4696)
		{
		};
		typedef Memory::DataArray<s_cloth_datum, 8> t_cloth_data;
		t_cloth_data* _Cloth();

		struct s_liquid_datum : TStructImpl(272)
		{
		};
		typedef Memory::DataArray<s_liquid_datum, 64> t_liquid_data;
		t_liquid_data* _Liquid();

		struct s_lights_datum : TStructImpl(272)
		{
		};
		typedef Memory::DataArray<s_lights_datum, 350> t_lights_data;
		t_lights_data* _Lights();

		struct s_light_globals
		{
			bool CanUseFlashlights;
			PAD24;
			uint32 Time;
			bool RenderLights;
			PAD24;
		};
		s_light_globals* _LightGlobals();

		struct s_light_cluster_datum : TStructImpl(12)
		{
		};
		struct s_light_cluster_data : Memory::DataArray<s_light_cluster_datum, 2048>
		{
			byte UNKNOWN(0)[256];
		};
		struct s_light_clusters
		{
			datum_index References[512];
			s_light_cluster_data Clusters[2];
		};
		s_light_clusters* _LightClusters();
	};
};