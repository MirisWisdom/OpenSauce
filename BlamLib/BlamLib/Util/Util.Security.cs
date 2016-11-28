/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace BlamLib
{
	partial class Util
	{
		#region Hash Encryption
		static byte[] BlamHashTable = {
			0x03, //  3
			0x07, //  7
			0x0B, // 11
			0x0D, // 13
			0x11, // 17
			0x13, // 19
			0x17, // 23
			0x1D, // 29
			0x1F, // 31
			0x25, // 37
			0x29, // 41
			0x2B, // 43
			0x2F, // 47
			0x35, // 53
			0x3B, // 59
		};
		/// <summary>
		/// Returns a hash of a string using the halo 2 algorithm
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		public static uint BlamHashString(string s)
		{
			uint result = 0;

			int table_index = 0;
			uint temp;
			for (int x = 0; x < s.Length; x++)
			{
				temp = BlamHashTable[table_index++];
				if (table_index >= BlamHashTable.Length) table_index = 0;

				result += temp * ((byte)s[x]);
			}

			return result;
		}

		/// <summary>
		/// Returns a hash of a buffer using the halo 2 algorithm
		/// </summary>
		/// <param name="array"></param>
		/// <returns></returns>
		public static uint BlamHashString(byte[] array)
		{
			uint result = 0;

			int table_index = 0;
			uint temp;
			for (int x = 0; x < array.Length; x++)
			{
				temp = BlamHashTable[table_index++];
				if (table_index >= BlamHashTable.Length) table_index = 0;

				result += temp * array[x];
			}

			return result;
		}
		#endregion

		#region CRC
		static readonly uint[] _CrcTable = BuildCrcTable();

		static uint[] BuildCrcTable()
		{
			uint[] crc_table = new uint[256];

			uint index, j;
			uint crc;

			for (index = 0; index < 256; index++)
			{
				crc = index;
				for (j = 0; j < 8; j++)
				{
					if ((crc & 1) == 1) crc = (crc >> 1) ^ 0xEDB88320;
					else crc >>= 1;
				}
				crc_table[index] = crc;
			}

			return crc_table;
		}

		public static uint CRC(ref uint crc, byte[] buffer, int size)
		{
			if (crc == 0)
				crc = 0xFFFFFFFF;

			uint a, b;
			int index = 0;

			while (size-- != 0)
			{
				a = (crc >> 8) & 0x00FFFFFF;
				b = _CrcTable[(int)crc ^ buffer[index++] & 0xFF];
				crc = a ^ b;
			}

			return crc;
		}
		#endregion
	};
}