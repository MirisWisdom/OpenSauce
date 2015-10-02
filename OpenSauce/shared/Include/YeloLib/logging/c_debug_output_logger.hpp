/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <YeloLib/logging/c_logger_base.hpp>

namespace Yelo
{
    namespace Logging
    {
        class c_debug_output_logger final : public c_logger_base
        {
        public:
            explicit c_debug_output_logger(const LogVerbosity filter);

        private:
            void Write(const std::string& message) override;
        };
    };
};
#endif
