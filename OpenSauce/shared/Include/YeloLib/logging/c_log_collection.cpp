/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/logging/c_log_collection.hpp>

#ifdef API_DEBUG

#include <YeloLib/automation/automation_macros.hpp>
#include <YeloLib/logging/c_memory_logger.hpp>

namespace Yelo
{
    namespace Logging
    {
        c_log_collection::c_log_collection()
            : m_loggers() {}

        void c_log_collection::AddLogger(i_logger* logger)
        {
            if (!logger)
            {
                throw std::exception("Logger pointer can not be null");
            }
            if (find(m_loggers.begin(), m_loggers.end(), logger) != m_loggers.end())
            {
                return;
            }
            m_loggers.push_back(logger);
        }

        void c_log_collection::RemoveLogger(i_logger* logger)
        {
            if (!logger)
            {
                throw std::exception("Logger pointer can not be null");
            }
            auto location = find(m_loggers.begin(), m_loggers.end(), logger);
            if (location == m_loggers.end())
            {
                return;
            }
            m_loggers.erase(location);
        }

        void c_log_collection::Log(const LogVerbosity verbosity, const std::string& message)
        {
            for (auto logger : m_loggers)
            {
                logger->Log(verbosity, message);
            }
        }

#define YELO_UNIT_TEST_LOG_COLLECTION(name) YELO_UNIT_TEST(name, Yelo.Logging)

        YELO_UNIT_TEST_LOG_COLLECTION(AddLogger_WithNullLogger_Throws)
        {
            c_log_collection collection;

            AssertThrows(std::exception, collection.AddLogger(nullptr));
        }

        YELO_UNIT_TEST_LOG_COLLECTION(AddLogger_WithLogger_AddsLoggerToCollection)
        {
            c_log_collection collection;
            c_memory_logger logger(LogVerbosity::Verbose);

            collection.AddLogger(&logger);
            collection.Log(LogVerbosity::Log, "anyMessage");

            AssertThat(logger.GetMessages(), Contains("anyMessage"));
        }

        YELO_UNIT_TEST_LOG_COLLECTION(AddLogger_WithAlreadyAddedLogger_DoesNotAddLoggerMultipleTimes)
        {
            c_log_collection collection;
            c_memory_logger logger(LogVerbosity::Verbose);

            collection.AddLogger(&logger);
            collection.AddLogger(&logger);
            collection.Log(LogVerbosity::Log, "anyMessage");

            AssertThat(logger.GetMessages(), HasLength(1));
        }

        YELO_UNIT_TEST_LOG_COLLECTION(RemoveLogger_WithNullLogger_Throws)
        {
            c_log_collection collection;

            AssertThrows(std::exception, collection.RemoveLogger(nullptr));
        }

        YELO_UNIT_TEST_LOG_COLLECTION(RemoveLogger_WithLogger_RemovesLoggerFromCollection)
        {
            c_log_collection collection;
            c_memory_logger logger(LogVerbosity::Verbose);

            collection.AddLogger(&logger);
            collection.Log(LogVerbosity::Log, "anyMessage");
            collection.RemoveLogger(&logger);
            collection.Log(LogVerbosity::Log, "anyOtherMessage");

            AssertThat(logger.GetMessages(), HasLength(1));
        }

        YELO_UNIT_TEST_LOG_COLLECTION(Log_WithLoggersAdded_PassesLogThroughToLoggers)
        {
            c_log_collection collection;
            c_memory_logger loggerOne(LogVerbosity::Verbose);
            c_memory_logger loggerTwo(LogVerbosity::Verbose);

            collection.AddLogger(&loggerOne);
            collection.AddLogger(&loggerTwo);
            collection.Log(LogVerbosity::Error, "anyMessage");

            AssertThat(loggerOne.GetMessages(), Contains("Error: anyMessage"));
            AssertThat(loggerTwo.GetMessages(), Contains("Error: anyMessage"));
        }

#undef YELO_UNIT_TEST_LOG_COLLECTION
    };
};
#endif
