/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/ShaderDraw.hpp"

#if !PLATFORM_IS_DEDI
#include "Memory/MemoryInterface.hpp"

#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"

namespace Yelo { namespace Rasterizer { namespace ShaderDraw
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_SHADERDRAW
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_SHADERDRAW
#include "Memory/_EngineLayout.inl"

	static void PLATFORM_API Environment_ShaderLightmapDraw(void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6)
	{
		typedef void (PLATFORM_API *t_shader_draw_func)(void*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;

		static t_shader_draw_func STOCK_DRAW_FUNC = CAST_PTR(t_shader_draw_func, GET_FUNC_VPTR(RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP));
		STOCK_DRAW_FUNC(arg1, arg2, arg3, arg4, arg5, arg6);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	static void PLATFORM_API Model_ShaderEnvironmentDraw(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
	{
		typedef void (PLATFORM_API *t_shader_draw_func)(void*, void*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;
		Rasterizer::ShaderExtension::Model::SetModelNormSpec(shader_pointer);

		static t_shader_draw_func STOCK_DRAW_FUNC = CAST_PTR(t_shader_draw_func, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_ENVIRONMENT));
		STOCK_DRAW_FUNC(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	static void PLATFORM_API Model_ShaderModelDraw(void* shader_pointer, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7)
	{
		typedef void (PLATFORM_API *t_shader_draw_func)(void*, void*, void*, void*, void*, void*, void*);

		DX9::c_gbuffer_system::RenderGBuffer() = true;
		Rasterizer::ShaderExtension::Model::SetModelNormSpec(shader_pointer);

		static t_shader_draw_func STOCK_DRAW_FUNC = CAST_PTR(t_shader_draw_func, GET_FUNC_VPTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_MODEL));
		STOCK_DRAW_FUNC(shader_pointer, arg2, arg3, arg4, arg5, arg6, arg7);

		DX9::c_gbuffer_system::RenderGBuffer() = false;
	}

	void Initialize()
	{
		GET_PTR(RASTERIZER_ENVIRONMENT_DRAW_LIGHTMAP_SHADER_DRAW__DRAW_SHADER_LIGHTMAP_PTR) = &Environment_ShaderLightmapDraw;

		GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_ENVIRONMENT_PTR) = &Model_ShaderEnvironmentDraw;
		GET_PTR(RASTERIZER_MODEL_DRAW_ENVIRONMENT_SHADER_DRAW__DRAW_SHADER_MODEL_PTR) = &Model_ShaderModelDraw;
	}

	void Dispose()
	{

	}
};};};
#endif