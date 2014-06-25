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

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets lightmap sampler. </summary>
		///
		/// <param name="sampler_index">	Zero-based index of the sampler. </param>
		static void SetLightmapSampler(const int32 sampler_index)
		{
			g_lightmap_globals.m_lightmap_manager.SetLightmapSamplers(
				DX9::Direct3DDevice(),
				g_lightmap_globals.m_lightmap_index,
				[](const datum_index datum, const int32 index) -> TagGroups::s_bitmap_data*
				{				
					auto bitmap = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(datum);
					auto bitmap_data = CAST_PTR(TagGroups::s_bitmap_data*, &bitmap->bitmaps[index]);

					return bitmap_data;
				}
			);
		}

		/// <summary>	Lightmap sampler call hook. </summary>
		static API_FUNC_NAKED void SetLightmapSamplerHook()
		{
			API_FUNC_NAKED_START()
				push	esi
				call	SetLightmapSampler
				pop		ebp
			API_FUNC_NAKED_END_();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Returns true if directional lightmaps are in use. </summary>
		///
		/// <returns>	true if directional lightmaps are in use, otherwise false. </returns>
		bool UsingDirectionalLightmaps()
		{
			return g_lightmap_globals.m_lightmap_manager.HasLightmaps(Flags::_render_lightmaps_flags_directional);
		}

		/// <summary>	Inserts hooks to override the stock lightmap management. </summary>
		void Initialize()
		{
			// Hook lightmap set code
			Memory::WriteRelativeJmp(&Hook_BSPLightmapIndex,	GET_FUNC_VPTR(BSP_LIGHTMAP_INDEX_HOOK), true);
			Memory::WriteRelativeCall(&SetLightmapSamplerHook,	GET_FUNC_VPTR(SET_LIGHTMAP_SAMPLER_CALL), true);
		}

		/// <summary>	Unused. </summary>
		void Dispose()
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap datums. </summary>
		///
		/// <param name="standard">			The standard lightmap datum. </param>
		/// <param name="directional_1">	The first directional lightmap datum. </param>
		/// <param name="directional_2">	The second directional lightmap datum. </param>
		/// <param name="directional_3">	The third directional lightmap datum. </param>
		void SetLightmaps(const datum_index standard, const datum_index directional_1, const datum_index directional_2, const datum_index directional_3)
		{
			if(Rasterizer::ShaderExtension::ExtensionsEnabled())
			{
				g_lightmap_globals.m_lightmap_manager.SetLightmapDatums(standard, directional_1, directional_2, directional_3);
			}
			else
			{
				g_lightmap_globals.m_lightmap_manager.SetLightmapDatums(standard, datum_index::null, datum_index::null, datum_index::null);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the lightmap datums. </summary>
		///
		/// <param name="standard">	The standard lightmap datum. </param>
		void SetLightmaps(const datum_index standard)
		{
			SetLightmaps(standard, datum_index::null, datum_index::null, datum_index::null);
		}
	};};
};
#endif