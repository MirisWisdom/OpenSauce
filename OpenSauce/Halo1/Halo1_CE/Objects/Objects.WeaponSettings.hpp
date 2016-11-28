/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		namespace Weapon
		{
			void Initialize();
			void Dispose();

			real_vector3d GetWeaponPosition();
			void SetWeaponPosition(const real_vector3d& position);
		};
	};
};