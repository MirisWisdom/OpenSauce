/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifdef API_DEBUG

#include <fstream>
#include <YeloLib/logging/c_logger_base.hpp>

namespace Yelo
{
    namespace Logging
    {
        class c_file_logger final : public c_logger_base
        {
            std::ofstream m_output_file;

        public:
            explicit c_file_logger(const std::string& file, const LogVerbosity filter);
            ~c_file_logger();

        private:
            void Write(const std::string& message) override;
        };
    };
};
#endif
