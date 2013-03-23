/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		// Saves the current camera location to "camera.txt"
		void PLATFORM_API director_save_camera();

		// Loads the camera location from "camera.txt" and places the camera there
		void PLATFORM_API director_load_camera();
	};
};