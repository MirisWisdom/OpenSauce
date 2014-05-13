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

		bool UsingDirectionalLightmaps();
		
		void SetLightmaps(datum_index standard);
		void SetLightmaps(datum_index standard, datum_index directional_1, datum_index directional_2, datum_index directional_3);
	};};
};