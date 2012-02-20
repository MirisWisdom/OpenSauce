/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Rasterizer.hpp"
#ifndef PLATFORM_NO_DX9

#include <TagGroups/Halo1/model_definitions.hpp>

#include "Common/YeloSettings.hpp"

namespace Yelo
{
	namespace Rasterizer
	{
#if PLATFORM_ID == PLATFORM_SAPIEN
	#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
	#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_RASTERIZER
	#include "Memory/_EngineLayout.inl"

	#include <Rasterizer/Halo1/Render.Upgrades.inl>

		s_rasterizer_globals* RasterizerGlobals()	PTR_IMP_GET2(rasterizer_globals);
		s_render_globals* RenderGlobals()			PTR_IMP_GET2(render_globals);
		s_rasterizer_debug_options* DebugOptions()	PTR_IMP_GET2(rasterizer_debug_data);
#endif

		void Initialize()
		{
#if PLATFORM_ID == PLATFORM_SAPIEN
			g_render_upgrades.Initialize();
			InitializeMaximumNodesPerModelFixes();
#endif
		}

		void Dispose()
		{
#if PLATFORM_ID == PLATFORM_SAPIEN
			g_render_upgrades.Dispose();
#endif
		}

		void LoadSettings(TiXmlElement* dx9_element)
		{
#if PLATFORM_ID == PLATFORM_SAPIEN
			g_render_upgrades.LoadSettings(dx9_element);
#endif
		}

		void SaveSettings(TiXmlElement* dx9_element)
		{
#if PLATFORM_ID == PLATFORM_SAPIEN
			g_render_upgrades.SaveSettings(dx9_element);
#endif
		}
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