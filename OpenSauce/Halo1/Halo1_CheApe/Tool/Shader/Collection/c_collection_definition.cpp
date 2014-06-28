/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Collection/c_collection_definition.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

using namespace boost::filesystem;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		c_collection_definition_container::c_collection_definition_container()
			: Configuration::c_configuration_container("ShaderCollection")
			, m_name("Name", "")
			, m_source_directory("SourceDirectory", "")
			, m_shader_type("ShaderType", "")
			, m_compilation_target("CompilationTarget", "")
			, m_effects("Effect", "")
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_collection_definition_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_name,
				&m_source_directory,
				&m_shader_type,
				&m_compilation_target,
				&m_effects
			};
		}

		void c_collection_definition::Load(const path& collection_path)
		{
			YELO_ASSERT_DISPLAY(exists(collection_path), "ERROR: collection definition does not exist");

			auto collection_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(collection_path.string());
			collection_file->Load();

			m_shader_collection.GetValueFromParent(*collection_file->Root()->GetChild("OpenSauce"));
		}

		const std::string c_collection_definition::GetName()
		{
			return m_shader_collection.m_name;
		}

		const std::string c_collection_definition::GetSourceDirectory()
		{
			return m_shader_collection.m_source_directory;
		}

		const std::string c_collection_definition::GetShaderType()
		{
			return m_shader_collection.m_shader_type;
		}

		const std::string c_collection_definition::GetCompilationTarget()
		{
			return m_shader_collection.m_compilation_target;
		}

		const std::vector<std::string> c_collection_definition::GetEffectList()
		{
			std::vector<std::string> effect_list;

			auto& effects = m_shader_collection.m_effects.Get();

			for (auto& effect : effects)
			{
				effect_list.push_back(effect);
			}

			return effect_list;
		}
	};};};
};
#endif