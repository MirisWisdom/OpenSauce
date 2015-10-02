/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifdef API_DEBUG

namespace Yelo
{
    namespace Logging
    {
        enum class LogVerbosity
        {
            Error,
            Warning,
            Log,
            Verbose
        };

        class i_logger
        {
        public:
            virtual ~i_logger() {}

            virtual void Log(const LogVerbosity verbosity, const std::string& message) = 0;
        };
    };
};
#endif
