/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once
#include "Precompile.hpp"
#include "Cryptography/XXTEA.hpp"

namespace LowLevel { namespace Cryptography
{
	using namespace System;

	#define MX (z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + (key[p & 3 ^ e] ^ z);

	static void XXTEAEncrypt(unsigned long* data, unsigned long block_size, int* key)
	{
		unsigned long z = data[block_size - 1], y = data[0], sum = 0, e, DELTA = 0x9e3779b9;
		long p, q ;

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

	static void XXTEADecrypt(unsigned long* data, unsigned long block_size, int* key)
	{
		unsigned long z = data[block_size - 1], y = data[0], sum = 0, e, DELTA = 0x9e3779b9;
		long p, q ;

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

	#define BLOCK_SIZE 256
	mcpp_bool XXTEA::Encrypt256(array<Byte>^ data, int key_1, int key_2, int key_3, int key_4)
	{
		int key[4] = { key_1, key_2, key_3, key_4 };

		// get a pointer to the managed data
		pin_ptr<Byte> data_pointer = &data[0];

		// get the block size in longs and encrypt the block
		unsigned long* data_long = CAST_PTR(unsigned long*, data_pointer);
		int block_size_long = BLOCK_SIZE / sizeof(long);

		XXTEAEncrypt(data_long, block_size_long, &key[0]);

		return mcpp_true;
	}

	mcpp_bool XXTEA::Decrypt256(array<Byte>^ data, int key_1, int key_2, int key_3, int key_4)
	{
		int key[4] = { key_1, key_2, key_3, key_4 };

		// get a pointer to the managed data
		pin_ptr<unsigned char> data_pointer = &data[0];

		// get the block size in longs and decrypt the block
		unsigned long* data_long = CAST_PTR(unsigned long*, data_pointer);
		int block_size_long = BLOCK_SIZE / sizeof(long);

		XXTEADecrypt(data_long, block_size_long, &key[0]);

		return mcpp_true;
	}
	#undef BLOCK_SIZE
};};