/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/rasterizer/rasterizer.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
		struct s_rasterizer_resolution
		{
			DWORD			width;
			DWORD			height;
			wchar_t			resolution_string[16];
			DWORD			refresh_rate_count;
			DWORD			refresh_rates[8];
		}; BOOST_STATIC_ASSERT(sizeof(s_rasterizer_resolution) == 0x4C);

		void Initialize();
		void Dispose();

		void Update();

		void LoadSettings(TiXmlElement* dx9_element);
		void SaveSettings(TiXmlElement* dx9_element);
	};

	namespace Render
	{
		bool IsRenderingReflection();

		void Initialize();
		void Dispose();

		void SetRenderStates();
	};
};
#endif