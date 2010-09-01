/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Lights
	{
		struct s_lights_datum : TStructImpl(124)
		{
		};
		typedef Memory::DataArray<s_lights_datum, 896> t_lights_data;
		t_lights_data*					Lights();


		struct s_lights_globals_data
		{
			bool enabled;
			PAD24;
		};
		s_lights_globals_data*			LightsGlobals();

		struct s_light_cluster_data : TStructImpl(2048)
		{
		};
		s_light_cluster_data*				LightCluster();


		struct s_cluster_light_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<s_cluster_light_reference_datum, 2048> t_cluster_light_reference_data;
		t_cluster_light_reference_data*	ClusterLightReference();


		struct s_light_cluster_reference_datum : TStructImpl(12)
		{
		};
		typedef Memory::DataArray<s_light_cluster_reference_datum, 2048> t_light_cluster_reference_data;
		t_light_cluster_reference_data*	LightClusterReference();
	};
};