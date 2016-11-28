/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		struct resource_handle
		{
			public Blam.DatumIndex tag_index;
			public int resource_type;
			public int start_offset;
			public int total_size;
			public int offset;
			public int resources_count;
			public uint resources_offset;
			public int definitions_count;
			public uint definitions_offset;

			public void Read(Blam.Halo3.CacheFileBase cf)
			{
				cf.InputStream.Seek(4 + 4 + 4, SeekOrigin.Current);
				tag_index.Read(cf.InputStream);

				cf.InputStream.ReadInt16();
				resource_type = cf.InputStream.ReadByte();
				cf.InputStream.ReadByte();

				start_offset = cf.InputStream.ReadInt32();
				total_size = cf.InputStream.ReadInt32();
				offset = cf.InputStream.ReadInt32();
				cf.InputStream.ReadInt16();
				cf.InputStream.ReadInt16();
				cf.InputStream.ReadInt32();

				resources_count = cf.InputStream.ReadInt32();
				resources_offset = cf.InputStream.ReadPointer();
				cf.InputStream.ReadInt32();
				definitions_count = cf.InputStream.ReadInt32();
				definitions_offset = cf.InputStream.ReadPointer();
				cf.InputStream.ReadInt32();
			}

			public void ToString(int index, Blam.Halo3.CacheFileBase cf, StreamWriter sw)
			{
				if (tag_index.Index == ushort.MaxValue)
				{
					sw.WriteLine("{0:X}\t(NULL)", index);
					return;
				}

				var ci = cf.IndexHalo3[tag_index.Index];
				sw.WriteLine("{0:X}\t{1} {2}", index, ci.GroupTag.Name, cf.GetReferenceName(ci));
				sw.WriteLine(
					"\ttype: {0}" + Program.NewLine +
					"\tstart: {1:X}" + Program.NewLine +
					"\tsize: {2:X}" + Program.NewLine +
					"\toffset: {3:X}" + Program.NewLine +
					"\tresources: {4:X}\t{5:X}" + Program.NewLine +
					"\tdefinitions: {6:X}\t{7:X}",
					resource_type, start_offset, total_size,
					offset, resources_count, resources_offset,
					definitions_count, definitions_offset
					);
			}
		};

		internal static void DumpZoneDataMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;
			
			string out_dir = Path.Combine(kTestResultsPath, @"\resource_dump\");
			if (!Directory.Exists(out_dir))
				Directory.CreateDirectory(out_dir);

			using (var handler = new CacheHandler<Blam.Halo3.CacheFileBase>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				string header_name = cache.Header.Name;
				if ((args.Game & BlamVersion.HaloOdst) == 0 && MapNeedsUniqueName(header_name))
					header_name = cache.GetUniqueName();

				var ci = cache.IndexHalo3[3]; // zone
				cache.InputStream.Seek(ci.Offset);

				using (var fs = File.Create(Path.Combine(out_dir, header_name) + ".zone"))
				{
					fs.Write(cache.InputStream.ReadBytes(532), 0, 532);
				}

				using (var sw = File.CreateText(Path.Combine(out_dir, header_name) + ".zone.txt"))
				{
					sw.WriteLine("Mask: {0:X}", cache.AddressMask);
					sw.WriteLine();
					sw.WriteLine("Zone: {0:X}", ci.Offset);
					sw.WriteLine();
					cache.InputStream.Seek(ci.Offset + 0x58);
					int count;
					sw.WriteLine("Count: {0:X}", count = cache.InputStream.ReadInt32());

					var handles = new resource_handle[count];
					handles.Initialize();
					cache.InputStream.Seek(cache.InputStream.ReadPointer());
					for (int x = 0; x < count; x++)
					{
						handles[x].Read(cache);
						handles[x].ToString(x, cache, sw);
						sw.WriteLine();
						sw.Flush();
					}
					handles = null;
				}
			}

			args.SignalFinished();
		}

		[TestMethod]
		public void Halo3TestDumpZoneDataXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext, DumpZoneDataMethod,
				BlamVersion.Halo3_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		[TestMethod]
		public void Halo3TestDumpZoneDataXboxEpsilon()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext, DumpZoneDataMethod,
				BlamVersion.Halo3_Epsilon, kDirectoryXbox, kMapNames_Epsilon);
		}
	};
}