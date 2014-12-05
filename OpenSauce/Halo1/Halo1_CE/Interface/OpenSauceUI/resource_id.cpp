/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/resource_id.hpp"

#if !PLATFORM_IS_DEDI && defined(DEBUG)

#include <boost/crc.hpp>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Prints a CRC to the debug output. </summary>
		///
		/// <param name="str">	The string. </param>
		/// <param name="crc">	The CRC of the string. </param>
		static void DumpCRC(const char* str, uint32 crc)
		{
			OutputDebugString("CRC - ");
			OutputDebugString(str);
			OutputDebugString("\t\t");

			char crc_str[32] = "";
			sprintf_s(crc_str, sizeof(crc_str), "%d\t0x%08X", crc, crc);
				
			OutputDebugString(crc_str);
			OutputDebugString("\r\n");
		}

		const uint32 CRCString(const char* str)
		{
			// CRC the string
			boost::crc_32_type result;
			result.process_bytes(str, strlen(str));

			// Print the checksum then return it
			uint32 crc = result.checksum();

#ifdef DUMP_CRCS
			DumpCRC(str, crc);
#endif

			return crc;
		}

		const uint32 VerifyCRC(const char* str, uint32 crc)
		{
			// Generate the CRC for the provided string and compare it against the expected crc
			auto test_crc = CRCString(str);

			YELO_ASSERT_DISPLAY(test_crc == crc, "A crc has been used that does not match the associated string");

			return crc;
		}
	};};
};
#endif