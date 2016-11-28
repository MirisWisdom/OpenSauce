/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cryptography/TEA.hpp>

namespace Yelo
{
	namespace Cryptography { namespace TEA
	{
		static const ULONG g_encryption_key[4] = {	0x3FFFFFDD, 0x7FC3, 0x0E5, 0x3FFFEF	};

		void Encipher(ULONG *const data_in, ULONG *const data_out, const ULONG *const key)
		{
			ULONG
				y = data_in[0], z = data_in[1], 
				sum = 0,
				delta = 0x9E3779B9,
				a = key[0], b = key[1], c = key[2], d = key[3],
				n = 32;

			while(n-->0)
			{
				sum += delta;
				y += (z << 4) + a ^ z + sum ^ (z >> 5) + b;
				z += (y << 4) + c ^ y + sum ^ (y >> 5) + d;
			}

			data_out[0] = y; data_out[1] = z;
		}

		void Decipher(ULONG *const data_in, ULONG *const data_out, const ULONG *const key)
		{
			ULONG
				y = data_in[0], z = data_in[1], 
				sum = 0xC6EF3720,
				delta = 0x9E3779B9,
				a = key[0], b = key[1], c = key[2], d = key[3],
				n = 32;

			/* sum = delta<<5, in general sum = delta * n */

			while(n-->0)
			{
				z -= (y << 4) + c ^ y + sum ^ (y >> 5) + d;
				y -= (z << 4) + a ^ z + sum ^ (z >> 5) + b;
				sum -= delta;
			}

			data_out[0] = y; data_out[1] = z;
		}

		void Encode(char* data, DWORD file_data_size)
		{
			DWORD* data_pointer = CAST_PTR(DWORD*, data);
			// get how many bytes are at the end of the file, that wouldn't fill a cipher block
			DWORD block_overlap = file_data_size % 8;
			// get the block count
			DWORD block_count = ((file_data_size - block_overlap) / sizeof(DWORD)) / 2;
			// encode the TEA encrypted file
			DWORD* block_pointer = NULL;
			for(DWORD i = 0; i < block_count; i++)
			{
				block_pointer = data_pointer + (i * 2);
				Encipher(block_pointer, block_pointer, &g_encryption_key[0]);
			}
			// if there is overlap, encode the last 8 bytes of data
			if(block_overlap)
			{
				block_pointer = CAST_PTR(DWORD*, (data + file_data_size) - 8);
				Encipher(block_pointer, block_pointer, &g_encryption_key[0]);
			}
		}

		void Decode(char* data, DWORD data_size)
		{
			DWORD* data_pointer = CAST_PTR(DWORD*, data);
			// get how many bytes are at the end of the file, that wouldn't fill a cipher block
			DWORD block_overlap = data_size % 8;
			// if there is overlap, decode the last 8 bytes of data
			DWORD* block_pointer = NULL;
			if(block_overlap)
			{
				block_pointer = CAST_PTR(DWORD*, (data + data_size) - 8);
				Decipher(block_pointer, block_pointer, &g_encryption_key[0]);
			}

			// get the block count
			DWORD block_count = ((data_size - block_overlap) / sizeof(DWORD)) / 2;
			// decode the TEA encrypted file
			for(DWORD i = 0; i < block_count; i++)
			{
				block_pointer = data_pointer + (i * 2);
				Decipher(block_pointer, block_pointer, &g_encryption_key[0]);
			}
		}
	};};
};