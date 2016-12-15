using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.ComponentModel;
using System.Windows.Forms;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	public static class MapCompressor
	{
		public struct MapCompressorArgs
		{
			public string Map;
			public string PartsFolder;
			public string DefinitionsFolder;
			public bool EncryptArchive;
			public string ServerPassword;
		}

		/// <summary>
		/// Creates the directories the map data will be saved to.
		/// </summary>
		/// <param name="map">Name of the map being compressed</param>
		/// <param name="output_root">The output directory, chosen by the user</param>
		/// <param name="output_folder">Output string for the download files folder</param>
		/// <param name="archive_folder">Output string for the map parts folder</param>
		/// <returns>Returns true if successfull, otherwise returns false</returns>
		private static bool BuildOutputFolders(string map, string output_root, out string output_folder)
		{
			// build the output paths
			string map_name = Path.GetFileNameWithoutExtension(map);
			output_folder = Path.Combine(output_root, map_name);

			// delete the output folder to ensure no left over files remain from previous compressions
			if (Directory.Exists(output_folder))
				Directory.Delete(output_folder, true);

			// create the folders
			try
			{
				Directory.CreateDirectory(output_folder);
			}
			catch (Exception)
			{
				return false;
			}
			return true;
		}

		/// <summary>
		/// Compresses the chosen map.
		/// </summary>
		/// <param name="worker_thread">The worker hred this function is being run by</param>
		/// <param name="args">Generic variable for passing an arguments class</param>
		public static void CompressMap(BackgroundWorker worker_thread, object args)
		{
			if (args == null)
			{
				throw new Exception("No arguments or an invalid arguments class passed to CompressMap");
			}

			MapCompressorArgs arguments = (MapCompressorArgs)args;

			// create the output folders
			string output_folder;

			if (!BuildOutputFolders(arguments.Map, arguments.PartsFolder, out output_folder))
			{
				throw new Exception("failed to create the output directories for map compression");
			}

			string map_filename = Path.GetFileNameWithoutExtension(arguments.Map);

			MapDownloadList.MapDownloadClass map_entry = null;
			List<FileSplitter.FileListEntry> file_list = null;

			uint UncompressedSize;
			uint CompressedSize;

			using(FileStream map_file = File.OpenRead(arguments.Map))
				UncompressedSize = (uint)map_file.Length;

			using (MemoryStream compressed_file = new MemoryStream())
			{
				try
				{
					SevenZip.SevenZipCompressor compressor = new SevenZip.SevenZipCompressor();

					compressor.ArchiveFormat = SevenZip.OutArchiveFormat.SevenZip;
					compressor.CompressionLevel = SevenZip.CompressionLevel.Ultra;
					compressor.CompressionMethod = SevenZip.CompressionMethod.Lzma;
					compressor.CompressionMode = SevenZip.CompressionMode.Create;

					// compress the map
					compressor.CompressFiles(compressed_file, arguments.Map);
				}
				catch (Exception e)
				{
					Exception temp = e;
					do
					{
						MessageBox.Show(temp.Message);
					} while ((temp = temp.InnerException) != null);
				}

				string archive_filename = string.Format("{0}.7z", map_filename);

				CompressedSize = (uint)compressed_file.Length;

				// save the archive parts, encrypting if necessary
				file_list = FileSplitter.SaveStream(compressed_file,
					1048576,
					output_folder,
					archive_filename,
					arguments.EncryptArchive,
					arguments.ServerPassword);
			}

			// create the map entry
			if (file_list != null)
			{
				map_entry = new MapDownloadList.MapDownloadClass();

				map_entry.Algorithm = MapDownloadList.MapDownloadCompressionFormat.SevenZip;
				map_entry.Name = Path.GetFileName(arguments.Map);
				map_entry.MD5 = BlamLib.Cryptography.MD5.GenerateFileMD5String(arguments.Map);
				map_entry.UncompressedSize = UncompressedSize;
				map_entry.CompressedSize = CompressedSize;
				map_entry.HostDirectory = Path.GetFileNameWithoutExtension(arguments.Map);

				map_entry.Parts = new List<MapDownloadList.MapPartClass>();

				// add all of the map parts to the map entry
				foreach (var file in file_list)
				{
					MapDownloadList.MapPartClass part = new MapDownloadList.MapPartClass();

					part.Name = Path.GetFileName(file.Location);
					part.MD5 = file.MD5;
					part.Index = file.Index;
					part.Size = file.Size;
					if (file.Encrypted)
					{
						part.Encrypted = true;
						part.UnencryptedMD5 = file.UnencryptedMD5;
					}

					map_entry.Parts.Add(part);
				}
			}

			// save the map entry to a map entry include xml
			MapDownloadList.MapIncludeClass map_include = new MapDownloadList.MapIncludeClass();

			map_include.MapDownload = map_entry;

			string xml_path = Path.Combine(arguments.DefinitionsFolder, String.Format("{0}.xml", map_filename));
			Util.SerializeObjectXML(map_include, xml_path);
		}
	};
}
