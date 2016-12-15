/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		class c_file_definition_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_id;
			Configuration::c_configuration_value<std::string> m_type;
			Configuration::c_configuration_value<std::string> m_path;

			c_file_definition_container();

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() override;
		};

		class c_packed_file_definition_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_name;
			Configuration::c_configuration_container_list<c_file_definition_container> m_files;

			c_packed_file_definition_container();

		protected:
			const std::vector<Configuration::i_configuration_value* const> GetMembers() override;
		};

		class c_packed_file_definition final
		{
		public:
			c_packed_file_definition_container m_packed_file_defintion;

			void Load(const boost::filesystem::path& definition_path);
		};
	};};
};
#endif