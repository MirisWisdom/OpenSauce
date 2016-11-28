using System;
using System.Text;
using System.IO;

namespace BlamLib.Cryptography
{
	public static class MD5
	{
		/// <summary>
		/// Calculates an MD5 hash from a Stream
		/// </summary>
		/// <param name="data">The stream to generate an MD5 for</param>
		/// <returns>A 16 byte array containing the MD5 hash</returns>
		public static byte[] GenerateMD5(Stream data)
		{
			using (System.Security.Cryptography.MD5 md5hash = System.Security.Cryptography.MD5.Create())
			{
				// store the current position then set to the beginning
				long position = data.Position;
				data.Seek(0, SeekOrigin.Begin);

				byte[] computed_hash = md5hash.ComputeHash(data);

				// restore the stream position
				data.Seek(position, SeekOrigin.Begin);

				return computed_hash;
			}
		}

		/// <summary>
		/// Calculates an MD5 hash from an array of bytes
		/// </summary>
		/// <param name="data">The byte array to generate an MD5 for</param>
		/// <returns>A 16 byte array containing the MD5 hash</returns>
		public static byte[] GenerateMD5(byte[] data)
		{
			using (System.Security.Cryptography.MD5 md5hash = System.Security.Cryptography.MD5.Create())
				return md5hash.ComputeHash(data, 0, data.Length);
		}

		/// <summary>
		/// Calculates the MD5 hash of a file
		/// </summary>
		/// <param name="file_path">The file to generate an MD5 hash for</param>
		/// <returns>A 16 byte array containing the MD5 hash</returns>
		public static byte[] GenerateFileMD5(string file_path)
		{
			using (FileStream input_file = new FileStream(file_path, FileMode.Open, FileAccess.Read))
				return GenerateMD5(input_file);
		}

		/// <summary>
		/// Calculates the MD5 hash string of a file
		/// </summary>
		/// <param name="file_path">The file to generate an MD5 hash for</param>
		/// <returns>A string containing the MD5 hash</returns>
		public static string GenerateFileMD5String(string file_path)
		{
			byte[] md5;

			using (FileStream input_file = new FileStream(file_path, FileMode.Open, FileAccess.Read))
				md5 = GenerateMD5(input_file);

			return Util.ByteArrayToString(md5);
		}

		/// <summary>
		/// Calculates an MD5 hash string from a Stream
		/// </summary>
		/// <param name="data">The stream to generate an MD5 for</param>
		/// <returns>A string containing the MD5 hash</returns>
		public static string GenerateMD5String(Stream data)
		{
			byte[] md5 = GenerateMD5(data);

			return Util.ByteArrayToString(md5);
		}

		/// <summary>
		/// Calculates an MD5 hash string from an array of bytes
		/// </summary>
		/// <param name="data">The byte array to generate an MD5 for</param>
		/// <returns>A string containing the MD5 hash</returns>
		public static string GenerateMD5String(byte[] data)
		{
			byte[] md5 = GenerateMD5(data);

			return Util.ByteArrayToString(md5);
		}
	}
}
