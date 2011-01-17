/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region cache_file_resource_layout_table
	partial class cache_file_resource_layout_table
	{
		#region compression_codec_block
		partial class compression_codec_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<compression_codec_block> block)
			{
				s.WriteLine("{0}\tcompression_codec_block", block.Count);
				int x = 0;
				foreach (compression_codec_block def in block)
				{
					s.WriteLine("\t{0}\t\t{1}", (x++).ToString(),
						Util.ByteArrayToString(def.Guid.Data));
				}
				s.WriteLine();
			}
		};
		#endregion

		#region shared_cache_block
		partial class shared_cache_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<shared_cache_block> block)
			{
				s.WriteLine("{0}\tshared_cache_block", block.Count);
				int x = 0;
				foreach (shared_cache_block def in block)
				{
					s.WriteLine("\t{0}\t\t{1}\t{2}\t{3}\t{4}", (x++).ToString(),
						def.Unknown100.Value.ToString("X4"), def.Flags.Value.ToString("X4"), def.Unknown104.Value.ToString("X8"), def.CachePath.Value);
				}
				s.WriteLine();
			}
		};
		#endregion

		#region pages_block
		partial class pages_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<pages_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}\t{3}\t{4}\t{5}" + Program.NewLine +
					"\t\t\t{6}\t{7}\t{8}" + Program.NewLine +
					"\t\t\t{9}" + Program.NewLine +
					"\t\t\t{10}" + Program.NewLine +
					"\t\t\t{11}" + Program.NewLine +
					"\t\t\t{12}" + Program.NewLine +
					"\t\t\t{13}\t{14}"
					;

				const string format_minmax =
					"\t\t\t{0:X4},{1:X4}\t{2},{3}" + Program.NewLine +		// Unknown006 short
					"\t\t\t{4:X4},{5:X4}\t{6},{7}" + Program.NewLine +		// Unknown054 short
					"\t\t\t{8:X4},{9:X4}\t{10},{11}"						// Unknown056 short
					;

				short Unknown006_Min = short.MaxValue, Unknown006_Max = 0; int Unknown006_MinIndex = -1, Unknown006_MaxIndex = -1;
				short Unknown054_Min = short.MaxValue, Unknown054_Max = 0; int Unknown054_MinIndex = -1, Unknown054_MaxIndex = -1;
				short Unknown056_Min = short.MaxValue, Unknown056_Max = 0; int Unknown056_MinIndex = -1, Unknown056_MaxIndex = -1;

				short s_val;

				s.WriteLine("{0}\tpages_block", block.Count);
				int x = 0;
				foreach (pages_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Header.Value.ToString("X4"), def.Flags.Value.ToString("X2"), def.CompressionCodec.Value.ToString(), def.SharedCache.Value.ToString("X4"), def.Unknown006.Value.ToString("X4"),
						def.BlockOffset.Value.ToString("X8"), def.BlockSizeCompressed.Value.ToString("X8"), def.BlockSizeUncompressed.Value.ToString("X8"),
						def.Crc.Value.ToString("X8"),
						Util.ByteArrayToString(def.EntireBufferHash.Data), Util.ByteArrayToString(def.FirstChunkHash.Data), Util.ByteArrayToString(def.LastChunkHash.Data),
						def.Unknown054.Value.ToString("X4"), def.Unknown056.Value.ToString("X4"));


					s_val = def.Unknown006.Value;
					if (s_val < Unknown006_Min) { Unknown006_Min = s_val; Unknown006_MinIndex = x; }
					if (s_val > Unknown006_Max) { Unknown006_Max = s_val; Unknown006_MaxIndex = x; }

					s_val = def.Unknown054.Value;
					if (s_val < Unknown054_Min) { Unknown054_Min = s_val; Unknown054_MinIndex = x; }
					if (s_val > Unknown054_Max) { Unknown054_Max = s_val; Unknown054_MaxIndex = x; }

					s_val = def.Unknown056.Value;
					if (s_val < Unknown056_Min) { Unknown056_Min = s_val; Unknown056_MinIndex = x; }
					if (s_val > Unknown056_Max) { Unknown056_Max = s_val; Unknown056_MaxIndex = x; }
				}
				s.WriteLine();

				s.WriteLine("Min\\Max");
				s.WriteLine(format_minmax,
					Unknown006_Min, Unknown006_Max, Unknown006_MinIndex, Unknown006_MaxIndex,
					Unknown054_Min, Unknown054_Max, Unknown054_MinIndex, Unknown054_MaxIndex,
					Unknown056_Min, Unknown056_Max, Unknown056_MinIndex, Unknown056_MaxIndex
					);

				s.WriteLine();
			}

			[System.Diagnostics.Conditional("DEBUG")]
			public static void OutputResources(string dir, CacheFileBase cf, TI.Block<pages_block> block)
			{
				uint offset;
				int size;
				uint base_offset = (uint)cf.HeaderHalo3.Interop[CacheSectionType.Resource].CacheOffset;
				string path = string.Format(@"{0}\\{1}_resources\\", dir, cf.Header.Name);
				if (!System.IO.Directory.Exists(path))
					System.IO.Directory.CreateDirectory(path);

				int x = -1;
				foreach (pages_block def in block)
				{
					x++;
					if (def.SharedCache.Value != -1) continue;
					size = def.BlockSizeUncompressed.Value;
					if (size <= 0) continue;

					offset = base_offset + (uint)def.BlockOffset.Value;
					cf.InputStream.Seek(offset);
					byte[] data;
					if (def.CompressionCodec.Value != -1)
					{
						using (var deflate = new System.IO.Compression.DeflateStream(cf.InputStream.BaseStream, System.IO.Compression.CompressionMode.Decompress, true))
						{
							data = new byte[size];
							deflate.Read(data, 0, data.Length);
						}
					}
					else
						data = cf.InputStream.ReadBytes(size);

					using(var fs = new System.IO.FileStream(string.Format("{0}\\{1:X8}.bin", path, x), System.IO.FileMode.Create, System.IO.FileAccess.Write))
					{
						fs.Write(data, 0, data.Length);
					}
				}
			}
		};
		#endregion

		#region cache_file_resource_layout_table_24_block
		partial class cache_file_resource_layout_table_24_block
		{
			#region block_4
			partial class block_4
			{
				[System.Diagnostics.Conditional("DEBUG")]
				public static void Output(System.IO.StreamWriter s, TI.Block<block_4> block)
				{
					const string format = "\t\t\t{0}\t\t{1}\t{2}\t{3}\t{4}";
					s.WriteLine("\t\t{0}\tblock_4", block.Count);
					int x = 0;
					foreach (block_4 def in block)
					{
						s.WriteLine(format, (x++).ToString(), def.Unknown00.Value.ToString("X8"), def.Unknown04.Value.ToString("X8"), def.Unknown08.Value.ToString("X8"), def.Unknown0C.Value.ToString("X8"));
					}
				}
			};
			#endregion

			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_layout_table_24_block> block)
			{
				s.WriteLine("{0}\tcache_file_resource_layout_table_24_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_layout_table_24_block def in block)
				{
					s.WriteLine("\t{0}\t\t{1}", (x++).ToString(),
						def.Size.Value.ToString("X8"));
					block_4.Output(s, def.Unknown04);
					s.WriteLine();
				}
			}
		};
		#endregion

		#region page_segment_block
		partial class page_segment_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<page_segment_block> block)
			{
				s.WriteLine("{0}\tpage_segment_block", block.Count);
				int x = 0;
				foreach (page_segment_block def in block)
				{
					s.WriteLine("\t{0}\t\t{1}\t{2}\t{3}\t{4}\t{5}", (x++).ToString(),
						(def.RequiredPageIndex.Value & 0xFFFF).ToString("X4"),
						(def.OptionalPageIndex.Value & 0xFFFF).ToString("X4"),
						def.RequiredSegmentOffset.Value.ToString("X8"),
						def.OptionalSegmentOffset.Value.ToString("X8"),
						DatumIndex.ToIndex(def.DatumIndexBlock24.Value).ToString("X4"));
				}
				s.WriteLine();
			}
		};
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.IO.StreamWriter s, cache_file_resource_layout_table def)
		{
			compression_codec_block.Output(s, def.CompressionCodecs);
			shared_cache_block.Output(s, def.SharedCaches);
			pages_block.Output(s, def.Pages);
			cache_file_resource_layout_table_24_block.Output(s, def.Block24);
			page_segment_block.Output(s, def.PageSegments);
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public static void OutputResources(string dir, CacheFileBase cf, cache_file_resource_layout_table def)
		{
			pages_block.OutputResources(dir, cf, def.Pages);
		}
	};
	#endregion

	#region cache_file_resource_gestalt
	partial class cache_file_resource_gestalt_group
	{
		#region resource_type_block
		partial class resource_type_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<resource_type_block> block)
			{
				const string format =
					"\t{0}\t\t{1}" + Program.NewLine + // Name
					"\t\t\t{2}\t{3}\t{4}\t{5}" + Program.NewLine + // 010,012,014,016
					"\t\t\t{6}" // Guid
					;

				s.WriteLine("{0}\tresource_type_block", block.Count);
				int x = 0;
				foreach (resource_type_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Name.ToString(),
						def.Unknown010.Value.ToString("X4"), def.Unknown012.Value.ToString("X4"), def.Unknown014.Value.ToString("X4"), def.Unknown016.Value.ToString("X4"),
						Util.ByteArrayToString(def.Guid.Data));
				}
			}
		};
		#endregion

		#region resource_structure_type_block
		partial class resource_structure_type_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<resource_structure_type_block> block)
			{
				const string format =
					"\t{0}\t\t{1}" + Program.NewLine + // Name
					"\t\t\t{2}" // Guid
					;

				s.WriteLine("{0}\tresource_structure_type_block", block.Count);
				int x = 0;
				foreach (resource_structure_type_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Name.ToString(),
						Util.ByteArrayToString(def.Guid.Data));
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_tag_resource_block
		partial class cache_file_resource_gestalt_tag_resource_block
		{
			#region resource_fixup_block
			partial class resource_fixup_block
			{
				[System.Diagnostics.Conditional("DEBUG")]
				public static void Output(System.IO.StreamWriter s, TI.Block<resource_fixup_block> block)
				{
					const string format = "\t\t\t{0}\t\t{1}\t{2}";
					s.WriteLine("\t\t{0}\tresource_fixup_block", block.Count);
					int x = 0;
					foreach (resource_fixup_block def in block)
					{
						s.WriteLine(format, (x++).ToString(), def.BlockOffset.Value.ToString("X8"), def.Address.Value.ToString("X8"));
					}
				}
			};
			#endregion

			#region resource_definition_fixup_block
			partial class resource_definition_fixup_block
			{
				[System.Diagnostics.Conditional("DEBUG")]
				public static void Output(System.IO.StreamWriter s, TI.Block<resource_definition_fixup_block> block)
				{
					const string format = "\t\t\t{0}\t\t{1}\t{2}";
					s.WriteLine("\t\t{0}\tresource_definition_fixup_block", block.Count);
					int x = 0;
					foreach (resource_definition_fixup_block def in block)
					{
						s.WriteLine(format, (x++).ToString(), def.Offset.Value.ToString("X8"), def.StructureTypeIndex.Value.ToString());
					}
				}
			};
			#endregion

			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_tag_resource_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}\t{3}\t{4}.{5}" + Program.NewLine + // Header,Type,Flags,Reference,ReferenceTagGroup
					"\t\t\t{6:X8}\t{7:X8}\t{8:X8}" + Program.NewLine + // Offset, Size, ?Offset
					"\t\t\t{9:X4}\t{10:X4}\t{11:X8}" // 020, SegmentIndex, DefinitionOffset
					;

				s.WriteLine("{0}\ttag_resource_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_tag_resource_block def in block)
				{
					if (def.Reference.Datum != DatumIndex.Null)
					{
						s.WriteLine(format, x.ToString(),
							def.Header.Value.ToString("X4"), def.ResourceType.Value.ToString("X2"), def.Flags.Value.ToString("X2"), def.Reference.ToString(), def.Reference.GroupTag.Name,
							def.BlockOffset.Value.ToString("X8"), def.BlockSize.Value.ToString("X8"), def.UnknownOffset.Value.ToString("X8"),
							def.Unknown020.Value.ToString("X4"), def.SegmentIndex.Value.ToString("X4"), def.DefinitionOffset.Value.ToString("X8")
						);
						s.WriteLine();
						resource_fixup_block.Output(s, def.ResourceFixups);
						s.WriteLine();
						resource_definition_fixup_block.Output(s, def.ResourceDefinitionFixups);
					}
					else
						s.WriteLine("\t{0}\t\tNULL", x.ToString());
					x++;

					s.WriteLine();
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_64_block
		partial class cache_file_resource_gestalt_64_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_64_block> block, string name)
			{
				if (block.Count == 0) return;

				const string format =
					"\t{0}\t\t{1}" + Program.NewLine + // Unknown044
					"\t\t\t{2}\t{3}\t{4}\t{5}\t{6}" // Unknown030-Unknown040
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_64_block\t({1})", block.Count, name);
				int x = 0;
				foreach (cache_file_resource_gestalt_64_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Unknown044.ToString(),
						def.Unknown030.Value.ToString("X8"), def.Unknown034.Value.ToString("X8"),
						def.Unknown038.Value.ToString("X8"), def.Unknown03C.Value.ToString("X8"),
						def.Unknown040.Value.ToString("X8"));
					s.WriteLine();
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_100_block
		public partial class cache_file_resource_gestalt_100_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_100_block> block)
			{
				const string format =
					"\t{0}\t\t{1}" + Program.NewLine + // Name
					"\t\t\t{2}\t{3}" + Program.NewLine +
					"\t\t\t{4}\t{5}" + Program.NewLine +
					"\t\t\t{6}\t{7}" + Program.NewLine +
					"\t\t\t{8}\t\t{9}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_100_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_100_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Name.ToString(),
						def.Unknown04.Value.ToString("X8"), def.Unknown08.Value.ToString("X8"),
						def.Unknown0C.Value.ToString("X8"), def.Unknown10.Value.ToString("X8"),
						def.Unknown14.Value.ToString("X8"), def.Unknown18.Value.ToString("X8"),
						def.Unknown1C.Value.ToString("X8"), def.PrevZoneSet.Value.ToString());
					s.WriteLine();
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_164_block
		public partial class cache_file_resource_gestalt_164_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_164_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}" + Program.NewLine +
					"\t\t\t{3}\t{4}" + Program.NewLine +
					"\t\t\t{5}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_164_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_164_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Unknown00.Value.ToString("X8"), def.Unknown04.Value.ToString("X8"),
						def.Unknown08.Value.ToString("X8"), def.Unknown0C.Value.ToString("X8"),
						def.Unknown10.Value.ToString("X8"));
					s.WriteLine();
				}
			}
		};
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		static void Output(System.IO.StreamWriter s, TI.Block<field_block<TI.LongInteger>> block)
		{
			const string format = "\t{0}\t\t{1}";

			s.WriteLine("{0}\tcache_file_resource_gestalt_1D0_block", block.Count);
			int x = 0;
			foreach (field_block<TI.LongInteger> def in block)
			{
				s.WriteLine(format, (x++).ToString(), def.Value.Value.ToString("X8"));
			}
		}

		#region cache_file_resource_gestalt_1DC_block
		partial class cache_file_resource_gestalt_1DC_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_1DC_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}\t{3}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_1DC_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_1DC_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.ThisIndex.Value.ToString("X4"), def.ElementCount.Value.ToString("X4"), def.BlockIndex.Value.ToString("X8")
						);
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_1E8_block
		partial class cache_file_resource_gestalt_1E8_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_1E8_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_1E8_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_1E8_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Unknown00.Value.ToString("X4"), def.Unknown02.Value.ToString("X4")
						);
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_1F4_block
		partial class cache_file_resource_gestalt_1F4_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_1F4_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}\t{3}\t{4}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_1F4_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_1F4_block def in block)
				{
					s.WriteLine(format, (x++).ToString(),
						def.Unknown00.Value.ToString("X4"), def.Unknown02.Value.ToString("X4"), def.Unknown04.Value.ToString("X4"), def.Unknown06.Value.ToString("X4")
						);
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_200_block
		partial class cache_file_resource_gestalt_200_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(Blam.Halo3.CacheFileBase c, System.IO.StreamWriter s, TI.Block<cache_file_resource_gestalt_200_block> block)
			{
				const string format =
					"\t{0}\t\t{1}\t{2}\t{3}"
					;

				s.WriteLine("{0}\tcache_file_resource_gestalt_200_block", block.Count);
				int x = 0;
				foreach (cache_file_resource_gestalt_200_block def in block)
				{
					if (def.TagIndex.Value != -1)
						s.WriteLine(format, x.ToString(),
							def.Unknown08.Value.ToString("X8"), def.Unknown0C.Value.ToString("X8"), 
							c.GetTagIndexName((DatumIndex)def.TagIndex.Value, true)
							);
					else
						s.WriteLine("NULL");
					x++;
				}
			}
		};
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(Blam.Halo3.CacheFileBase c, System.IO.StreamWriter s, cache_file_resource_gestalt_group def)
		{
			resource_type_block.Output(s, def.ResourceTypes);
			s.WriteLine();
			resource_structure_type_block.Output(s, def.ResourceStructureTypes);
			s.WriteLine();
			cache_file_resource_gestalt_tag_resource_block.Output(s, def.TagResources);
			s.WriteLine();
			cache_file_resource_gestalt_64_block.Output(s, def.Block64, "64-general");
			cache_file_resource_gestalt_64_block.Output(s, def.Block70, "70-global");
			cache_file_resource_gestalt_64_block.Output(s, def.Block7C, "7C-attached?");
			cache_file_resource_gestalt_64_block.Output(s, def.Block88, "88-unattached");
			cache_file_resource_gestalt_64_block.Output(s, def.Block94, "94-dvd_forbidden");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockA0, "A0-dvd_always_streaming");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockAC, "AC-bsp zones-1");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockB8, "B8-bsp zones-2");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockC4, "C4-bsp zones-3");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockD0, "D0-?");
			cache_file_resource_gestalt_64_block.Output(s, def.BlockDC, "DC-zone sets");

			cache_file_resource_gestalt_100_block.Output(s, def.Block100);

			cache_file_resource_gestalt_164_block.Output(s, def.Block164);
			Output(s, def.Block1D0);
			s.WriteLine();
			cache_file_resource_gestalt_1DC_block.Output(s, def.Block1DC);
			s.WriteLine();
			cache_file_resource_gestalt_1E8_block.Output(s, def.Block1E8);
			s.WriteLine();
			cache_file_resource_gestalt_1F4_block.Output(s, def.Block1F4);
			s.WriteLine();
			cache_file_resource_gestalt_200_block.Output(c, s, def.Block200);
			s.WriteLine();
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public static void OutputData(System.IO.FileStream fs, cache_file_resource_gestalt_group def)
		{
			fs.Write(def.ResourceDefinitionData.Value, 0, def.ResourceDefinitionData.Value.Length);
		}
	};
	#endregion


	#region cache_file_sound
	partial class cache_file_sound_group
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.IO.StreamWriter s, cache_file_sound_group def)
		{
			s.WriteLine(
				"\t\t{0}\t{1}\t{2}" + Program.NewLine + // Flags,SoundClass,SampleRate
				"\t\t{3}\t{4}\t{5}" + Program.NewLine + // Encoding,CodecIndex,PlaybackIndex
				"\t\t{6}\t{7}" + Program.NewLine + // 08,0A
				"\t\t{8}\t{9}\t{10}" + Program.NewLine + // FirstPitchRangeIndex,PitchRangeIndex,ScaleIndex
				"\t\t{11}\t{12}\t{13}" + Program.NewLine + // PromotionIndex,CustomPlaybackIndex,ExtraInfoIndex
				"\t\t{14}\t{15}\t{16}", // 14,ResourceIndex,MaximumPlayTime
				def.Flags.Value.ToString("X4"),def.SoundClass.Value.ToString("X2"), def.SampleRate.Value.ToString("X2"),
				def.Encoding.Value.ToString("X2"), def.CodecIndex.Value.ToString("X2"), def.PlaybackIndex.Value.ToString("X4"),
				def.Unknown08.Value.ToString("X4"), def.Unknown0A.Value.ToString("X4"),
				def.FirstPitchRangeIndex.Value.ToString("X4"), def.PitchRangeIndex.Value.ToString("X2"), def.ScaleIndex.Value.ToString("X2"),
				def.PromotionIndex.Value.ToString("X2"), def.CustomPlaybackIndex.Value.ToString("X2"), def.ExtraInfoIndex.Value.ToString("X4"),
				def.Unknown14.Value.ToString("X8"), def.ResourceIndex.Value.ToString("X8"), def.MaximumPlayTime.Value.ToString("X8")
				);
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.Collections.Generic.Dictionary<int, System.Collections.Generic.List<string>> dic, 
			cache_file_sound_group def,
			string header)
		{
			System.Text.StringBuilder sb = new System.Text.StringBuilder();
			sb.AppendFormat("\t\t{0}" + Program.NewLine, header);
			sb.AppendFormat(
				"\t\t\t{0}\t{1}\t{2}" + Program.NewLine + // Flags,SoundClass,SampleRate
				"\t\t\t{3}\t{4}\t{5}" + Program.NewLine + // Encoding,CodecIndex,PlaybackIndex
				"\t\t\t{6}\t{7}" + Program.NewLine + // 08,0A
				"\t\t\t{8}\t{9}\t{10}" + Program.NewLine + // FirstPitchRangeIndex,PitchRangeIndex,ScaleIndex
				"\t\t\t{11}\t{12}\t{13}" + Program.NewLine + // PromotionIndex,CustomPlaybackIndex,ExtraInfoIndex
				"\t\t\t{14}\t{15}\t{16}", // 14,ResourceIndex,MaximumPlayTime
				def.Flags.Value.ToString("X4"),def.SoundClass.Value.ToString("X2"), def.SampleRate.Value.ToString("X2"),
				def.Encoding.Value.ToString("X2"), def.CodecIndex.Value.ToString("X2"), def.PlaybackIndex.Value.ToString("X4"),
				def.Unknown08.Value.ToString("X4"), def.Unknown0A.Value.ToString("X4"),
				def.FirstPitchRangeIndex.Value.ToString("X4"), def.PitchRangeIndex.Value.ToString("X2"), def.ScaleIndex.Value.ToString("X2"),
				def.PromotionIndex.Value.ToString("X2"), def.CustomPlaybackIndex.Value.ToString("X2"), def.ExtraInfoIndex.Value.ToString("X4"),
				def.Unknown14.Value.ToString("X8"), def.ResourceIndex.Value.ToString("X8"), def.MaximumPlayTime.Value.ToString("X8")
				);
			dic[def.CodecIndex.Value].Add(sb.ToString());
			//dic[def.SampleRate.Value].Add(sb.ToString());
		}
	};
	#endregion

	#region sound_permutation_chunk_block
	partial class sound_permutation_chunk_block
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.IO.StreamWriter s, TI.Block<sound_permutation_chunk_block> block)
		{
			const string format = "\t{0}\t\t{1}\t{2}\t{3}\t{4}";

			s.WriteLine("{0}\tsound_permutation_chunk_block", block.Count);
			int x = 0;
			foreach (sound_permutation_chunk_block def in block)
			{
				s.WriteLine(format, (x++).ToString(),
					def.FileOffset.Value.ToString("X8"), def.SizeFlags.Value.ToString("X8"), 
					def.Unknown0C.Value.ToString("X4"), def.Unknown10.Value.ToString("X8"));
			}
		}
	};
	#endregion

	#region sound_gestalt
	#region sound_gestalt_platform_codec_block
	partial class sound_gestalt_platform_codec_block
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.IO.StreamWriter s, TI.Block<sound_gestalt_platform_codec_block> block)
		{
			const string format = "\t{0}\t\t{1}\t{2}\t{3}";

			s.WriteLine("{0}\tsound_gestalt_platform_codec_block", block.Count);
			int x = 0;
			foreach (sound_gestalt_platform_codec_block def in block)
			{
				s.WriteLine(format, (x++).ToString(), 
					def.Unknown00.Value.ToString(), def.Type.ToString(), def.Flags.ToString());
			}
		}
	};
	#endregion


	#region sound_gestalt_pitch_ranges_block
	partial class sound_gestalt_pitch_ranges_block
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(CacheFileBase c, System.IO.StreamWriter s, sound_cache_file_gestalt_group owner, TI.Block<sound_gestalt_pitch_ranges_block> block)
		{
			const string format = "\t{0}\t\t{1}\t{2}\t{3}\t{4}\t{5}\t{6}";

			s.WriteLine("{0}\tsound_gestalt_pitch_ranges_block", block.Count);
			int x = 0;
			foreach (sound_gestalt_pitch_ranges_block def in block)
			{
				int name_index = def.Name.Value;
				Blam.StringID name = name_index >= 0 ? owner.ImportNames[name_index].ImportName.Handle : Blam.StringID.Null;

				s.WriteLine(format, (x++).ToString(),
					def.Parameters.Value.ToString(), def.Unknown04.Value.ToString(),
					def.EncodedPermutationData.Value.ToString("X4"), def.FirstPermutation.Value.ToString(),
					def.GetPermutationCount().ToString(),
					c.StringIdResolve(name));
			}
		}
	};
	#endregion

	#region sound_gestalt_permutations_block
	partial class sound_gestalt_permutations_block
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(CacheFileBase c, System.IO.StreamWriter s, sound_cache_file_gestalt_group owner, TI.Block<sound_gestalt_permutations_block> block)
		{
			const string format = "\t{0}\t\t{1}\t{2}\t{3}\t{4}\t{5}";

			s.WriteLine("{0}\tsound_gestalt_permutations_block", block.Count);
			int x = 0;
			foreach (sound_gestalt_permutations_block def in block)
			{
				int name_index = def.Name.Value;
				Blam.StringID name = name_index >= 0 ? owner.ImportNames[name_index].ImportName.Handle : Blam.StringID.Null;

				s.WriteLine(format, (x++).ToString(),
					def.PermutationInfoIndex.ToString(), def.FirstChunk.Value.ToString(), def.ChunkCount.Value.ToString(), def.GetPermutationIndex().ToString(), 
					c.StringIdResolve(name));
			}
		}
	};
	#endregion

	#region sound_cache_file_gestalt
	partial class sound_cache_file_gestalt_group
	{
		#region sound_gestalt_60_block
		partial class sound_gestalt_60_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			static void Output(System.IO.StreamWriter s, TI.Block<field_block<TI.ShortInteger>> block)
			{
				const string format = "\t\t\t{0}\t\t{1}";

				s.WriteLine("\t\t{0}\tblock_4", block.Count);
				int x = 0;
				foreach (field_block<TI.ShortInteger> def in block)
				{
					s.WriteLine(format, (x++).ToString(), def.Value.Value.ToString("X4"));
				}
			}

			#region block_10
			partial class block_10
			{
				[System.Diagnostics.Conditional("DEBUG")]
				public static void Output(System.IO.StreamWriter s, TI.Block<block_10> block)
				{
					const string format = "\t\t\t{0}\t\t{1}\t{2}";

					s.WriteLine("\t\t{0}\tblock_10", block.Count);
					int x = 0;
					foreach (block_10 def in block)
					{
						s.WriteLine(format, (x++).ToString(), def.Start.Value.ToString("X4"), def.Length.Value.ToString("X4"));
					}
				}
			};
			#endregion

			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(System.IO.StreamWriter s, TI.Block<sound_gestalt_60_block> block)
			{
				const string format = "\t{0}\t\t{1}";

				s.WriteLine("{0}\tsound_gestalt_60_block", block.Count);
				int x = 0;
				foreach (sound_gestalt_60_block def in block)
				{
					s.WriteLine(format, (x++).ToString(), def.Unknown00.Value.ToString("X8"));

					Output(s, def.Unknown04);
					block_10.Output(s, def.Unknown10);
				}
			}
		};
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(CacheFileBase c, System.IO.StreamWriter s, sound_cache_file_gestalt_group def)
		{
			sound_gestalt_platform_codec_block.Output(s, def.PlatformCodecs);
			s.WriteLine("{0}\tsound_gestalt_playback_block", def.Playbacks.Count);
			s.WriteLine("{0}\tsound_gestalt_scale_block", def.Scales.Count);
			s.WriteLine("{0}\tsound_gestalt_import_names_block", def.ImportNames.Count);
			s.WriteLine("{0}\tsound_gestalt_pitch_range_parameters_block", def.PitchRangeParameters.Count);
			sound_gestalt_pitch_ranges_block.Output(c, s, def, def.PitchRanges);
			sound_gestalt_permutations_block.Output(c, s, def, def.Permutations);
			s.WriteLine("{0}\tsound_gestalt_custom_playback_block", def.CustomPlaybacks.Count);
			sound_gestalt_60_block.Output(s, def.Unknown60);
			s.WriteLine("{0}\tsound_gestalt_runtime_permutation_bit_vector_block", def.RuntimePermutationFlags.Count);
			sound_permutation_chunk_block.Output(s, def.Chunks);
			s.WriteLine("{0}\tsound_gestalt_promotions_block", def.Promotions.Count);
			s.WriteLine("{0}\tsound_gestalt_extra_info_block", def.ExtraInfos.Count);
		}
	}
	#endregion
	#endregion
}