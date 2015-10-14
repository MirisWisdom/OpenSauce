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
#include "Rasterizer/PostProcessing/c_effect_render_set.hpp"

namespace Yelo
{
    namespace TagGroups
    {
        struct s_effect_postprocess_generic_effect_instance;
        struct s_effect_postprocess_collection_effect;
        struct s_effect_postprocess_collection;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_postprocess;

            namespace Generic
            {
                class c_shader_instance_generic;
                class c_effect_instance_generic;

                namespace Internal
                {
                    class c_effect_internal;
                    class c_shader_internal;

                    class c_system_internal
                        : public IPostProcessingCacheComponent,
                          public IPostProcessingUpdatable,
                          public IPostProcessingRenderable
                    {
                        struct s_effect_set
                        {
                            c_effect_internal* effect;

                            uint32 shader_instance_count;
                            c_shader_instance_generic* shader_instances;
                        };

                        static c_system_internal g_internal_system;
                    public:
                        static c_system_internal& Instance();

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

                        struct
                        {
                            TagGroups::s_effect_postprocess_collection* cache_shader_collection;

                            c_effect_render_set m_render_sets[5];

                            struct
                            {
                                uint16 count;
                                PAD16;
                                c_shader_internal* shader_list;
                            } m_shaders;

                            struct
                            {
                                uint16 count;
                                PAD16;
                                s_effect_set* effect_list;
                            } m_effects;

                            struct
                            {
                                uint16 count;
                                PAD16;
                                c_effect_instance_generic* effect_instance_list;
                            } m_effect_instances;
                        } m_members_internal;

                        void ClearMembers();

                    public:
                        bool& Enabled();
                        bool IsReady() override;
                        bool IsUnloaded() override;

                        void Initialize() override;
                        void Dispose() override;

                        void OnLostDevice_Base() override;
                        void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters) override;

                        void Unload() override;
                        void Load() override;

                        bool Render(Enums::postprocess_render_stage render_stage) override;
                        void Update(real delta_time) override;

                        void Initialize_Cache() override;
                        void Dispose_Cache() override;

                        void InitializeResources_Cache() override;
                        void ReleaseResources_Cache() override;

                    private:
                        void GetShaderCollection();
                        void ClearShaderCollection();

                        void GetInternalShaders();
                        void ClearInternalShaders();
                        void GetInternalEffects();
                        void ClearInternalEffects();

                        HRESULT LoadShaders();
                        void UnloadShaders();
                        HRESULT LoadEffects();
                        void UnloadEffects();

                        void SetupEffect(s_effect_set& effect_set, TagGroups::s_effect_postprocess_collection_effect* definition);
                        void DestroyEffect(s_effect_set& effect_set);
                        void SetupEffectInstance(c_effect_instance_generic* instance, TagGroups::s_effect_postprocess_generic_effect_instance* definition);
                        void DestroyEffectInstance(c_effect_instance_generic* instance);

                        c_shader_postprocess* GetShaderByIndex(datum_index index);

                        void SetupRenderSets();
                        void ClearRenderSets();
                        void SetRenderSet(c_effect_render_set& set, Enums::postprocess_render_stage render_stage);

                        void ValidateSystem();

                        void UpdateStatus();

                    public:
                        int16 GetEffectInstanceIndexByName(const char* name);
                        void SetEffectInstanceActive(int16 index, bool active);
                        void SetEffectInstanceFade(int16 index, real start, real end, real time);
                        real GetEffectInstanceCurrentFade(int16 index);
                        int16 GetEffectInstanceFadeDirection(int16 index);

                        int16 GetEffectIndexByName(const char* name);
                        bool GetEffectIsValid(int16 index);
                        int16 GetEffectShaderVariableIndexByName(int16 index, const char* name);
                        void SetEffectShaderVariableBoolean(int16 index, int16 variable_index, bool value, real time);
                        void SetEffectShaderVariableInteger(int16 index, int16 variable_index, int32 value, real time);
                        void SetEffectShaderVariableReal(int16 index, int16 variable_index, real value0, real value1, real value2, real value3, real time);

                        void SetEffectShaderInstanceActive(int16 index, int16 instance_index, bool active);
                    };
                };
            };
        };
    };
};
#endif
