/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		class c_shader_data_pp_shaders : public c_shader_data_base
		{
			char m_shader_id[MAX_PATH];

		public:
			void SetShaderID(const char* id) { strcpy_s(m_shader_id, MAX_PATH, id); }

			void Ctor()
			{
				m_shader_id[0] = 0;
			}
			void Dtor()
			{
				m_shader_id[0] = 0;
			}

			const char* DataSourceID();
			const void* GetData(uint32& data_size);
			void Dispose() {}
			const bool IsBinary() { return true; }
			const char* GetIncludePath() { return NULL; }
			const D3DXMACRO* GetMacros() { return NULL; }
		};
	};};
};
#endif