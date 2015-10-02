/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/logging/c_memory_logger.hpp>

#ifdef API_DEBUG

#include <YeloLib/automation/automation_macros.hpp>

namespace Yelo
{
    namespace Logging
    {
        c_memory_logger::c_memory_logger(const LogVerbosity filter)
            : c_logger_base(filter),
              m_messages() {}

        const std::vector<const std::string>& c_memory_logger::GetMessages()
        {
            return m_messages;
        }

        void c_memory_logger::Write(const std::string& message)
        {
            m_messages.push_back(message);
        }

#define YELO_UNIT_TEST_MEMORY_LOGGER(name) YELO_UNIT_TEST(name, Yelo.Logging)

        YELO_UNIT_TEST_MEMORY_LOGGER(Log_WithAnyMessage_AddsMessageToList)
        {
            c_memory_logger logger(LogVerbosity::Verbose);

            logger.Log(LogVerbosity::Verbose, "anyMessage");

            AssertThat(logger.GetMessages(), Contains("anyMessage"));
        }

        YELO_UNIT_TEST_MEMORY_LOGGER(Log_WithErrorMessage_PrependsError)
        {
            c_memory_logger logger(LogVerbosity::Verbose);

            logger.Log(LogVerbosity::Error, "anyMessage");

            AssertThat(logger.GetMessages(), Contains("Error: anyMessage"));
        }

        YELO_UNIT_TEST_MEMORY_LOGGER(Log_WithWarningMessage_PrependsWarning)
        {
            c_memory_logger logger(LogVerbosity::Verbose);

            logger.Log(LogVerbosity::Warning, "anyMessage");

            AssertThat(logger.GetMessages(), Contains("Warning: anyMessage"));
        }

        YELO_UNIT_TEST_MEMORY_LOGGER(Log_WithVerbosityFilter_FiltersMessages)
        {
            c_memory_logger logger(LogVerbosity::Warning);

            logger.Log(LogVerbosity::Error, "anyErrorMessage");
            logger.Log(LogVerbosity::Warning, "anyWarningMessage");
            logger.Log(LogVerbosity::Log, "anyLogMessage");
            logger.Log(LogVerbosity::Verbose, "anyVerboseMessage");

            AssertThat(logger.GetMessages(), Contains("Error: anyErrorMessage") && Contains("Warning: anyWarningMessage") && !Contains("anyLogMessage") && !Contains("anyVerboseMessage"));
        }

#undef YELO_UNIT_TEST_MEMORY_LOGGER
    };
};
#endif
