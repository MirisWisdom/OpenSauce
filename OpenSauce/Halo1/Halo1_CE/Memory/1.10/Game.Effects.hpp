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

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Game/Effects.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Effects
	{
		ENGINE_DPTR(contrail_data_t, contrails, DUO_PTR(K_CONTRAILS));
		ENGINE_DPTR(contrail_point_data_t, contrail_points, DUO_PTR(K_CONTRAIL_POINTS));
		ENGINE_DPTR(particle_data_t, particles, DUO_PTR(K_PARTICLES));
		ENGINE_DPTR(effect_data_t, effects, DUO_PTR(K_EFFECTS));
		ENGINE_DPTR(effect_location_data_t, effect_locations, DUO_PTR(K_EFFECT_LOCATIONS));
		ENGINE_DPTR(particle_systems_data_t, particle_systems, DUO_PTR(K_PARTICLE_SYSTEMS));
		ENGINE_DPTR(particle_system_particles_data_t, particle_system_particles, DUO_PTR(K_PARTICLE_SYSTEM_PARTICLES));
		ENGINE_DPTR(s_structure_detail_objects_data, structure_detail_objects, DUO_PTR(K_STRUCTURE_DETAIL_OBJECTS));
		ENGINE_DPTR(s_structure_decals_data, structure_decals, DUO_PTR(K_STRUCTURE_DECALS));
		ENGINE_DPTR(s_breakable_surface_globals_data, breakable_surface_globals, DUO_PTR(K_BREAKABLE_SURFACE_GLOBALS));
		ENGINE_DPTR(decals_data_t, decals, DUO_PTR(K_DECALS));
		ENGINE_DPTR(s_decal_globals_data, decal_globals, DUO_PTR(K_DECAL_GLOBALS));
		ENGINE_DPTR(s_decal_vertex_cache_data, decal_vertex_cache, K_DECAL_VERTEX_CACHE, PTR_NULL);

		FUNC_PTR(EFFECTS_UPDATE_HOOK, DUO_PTR(K_EFFECTS_UPDATE_HOOK));

		#if PLATFORM_IS_USER
		static void* K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[] = {
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_0),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_1),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_2),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_3),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_4),
		};

		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES, K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS, K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS, K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES, K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES, FUNC_PTR_NULL);
		#endif
	}
}
