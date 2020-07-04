/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using SND = BlamLib.Blam.Halo1.Tags.sound_group;

namespace BlamLib.Blam.Halo2.Tags
{
	#region sound
	#region sound_playback_parameters_struct
	partial class sound_playback_parameters_struct
	{
		public void Convert(SND h1)
		{
			this.MinDist.Value = h1.Distance.Lower; this.MaxDist.Value = h1.Distance.Upper;
			this.SkipFraction.Value = h1.SkipFraction.Value;
			////this.RandomPitchBounds.Lower = (short)h1.RandomPitchBounds.Lower;
			////this.RandomPitchBounds.Upper = (short)h1.RandomPitchBounds.Upper;
			this.InnerConeAngle.Value = h1.InnerConeAngle.Value;
			this.OuterConeAngle.Value = h1.OuterConeAngle.Value;
			this.OuterConeGain.Value = h1.OuterConeGain.Value;
			this.MaxBendPerSec.Value = h1.MaxBendPerSecond.Value;
		}
	};
	#endregion

	#region sound_scale_modifiers_struct
	partial class sound_scale_modifiers_struct
	{
		public void Convert(SND h1)
		{
			this.GainModifier.Lower = h1.GainModifier1.Value;
			this.GainModifier.Upper = h1.GainModifier2.Value;
			//this.PitchModifier.Lower = (short)h1.PitchModifier1.Value;
			//this.PitchModifier.Upper = (short)h1.PitchModifier2.Value;
			this.SkipFractionModifier.Lower = h1.SkipFractionModifier1.Value;
			this.SkipFractionModifier.Upper = h1.SkipFractionModifier2.Value;
		}
	};
	#endregion


	#region sound_permutation_chunk_block
	partial class sound_permutation_chunk_block
	{
		public ResourcePtr GetOffset() { return FileOffset.Value; }

		internal byte[] Data;
		internal override bool Reconstruct(Blam.CacheFile c)
		{
			var rsrc_cache = Program.Halo2.FromLocation(c as Halo2.CacheFile, GetOffset());

			// the shared cache isn't loaded, break
			if (rsrc_cache == null) return false;

			rsrc_cache.InputStream.Seek(GetOffset().Offset);
			Data = rsrc_cache.InputStream.ReadBytes(GetSize());

			return true;
		}
	};
	#endregion

	#region sound_permutations_block
	partial class sound_permutations_block
	{
		public void Convert(int index, SND.sound_pitch_range_block.sound_permutations_block h1)
		{
			this.Name.ResetFromString(h1.Name);
			this.SkipFraction.Value = h1.SkipFraction.Value;
			this.Gain.Value = h1.Gain.Value;
			this.SamplesSize.Value = h1.Samples.Value.Length;
			this.RawInfo.Value = index;
		}

		internal void Reconstruct(sound_cache_file_gestalt_group gestalt,
			sound_gestalt_permutations_block gestalt_perm)
		{
			int index = gestalt_perm.Name.Value;
			if (index != -1)
				this.Name.Handle = gestalt.ImportNames[index].ImportName.Handle;

			// TODO: do the tools actually "encode" these values or just cast them?
			this.SkipFraction.Value = (float)gestalt_perm.EncodedSkipFraction.Value;
			this.Gain.Value = (float)gestalt_perm.EncodedGain.Value;

			this.SamplesSize = gestalt_perm.SampleSize.Value;

			//RawInfo = PermutationInfoIndex?
			// TODO: do the tools multiply this by a constant (ie, 30) or leave it the same?
			this.LanguageNeutralMilliseconds.Value = gestalt_perm.LanguageNeutralTime.Value;

			index = gestalt_perm.FirstChunk.Value;
			if (index != -1)
			{
				int count = gestalt_perm.ChunkCount.Value;

				for (int x = 0; x < count; x++)
					this.Chunks.AddElement(gestalt.Chunks[index+x].Clone(this.Chunks)
						as sound_permutation_chunk_block);
			}
		}
	};
	#endregion

	#region sound_pitch_range_block
	partial class sound_pitch_range_block
	{
		public void Convert(SND.sound_pitch_range_block h1)
		{
			this.Name.ResetFromString(
				(h1.Name == "default" ? "|default|" : h1.Name));
			//this.NaturalPitch.Value = (short)h1.NaturalPitch.Value;
			//this.BendBounds.Lower = (short)h1.BendBounds.Lower;
			//this.BendBounds.Upper = (short)h1.BendBounds.Upper;

			sound_permutations_block perm;
			int count = h1.Permutations.Count;
			for (int x = 0; x < count; x++)
			{
				this.Permutations.Add(out perm);
				perm.Convert(x, h1.Permutations[x]);
			}
		}

		void Reconstruct(sound_gestalt_pitch_range_parameters_block data)
		{
			NaturalPitch.Value = data.NaturalPitch.Value;
			BendBounds.Lower = data.BendBounds.Lower;
			BendBounds.Upper = data.BendBounds.Upper;
			// TODO: do we need to do more than just copy this?
			PitchBounds.Lower = data.MaxGainPitchBounds.Lower;
			PitchBounds.Upper = data.MaxGainPitchBounds.Upper;
		}
		internal void Reconstruct(sound_cache_file_gestalt_group gestalt,
			sound_gestalt_pitch_ranges_block gestalt_pr)
		{
			int index = gestalt_pr.Name.Value;
			if (index != -1)
				this.Name.Handle = gestalt.ImportNames[index].ImportName.Handle;

			index = gestalt_pr.Parameters.Value;
			if (index != -1)
				Reconstruct(gestalt.PitchRangeParameters[index]);

			//? = EncodedPermutationData
			//? = FirstRuntimePermutationFlagIndex

			index = gestalt_pr.FirstPermutation.Value;
			if (index != -1)
			{
				int count = gestalt_pr.PermutationCount.Value;
				for(int x = 0; x < count; x++)
				{
					sound_permutations_block perm;
					this.Permutations.Add(out perm);

					perm.Reconstruct(gestalt, gestalt.Permutations[index+x]);
				}
			}
		}
	};
	#endregion


	#region sound_permutation_raw_info_block
	partial class sound_permutation_raw_info_block
	{
		public void Convert(SND.sound_pitch_range_block.sound_permutations_block h1)
		{
			Debug.Assert.If(h1.Compression.Value != 3, "Can't convert ogg sounds! '{0}'", h1.Name.Value);

			h1.Samples.CopyTo(this.Samples);
			h1.Mouth.CopyTo(this.MouthData);
			this.Compression.Value = h1.Compression.Value;
		}
	};
	#endregion

	#region sound_definition_language_permutation_info_block
	partial class sound_definition_language_permutation_info_block
	{
		public void Convert(SND.sound_pitch_range_block.sound_permutations_block perm)
		{
			sound_permutation_raw_info_block raw;
			RawInfoBlock.Add(out raw);
			raw.Convert(perm);
		}
	};
	#endregion

	#region sound_encoded_dialogue_section_block
	partial class sound_encoded_dialogue_section_block
	{
		internal bool Reconstruct(geometry_block_info_struct gbi)
		{
			int index = 0;
			byte[][] data = gbi.GeometryBlock;

			if (data == null) return false;

			foreach (geometry_block_resource_block gb in gbi.Resources)
			{
				using (IO.EndianReader er = new BlamLib.IO.EndianReader(data[index]))
				{
					switch (gb.Type.Value)
					{
						#region TagBlock
						case (int)geometry_block_resource_type.TagBlock:
							int count = gb.GetCount();
							switch (gb.PrimaryLocater.Value)
							{
								case OffsetSoundDialogueInfo:
									SoundDialogueInfo.Resize(count);
									SoundDialogueInfo.Read(er);
									break;
							}
							break;
						#endregion
						#region TagData
						case (int)geometry_block_resource_type.TagData:
							if (gb.PrimaryLocater.Value == OffsetEncodedData)
								EncodedData.Reset(er.ReadBytes(gb.Size));
							break;
						#endregion
					}
				}

				index++;
			}

			return true;
		}
	};
	#endregion

	#region sound_extra_info_block
	partial class sound_extra_info_block
	{
		public void Convert(SND h1)
		{
			if (h1.PitchRanges.Count < 1) return;

			sound_definition_language_permutation_info_block perm;
			foreach (SND.sound_pitch_range_block.sound_permutations_block pr in h1.PitchRanges[0].Permutations)
			{
				LanguagePermutationInfo.Add(out perm);
				perm.Convert(pr);
			}
		}
	};
	#endregion

	#region sound
	partial class sound_group
	{
		#region Convert
		void ConvertClass()
		{
			// currently nothing is known to need to be tweaked
		}

		void ConvertPermutations(SND h1)
		{
			sound_pitch_range_block pitchrange;
			foreach(SND.sound_pitch_range_block pr in h1.PitchRanges)
			{
				PitchRanges.Add(out pitchrange);
				pitchrange.Convert(pr);
			}

			sound_extra_info_block extra;
			ExtraInfo.Add(out extra);
			extra.Convert(h1);
		}

		public void Convert(SND h1)
		{
			Debug.Assert.If(h1.Compression.Value != 3, "Can't convert ogg sounds!");

			this.Flags.Value = h1.Flags.Value;
			this.Class.Value = h1.Class.Value; ConvertClass();
			this.SampleRate.Value = h1.SampleRate.Value;
			this.ImportType.Value = 2; // single-layer
			this.Playback.Value.Convert(h1);
			this.Scale.Value.Convert(h1);
			this.Encoding.Value = h1.Encoding.Value;
			this.Compression.Value = h1.Compression.Value;
			ConvertPermutations(h1);
		}
		#endregion

		void Reconstruct(sound_cache_file_gestalt_group gestalt, cache_file_sound_group snd)
		{
			int index;

			this.Flags.Value = snd.Flags.Value;
			this.Class.Value = snd.SoundClass.Value;
			this.SampleRate.Value = snd.SampleRate.Value;
			this.Encoding.Value = snd.Encoding.Value;
			this.Compression.Value = snd.Compression.Value;

			index = snd.PlaybackIndex.Value;
			if(index != -1)
				this.Playback.Value = gestalt.Playbacks[index].Playback.Value.Clone(this) 
					as sound_playback_parameters_struct;

			index = snd.ScaleIndex.Value;
			if (index != -1)
				this.Scale.Value = gestalt.Scales[index].Scale.Value.Clone(this) 
					as sound_scale_modifiers_struct;

			index = snd.PromotionIndex.Value;
			if (index != -1)
				this.Promotion.Value = gestalt.Promotions[index].Promotion.Value.Clone(this)
					as sound_promotion_parameters_struct;

			index = snd.FirstPitchRangeIndex.Value;
			int count = snd.PitchRangeIndex.Value;
			if (index != -1)
			{
				this.PitchRanges.Resize(count);

				for (int x = 0; x < count; x++)
					this.PitchRanges[x].Reconstruct(gestalt,
						gestalt.PitchRanges[index + x]);
			}

			index = snd.CustomPlaybackIndex.Value;
			if (index != -1)
			{
				sound_platform_sound_playback_block param;
				this.PlatformParameters.Add(out param);

				param.PlaybackDefinition.Value = gestalt.CustomPlaybacks[index].PlaybackDefinition.Value.Clone(this)
					as simple_platform_sound_playback_struct;
			}

			index = snd.ExtraInfoIndex.Value;
			if (index != -1)
				ExtraInfo.AddElement(gestalt.ExtraInfos[index].Clone(this) 
					as sound_extra_info_block);

			//snd.MaximumPlayTime.Value?
		}

		public void FromCache(Halo2.CacheFile c, cache_file_sound_group snd)
		{
			var icti = c.TagIndexManager as InternalCacheTagIndex;
			var ugh_ = icti[icti.SoundGestaltTagIndex].TagDefinition as sound_cache_file_gestalt_group;

			Reconstruct(ugh_, snd);
		}
	};
	#endregion
	#endregion

	#region sound_gestalt
	#region sound_gestalt_extra_info_block
	partial class sound_gestalt_extra_info_block
	{
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			if (EncodedPermutationSection.Count != 1)
			{
				sound_encoded_dialogue_section_block section;
				EncodedPermutationSection.Add(out section);

				result = section.Reconstruct(GeometryBlockInfo.Value);
			}

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
	};
	#endregion
	#endregion
}