/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Render
    {
        class c_quad_instance;
    }

    namespace TagGroups
    {
        struct s_technique_definition;
        struct s_shader_postprocess_definition;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_data_base;

            class c_shader_postprocess
            {
            protected:
                struct
                {
                    TagGroups::s_shader_postprocess_definition* definition;
                    c_shader_data_base* source_data;
                } m_members;

            private:
                void ClearMembers();

            public:
                virtual ~c_shader_postprocess() {}

                virtual void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition);

                template<class T>
                T* GetShaderDefinition()
                {
                    return CAST_PTR(T*, m_members.definition);
                }

                void SetSourceData(c_shader_data_base* source);
                LPD3DXEFFECT GetEffect();
                bool IsValid();

                virtual void Ctor();
                virtual void Dtor();

                virtual void SetupShader() {}

                HRESULT LoadShader(IDirect3DDevice9* render_device);
                void UnloadShader();
                void OnDeviceLost();
                HRESULT OnDeviceReset();
                void Validate();
            protected:
                virtual void GetHandles();
                virtual void ClearHandles();
                virtual bool ValidateImpl();
            private:
                HRESULT LoadShaderImpl(IDirect3DDevice9* render_device);
                void UnloadShaderImpl();
                void OnDeviceLostImpl();
                HRESULT OnDeviceResetImpl();
                bool ValidateShaderVersions();
                TagGroups::s_technique_definition* GetActiveTechnique();
                D3DXHANDLE GetTechniqueHandle();

            public:
                virtual void SetVariables();
                virtual void UpdateVariables();

                HRESULT Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad);
            };
        }
    }
}
#endif
