/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/logging/c_logger_base.hpp>

#ifdef API_DEBUG

namespace Yelo
{
    namespace Logging
    {
        c_logger_base::c_logger_base(const LogVerbosity filter)
            : m_filter(filter) { }
        
        void c_logger_base::Log(const LogVerbosity verbosity, const std::string& message)
        {
            if (verbosity > m_filter)
            {
                return;
            }
            auto prefix = "";
            switch (verbosity)
            {
                case LogVerbosity::Error:
                    prefix = "Error: ";
                    break;
                case LogVerbosity::Warning:
                    prefix = "Warning: ";
                    break;
                default:
                    break;
            }
            Write(prefix + message);
        }
    };
};
#endif