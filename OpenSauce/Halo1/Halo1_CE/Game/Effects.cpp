/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Effects.hpp"

#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Effects
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_EFFECTS
#include "Memory/_EngineLayout.inl"

		t_contrail_data* Contrails()									DPTR_IMP_GET(contrails);
		t_contrail_point_data* ContrailPoints()							DPTR_IMP_GET(contrail_points);
		t_particle_data* Particles()									DPTR_IMP_GET(particles);
		t_effect_data* Effects()										DPTR_IMP_GET(effects);
		t_effect_location_data* EffectLocations()						DPTR_IMP_GET(effect_locations);
		t_particle_systems_data* ParticleSystems()						DPTR_IMP_GET(particle_systems);
		t_particle_system_particles_data* ParticleSystemParticles()		DPTR_IMP_GET(particle_system_particles);

		s_structure_detail_objects_data* StructureDetailObjects()		DPTR_IMP_GET(structure_detail_objects);
		s_structure_decals_data* StructureDecals()						DPTR_IMP_GET(structure_decals);
		s_breakable_surface_globals_data* BreakableSurfaceGlobals()		DPTR_IMP_GET(breakable_surface_globals);
		t_decals_data* Decals()											DPTR_IMP_GET(decals);
		s_decal_globals_data* DecalGlobals()							DPTR_IMP_GET(decal_globals);
		s_decal_vertex_cache_data* DecalVertexCache()					DPTR_IMP_GET(decal_vertex_cache);


		static void InitializePerMapUpgradesParticles()
		{
			return;
#if PLATFORM_IS_USER
			struct s_rendered_particle
			{
				int16 particle_index;
				int16 particle_datum_field_04;
				int16 particle_datum_field_2C;
				bool particle_datum_flags_bit5;
				PAD8;
			}; BOOST_STATIC_ASSERT( sizeof(s_rendered_particle) == 8 );
			static s_rendered_particle render_particles__rendered_particles[Enums::k_maximum_number_of_particles_per_map_upgrade];

			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES)) = 
				Enums::k_maximum_number_of_particles_per_map_upgrade;
#endif
		}
		static void InitializePerMapUpgrades()
		{
			InitializePerMapUpgradesParticles();
#if PLATFORM_IS_USER
			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS)) = 
				Enums::k_maximum_number_of_effects_per_map_upgrade;
			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS)) = 
				Enums::k_maximum_number_of_effect_locations_per_map_upgrade;

			*CAST_PTR(size_t*, GET_FUNC_VPTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES)) = 
				Enums::k_maximum_number_of_particle_system_particles_per_map_upgrade;
#endif
		}
		void Initialize()
		{
			if(GameState::YeloGameStateEnabled())
				InitializePerMapUpgrades();

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