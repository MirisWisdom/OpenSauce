/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_point_physics_globals
		{
		private:
			real water_mass;
			real air_mass;

			static const real DensityToMass(real density) { return density * 118613.34f; }
		public:

			void SetWaterMass(real density)	{ water_mass = DensityToMass(density); }
			void SetAirMass(real density)	{ air_mass = DensityToMass(density); }
		};

		s_point_physics_globals*	PointPhysics();
	};
};