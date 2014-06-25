/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/configuration/c_configuration_value_list.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

#include "Tool/Shader/Compiler/Collection/i_collection_definition.hpp"

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		class c_collection_definition_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_name;
			Configuration::c_configuration_value<std::string> m_source_directory;
			Configuration::c_configuration_value<std::string> m_shader_type;
			Configuration::c_configuration_value<std::string> m_compilation_target;
			Configuration::c_configuration_value_list<std::string> m_effects;

			c_collection_definition_container();

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers();
		};

		class c_collection_definition
			: public i_collection_definition
		{
			c_collection_definition_container m_shader_collection;

		public:
			void Load(const boost::filesystem::path& collection_path);

			const std::string GetName() final override;
			const std::string GetSourceDirectory() final override;
			const std::string GetShaderType() final override;
			const std::string GetCompilationTarget() final override;
			const std::vector<std::string> GetEffectList() final override;
		};
	};};};
};
#endif