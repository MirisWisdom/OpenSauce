/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Text;
using System.IO;
using LowLevel;

namespace BlamLib.Cryptography
{
	public class XXTEA
	{
		/// <summary>
		/// Generates an XXTEA encryption/decryption key using the MD5 hash of the provided password
		/// </summary>
		/// <param name="key">The 4 int output array for the generated password key</param>
		/// <param name="password">The password to generate a key with</param>
		public static void BuildPasswordKey(int[] key, string password)
		{
			if (key.Length != 4)
				throw new Exception("Output key array for XXTEA encryption is too short");

			// get the md5 hash of the password
			byte[] password_md5 = BlamLib.Cryptography.MD5.GenerateMD5(Encoding.ASCII.GetBytes(password.ToCharArray()));

			// copy the md5 has to the key array
			int password_index = 0;
			for (int i = 0; i < 4; i++)
			{
				key[i] = ((int)password_md5[password_index] << 0) |
					((int)password_md5[password_index + 1] << 8) |
					((int)password_md5[password_index + 2] << 16) |
					((int)password_md5[password_index + 3] << 24);

				password_index += 4;
			}
		}

		/// <summary>
		/// Encrypts a data stream using the XXTEA algorithm at a 256 byte block length
		/// </summary>
		/// <param name="stream_in">The stream to encrypt</param>
		/// <param name="password">The 16 byte encryption key to use</param>
		/// <returns>True if the encryption was successful</returns>
		public static bool EncryptStream256(Stream stream_in, int[] password)
		{
			if (password.Length != 4)
				throw new Exception("Password supplied for XXTEA encryption is not 4 ints long");

			// get the number of blocks and any remainder
			int block_count = (int)stream_in.Length / 256;
			int block_remainder = (int)stream_in.Length % 256;

			byte[] block = new byte[256];

			stream_in.Seek(0, SeekOrigin.Begin);

			// encrypt the stream in 256 byte blocks
			for (int i = 0; i < block_count; i++)
			{
				// read the block to encrypt
				stream_in.Read(block, 0, 256);

				if (!LowLevel.Cryptography.XXTEA.Encrypt256(block, password[0], password[1], password[2], password[3]))
					return false;

				// overwrite the original data with the encrypted block
				stream_in.Seek(-256, SeekOrigin.Current);
				stream_in.Write(block, 0, 256);
			}

			// if there is some remainder, encrypt the last 256 bytes with some overlap so that the entire file is encrypted
			if (block_remainder > 0)
			{
				// read the last 256 byte block
				stream_in.Seek(-256, SeekOrigin.End);

				stream_in.Read(block, 0, 256);

				if (!LowLevel.Cryptography.XXTEA.Encrypt256(block, password[0], password[1], password[2], password[3]))
					return false;

				// overwrite the original data with the encrypted block
				stream_in.Seek(-256, SeekOrigin.Current);
				stream_in.Write(block, 0, 256);
			}

			return true;
		}

		/// <summary>
		/// Decrypts a data stream using the XXTEA algorithm at a 256 byte block length
		/// </summary>
		/// <param name="stream_in">The stream to decrypt</param>
		/// <param name="password">The 16 byte encryption key to use</param>
		/// <returns>True if the decryption was successful</returns>
		public static bool DecryptStream256(Stream stream_in, int[] password)
		{
			if (password.Length != 4)
				throw new Exception("Password supplied for XXTEA decryption is not 4 ints long");

			// get the number of blocks and any remainder
			int block_count = (int)stream_in.Length / 256;
			int block_remainder = (int)stream_in.Length % 256;

			byte[] block = new byte[256];

			// decrypt the last 256 bytes first if there is a remainder
			if (block_remainder > 0)
			{
				// read the last 256 byte block
				stream_in.Seek(-256, SeekOrigin.End);

				stream_in.Read(block, 0, 256);

				if (!LowLevel.Cryptography.XXTEA.Decrypt256(block, password[0], password[1], password[2], password[3]))
					return false;

				// overwrite the original data with the decrypted block
				stream_in.Seek(-256, SeekOrigin.Current);
				stream_in.Write(block, 0, 256);
			}

			stream_in.Seek(0, SeekOrigin.Begin);

			// decrypt the stream in 256 byte blocks
			for (int i = 0; i < block_count; i++)
			{
				// read the block to decrypt
				stream_in.Read(block, 0, 256);

				if (!LowLevel.Cryptography.XXTEA.Decrypt256(block, password[0], password[1], password[2], password[3]))
					return false;

				// overwrite the original data with the decrypted block
				stream_in.Seek(-256, SeekOrigin.Current);
				stream_in.Write(block, 0, 256);
			}

			return true;
		}
	}
}
