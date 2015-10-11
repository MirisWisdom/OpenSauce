/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingCacheComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingUpdatable.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingRenderable.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Bloom
            {
                class c_system_bloom
                    : public IPostProcessingCacheComponent,
                      public IPostProcessingUpdatable,
                      public IPostProcessingRenderable
                {
                    static c_system_bloom g_bloom_system;

                public:
                    static c_system_bloom& Instance();

                private:
                    struct
                    {
                        struct
                        {
                            bool is_ready;
                            bool is_unloaded;
                            bool is_enabled;
                            PAD8;
                        } m_flags;

                        Enums::pp_component_status status;
                        PAD16;
                    } m_members;

                public:
                    bool& Enabled();
                    bool IsReady() override;
                    bool IsUnloaded() override;

                    void Initialize() override;
                    void Dispose() override;

                    void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters) override;
                    void OnLostDevice_Base() override;
                    void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters) override;
                    void ReleaseResources_Base() override;

                    void Unload() override;
                    void Load() override;

                    bool Render(Enums::postprocess_render_stage render_stage) override;
                    void Update(real delta_time) override;
                    void Initialize_Cache() override;
                    void Dispose_Cache() override;

                private:
                    HRESULT CreateShader() const;
                    void DestroyShader() const;

                    HRESULT CreateEffect() const;
                    void DestroyEffect() const;

                    void SetDeviceLost() const;
                    HRESULT SetDeviceReset() const;

                    void SetBloomShaderVariables() const;

                    void Validate() const;

                    void UpdateStatus();

                    bool RenderBloom(IDirect3DDevice9* render_device) const;

                public:
                    void SetBloomSize(real size, real change_time);
                    void SetBloomExposure(real exposure, real change_time);
                    void SetBloomMixAmount(real mix_amount, real change_time);
                    void SetBloomMinimumColor(const real_rgb_color& minimum_color, real change_time);
                    void SetBloomMaximumColor(const real_rgb_color& maximum_color, real change_time);
                };
            }
        }
    }
}
#endif
