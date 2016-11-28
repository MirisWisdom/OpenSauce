/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace BlamLib.IO
{
	/// <summary>
	/// Creates a listing of a directory using a file extension
	/// as the key and a file listing as the value
	/// </summary>
	[IO.Class((int)IO.TagGroups.Enumerated.DirectoryListingByExtension, 0)]
	public sealed class DirectoryListingByExtension : FileManageable
	{
		#region WorkingDirectory
		string Directory;
		/// <summary>
		/// Directory path used for this listing
		/// </summary>
		public string WorkingDirectory { get { return Directory; } }
		#endregion

		Dictionary<string, List<string>> Files = new Dictionary<string, List<string>>();

		#region Creation
		/// <summary>
		/// Create database
		/// </summary>
		/// <remarks>Used for reading database from a file</remarks>
		internal DirectoryListingByExtension() : base() {}
		/// <summary>
		/// Create database from an existing directory
		/// </summary>
		/// <param name="start"></param>
		public DirectoryListingByExtension(string start) : base()
		{
			start = Path.GetDirectoryName(start) + "\\";
			Debug.Assert.If(System.IO.Directory.Exists(start),
				"Couldn't create directory listing! Path doesn't exist:'{0}'", start);
			Directory = start;
			DirectoryInfo i = new DirectoryInfo(start);
			Add(i);
		}

		/// <summary>
		/// Add a file to the listing
		/// </summary>
		/// <param name="file">Info data for a file on disk</param>
		void Add(FileInfo file)
		{
			List<string> list;
			string tmp = file.Extension.Remove(0, 1);
			if (!Files.TryGetValue(tmp, out list))
			{
				list = new List<string>();
				Files.Add(tmp, list);
			}
			list.Add(file.FullName.Replace(Directory, "").Replace(file.Extension, ""));
		}

		/// <summary>
		/// Add a directory to the listing
		/// </summary>
		/// <param name="dir">Info data for a directory on disk</param>
		void Add(DirectoryInfo dir)
		{
			FileInfo[] files = dir.GetFiles();
			foreach (FileInfo fi in files) Add(fi);

			DirectoryInfo[] dirs = dir.GetDirectories();
			foreach (DirectoryInfo di in dirs) Add(di);
		}
		#endregion

		/// <summary>
		/// Tries to find a file in the database
		/// </summary>
		/// <param name="file">relative file path</param>
		/// <param name="ext">extension of the file</param>
		/// <returns></returns>
		public bool Query(string file, string ext)
		{
			if (Files.Count == 0) return false;

			List<string> list;
			if(Files.TryGetValue(ext, out list))
			{
				foreach (string s in list)
					if (file == s) return true;
			}
			return false;
		}

		#region IStreamable Members
		public override void Read(EndianReader s)
		{
			Directory = s.ReadString();
			int count = s.ReadInt32();
			int subcount;
			List<string> list;
			string str;
			for(int x = 0; x < count; x++)
			{
				str = s.ReadString();
				subcount = s.ReadInt32();
				list = new List<string>(subcount);
				for (int y = 0; y < subcount; y++)
					list.Add(s.ReadString());
				Files.Add(str, list);
			}
		}

		public override void Write(EndianWriter s)
		{
			s.Write(Directory);
			s.Write(Files.Count);
			foreach (string str in Files.Keys)
			{
				List<string> files = Files[str];
				s.Write(str);
				s.Write(files.Count);
				foreach (string file in files)
					s.Write(file);
			}
		}
		#endregion
	};
}