/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo4 : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
// 			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
// 				.StringIdCacheOpen(BlamVersion.Halo4_Xbox);

			Directory.CreateDirectory(kTestResultsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
// 			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
// 				.StringIdCacheClose(BlamVersion.Halo4_Xbox);
		}

		static readonly string[] kMapNames_Retail = {
		};

		internal IO.EndianReader DecryptCacheSegment(IO.EndianReader InputStream, Blam.CacheSectionType section_type, int segment_offset, int segment_size)
		{
			InputStream.Seek(segment_offset);
			uint buffer_size = Util.Align(16, (uint)segment_size);

			byte[] encrypted = InputStream.ReadBytes(buffer_size);
			byte[] decrypted;
			Blam.Halo4.GameDefinition.SecurityAesDecrypt(BlamVersion.Halo4_Xbox, section_type, encrypted, out decrypted);

			return decrypted != null ? new IO.EndianReader(decrypted) : null;
		}
		[TestMethod]
		public void Halo4StringIds()
		{
			Blam.Halo4.CacheHeader ch = new Blam.Halo4.CacheHeader();

			using (var s = new IO.EndianReader(kDirectoryXbox + @"Retail\maps\mainmenu.map", IO.EndianState.Big))
			{
				ch.Read(s);

				using (var buffer = DecryptCacheSegment(s, Blam.CacheSectionType.Tag, ch.TagNamesBufferOffset, ch.TagNamesBufferSize))
				{
					using (var fs = new FileStream(kTestResultsPath + "mainmenu_tag_names.bin", FileMode.Create))
					{
						(buffer.BaseStream as MemoryStream).WriteTo(fs);
					}
				}

				if(false)using (var buffer = DecryptCacheSegment(s, Blam.CacheSectionType.Debug, ch.StringIdsBufferOffset, ch.StringIdsBufferSize))
				{
					if (false) using (var o = new StreamWriter(kTestResultsPath + "mainmenu_string_ids.txt"))
					{
						for (int x = 0; x < ch.StringIdsCount; x++)
							o.WriteLine("{0}\t{1}", x.ToString("X5"), buffer.ReadCString());
					}
				}
			}
		}
	};
}