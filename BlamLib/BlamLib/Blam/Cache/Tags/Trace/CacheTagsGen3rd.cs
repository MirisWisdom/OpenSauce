/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.IO;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Cache.Tags
{
	#region render_geometry_api_resource_definition
	partial class render_geometry_api_resource_definition
	{
		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVerticesOld(StreamWriter s, Render.VertexBufferInterface.VertexBuffersGen3 gr, bool denormalize,
			Render.VertexBufferInterface.VertexBuffersGen3.Definition def,
			s_tag_d3d_vertex_buffer vb)
		{
			LowLevel.Math.real_quaternion quat;

			var elements = new Render.DeclarationTypes.IDeclType[def.Elements.Length];
			for (int x = 0; x < elements.Length; x++)
				elements[x] = def.Elements[x].DeclarationType.AllocateDeclType();

			using (var er = new IO.EndianReader(vb.VertexBuffer.Value, IO.EndianState.Big, this))
			{
				for (int x = 0; x < vb.VertexCount.Value; x++) // foreach vertex...
				{
					s.WriteLine("\tVertex\t{0}", x.ToString("X8"));
					foreach (Render.DeclarationTypes.IDeclType dt in elements) // and foreach type in the vertex...
					{
						dt.Read(er); // load the type data
						if (denormalize)
						{
							dt.Denormalize(out quat);
							s.WriteLine("\t\t{0}", dt.ToString(quat));
						}
						else s.WriteLine("\t\t{0}", dt.ToString());
						s.Flush();
					}
					s.WriteLine();
				}
				s.WriteLine(); s.WriteLine();
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVertices(StreamWriter s, Render.VertexBufferInterface.VertexBuffersGen3 gr, bool denormalize,
			Render.VertexBufferInterface.VertexBuffersGen3.Definition def,
			s_tag_d3d_vertex_buffer vb)
		{
			var stream_r = new Render.VertexBufferInterface.StreamReader(def);

			using (var er = new IO.EndianReader(vb.VertexBuffer.Value, IO.EndianState.Big, this))
			{
				for (int x = 0; x < vb.VertexCount.Value; x++) // foreach vertex...
				{
					s.WriteLine("\tVertex\t{0}", x.ToString("X8"));
					{
						stream_r.Read(er);
						if (denormalize)
						{
							foreach (string str in stream_r.GetDenormalizedStrings())
								s.WriteLine("\t\t{0}", str);
						}
						else
							foreach (string str in stream_r.GetNormalizedStrings())
								s.WriteLine("\t\t{0}", str);
					}
					s.WriteLine();
				}
				s.WriteLine(); s.WriteLine();
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		void DebugVertices(StreamWriter s, Render.VertexBufferInterface.VertexBuffersGen3 gr, bool denormalize)
		{
			int index = 0;
			foreach (s_tag_d3d_vertex_buffer_reference vbr in VertexBuffers)
			{
				var vb = vbr.Reference.Value;
				if (vb == null)
				{
					s.WriteLine("VertexBuffer\t{0}\tNull", index.ToString("X8"));
					s.WriteLine(); s.WriteLine();
					index++;
					continue; // wtf??
				}
				else s.WriteLine("VertexBuffer\t{0}", index.ToString("X8"));

				var def = gr.DefinitionFind((short)vb.VertxType.Value) as Render.VertexBufferInterface.VertexBuffersGen3.Definition;
				s.WriteLine("\tSize\t{0}\tCount\t{1}\tType\t{2}",
					vb.VertexSize.Value.ToString("X4"), vb.VertexCount.Value.ToString("X8"), vb.VertxType.Value.ToString("X2"));
				s.WriteLine("\tDefinition Size\t{0}", def.Size.ToString("X4"));
				s.WriteLine();

				//DebugVerticesOld(s, gr, denormalize, def, vb); // from before StreamReader was written!
				DebugVertices(s, gr, denormalize, def, vb);

				index++;
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public void Debug(Managers.TagManager owner_tag, string file, bool denormalize)
		{
			using(var s = new StreamWriter(file))
			{
				s.WriteLine("{0} {1}", owner_tag.GroupTag.NameToRightPaddedString(), owner_tag.Name);
				s.WriteLine();

				Program.Halo3.Manager.VertexBufferCacheOpen(BlamVersion.Halo3_Xbox);
				var gr = Program.Halo3.Manager[BlamVersion.Halo3_Xbox].
					GetResource<Render.VertexBufferInterface.VertexBuffersGen3>(Managers.BlamDefinition.ResourceVertexBuffers);

				// vertex
				DebugVertices(s, gr, denormalize);

				#region index
				int index = 0;
				foreach (s_tag_d3d_index_buffer_reference ibr in IndexBuffers)
				{
					var ib = ibr.Reference.Value;
					if (ib == null)
					{
						s.WriteLine("IndexBuffer\t{0}\tNull", index.ToString("X8"));
						s.WriteLine(); s.WriteLine();
						index++;
						continue; // wtf??
					}
					else s.WriteLine("IndexBuffer\t{0}", index.ToString("X8"));

					s.WriteLine("\tSize\t{0}\tUnknown\t{1}",
						ib.IndexBuffer.Value.Length.ToString("X8"), ib.Unknown.Value.ToString("X8"));
				}
				#endregion

				Program.Halo3.Manager.VertexBufferCacheClose(BlamVersion.Halo3_Xbox);
			}
		}
	};
	#endregion


	#region sound_cache_file_gestalt
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
	#endregion

	#region cache_file_resource_layout_table
	partial class cache_file_resource_layout_table
	{
		#region compression_codec_block
		partial class compression_codec_block
		{
			[System.Diagnostics.Conditional("DEBUG")]
			public static void Output(StreamWriter s, TI.Block<compression_codec_block> block)
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
			public static void Output(StreamWriter s, TI.Block<shared_cache_block> block)
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
			public static void Output(StreamWriter s, TI.Block<pages_block> block)
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
			public static void OutputResources(string dir, CacheFileGen3 cf, TI.Block<pages_block> block)
			{
				uint offset;
				int size;
				uint base_offset = (uint)(cf.Header as CacheHeaderGen3).Interop[CacheSectionType.Resource].CacheOffset;
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

					using(var fs = new FileStream(string.Format("{0}\\{1:X8}.bin", path, x), System.IO.FileMode.Create, System.IO.FileAccess.Write))
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
				public static void Output(StreamWriter s, TI.Block<block_4> block)
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
			public static void Output(StreamWriter s, TI.Block<cache_file_resource_layout_table_24_block> block)
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
			public static void Output(StreamWriter s, TI.Block<page_segment_block> block)
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
		public static void Output(StreamWriter s, cache_file_resource_layout_table def)
		{
			compression_codec_block.Output(s, def.CompressionCodecs);
			shared_cache_block.Output(s, def.SharedCaches);
			pages_block.Output(s, def.Pages);
			cache_file_resource_layout_table_24_block.Output(s, def.Block24);
			page_segment_block.Output(s, def.PageSegments);
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public static void OutputResources(string dir, CacheFileGen3 cf, cache_file_resource_layout_table def)
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
			public static void Output(StreamWriter s, TI.Block<resource_type_block> block)
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
			public static void Output(StreamWriter s, TI.Block<resource_structure_type_block> block)
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
				public static void Output(StreamWriter s, TI.Block<resource_fixup_block> block)
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
				public static void Output(StreamWriter s, TI.Block<resource_definition_fixup_block> block)
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
			public static void Output(StreamWriter s, TI.Block<cache_file_resource_gestalt_tag_resource_block> block)
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
		public static void OutputData(FileStream fs, cache_file_resource_gestalt_group def)
		{
			fs.Write(def.ResourceDefinitionData.Value, 0, def.ResourceDefinitionData.Value.Length);
		}
	};
	#endregion
}