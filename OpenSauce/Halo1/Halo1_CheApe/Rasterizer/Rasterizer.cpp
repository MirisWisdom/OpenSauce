/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Rasterizer.hpp"
#ifndef PLATFORM_NO_DX9

#include <blamlib/Halo1/models/model_definitions.hpp>

#include "Settings/Settings.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_RASTERIZER
#include "Memory/_EngineLayout.inl"

	namespace Render
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
		#include <YeloLib/Halo1/render/render_particles_upgrades.inl>

		s_render_globals* RenderGlobals()			PTR_IMP_GET2(render_globals);
#endif
	};

	namespace Rasterizer
	{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
	#include <YeloLib/Halo1/rasterizer/rasterizer_upgrades.inl>

		s_rasterizer_config* RasterizerConfig()		PTR_IMP_GET2(rasterizer_config);
		s_rasterizer_globals* RasterizerGlobals()	PTR_IMP_GET2(rasterizer_globals);
		s_rasterizer_debug_options* DebugOptions()	PTR_IMP_GET2(rasterizer_debug_data);
#endif

		void Initialize()
		{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
			g_render_upgrades.InitializeRenderedTrianglesUpgrade();
			g_render_upgrades.InitializeMaximumNodesPerModelFixes();
			// TODO: remove this once/if we renable effects rendering in sapien
#if !PLATFORM_DISABLE_UNUSED_CODE
			Render::render_particles_mods::Initialize();
#endif

#endif
		}

		void Dispose()
		{ }
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