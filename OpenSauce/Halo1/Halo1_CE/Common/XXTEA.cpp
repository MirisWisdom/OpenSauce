/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/XXTEA.hpp"

namespace Yelo
{
	namespace Cryptography { namespace XXTEA
	{
		#define MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z);

		/*!
		 * \brief
		 * Encrypts a block of data using the XXTEA algorithm.
		 * 
		 * \param data
		 * Pointer to the data to encrypt.
		 * 
		 * \param block_size
		 * The number of 32bit longs that "data" points to.
		 * 
		 * \param key
		 * Pointer to a 16 byte (128bit) encryption key.
		 * 
		 * Encrypts a block of data using the XXTEA algorithm.
		 */
		void XXTEAEncrypt(unsigned long* data, unsigned long block_size, long* key)
		{
			unsigned long z = data[block_size - 1], y = data[0], sum = 0, e, DELTA = 0x9e3779b9;
			unsigned long p, q ;

			q = 6 + 52 / block_size;
			while (q-- > 0)
			{
				sum += DELTA;
				e = (sum >> 2) & 3;
				for (p = 0; p < block_size - 1; p++) y = data[p + 1], z = data[p] += MX;
				y = data[0];
				z = data[block_size - 1] += MX;
			}
		}

		/*!
		 * \brief
		 * Decrypts a block of data using the XXTEA algorithm.
		 * 
		 * \param data
		 * Pointer to the data to decrypt.
		 * 
		 * \param block_size
		 * The number of 32bit longs that "data" points to.
		 * 
		 * \param key
		 * Pointer to a 16 byte (128bit) encryption key.
		 * 
		 * Decrypts a block of data using the XXTEA algorithm.
		 */
		void XXTEADecrypt(unsigned long* data, unsigned long block_size, long* key)
		{
			unsigned long z = data[block_size - 1], y = data[0], sum = 0, e, DELTA = 0x9e3779b9;
			unsigned long p, q ;

			q = 6 + 52 / block_size;
			sum = q * DELTA;
			while (sum != 0)
			{
				e = (sum >> 2) & 3;
				for (p = block_size - 1; p > 0; p--) z = data[p - 1], y = data[p] -= MX;
				z = data[block_size - 1];
				y = data[0] -= MX;
				sum -= DELTA;
			}
		}

		/*!
		 * \brief
		 * Encrypts a block of data using the XXTEA algorithm.
		 * 
		 * \param block_size
		 * The size to use for each encrypted data block.
		 * 
		 * \param data
		 * Pointer to the data being encrypted.
		 * 
		 * \param data_length
		 * Length of the data to encrypt.
		 * 
		 * \param key
		 * The 128 bit encryption key to use (16 bytes).
		 * 
		 * \returns
		 * Returns true of the encryption succeeded, otherwise returns false.
		 * 
		 * Encrypts a block of data using the XXTEA algorithm.
		 * 
		 * \remarks
		 * If the encrypted data length is not a multiple of the block size, the remaining data at the end will be encrypted
		 * seperately, with the overlap getting double encrypted.
		 */
		bool Encrypt(Enums::cryptography_xxtea_block_size block_size,
			char* data,
			uint32 data_length,
			byte* key)
		{
			ASSERT(block_size <= Enums::_cryptography_xxtea_block_size_256, "invalid xxtea block size");

			// if there is not enough data to fill at least one encryption block then it cannot be encrypted
			if(data_length < (uint32)block_size)
				return false;

			uint32 block_count = data_length / block_size;
			uint32 block_remainder = data_length % block_size;

			// encrypt the data in individual blocks, up to however many can be fit in the data provided
			char* data_pointer = data;
			for(uint32 i = 0; i < block_count; i++)
			{
				XXTEAEncrypt(CAST_PTR(unsigned long*, data_pointer), block_size / 4, CAST_PTR(long*, key));
				data_pointer += block_size;
			}

			// if there is some data remaining at the end that does not fill an encryption block a full block of data
			// at the end is encrypted seperately, with the overlap getting double encrypted.
			if(block_remainder)
			{
				data_pointer = data + (data_length - block_size);
				XXTEAEncrypt(CAST_PTR(unsigned long*, data_pointer), block_size / 4, CAST_PTR(long*, key));
			}

			return true;
		}


		/*!
		 * \brief
		 * Decrypts a block of data using the XXTEA algorithm.
		 * 
		 * \param block_size
		 * The size to use for each decrypted data block.
		 * 
		 * \param data
		 * Pointer to the data being decrypted.
		 * 
		 * \param data_length
		 * Length of the data to decrypt.
		 * 
		 * \param key
		 * The 128 bit decryption key to use (16 bytes).
		 * 
		 * \returns
		 * Returns true of the decryption succeeded, otherwise returns false.
		 * 
		 * Decrypts a block of data using the XXTEA algorithm.
		 * 
		 * \remarks
		 * If the decrypted data length is not a multiple of the block size, the remaining data at the end will be decrypted
		 * first, with the overlap getting double decrypted.
		 */
		bool Decrypt(Enums::cryptography_xxtea_block_size block_size,
			char* data,
			uint32 data_length,
			byte* key)
		{
			ASSERT(block_size <= Enums::_cryptography_xxtea_block_size_256, "invalid xxtea block size");

			// if there is not enough data to fill at least one decryption block then it cannot be decrypted
			if(data_length < (uint32)block_size)
				return false;

			uint32 block_count = data_length / block_size;
			uint32 block_remainder = data_length % block_size;

			// if there is data left over at the end that does not fill a decryption block, a full block is taken and decrypted seperately,
			// resulting in the overlap getting double decrypted
			char* data_pointer;
			if(block_remainder)
			{
				data_pointer = data + (data_length - block_size);
				XXTEADecrypt(CAST_PTR(unsigned long*, data_pointer), block_size / 4, CAST_PTR(long*, key));
			}

			// decrypt the data in individual blocks, up to however many can be fit in the data provided
			data_pointer = data;
			for(uint32 i = 0; i < block_count; i++)
			{
				XXTEADecrypt(CAST_PTR(unsigned long*, data_pointer), block_size / 4, CAST_PTR(long*, key));
				data_pointer += block_size;
			}

			return true;
		}
	};};
};