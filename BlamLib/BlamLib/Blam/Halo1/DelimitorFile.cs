/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace BlamLib.Blam.Halo1
{
#if DEBUG
	/// <summary>
	/// OpenSauce: Halo1_Xbox code related, for adding compatibility to custom UIs
	/// </summary>
	public class DelimitorFile : IO.IStreamable
	{
		public static readonly char[] Ext = "delf".ToCharArray();
		const int StringPoolSize = 0x2B11;
		const int MaxCount = 0xF5;

		#region Maps
		private int n_offset = 0, s_offset = 0;
		private List<int> NameOffsets = new List<int>();
		private List<string> Names = new List<string>();
		private List<int> ScenarioOffsets = new List<int>();
		private List<string> Scenarios = new List<string>();
		public void Add(string path, string file)
		{
			Names.Add(file);
			string scnr = Path.Combine(path, file);
			Scenarios.Add(scnr);

			n_offset += file.Length + 1;
			NameOffsets.Add(n_offset);

			ScenarioOffsets.Add(s_offset);
			s_offset += scnr.Length + 1;
		}
		#endregion

		#region Building
		private MemoryStream PrepareStrings()
		{
			MemoryStream ms = new MemoryStream(StringPoolSize + ((MaxCount * 2) * sizeof(int)));
			IO.EndianWriter s = new BlamLib.IO.EndianWriter(ms);
			//int offset = 0;

			//NameOffsets = new List<int>(Names.Count);
			//foreach (string str in Names)
			//{
			//    offset = (int)ms.Position;
			//    NameOffsets.Add(offset);
			//    s.Write(str, true);
			//}

			//ScenarioOffsets = new List<int>(Scenarios.Count);
			//foreach (string str in Scenarios)
			//{
			//    offset = (int)ms.Position;
			//    ScenarioOffsets.Add(offset);
			//    s.Write(str, true);
			//}

			foreach (string str in Scenarios)
				s.Write(str, true);
			int len = StringPoolSize - (int)ms.Position;
			if (len > 0)
				for (int x = 0; x < len; x++)
					s.Write(byte.MinValue);

			len = MaxCount - ScenarioOffsets.Count;

			foreach (int i in ScenarioOffsets)
				s.Write(i);

			if(len > 0)
				for (int x = 0; x < len; x++)
					s.Write(0);

			foreach (int i in NameOffsets)
				s.Write(i);

			if (len > 0)
				for (int x = 0; x < len; x++)
					s.Write(0);

			return ms;
		}

		public void Build(string path)
		{
			IO.EndianWriter file = new BlamLib.IO.EndianWriter(path, BlamLib.IO.EndianState.Little, this, true);

			MemoryStream ms = PrepareStrings();

			file.Write(Names.Count);
			//file.Write(ms.GetBuffer().Length);

			//foreach (int offset in NameOffsets)
			//    file.Write(offset);

			//foreach (int offset in ScenarioOffsets)
			//    file.Write(offset);

			file.Write(ms.GetBuffer());

			ms.Close();
			file.Close();
		}
		#endregion

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			char[] tag = s.ReadTag();
			if (!TagInterface.TagGroup.Test(tag, Ext))
				throw new Debug.ExceptionLog("File '{0}' had a bad header '{1}'", s.FileName, new string(tag));

			int count = s.ReadInt32();

			//Names = new List<string>(count);
			//ScenarioOffsets = new List<int>(count);

			for (int x = 0; x < count; x++)
			{
				Names.Add(s.ReadString());
				Scenarios.Add(s.ReadString());
			}
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.WriteTag(Ext);
			int count = Names.Count;
			s.Write(count);
			for (int x = 0; x < count; x++)
			{
				s.Write(Names[x]);
				s.Write(Scenarios[x]);
			}
		}
		#endregion
	};
#endif
}