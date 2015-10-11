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
    namespace Render
    {
        class c_quad_instance;
    }

    namespace TagGroups
    {
        struct s_effect_postprocess_definition;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_instance;

            class c_effect_postprocess
            {
                /////////////////////////////////////////////////
                // members
            protected:
                struct
                {
                    TagGroups::s_effect_postprocess_definition* effect_definition;

                    struct
                    {
                        int32 count;
                        c_shader_instance* list;
                    } m_shaders;
                } m_members;

            private:
                void ClearMembers()
                {
                    m_members.effect_definition = nullptr;
                    m_members.m_shaders.list = nullptr;
                    m_members.m_shaders.count = 0;
                }

                /////////////////////////////////////////////////
                // member accessors
            public:
                virtual void SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition);
                bool IsValid();
            protected:
                template<class T>
                T* GetShaderInstance(int index)
                {
                    YELO_ASSERT_DISPLAY(index < m_members.m_shaders.count, "shader instance index outside the bounds of the array");

                    return CAST_PTR(T*, GetNodeByIndex(m_members.m_shaders.list, index));
                }

                /////////////////////////////////////////////////
                // initializers
            public:
                virtual void Ctor()
                {
                    ClearMembers();
                }

                virtual void Dtor()
                {
                    ClearMembers();
                }

                /////////////////////////////////////////////////
                // effect setup
            public:
                virtual void SetupEffect() {}

                virtual void SetupEffectPostCreation();
                void AddShaderInstance(c_shader_instance* instance);
                void Validate();
            private:
                virtual bool ValidateImpl();

                /////////////////////////////////////////////////
                // effect application
            public:
                void Update(real delta_time);
                HRESULT Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad);
                HRESULT Render(IDirect3DDevice9* render_device, Render::c_quad_instance* render_quad, real fade_amount);
            protected:
                virtual bool IsActive();
            };
        };
    };
};
#endif
