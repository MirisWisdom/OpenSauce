/*
	OpenSauce: HaloWars Data Interop

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Phoenix
{
	boost::uint32_t adler32(const void* pBuf, size_t buflen, boost::uint32_t adler32 = 1);

	typedef boost::uint64_t ecf_id_t;

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
		ecf_id_t id;						// Id of this chunk, doesn't have to be unique
		boost::uint32_t offset;				// Offset within the parent block
		boost::uint32_t size;
		boost::uint32_t checksum;
		// BIT(0) - compressed stream
		// BIT(1) - raw compressed data (in the case of "filesystem" chunks, the filename table will have this set)
		boost::uint8_t flags;
		boost::uint8_t alignment_bit;		// data alignment = 1<<alignment_bit (offset must be aligned to this)
		boost::uint16_t unknown16;
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_chunk_header) == 0x18 );

	struct s_era_filesystem_header
	{
		enum { k_signature = 0x05ABDBD8 };

		boost::uint32_t signature;
		boost::uint32_t size;
		PAD64;
	}; BOOST_STATIC_ASSERT( sizeof(s_era_filesystem_header) == 0x10 );

#include <PshPack1.h>
	// Digital signature for the ERA
	struct s_era_filesystem_signature
	{
		struct s_sha_hash
		{
			boost::uint8_t data[0x14];
		};

		// signature used before and after the size_bit and hashes data
		enum { k_signature = 0xAAC94350 };

		boost::uint32_t head_signature;
		boost::uint8_t size_bit; // size = 1<<size_bit, only ever seen 0x13 (512KB)

#pragma warning( push )
#pragma warning( disable : 4200 ) // nonstandard extension used : zero-sized array in struct/union, Cannot generate copy-ctor or copy-assignment operator when UDT contains a zero-sized array
		s_sha_hash sha_hashes[];
#pragma warning( pop )

		//boost::uint32_t tail_signature;

		inline bool IsValid() const { return head_signature == k_signature; }
	}; BOOST_STATIC_ASSERT( sizeof(s_era_filesystem_signature) == 0x5 );
#include <PopPack.h>

#include <PshPack1.h>
	// file system chunk's extra data
	struct s_ecf_filesystem_chunk
	{
		enum { k_id_signature = ecf_block_ids::k_filesystem };

		ecf_id_t unknown18;					// The (internal) file names chunk zeros this out
		boost::uint32_t uncompressed_size;	// If uncompressed_size == size, then this chunk isn't compressed
		ecf_id_t id;						// same id as s_ecf_chunk_header::id
		ecf_id_t id2;
		boost::uint32_t filename_offset;	// offset is actually shifted by 4 bits (or maybe this is a uint16 field in b/w two byte fields?)

		boost::uint32_t GetFilenameOffset() { return filename_offset >> 4; }
	}; BOOST_STATIC_ASSERT( sizeof(s_ecf_filesystem_chunk) == 0x20 );
#include <PopPack.h>

	// Typical file layout:
	// s_ecf_header
		// s_era_filesystem_signature
		// s_era_filesystem_hashtable
			// s_ecf_chunk_header+s_ecf_filesystem_chunk
				// chunk's data...

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