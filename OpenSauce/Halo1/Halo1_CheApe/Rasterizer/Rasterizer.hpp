/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Rasterizer/Halo1/RasterizerDefinitions.hpp>

namespace Yelo
{
	namespace Rasterizer
	{
		void Initialize();
		void Dispose();
		void LoadSettings(TiXmlElement* dx9_element);
		void SaveSettings(TiXmlElement* dx9_element);
	};
};