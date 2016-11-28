/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/physics/point_physics.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_physics_globals
		{
			real gravity;
		private:
			real water_density;
			real air_density;
		public:

			// Halo's normal gravity value constant
			static const real	GravityConstant()		{ return 3.5651792e-3f; }
			static const real	WaterDensityConstant()	{ return 1.0f; }
			static const real	AirDensityConstant()	{ return 0.0011f; }

			void SetGravityScale(real scale)
			{
				gravity = GravityConstant() * scale;
			}
			void SetWaterDensity(real density)
			{
				PointPhysics()->SetWaterMass(water_density = density);
			}
			void SetAirDensity(real density)
			{
				PointPhysics()->SetAirMass(air_density = density);
			}

			void Reset()
			{
				gravity = GravityConstant();
				SetWaterDensity(WaterDensityConstant());
				SetAirDensity(AirDensityConstant());
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_physics_globals) == 0xC );

		// Reference to the current platform's physics globals
		s_physics_globals*			Physics();
	};
};