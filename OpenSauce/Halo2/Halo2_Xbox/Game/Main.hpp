/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	void Initialize();
	void Dispose();
	void Update();

	namespace Game
	{
		void PrintScreen(cstring str);
		void DisplayLastError();
	};
};