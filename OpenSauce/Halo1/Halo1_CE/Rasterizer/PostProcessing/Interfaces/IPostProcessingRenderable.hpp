/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing 
	{
		interface IPostProcessingRenderable
			abstract
		{
			virtual bool Render(Enums::postprocess_render_stage render_stage) { return false; }
		};
	};};
};
#endif