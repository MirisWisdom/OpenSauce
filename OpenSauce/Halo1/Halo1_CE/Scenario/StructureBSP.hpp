/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace StructureBSP
	{
		void Initialize();
		void Dispose();

		void InitializeForNewGameState();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void InitializeForNewBSP();
		void DisposeFromOldBSP();

		void HandleGameStateLifeCycle(_enum life_state);
	};
};