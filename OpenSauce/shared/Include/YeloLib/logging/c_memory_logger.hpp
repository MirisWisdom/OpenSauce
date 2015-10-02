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
        class c_memory_logger final : public c_logger_base
        {
            std::vector<const std::string> m_messages;

        public:
            explicit c_memory_logger(const LogVerbosity filter);
            
            const std::vector<const std::string>& GetMessages();

        private:
            void Write(const std::string& message) override;
        };
    };
};
#endif
