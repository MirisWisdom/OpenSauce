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
﻿using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		const string kSoundExtractionTagName = @"sound\levels\010_jungle\sound_scenery\insects_in_tree\insects_in_tree\loop";

		static byte[] GenerateSoundFile(byte[] buffer, byte[] footer)
		{
			byte[] sound_file;

			using (MemoryStream ms = new MemoryStream())
			using (IO.EndianWriter write = new IO.EndianWriter(ms))
			{
				write.Write("RIFF", "RIFF".Length);
				write.Write(uint.MinValue);
				write.Write("WAVEdata", "WAVEdata".Length);
				write.Write(uint.MinValue);

				write.Write(buffer);

				if(footer != null)
					write.Write(footer);

				int size = (int)ms.Length;

				// Write file size to RIFF header
				write.Seek(4);
				write.Write(size - 8);

				// Write data size to WAVE header
				write.Seek(16);
				write.Write(buffer.Length);

				sound_file = ms.ToArray();
			}

			return sound_file;
		}
		static byte[] GetXmaFooterFromCodecType(int codec_type)
		{
			switch (codec_type)
			{
				case 0: return LowLevel.Xma.Interface.kMonoFooter;

				default: return LowLevel.Xma.Interface.kStereoFooter;
			}
		}
		static byte[] ConsolidateBuffers(List<List<byte[]>> perm_buffers, int additional_space_size)
		{
			int total_size = additional_space_size;

			for (int x = 0; x < perm_buffers.Count; x++)
			{
				var chunks = perm_buffers[x];
				for (int y = 0; y < chunks.Count; y++)
					total_size += chunks[y].Length;
			}

			byte[] buffer = new byte[total_size];

			int index = 0;
			for (int x = 0; x < perm_buffers.Count; x++)
			{
				var chunks = perm_buffers[x];
				for (int y = 0; y < chunks.Count; index += chunks[y++].Length)
					Array.Copy(chunks[y], 0, buffer, index, chunks[y].Length);
			}

			return buffer;
		}
		static void ProcessXmaBuffers(int codec_type, List<List<byte[]>> perm_buffers)
		{
			byte[] footer = GetXmaFooterFromCodecType(codec_type);
			byte[] buffer = ConsolidateBuffers(perm_buffers, /*footer.Length*/0);
			//Array.Copy(footer, 0, buffer, buffer.Length-footer.Length, footer.Length);

			if (!(codec_type == 2 || codec_type == 3))
			{
				byte[] sound_file = GenerateSoundFile(buffer, footer);

				LowLevel.Xma.Interface.Decode("", "");
			}
			else // 4 or 6 channel sound
			{

			}
		}
		static void ExtractSoundResource(Blam.Halo3.Tags.sound_cache_file_gestalt_group ughdef, Blam.Halo3.Tags.cache_file_sound_group snddef, 
			Blam.Halo3.Tags.sound_resource_definition snd_rsrc)
		{
			int codec_type = ughdef.PlatformCodecs[snddef.CodecIndex.Value].Type;

			int first_perm_index, perm_count;
			{
				var pitch_range = ughdef.PitchRanges[snddef.FirstPitchRangeIndex.Value];
				first_perm_index = pitch_range.FirstPermutation.Value;
				perm_count = pitch_range.GetPermutationCount();
			}

			var perm_buffers = new List<List<byte[]>>(perm_count);
			using (System.IO.MemoryStream read = new System.IO.MemoryStream(snd_rsrc.Data.Value, false))
			{
				for (int x = 0; x < perm_count; x++)
				{
					var perm = ughdef.Permutations[first_perm_index + x];

					var chunk_buffers = new List<byte[]>(perm.ChunkCount.Value);
					perm_buffers.Add(chunk_buffers);
					for (int y = 0; y < perm.ChunkCount.Value; y++)
					{
						var chunk = ughdef.Chunks[perm.FirstChunk.Value + y];

						int offset = chunk.FileOffset.Value;
						if (offset < 0) continue;

						int size = chunk.GetSize();
						byte[] buffer = new byte[size];
						int result = read.Read(buffer, offset, size);
						Assert.IsTrue(result == size);

						chunk_buffers.Add(buffer);
					}
				}
			}
			//ProcessXmaBuffers(codec_type, perm_buffers);
			perm_buffers = null;
		}
		internal static void CacheExtractSoundMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Halo3.CacheFileBase>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				var play_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.play);
				var playman = cache.TagIndexManager[play_index];
				var playdef = playman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_layout_table_group;

				var zone_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.zone);
				var zoneman = cache.TagIndexManager[zone_index];
				var zonedef = zoneman.TagDefinition as Blam.Halo3.Tags.cache_file_resource_gestalt_group;

				var ugh_index = cache.TagIndexManager.OpenFirstInstance(Blam.Halo3.TagGroups.ugh_);
				var ughman = cache.TagIndexManager[ugh_index];
				var ughdef = ughman.TagDefinition as Blam.Halo3.Tags.sound_cache_file_gestalt_group;

				var snd_index = cache.TagIndexManager.Open(kSoundExtractionTagName, Blam.Halo3.TagGroups.snd_);
				var sndman = cache.TagIndexManager[snd_index];
				var snddef = sndman.TagDefinition as Blam.Halo3.Tags.cache_file_sound_group;

				Blam.DatumIndex rsrc_index = snddef.ResourceDatumIndex;
				TagInterface.Definition rsrcdef = zonedef.LoadResources(rsrc_index, cache, playdef.ResourceLayoutTable);

				ExtractSoundResource(ughdef, snddef, rsrcdef as Blam.Halo3.Tags.sound_resource_definition);

				snddef = null;
				cache.TagIndexManager.Unload(snd_index);
				ughdef = null;
				cache.TagIndexManager.Unload(ugh_index);
				zonedef = null;
				cache.TagIndexManager.Unload(zone_index);
				playdef = null;
				cache.TagIndexManager.Unload(play_index);
			}
		}
		[TestMethod]
		public void Halo3TestCacheExtractSoundXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheExtractSoundMethod,
				BlamVersion.Halo3_Xbox, kDirectoryXbox, kMapNames_Retail[0]);
		}

		[TestMethod]
		public void TestXmaConversion()
		{
			const string k_folder = @"C:\Users\Sean\Downloads\sound\";
			const string k_temp_file = k_folder + "test_temp.bin";
			const string k_rebuild_file1 = k_folder + "test_stream1.xma";
			const string k_rebuild_file2 = k_folder + "test_stream2.xma";
			const int k_codec = 2;

			LowLevel.Xma.RebuildParameters parameters;
			parameters.BlockSize = 0x4000; parameters.Channels = 2; parameters.IgnorePacketSkip = false;
			parameters.Offset = 0; parameters.Strict = false; parameters.Version = 2;
			
			bool result = LowLevel.Xma.Interface.Rebuild(k_temp_file, null, k_rebuild_file1, parameters);
			Assert.IsTrue(result);

			parameters.Offset = 0x800;
			LowLevel.Xma.Interface.Rebuild(k_temp_file, null, k_rebuild_file2, parameters);
			Assert.IsTrue(result);
		}
	};
}