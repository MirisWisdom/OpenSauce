/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Precompile.hpp"
#include "Resource/ECF.hpp"

namespace Phoenix
{
	void s_ecf_compressed_stream::InitializeHeaderChecksum()
	{
		struct s_compressed_stream
		{
			boost::uint64_t uncompressed_size;
			boost::uint32_t uncompressed_crc;
			boost::uint64_t compressed_size;
			boost::uint32_t compressed_crc;
			boost::uint32_t unknown08;
		};

		// Compressed stream fields, as they appear in-memory in HaloWars' code
		s_compressed_stream in_memory_data = {
			this->uncompressed_size, this->uncompressed_crc,
			this->compressed_size, this->compressed_crc,
			this->unknown08
		};

		header_checksum = adler32(&in_memory_data, sizeof(in_memory_data));
	}

	void s_ecf_header::InitializeChecksum(const void* header_data, size_t header_data_size)
	{
		const size_t k_checksum_size = sizeof(s_ecf_header) - offsetof(s_ecf_header, total_size);

		checksum = adler32(&this->total_size, k_checksum_size);

		checksum = adler32(header_data, header_data_size, checksum);
	}

	boost::uint32_t adler32(const void* pBuf, size_t buflen, boost::uint32_t adler32)
	{
		// Originally from the public domain stb.h header.

		const boost::uint8_t* buffer = CAST_PTR(const boost::uint8_t*, pBuf);

		const boost::uint32_t k_adler_mod = 65521;
		boost::uint32_t s1 = adler32 & 0xFFFF, s2 = adler32 >> 16;
		size_t blocklen;
		boost::uint32_t i;

		blocklen = buflen % 5552;
		while (buflen) {
			for (i=0; i + 7 < blocklen; i += 8) {
				s1 += buffer[0], s2 += s1;
				s1 += buffer[1], s2 += s1;
				s1 += buffer[2], s2 += s1;
				s1 += buffer[3], s2 += s1;
				s1 += buffer[4], s2 += s1;
				s1 += buffer[5], s2 += s1;
				s1 += buffer[6], s2 += s1;
				s1 += buffer[7], s2 += s1;

				buffer += 8;
			}

			for (; i < blocklen; ++i)
				s1 += *buffer++, s2 += s1;

			s1 %= k_adler_mod, s2 %= k_adler_mod;
			buflen -= blocklen;
			blocklen = 5552;
		}

		return (s2 << 16) + s1;
	}
};