/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/Definition/c_packed_file_definition.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
#pragma region c_file_definition_container
		c_file_definition_container::c_file_definition_container()
			: Configuration::c_configuration_container("File")
			, m_id("ID", "")
			, m_type("Type", "")
			, m_path("Path", "")
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_file_definition_container::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const>
			{
				&m_id,
				&m_type,
				&m_path
			};
		}
#pragma endregion

#pragma region c_packed_file_definition_container
		c_packed_file_definition_container::c_packed_file_definition_container()
			: Configuration::c_configuration_container("PackedFile")
			, m_name("Name", "")
			, m_files("File", [](){ return c_file_definition_container(); })
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_packed_file_definition_container::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const>
			{
				&m_name,
				&m_files
			};
		}
#pragma endregion

#pragma region c_packed_file_definition
		void c_packed_file_definition::Load(const boost::filesystem::path& definition_path)
		{
			YELO_ASSERT_DISPLAY(exists(definition_path), "ERROR: definition does not exist");

			auto definition_file = Configuration::c_configuration_file_factory::CreateConfigurationFile(definition_path.string());
			definition_file->Load();

			m_packed_file_defintion.GetValueFromParent(*definition_file->Root()->GetChild("OpenSauce"));
		}
#pragma endregion
	};};
};
#endif