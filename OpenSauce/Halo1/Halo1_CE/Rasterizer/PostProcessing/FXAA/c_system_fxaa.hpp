/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingComponent.hpp"
#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingRenderable.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace FXAA
            {
                class c_system_fxaa
                    : public IPostProcessingComponent,
                      public IPostProcessingRenderable
                {
                    static c_system_fxaa g_fxaa_system;
                public:
                    static c_system_fxaa& Instance();

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

                private:
                    HRESULT CreateShader();
                    void DestroyShader();

                    HRESULT CreateEffect();
                    void DestroyEffect();

                    void SetDeviceLost();
                    HRESULT SetDeviceReset();

                    void Validate();

                    void UpdateStatus();

                    bool Render(IDirect3DDevice9* render_device);
                };
            }
        }
    }
}
#endif
