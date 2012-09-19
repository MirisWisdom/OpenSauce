/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum cryptography_xxtea_block_size
		{
			_cryptography_xxtea_block_size_8	= 8, // minimum block size for XXTEA is 64 bits
			_cryptography_xxtea_block_size_16	= 16,
			_cryptography_xxtea_block_size_32	= 32,
			_cryptography_xxtea_block_size_64	= 64,
			_cryptography_xxtea_block_size_128	= 128,
			_cryptography_xxtea_block_size_256	= 256,

			_cryptography_xxtea_block_size,
		};
	};
	namespace Cryptography { namespace XXTEA
	{
		#define MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z);

		void XXTEAEncrypt(unsigned long* data, unsigned long block_size, long* key);
		void XXTEADecrypt(unsigned long* data, unsigned long block_size, long* key);
		bool Encrypt(Enums::cryptography_xxtea_block_size block_size,
			char* data,
			uint32 data_length,
			byte* key);
		bool Decrypt(Enums::cryptography_xxtea_block_size block_size,
			char* data,
			uint32 data_length,
			byte* key);
	};};
};