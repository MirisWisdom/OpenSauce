/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/GBuffer/Effects/c_gbuffer_effect_factory.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/files/packed_file.hpp>
#include <Game/GameState.hpp>
#include <Rasterizer/PostProcessing/PostProcessingErrorReporting.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            c_gbuffer_effect_factory c_gbuffer_effect_factory::m_instance;

            c_gbuffer_effect_factory& c_gbuffer_effect_factory::Get()
            {
                return m_instance;
            }

            void c_gbuffer_effect_factory::Initialize()
            {
                m_shader_package.OpenFile("GBUF_GBuffer_Shaders_SHD", true);
            }

            void c_gbuffer_effect_factory::Dispose()
            {
                m_shader_package.CloseFile();
            }

            bool c_gbuffer_effect_factory::CreateEffect(IDirect3DDevice9& device, LPD3DXEFFECT* effect, cstring effect_id)
            {
                LPD3DXBUFFER error_buffer = nullptr;
                uint32 data_size = 0;
                auto data_pointer = m_shader_package.GetDataPointer(effect_id, &data_size);

                if (!data_pointer || data_size == 0)
                {
                    return false;
                }

                auto result = D3DXCreateEffect(
                    &device,
                    data_pointer,
                    data_size,
                    nullptr,
                    nullptr,
                    0,
                    nullptr,
                    effect,
                    &error_buffer);

                if (FAILED(result))
                {
                    // if loading the shader failed, and we are in devmode
                    // print an error to the console
                    if (GameState::DevmodeEnabled())
                    {
                        PostProcessing::ErrorReporting::Write("Error: failed to load gbuffer shaders");
                        PostProcessing::ErrorReporting::WriteD3DXErrors(error_buffer);
                    }
                }
                safe_release(error_buffer);

                return SUCCEEDED(result);
            }

            D3DXHANDLE c_gbuffer_effect_factory::GetGBufferTechnique(ID3DXEffect& effect, cstring mesh_type, cstring rt_support)
            {
                auto technique_format = "VS_%s_%s";
                char technique[32] = "";
                sprintf_s(technique, sizeof(technique), technique_format, mesh_type, rt_support);
                return effect.GetTechniqueByName(technique);
            }

            D3DXHANDLE c_gbuffer_effect_factory::GetGBufferTechnique(ID3DXEffect& effect, const int32 rt_count, cstring mesh_type, cstring rt_support)
            {
                auto technique_format = "PS_MRT%i_%s_%s";
                char technique[32] = "";
                sprintf_s(technique, technique_format, rt_count, mesh_type, rt_support);
                return effect.GetTechniqueByName(technique);
            }
        };
    };
};
#endif
