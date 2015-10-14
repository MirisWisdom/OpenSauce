/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Generic/c_effect_generic.hpp"

namespace Yelo
{
    namespace TagGroups
    {
        struct s_effect_postprocess_collection_effect;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                namespace Internal
                {
                    class c_effect_internal : public c_effect_generic
                    {
                    protected:
                        struct
                        {
                            TagGroups::s_effect_postprocess_collection_effect* definition;
                        } m_members_internal;

                    public:
                        void SetEffectReferenceDefinition(TagGroups::s_effect_postprocess_collection_effect* definition);
                        const char* GetName();
                        int16 GetScriptedVariableIndex(const char* name);

                        void Ctor() override;
                        void Dtor() override;

                        void SetupEffect() override;
                    private:
                        void SetupScriptedVariables();

                    public:
                        void SetScriptedVariable(int16 index, real change_time, bool value);
                        void SetScriptedVariable(int16 index, real change_time, int32 value);
                        void SetScriptedVariable(int16 index, real change_time, real value0, real value1, real value2, real value3);
                    };
                };
            };
        };
    };
};
#endif
