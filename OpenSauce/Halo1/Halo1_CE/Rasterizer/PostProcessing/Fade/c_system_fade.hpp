/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Interfaces/IPostProcessingComponent.hpp"

namespace Yelo
{
    namespace Enums
    {
        enum pp_component_status : _enum;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Fade
            {
                class c_system_fade : public IPostProcessingComponent
                {
                    static c_system_fade g_fade_system;

                public:
                    static c_system_fade& Instance();

                private:
                    struct
                    {
                        struct
                        {
                            bool is_ready;
                            bool is_unloaded;
                            PAD16;
                        } m_flags;

                        Enums::pp_component_status status;
                        PAD16;
                    } m_members;

                public:
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

                private:
                    HRESULT CreateShader();
                    void DestroyShader();

                    void SetDeviceLost();
                    HRESULT SetDeviceReset();

                    void Validate();

                    void UpdateStatus();

                public:
                    HRESULT FadeCurrentResult(real fade_amount);
                };
            }
        }
    }
}
#endif
