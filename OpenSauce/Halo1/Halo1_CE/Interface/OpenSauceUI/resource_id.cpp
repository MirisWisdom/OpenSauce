/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/resource_id.hpp"

#if !PLATFORM_IS_DEDI

#include <boost/crc.hpp>

#define DUMP_CRCS

namespace Yelo
{
    namespace Interface
    {
        namespace OpenSauceUI
        {
#if defined(DUMP_CRCS) && defined(DEBUG)
            static std::vector<uint32> g_printed_crcs;

            ////////////////////////////////////////////////////////////////////////////////////////////////////
            /// <summary>Prints a CRC to the debug output.</summary>
            ///
            /// <param name="value">The string.</param>
            /// <param name="crc">  The CRC of the string.</param>
            static void DumpCRC(const std::string& value, const uint32 crc)
            {
                if (find(g_printed_crcs.begin(), g_printed_crcs.end(), crc) != g_printed_crcs.end())
                {
                    return;
                }
                g_printed_crcs.push_back(crc);

                char crc_str[32] = "";
                sprintf_s(crc_str, sizeof(crc_str), "%d\t0x%08X", crc, crc);
                LOG(Verbose, "CRC - " + value + "\t\t" + crc_str);
            }
#endif

            uint32 CRCString(const std::string& value)
            {
                // CRC the string
                boost::crc_32_type result;
                result.process_bytes(value.c_str(), value.size());

                // Print the checksum then return it
                uint32 crc = result.checksum();

#if defined(DUMP_CRCS) && defined(DEBUG)
                DumpCRC(value, crc);
#endif

                return crc;
            }

#ifdef DEBUG
            uint32 VerifyCRC(const std::string& value, const uint32 crc)
            {
                // Generate the CRC for the provided string and compare it against the expected crc
                auto test_crc = CRCString(value);

                YELO_ASSERT_DISPLAY(test_crc == crc, "A crc has been used that does not match the associated string: %s", value);

                return crc;
            }
#endif
        };
    };
};
#endif
