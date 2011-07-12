/*
    OpenSauce: HaloWars Data Interop

    Copyright (C)  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

namespace Phoenix
{
	boost::uint32_t adler32(const void* pBuf, size_t buflen, boost::uint32_t adler32 = 1);

	// For an idea on the compression used in HaloWars, visit:
	// http://code.google.com/p/lzham/wiki/CompressionTechniquesUsed
	// All fingers seem to point to them using something LZ based (compressed streams always end in a zero byte, 
	// and I remember seeing code, related to compressed streams, which read two 16-bit values in a loop...idk, 
	// this shit and PPC assembly has me burnt out)

#include <PshPack1.h>
	// This appears along with the archive filesystem's header
	struct s_ecf_compressed_stream_header_data
	{
		struct s_sha_hash
		{
			boost::uint8_t data[0x14];
		};

		// signature used before and after the size_bit and hashes data
		enum { k_signature = 0xAAC94350 };

		boost::uint32_t head_signature;
		// Window size?
		boost::uint8_t size_bit; // size = 1<<size_bit, only ever seen 0x13 (512KB)

#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
		s_sha_hash sha_hashes[];
#pragma warning( pop )

		//boost::uint32_t tail_signature;

		inline bool IsValid() const { return head_signature == k_signature; }
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_compressed_stream_header_data) == 0x5 );
#include <PopPack.h>

	struct s_ecf_compressed_stream_eos_marker // end of stream marker, can be found at the end of a compressed stream's data
	{
		enum { k_signature = 0xA5D91776 };

		boost::uint32_t signature;

		inline bool IsValid() const { return signature == k_signature; }

	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_compressed_stream_eos_marker) == 0x4 );

	struct s_ecf_compressed_stream
	{
		enum { k_id_signature = 0xCC34EEAD };

		boost::uint32_t signature;
		boost::uint32_t header_checksum;
		boost::uint32_t unknown08;
		boost::uint64_t uncompressed_size, compressed_size;
		boost::uint32_t uncompressed_crc,  compressed_crc;

		void InitializeHeaderChecksum();
	};

	struct s_ecf_header
	{
		enum { k_header_signature = 0xDABA7737 };

		boost::uint32_t header_signature;
		boost::uint32_t header_size;		// Size of this header, plus the wrapped ID's header data (if it has any)
		boost::uint32_t checksum;			// Checksum of the total_size field and onward, added to the checksum of everything after the header (header_size - sizeof(s_ecf_header))
		boost::uint32_t total_size;			// Total size of the block this header represents (header, chunk headers, and chunk data)
		boost::int16_t chunk_count;			// Number of chunks that proceed this header
		boost::uint16_t unknown12;			// Haven't seen this yet (so, zero)

		boost::uint32_t id;					// The signature of the data which this header encapsulates
		boost::uint16_t extra_data_size;	// Size of any extra chunk header data
		PAD16; PAD32;

		void InitializeChecksum(const void* header_data, size_t header_data_size);
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_header) == 0x20 );

	class ecf_block_ids
	{
	public:
		static const boost::uint32_t k_filesystem = 0x17FDBA9C;
		static const boost::uint32_t k_binary_xml = 0xE43ABC00;

		static const boost::uint32_t k_texture = 0x13CF5D01;
	};

	struct s_ecf_chunk_header
	{
		boost::uint64_t id;					// Id of this chunk...shouldn't be byteswapped (appears to be treated as a byte[8])
		boost::uint32_t offset;				// Offset within the parent block
		boost::uint32_t size;
		boost::uint32_t checksum;
		// BIT(0) - compressed
		// BIT(1) - internal data
		boost::uint8_t flags;
		boost::uint8_t alignment_bit;		// data alignment = 1<<alignment_bit (offset must be aligned to this)
		boost::uint16_t unknown16;
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_chunk_header) == 0x18 );

	struct s_ecf_filesystem_header
	{
		enum { k_signature = 0x05ABDBD8 };

		boost::uint32_t signature;
		boost::uint32_t size;
		PAD64;
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_filesystem_header) == 0x10 );

	// file system chunk's extra data
	struct s_ecf_filesystem_chunk
	{
		enum { k_id_signature = ecf_block_ids::k_filesystem };

		boost::uint8_t unknown18[8];
		boost::uint32_t uncompressed_size;	// If uncompressed_size == size, then this chunk isn't compressed
		boost::uint8_t unknown24[16];		// Possibly a hash?
		boost::uint32_t unknown34;			// Incremental, most like an offset. Maybe the virtual address offset?
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_filesystem_chunk) == 0x20 );

	// Typical file layout:
	// s_ecf_header
		// s_ecf_filesystem_header
		// s_ecf_compressed_stream_header_data
			// s_ecf_chunk_header+s_ecf_filesystem_chunk
				// chunks data...

	struct s_ecf_dxt_texture_resource
	{
		enum { k_signature = 0xDDBB7738 };

		boost::uint32_t signature;
		boost::uint32_t unknown04;
		boost::uint32_t unknown08;
		boost::uint16_t unknown0C, unknown0E;
		boost::uint8_t unknown10, unknown11, unknown12, unknown13;
		boost::uint32_t unknown14, unknown18, unknown1C;
		float unknown20;
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_dxt_texture_resource) == 0x24 );
};