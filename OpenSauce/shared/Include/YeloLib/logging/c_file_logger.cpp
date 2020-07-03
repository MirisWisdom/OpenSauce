/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/logging/c_file_logger.hpp>

#ifdef API_DEBUG

namespace Yelo
{
    namespace Logging
    {
        c_file_logger::c_file_logger(const std::string& file, const LogVerbosity filter)
            : c_logger_base(filter),
              m_output_file()
        {
            m_output_file.open(file, std::fstream::in | std::fstream::app);
        }

        c_file_logger::~c_file_logger()
        {
            m_output_file.close();
        }

        void c_file_logger::Write(const std::string& message)
        {
            m_output_file << message << "\r\n";
            m_output_file.flush();
        }

        class c_file_logger_test_fixture : public Automation::i_automated_test
        {
        protected:
            std::string m_file_path;

            void BeforeTest() override
            {
                char file_path[MAX_PATH] = "";
                char file_name[MAX_PATH] = "";
                GetTempPath(sizeof(file_path), file_path);
                GetTempFileName(file_path, TEXT("Log"), 0, file_name);
                m_file_path.assign(file_name);
            }

            void AfterTest() override
            {
                DeleteFile(m_file_path.c_str());
            }

            std::string GetFileContents(const std::string& path) const
            {
                std::ifstream filestream(path);
                return std::string(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>());
            }
        };

#define YELO_UNIT_TEST_FILE_LOGGER(name) YELO_UNIT_TEST_WITH_FIXTURE(name, Yelo.Logging, c_file_logger_test_fixture)

        YELO_UNIT_TEST_FILE_LOGGER(Ctor_WithFilePath_CreatesFile)
        {
            {
                c_file_logger logger(m_file_path, LogVerbosity::Verbose);

                logger.Log(LogVerbosity::Log, "anyMessage");
            }

            auto result = GetFileContents(m_file_path);

            AssertThat(result, Equals("anyMessage\r\n"));
        }

        YELO_UNIT_TEST_FILE_LOGGER(Ctor_WithExistingFile_AppendsFile)
        {
            {
                c_file_logger logger(m_file_path, LogVerbosity::Verbose);
                logger.Log(LogVerbosity::Log, "anyMessage");
            }
            {
                c_file_logger logger(m_file_path, LogVerbosity::Verbose);
                logger.Log(LogVerbosity::Log, "anyOtherMessage");
            }

            auto result = GetFileContents(m_file_path);

            AssertThat(result, Equals("anyMessage\r\nanyOtherMessage\r\n"));
        }

        YELO_UNIT_TEST_FILE_LOGGER(Write_WithMultipleCalls_WriteMessagesAsSeparateLines)
        {
            {
                c_file_logger logger(m_file_path, LogVerbosity::Verbose);

                logger.Log(LogVerbosity::Log, "anyMessage");
                logger.Log(LogVerbosity::Log, "anyOtherMessage");
            }

            auto result = GetFileContents(m_file_path);

            AssertThat(result, Equals("anyMessage\r\nanyOtherMessage\r\n"));
        }

#undef YELO_UNIT_TEST_FILE_LOGGER
    };
};
#endif
