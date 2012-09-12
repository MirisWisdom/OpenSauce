/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState { namespace RuntimeData {

		void Initialize();
		void Dispose();

		void InitializeForNewGameState();
		void InitializeForNewMap();

		void InitializeScripting();

		const real_vector3d* VectorValueGet(int16 value_index);

	}; };
};