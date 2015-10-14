/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/memory/linked_list.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_postprocess;

            class c_shader_instance : public LinkedListNode<c_shader_instance>
            {
            protected:
                struct
                {
                    struct
                    {
                        bool is_valid;
                        bool is_active;
                        PAD16;
                    } m_flags;

                    c_shader_postprocess* definition;
                } m_members;

            private:
                void ClearMembers();

            public:
                virtual void SetShader(c_shader_postprocess* definition);
                c_shader_postprocess* GetShader();
                bool IsValid();
                bool& IsActive();
                bool UsesGBuffer();

                virtual void Ctor();
                virtual void Dtor();

                virtual void SetupShaderInstance() {}

                void Validate();
            protected:
                virtual bool ValidateImpl();

            public:
                virtual void UpdateShaderInstance(real delta_time) { }

                virtual void SetShaderInstanceVariables() {}
            };
        }
    }
}
#endif
