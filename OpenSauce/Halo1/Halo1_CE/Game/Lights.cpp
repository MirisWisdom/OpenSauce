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
#include "Common/Precompile.hpp"
#include "Game/Lights.hpp"

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Lights
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_LIGHTS
#include "Memory/_EngineLayout.inl"

		t_lights_data* Lights()									DPTR_IMP_GET(lights);
		s_lights_globals_data* LightsGlobals()					DPTR_IMP_GET(light_game_globals);
		s_light_cluster_data* LightCluster()					DPTR_IMP_GET(light_cluster);
		t_cluster_light_reference_data* ClusterLightReference()	DPTR_IMP_GET(cluster_light_reference);
		t_light_cluster_reference_data* LightClusterReference()	DPTR_IMP_GET(light_cluster_reference);
	};
};