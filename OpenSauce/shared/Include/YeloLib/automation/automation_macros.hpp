/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/automation/i_automated_test.hpp>
#include <YeloLib/automation/c_automation_runner.hpp>
#include <snowhouse/snowhouse.h>
using namespace snowhouse;

#define YELO_UNIT_TEST(name, category) \
    class c_automation_##name final : public Yelo::Automation::i_automated_test \
    { \
        static c_automation_##name m_instance; \
        c_automation_##name##() { Yelo::Automation::c_automation_runner_singleton::Get().RegisterTest(#category "." #name, this); } \
    public: \
        void Run() override; \
    }; \
    c_automation_##name c_automation_##name##::m_instance; \
    void c_automation_##name##::Run()
#endif
