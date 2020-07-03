/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/logging/c_debug_output_logger.hpp>

#ifdef API_DEBUG

namespace Yelo
{
    namespace Logging
    {
        c_debug_output_logger::c_debug_output_logger(const LogVerbosity filter)
            : c_logger_base(filter) {}

        void c_debug_output_logger::Write(const std::string& message)
        {
            if(IsDebuggerPresent())
            {
                OutputDebugString(message.c_str());
                OutputDebugString("\r\n");
            }
        }
    };
};
#endif
