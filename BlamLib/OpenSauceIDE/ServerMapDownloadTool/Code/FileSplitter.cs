using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Security.Cryptography;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	public static class FileSplitter
	{
		public struct FileListEntry
		{
			public string Location;
			public string MD5;
			public int Index;
			public uint Size;
			public bool Encrypted;
			public string UnencryptedMD5;
		};

		public static List<FileListEntry> SaveStream(Stream input_stream,
			int max_part_size,
			string output_folder,
			string file_name,
			bool encrypt,
			string password)
		{
			List<FileListEntry> file_list = new List<FileListEntry>();

			// set the stream data read position to the beginning
			input_stream.Seek(0, SeekOrigin.Begin);

			int index = 0;
			int stream_size = (int)input_stream.Length;
			int stream_offset = 0;

			byte[] part_data = new byte[max_part_size];

			do
			{
				FileListEntry file_list_entry = new FileListEntry();
				Directory.CreateDirectory(output_folder);
				file_list_entry.Location = String.Format("{0}\\{1}.{2}", output_folder, file_name, (index + 1).ToString("D03"));
				file_list_entry.Index = index;

				// create a part file
				using (FileStream part_file = File.Open(file_list_entry.Location, FileMode.Create))
				{
					// calculate the part size
					int difference = stream_size - stream_offset;
					int part_size = (int)(difference < max_part_size ? difference : max_part_size);

					// if the last part is going to be less than the minimum encryption block size
					// reduce the size of the one before to make the last part the minimum size
					if ((part_size == max_part_size) && ((stream_size - stream_offset - max_part_size) < 256))
						part_size -= 256 - (stream_size - stream_offset - max_part_size);

					// read the data to an intermediary buffer and write the buffer to disc
					input_stream.Read(part_data, 0, part_size);
					part_file.Write(part_data, 0, part_size);

					if (encrypt)
					{
						// the password key is an MD5 hash of the password string
						int[] key = new int[4];
						BlamLib.Cryptography.XXTEA.BuildPasswordKey(key, password);

						// get the unencrypted md5 for per part decryption comparisons on the client
						file_list_entry.Encrypted = true;
						file_list_entry.UnencryptedMD5 = BlamLib.Cryptography.MD5.GenerateMD5String(part_file);

						// encrypt the file stream
						BlamLib.Cryptography.XXTEA.EncryptStream256(part_file, key);
					}
					else
					{
						file_list_entry.Encrypted = false;
						file_list_entry.UnencryptedMD5 = "";
					}

					part_file.Flush();

					// get the final files MD5 for download validation on the client
					file_list_entry.MD5 = BlamLib.Cryptography.MD5.GenerateMD5String(part_file);
					file_list_entry.Size = (uint)part_file.Length;

					file_list.Add(file_list_entry);
				}
				index++;

				stream_offset += max_part_size;
			}
			while (stream_offset < stream_size);

			return file_list;
		}
	}
}
