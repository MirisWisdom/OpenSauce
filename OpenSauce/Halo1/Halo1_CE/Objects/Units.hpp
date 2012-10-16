/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Objects/UnitDefinitions.hpp"

namespace Yelo
{
	namespace Objects { namespace Units {
		void Initialize();
		void Dispose();
		void InitializeForNewMap();
		void DisposeFromOldMap();

		void InitializeForYeloGameState(bool enabled);
	}; };
};