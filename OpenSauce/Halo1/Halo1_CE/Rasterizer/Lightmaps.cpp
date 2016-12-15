/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/Lightmaps.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <YeloLib/Halo1/render/lightmaps/c_lightmap_manager.hpp>

#include "Memory/MemoryInterface.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "DX9/DX9.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_LIGHTMAPS
#include "Memory/_EngineLayout.inl"

	namespace Render { namespace Lightmaps
	{
		/// <summary>	Container for the lightmap globals. </summary>
		struct s_lightmap_globals
		{
			/// <summary>	Index of the current lightmap being rendered. </summary>
			int32 m_lightmap_index;

			/// <summary>	Lightmap manager that maintains the current standard and directional lightmaps. </summary>
			c_lightmap_manager m_lightmap_manager;
		};
		/// <summary>	Contains all global variables of the lightmaps system. </summary>
		static s_lightmap_globals g_lightmap_globals;

		/// <summary>	Hook to get the current lightmap index being rendered. </summary>
		static API_FUNC_NAKED void Hook_BSPLightmapIndex()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(BSP_LIGHTMAP_INDEX_RETN);

			_asm
			{
				mov		g_lightmap_globals.m_lightmap_index, edx
				lea		ecx, [edx+edx*2]
				mov		edx, [eax+64h]

				jmp		RETN_ADDRESS
			};
		}

		/// <summary>	Lightmap sampler call hook. </summary>
		static API_FUNC_NAKED void SetLightmapSamplerHook()
		{
			API_FUNC_NAKED_START()
				push	edi
				call	Rasterizer::ShaderExtension::Environment::SetLightmapSampler
				pop		ebp
			API_FUNC_NAKED_END_();
		}

		/// <summary>	Hooks the lightmap tag index for the objects render state cache update. </summary>
		static API_FUNC_NAKED void ObjectRenderCacheLightmapTagIndexHook()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_RETN);
			static datum_index LIGHTMAP_TAG_INDEX;

			_asm
			{
				push	edx
				push	ecx
				
				lea		edx, [LIGHTMAP_TAG_INDEX]
				push	edx
				lea		ecx, g_lightmap_globals.m_lightmap_manager
				call	Yelo::Render::Lightmaps::c_lightmap_manager::GetStandardLightmapTagIndex
				mov		eax, [LIGHTMAP_TAG_INDEX]

				pop		ecx
				pop		edx

				cmp		eax, 0FFFFFFFFh

				jmp		RETN_ADDRESS
			}
		}

		/// <summary>	Inserts hooks to override the stock lightmap management. </summary>
		void Initialize()
		{
			// Hook lightmap set code
			Memory::WriteRelativeJmp(&Hook_BSPLightmapIndex,	GET_FUNC_VPTR(BSP_LIGHTMAP_INDEX_HOOK), true);
			Memory::WriteRelativeCall(&SetLightmapSamplerHook,	GET_FUNC_VPTR(SET_LIGHTMAP_SAMPLER_CALL), true);
			Memory::WriteRelativeJmp(&ObjectRenderCacheLightmapTagIndexHook, GET_FUNC_VPTR(OBJECT_RENDER_CACHE_LIGHTMAP_TAG_INDEX_HOOK), true);
		}

		/// <summary>	Unused. </summary>
		void Dispose()
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap tag indices. </summary>
		///
		/// <param name="standard_tag_index">	  	The standard lightmap tag index. </param>
		/// <param name="directional_1_tag_index">	The first directional lightmap tag index. </param>
		/// <param name="directional_2_tag_index">	The second directional lightmap tag index. </param>
		/// <param name="directional_3_tag_index">	The third directional lightmap tag index. </param>
		void SetLightmaps(const datum_index standard_tag_index
			, const datum_index directional_1_tag_index
			, const datum_index directional_2_tag_index
			, const datum_index directional_3_tag_index)
		{
			g_lightmap_globals.m_lightmap_manager.SetLightmapDatums(standard_tag_index, directional_1_tag_index, directional_2_tag_index, directional_3_tag_index);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap tag indices. </summary>
		///
		/// <param name="standard">	The standard lightmap tag index. </param>
		void SetLightmaps(const datum_index standard_tag_index)
		{
			SetLightmaps(standard_tag_index, datum_index::null, datum_index::null, datum_index::null);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap samplers. </summary>
		///
		/// <param name="use_directional">	The use directional lightmaps when true. </param>
		void SetLightmapSamplers(const bool use_directional)
		{
			g_lightmap_globals.m_lightmap_manager.SetLightmapSamplers(
				DX9::Direct3DDevice(),
				g_lightmap_globals.m_lightmap_index,
				use_directional,
				[](const datum_index tag_index, const int32 index) -> TagGroups::s_bitmap_data*
				{
					auto bitmap = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(tag_index);
					auto bitmap_data = CAST_PTR(TagGroups::s_bitmap_data*, &bitmap->bitmaps[index]);

					return bitmap_data;
				}
			);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Query if directional lightmaps are available. </summary>
		///
		/// <returns>	true if directional lightmaps are available, false if not. </returns>
		const bool HasDirectional()
		{
			return g_lightmap_globals.m_lightmap_manager.HasLightmaps(c_lightmap_manager::_lightmap_type_flags_directional_bit);
		}
	};};
};
#endif