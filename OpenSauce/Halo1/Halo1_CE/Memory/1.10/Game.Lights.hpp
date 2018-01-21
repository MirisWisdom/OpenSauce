/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <Game/Lights.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Lights
	{
		ENGINE_DPTR(lights_data_t, lights, DUO_PTR(K_LIGHTS));
		ENGINE_DPTR(s_lights_globals_data, light_game_globals, DUO_PTR(K_LIGHT_GAME_GLOBALS));
		ENGINE_DPTR(s_light_cluster_data, light_cluster, DUO_PTR(K_LIGHT_CLUSTER));
		ENGINE_DPTR(cluster_light_reference_data_t, cluster_light_reference, DUO_PTR(K_CLUSTER_LIGHT_REFERENCE));
		ENGINE_DPTR(light_cluster_reference_data_t, light_cluster_reference, DUO_PTR(K_LIGHT_CLUSTER_REFERENCE));
	}
}
