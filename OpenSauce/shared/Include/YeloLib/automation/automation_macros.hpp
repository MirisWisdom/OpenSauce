/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/i_automated_test.hpp>
#include <snowhouse/snowhouse.h>
using namespace snowhouse;

#define YELO_UNIT_TEST(name, category) \
    class c_automation_##name final : public i_automated_test \
    { \
        static c_automation_##name m_instance; \
        c_automation_##name##() { Yelo::Automation::c_automation_runner::Get().RegisterTest(#category "." #name, this); } \
    public: \
        bool Run() override; \
    }; \
    c_automation_##name c_automation_##name##::m_instance; \
    bool c_automation_##name##::Run()
#endif
