/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/input_output/i_test_list_output.hpp>
#include <YeloLib/automation/input_output/c_test_list_xml.hpp>

namespace Yelo
{
    namespace Automation
    {
        class c_test_list_output_xml final : public i_test_list_output
        {
            c_test_list_xml m_list;

        public:
            explicit c_test_list_output_xml();

            void TestFound(const std::string& name) override;
            void Save(const std::string& output);
        };
    };
};
#endif
