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
        class c_logger_base : public i_logger
        {
            const LogVerbosity m_filter;

        public:
            explicit c_logger_base(const LogVerbosity filter);
            virtual ~c_logger_base() {}

            virtual void Log(const LogVerbosity verbosity, const std::string& message) final override;

        private:
            virtual void Write(const std::string& message) = 0;
        };
    };
};
#endif