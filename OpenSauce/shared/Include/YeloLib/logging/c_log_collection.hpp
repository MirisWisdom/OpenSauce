/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/logging/i_logger.hpp>
#include <YeloLib/cseries/c_singleton.hpp>

namespace Yelo
{
    namespace Logging
    {
        class c_log_collection final : public i_logger
        {
            std::vector<i_logger*> m_loggers;

        public:
            c_log_collection();

            void AddLogger(i_logger* logger);
            void RemoveLogger(i_logger* logger);
            void Log(const LogVerbosity verbosity, const std::string& message) override;
        };

        class c_log_singleton : public c_singleton<c_log_collection> { };
    };
};
#endif