/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using CT = BlamLib.Blam.Cache.Tags;

namespace BlamLib.Blam.Halo3.Tags
{
	#region cache_file_resource_gestalt
	partial class cache_file_resource_gestalt_group
	{
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
			const string format = "\t{0}\t\t{1}\t{2}" + "\t{3}\t{4}\t{5}";

			s.WriteLine("{0}\tsound_permutation_chunk_block", block.Count);
			int x = 0;
			foreach (sound_permutation_chunk_block def in block)
			{
				s.WriteLine(format, (x++).ToString(),
					def.FileOffset.Value.ToString("X8"), def.GetSize().ToString("X7"), 
					def.GetFlags().ToString("X2"), def.Unknown0C.Value.ToString("X4"), def.Unknown10.Value.ToString("X8"));
			}
		}
	};
	#endregion

	#region sound_gestalt
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
				Blam.StringId name = name_index >= 0 ? owner.ImportNames[name_index].ImportName.Handle : Blam.StringId.Null;

				s.WriteLine(format, (x++).ToString(),
					def.Parameters.Value.ToString(), def.Unknown04.Value.ToString(),
					def.EncodedPermutationData.Value.ToString("X4"), def.FirstPermutation.Value.ToString(),
					def.GetPermutationCount().ToString(),
					c.StringIds.GetStringIdValue(name));
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
				Blam.StringId name = name_index >= 0 ? owner.ImportNames[name_index].ImportName.Handle : Blam.StringId.Null;

				s.WriteLine(format, (x++).ToString(),
					def.PermutationInfoIndex.ToString(), def.FirstChunk.Value.ToString(), def.ChunkCount.Value.ToString(), def.GetPermutationIndex().ToString(), 
					c.StringIds.GetStringIdValue(name));
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
			CT.sound_gestalt_platform_codec_block.Output(s, def.PlatformCodecs);
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