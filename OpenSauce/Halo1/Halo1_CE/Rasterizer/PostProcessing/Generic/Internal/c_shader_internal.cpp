/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/Internal/c_shader_internal.hpp"

#if !PLATFORM_IS_DEDI

#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_generic_definitions.hpp>

#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_postprocess_definition.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                namespace Internal
                {
                    void c_shader_internal::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
                    {
                        // if the definition is not null this shader has not been Dtor'd, which is bad
                        YELO_ASSERT_DISPLAY(m_members_generic.definition == nullptr, "Generic shader tag definition being set before it has been nulled");

                        c_shader_generic::SetShaderDefinition(definition);

                        // the generic postprocess class will keep track of the generic shader definition, but will pass the
                        // base shader definition to it's base class
                        auto current_definition = m_members_generic.definition;
                        while (!current_definition->base_shader.tag_index.IsNull())
                        {
                            current_definition = TagGroups::TagGetForModify<TagGroups::s_shader_postprocess_generic>(current_definition->base_shader.tag_index);
                        }
                        c_shader_postprocess::SetShaderDefinition(current_definition);
                    }

                    void c_shader_internal::SetShaderName(const char* name)
                    {
                        m_members_internal.shader_name = name;
                    }

                    void c_shader_internal::SetDatumIndex(datum_index shader_index)
                    {
                        m_members_internal.shader_tag_index = shader_index;
                    }

                    datum_index c_shader_internal::GetDatumIndex()
                    {
                        return m_members_internal.shader_tag_index;
                    }

                    void c_shader_internal::Ctor()
                    {
                        c_shader_generic::Ctor();

                        m_members_internal.shader_name = nullptr;
                        m_members_internal.shader_source_data = nullptr;
                        m_members_internal.shader_tag_index = datum_index::null;
                    }

                    void c_shader_internal::Dtor()
                    {
                        c_shader_generic::Dtor();

                        m_members_internal.shader_name = nullptr;
                        m_members_internal.shader_source_data->Dtor();
                        delete m_members_internal.shader_source_data;
                        m_members_internal.shader_source_data = nullptr;
                        m_members_internal.shader_tag_index = datum_index::null;
                    }

                    void c_shader_internal::SetupShader()
                    {
                        // if the source data is not null this shader has not been Dtor'd, which is bad
                        YELO_ASSERT_DISPLAY(m_members_internal.shader_source_data == nullptr, "source data pointer being set before it has been deleted and nulled");

                        // create a source data class for the base class to compile the shader from
                        m_members_internal.shader_source_data = new c_shader_data_postprocess_definition();
                        m_members_internal.shader_source_data->Ctor();
                        m_members_internal.shader_source_data->SetShaderName(m_members_internal.shader_name);
                        m_members_internal.shader_source_data->SetShaderDefinition(m_members.definition);
                        SetSourceData(m_members_internal.shader_source_data);

                        SetupBitmapParameters();

                        c_shader_generic::SetupShader();
                    }

                    void c_shader_internal::SetupBitmapParameters()
                    {
                        // set the bitmap data pointers
                        for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                        {
                            auto& parameter = m_members_generic.definition->parameters[i];

                            if (parameter.value_type.type != Enums::_shader_variable_base_type_texture)
                            {
                                continue;
                            }

                            auto bitmap_index = parameter.bitmap_value.bitmap.tag_index;
                            if (bitmap_index.IsNull())
                            {
                                return;
                            }

                            auto group = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(bitmap_index);

                            // invalid bitmap index
                            YELO_ASSERT_DISPLAY(group->bitmaps.Count > parameter.value.bitmap.bitmap_index, "parameter bitmap index is outside the bounds of the referenced bitmap");

                            parameter.bitmap_value.runtime._internal.bitmap = &group->bitmaps[parameter.value.bitmap.bitmap_index];
                        }
                    }
                }
            }
        }
    }
}
#endif
