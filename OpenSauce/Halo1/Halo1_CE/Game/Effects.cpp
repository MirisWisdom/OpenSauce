/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Effects.hpp"

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Effects
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_EFFECTS
#include "Memory/_EngineLayout.inl"

		contrail_data_t& Contrails()									DPTR_IMP_GET_BYREF(contrails);
		contrail_point_data_t& ContrailPoints()							DPTR_IMP_GET_BYREF(contrail_points);
		particle_data_t& Particles()									DPTR_IMP_GET_BYREF(particles);
		effect_data_t& Effects()										DPTR_IMP_GET_BYREF(effects);
		effect_location_data_t& EffectLocations()						DPTR_IMP_GET_BYREF(effect_locations);
		particle_systems_data_t& ParticleSystems()						DPTR_IMP_GET_BYREF(particle_systems);
		particle_system_particles_data_t& ParticleSystemParticles()		DPTR_IMP_GET_BYREF(particle_system_particles);

		s_structure_detail_objects_data* StructureDetailObjects()		DPTR_IMP_GET(structure_detail_objects);
		s_structure_decals_data* StructureDecals()						DPTR_IMP_GET(structure_decals);
		s_breakable_surface_globals_data* BreakableSurfaceGlobals()		DPTR_IMP_GET(breakable_surface_globals);
		decals_data_t* Decals()											DPTR_IMP_GET(decals);
		s_decal_globals_data* DecalGlobals()							DPTR_IMP_GET(decal_globals);
		s_decal_vertex_cache_data* DecalVertexCache()					DPTR_IMP_GET(decal_vertex_cache);


#include <YeloLib/Halo1/render/render_particles_upgrades.inl>
		static void InitializePerMapUpgrades()
		{
#if PLATFORM_IS_USER
			render_particles_mods::Initialize();

			// For now, we're leaving these upgrades off
	#if !PLATFORM_DISABLE_UNUSED_CODE
			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS)) = 
				Enums::k_maximum_number_of_effects_per_map_upgrade;
			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS)) = 
				Enums::k_maximum_number_of_effect_locations_per_map_upgrade;

			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES)) = 
				Enums::k_maximum_number_of_particle_system_particles_per_map_upgrade;
	#endif
#endif
		}
		void Initialize()
		{
#if PLATFORM_IS_USER
			if(!CMDLINE_GET_PARAM(no_os_gfx).ParameterSet())
				InitializePerMapUpgrades();
#endif

#if !PLATFORM_DISABLE_UNUSED_CODE
			Memory::CreateHookRelativeCall(&Effects::Update, GET_FUNC_VPTR(EFFECTS_UPDATE_HOOK), Enums::_x86_opcode_retn);
#endif
		}

		void Dispose()
		{
		}

		void PLATFORM_API Update()
		{
		}
	};
};