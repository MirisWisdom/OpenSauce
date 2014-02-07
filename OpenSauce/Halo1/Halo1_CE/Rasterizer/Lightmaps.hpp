/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		void Initialize();
		void Dispose();

		void Update(real delta);

		void InitializeForNewGameState();
		void HandleGameStateLifeCycle(_enum life_state);

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void InitializeForNewBSP();
		void DisposeFromOldBSP();

		void SetLightmapSequence(byte seq_index);
		void SetLightmapSequence(int16 bsp_index, byte seq_index);

		bool UsingDirectionalLightmaps();
	};};
};