/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.IO;
using System.Security.Cryptography;

namespace FilePatcher.Utilities
{
	public static class StreamIO
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Copies one stream to another. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="streamIn"> 	The stream in. </param>
		/// <param name="streamOut">	The stream out. </param>
		public static void CopyStream(Stream streamIn, Stream streamOut)
		{
			if((streamIn == null) || (streamOut == null))
			{
				throw new ArgumentNullException("streamIn/Out cannot be null");
			}

			if (!streamIn.CanRead)
			{
				throw new NotSupportedException("Cannot read from the input stream");
			}

			if (!streamOut.CanWrite)
			{
				throw new NotSupportedException("Cannot write to the output stream");
			}

			// Set both streams to the beginning
			streamIn.Seek(0, SeekOrigin.Begin);
			streamOut.Seek(0, SeekOrigin.Begin);

			var buffer = new byte[4096];

			// Read the source stream and write to the target stream in blocks of 4096 bytes
			long count = streamIn.Length;
			while (count > 0)
			{
				long readCount = (count < 4096 ? count : 4096);

				streamIn.Read(buffer, 0, (int)readCount);
				streamOut.Write(buffer, 0, (int)readCount);

				count -= readCount;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates an MD5 checksum for a stream. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="streamIn">	The stream to read. </param>
		///
		/// <returns>	An MD5 checksum. </returns>
		public static string MD5Stream(Stream streamIn)
		{
			if (streamIn == null)
			{
				throw new ArgumentNullException("streamIn cannot be null");
			}

			if (!streamIn.CanRead)
			{
				throw new NotSupportedException("Cannot read from the input stream");
			}

			// Create an md5 hash for the stream and return it's string representation
			using (var md5 = MD5.Create())
			{
				if (md5 == null)
				{
					return null;
				}

				streamIn.Seek(0, SeekOrigin.Begin);

				var md5Hash = md5.ComputeHash(streamIn);

				return BitConverter.ToString(md5Hash).Replace("-", "");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Reads a file to memory, optionally validating against a checksum after opening. </summary>
		///
		/// <param name="filePath">	Full pathname of the file. </param>
		/// <param name="testMd5"> 	(Optional) the MD5 checksum to validate the file with. </param>
		///
		/// <returns>	The file read into a MemoryStream. </returns>
		public static MemoryStream ReadFileToMemory(string filePath, string testMd5 = null)
		{
			if (filePath == null)
			{
				throw new ArgumentNullException("The file path cannot be null");
			}

			if (filePath == String.Empty)
			{
				throw new ArgumentException("The file path cannot be empty");
			}

			if (testMd5 == String.Empty)
			{
				throw new ArgumentException("The test MD5 checksum cannot be empty");
			}

			// Expand any environment variables, and check the file exists
			var filePathExpanded = Environment.ExpandEnvironmentVariables(filePath);
			if (!File.Exists(filePathExpanded))
			{
				return null;
			}

			MemoryStream fileData = null;

			// Open the source file
			using (var openedFile = File.OpenRead(filePath))
			{
				if (openedFile == null)
				{
					return null;
				}

				if(testMd5 != null)
				{
					// Check the source files md5 matches
					string fileMd5 = MD5Stream(openedFile);
					if ((fileMd5 == null) || !fileMd5.Equals(testMd5, StringComparison.CurrentCultureIgnoreCase))
					{
						return null;
					}
				}

				// Create a new stream in memory and copy the file to it
				fileData = new MemoryStream();

				try
				{
					CopyStream(openedFile, fileData);
				}
				catch (Exception)
				{
					return null;
				}
			}

			return fileData;
		}
	}
}
