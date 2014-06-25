/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Render { namespace Lightmaps
	{
		void Initialize();
		void Dispose();

		bool UsingDirectionalLightmaps();
		
		void SetLightmaps(const datum_index standard);
		void SetLightmaps(const datum_index standard, const datum_index directional_1, const datum_index directional_2, const datum_index directional_3);
	};};
};
#endif