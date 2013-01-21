/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		bool s_damage_definition::UseInstantaneousAcceleration3D() const
		{
			return	instantaneous_acceleration3d.j != 0.0f && 
					instantaneous_acceleration3d.k != 0.0f;
		}

		void s_damage_definition::GetAcceleration(const real acceleration_scale,
			const real_vector3d& direction, real_vector3d& acceleration, 
			const real secs_per_tick) const
		{
			real acceleration_i = (instantaneous_acceleration * acceleration_scale) * secs_per_tick;
			acceleration.Set(acceleration_i, acceleration_i, acceleration_i);
			if(UseInstantaneousAcceleration3D())
			{
				acceleration.j = (instantaneous_acceleration3d.j * acceleration_scale) * secs_per_tick;
				acceleration.k = (instantaneous_acceleration3d.k * acceleration_scale) * secs_per_tick;
			}

			acceleration.i *= direction.i;
			acceleration.j *= direction.j;
			acceleration.k *= direction.k;
		}
	};
};