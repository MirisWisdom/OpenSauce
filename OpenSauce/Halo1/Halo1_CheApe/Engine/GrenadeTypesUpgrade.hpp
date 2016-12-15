/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects { namespace Items {

		void GrenadeTypesUpgradeInitialize();
		void GrenadeTypesUpgradeDispose();

		void GrenadeTypesUpgradeInitializeTagDefinitions();
		void GrenadeTypesUpgradeDisposeTagDefinitions();

		void GrenadeTypesUpgrade(bool enabled);

#if PLATFORM_TYPE == PLATFORM_SAPIEN
		void InitializeGrenadesForNewMap();
#endif
	}; };
};