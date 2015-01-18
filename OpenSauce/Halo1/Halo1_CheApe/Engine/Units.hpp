/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_SAPIEN

namespace Yelo
{
	namespace Objects { namespace Units
	{
		void Initialize();
		void Dispose();

		void InitializeForNewMap();
		void DisposeFromOldMap();
	};};
};
#endif