/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;

namespace S3D.IO
{
	public enum DataType : int
	{
		Null,

		SceneData,
		CacheBlock,
		Shader,			ShaderCache,										// 0x3, 0x4
		TexturesInfo,	Textures,		TexturesMips64,						// 0x5, 0x6, 0x7
		SoundData,		Sounds,			WaveBanks_mem,	WaveBanks_strm_file,// 0x8, 0x9, 0xA, 0xB
		Templates,															// 0xC
		VoiceSplines,
		Strings,
		Ragdolls,
		Scene,																// 0x10
		Hkx,
		Gfx,
		TexturesDistanceFile,
		CheckPointTexFile,
		LoadingScreenGfx,

		SceneGrs,		SceneScr,		SceneAnimbin,	SceneRain,			// 0x16
		SceneCDT,		SceneSM,		SceneSLO,		SceneVis,			// 0x1A
		AnimStream,															// 0x1E
		AnimBank
	};
	public static class DataTypeUtil
	{
		public static string ToFileExtension(this DataType type)
		{
			return "." + type.ToString(); // TODO
		}

		public static DataType FromFileExtension(string ext)
		{
			string enum_member = ext.Substring(1);
			var type = (DataType)Enum.Parse(typeof(DataType), enum_member);

			return type;
		}

		public static bool IsTextBuffer(DataType type)
		{
			switch (type)
			{
				case DataType.Null:			//
				//case DataType.SceneData:

				case DataType.AnimStream:
					return true;

				default: return false;
			}
		}
	};

	// TexturesInfoEntry
	//	int name length
	//	string name
	//	tag type
	//	int width, height?
	//	int unknown1, unknown2, unknown3, unknown4

	public class PakFileEntry : BlamLib.IO.IStreamable
	{
		const int kSizeOf = 4 + 4 + 4 + 
			0 + // Name field is N-length
			4 + 4 + 4;

		public int DataOffset { get; private set; }
		public int DataSize { get; private set; }
		public string Name { get; private set; }
		public DataType Type { get; private set; }
		int unknown1, unknown2;

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			DataOffset = s.ReadInt32();
			DataSize = s.ReadInt32();
			int name_length = s.ReadInt32();
			if (name_length > 0) Name = s.ReadAsciiString(name_length);
			else Name = string.Empty;
			Type = (DataType)s.ReadInt32();
			unknown1 = s.ReadInt32();
			unknown2 = s.ReadInt32();

			if (unknown1 != 0) System.Diagnostics.Debugger.Break();
			if (unknown2 != 0) System.Diagnostics.Debugger.Break();
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(DataOffset);
			s.Write(DataSize);
			s.Write(Name.Length);
			if(Name.Length > 0) s.Write(Name.ToCharArray());
			s.Write((int)Type);
			s.Write(unknown1);
			s.Write(unknown2);
		}
		#endregion

		internal string GetFilename(int entry_index)
		{
			if (!string.IsNullOrEmpty(Name)) return Name;

			return string.Format("_Noname{0}", entry_index.ToString("X4"));
		}

		internal int CalculateSizeOfOnDisk(ref int disk_offset)
		{
			int size = kSizeOf;
			size += Name.Length;

			disk_offset += size;
			return size;
		}
		internal void CalculateDataValues(string base_directory, int entry_index, int disk_offset)
		{
			string filename = GetFilename(entry_index);
			filename = Path.Combine(base_directory, filename);
			filename += Type.ToFileExtension();

			var fi = new FileInfo(filename);
			DataSize = (int)fi.Length;
		}
	};
	public abstract class PakFile : BlamLib.IO.IStreamable
	{
		protected List<PakFileEntry> m_entries;

		#region IStreamable Members
		public virtual void Read(BlamLib.IO.EndianReader s)
		{
			int entries_count = s.ReadInt32();
			m_entries = new List<PakFileEntry>(entries_count);
			for (int x = 0; x < m_entries.Count; x++)
			{
				var entry = new PakFileEntry();
				entry.Read(s);

				m_entries.Add(entry);
			}
		}

		public virtual void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(m_entries.Count);
			foreach (var entry in m_entries) entry.Write(s);
		}
		#endregion
	};

	public sealed class PakFileExpander : PakFile
	{

	};

	public sealed class PakFileBuilder : PakFile
	{
		#region IStreamable Members
		void WritePreprocess(BlamLib.IO.EndianWriter s)
		{
			int offset = sizeof(int);
			foreach (var entry in m_entries) entry.CalculateSizeOfOnDisk(ref offset);

			// TODO: update data values
		}
		public override void Write(BlamLib.IO.EndianWriter s)
		{
			WritePreprocess(s);
			base.Write(s);
			// TODO: write entry data
		}
		#endregion
	};
};