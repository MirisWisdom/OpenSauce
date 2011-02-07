/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
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

//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#if __EL_INCLUDE_FILE_ID == __EL_EFFECTS_EFFECTS
namespace GameState
{
	ENGINE_DPTR(t_contrail_data, _Contrails,					0x65E374);
	ENGINE_DPTR(t_contrail_point_data, _ContrailPoints,			0x65E370);
	ENGINE_DPTR(t_effect_data, _Effects,						0x65E36C);
	ENGINE_DPTR(t_effect_location_data, _EffectLocations,		0x65E368);
	ENGINE_DPTR(t_particle_system_data, _ParticleSystems,		0x6D9E34);
	ENGINE_DPTR(t_particles_data, _Particles,					0x6DA6A8);
	ENGINE_DPTR(t_particle_emitter_data, _ParticleEmitters,		0x6DA6AC);
	ENGINE_DPTR(t_particle_location_data, _ParticleLocations,	0x6DA6B0);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_EFFECTS
#endif