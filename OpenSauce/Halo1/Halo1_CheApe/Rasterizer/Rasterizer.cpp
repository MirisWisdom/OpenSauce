/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Rasterizer.hpp"
#ifndef PLATFORM_NO_DX9

#include <blamlib/Halo1/models/model_definitions.hpp>

#include "Common/YeloSettings.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_RASTERIZER
#include "Memory/_EngineLayout.inl"
	namespace Rasterizer
	{
#if PLATFORM_ID == PLATFORM_SAPIEN
	#include <YeloLib/Halo1/rasterizer/rasterizer_upgrades.inl>

		s_rasterizer_config* RasterizerConfig()		PTR_IMP_GET2(rasterizer_config);
		s_rasterizer_globals* RasterizerGlobals()	PTR_IMP_GET2(rasterizer_globals);
		s_rasterizer_debug_options* DebugOptions()	PTR_IMP_GET2(rasterizer_debug_data);
#endif

		void Initialize()
		{
			PLATFORM_VALUE(__noop, __noop,
				g_render_upgrades.Initialize());
		}

		void Dispose()
		{
			PLATFORM_VALUE(__noop, __noop,
				g_render_upgrades.Dispose());
		}

		void LoadSettings(TiXmlElement* dx9_element)
		{
			PLATFORM_VALUE(__noop, __noop,
				g_render_upgrades.LoadSettings(dx9_element));
		}

		void SaveSettings(TiXmlElement* dx9_element)
		{
			PLATFORM_VALUE(__noop, __noop,
				g_render_upgrades.SaveSettings(dx9_element));
		}
	};

	namespace Render
	{
#if PLATFORM_ID == PLATFORM_SAPIEN
		s_render_globals* RenderGlobals()			PTR_IMP_GET2(render_globals);
#endif
	};
};
#else

namespace Yelo
{
	namespace Rasterizer
	{
		void Initialize()	{}
		void Dispose()		{}
		void LoadSettings(TiXmlElement* dx9_element) {}
		void SaveSettings(TiXmlElement* dx9_element) {}
	};
};
#endif