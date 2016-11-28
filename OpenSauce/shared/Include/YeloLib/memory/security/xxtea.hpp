/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
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

			k_number_of_cryptography_xxtea_block_sizes,
		};
	};
	namespace Cryptography { namespace XXTEA
	{
		void XXTEAEncrypt(uint32* data, uint32 block_size, int32* key);
		void XXTEADecrypt(uint32* data, uint32 block_size, int32* key);
		bool Encrypt(Enums::cryptography_xxtea_block_size block_size,
			void* data,
			uint32 data_length,
			byte* key);
		bool Decrypt(Enums::cryptography_xxtea_block_size block_size,
			void* data,
			uint32 data_length,
			byte* key);
	}; };
};