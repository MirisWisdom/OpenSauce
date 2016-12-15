/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Cache.Tags
{
	#region sound
	#region sound_permutation_chunk_block
	public abstract class sound_permutation_chunk_block_gen2 : TI.Definition
	{
		public TI.LongInteger FileOffset;
		public TI.LongInteger SizeFlags;
		public TI.LongInteger RuntimeIndex;

		protected sound_permutation_chunk_block_gen2(int field_count) : base(field_count) { }

		public virtual int GetSize() { return SizeFlags.Value & 0x3FFFFFFF; }
		// BIT(0) - is cached (set at runtime)
		public virtual int GetFlags() { return SizeFlags.Value >> 30; }
	};
	#endregion


	#region sound_encoded_dialogue_section_block
	public abstract class sound_encoded_dialogue_section_block : TI.Definition
	{
		#region sound_permutation_dialogue_info_block
		[TI.Definition(1, 16)]
		public class sound_permutation_dialogue_info_block : TI.Definition
		{

			public sound_permutation_dialogue_info_block() : base(4)
			{
				Add(/*mouth data offset = */ new TI.LongInteger());
				Add(/*mouth data length = */ new TI.LongInteger());
				Add(/*lipsync data offset = */ new TI.LongInteger());
				Add(/*lipsync data length = */ new TI.LongInteger());
			}
		}
		#endregion

		public TI.Data EncodedData;
		public TI.Block<sound_permutation_dialogue_info_block> SoundDialogueInfo;

		protected sound_encoded_dialogue_section_block(int field_count) : base(field_count) { }
	};
	#endregion
	#endregion

	#region cache_file_sound
	public abstract class cache_file_sound_group_gen2 : TI.Definition
	{
		public TI.Flags Flags;
		public TI.Enum SoundClass;
		public TI.Enum SampleRate;
		public TI.Enum Encoding;

		public TI.ShortInteger PlaybackIndex;
		public TI.ShortInteger FirstPitchRangeIndex;
		public TI.ByteInteger PitchRangeIndex;
		public TI.ByteInteger ScaleIndex;
		public TI.ByteInteger PromotionIndex;
		public TI.ByteInteger CustomPlaybackIndex;
		public TI.ShortInteger ExtraInfoIndex;
		public TI.LongInteger MaximumPlayTime;

		protected cache_file_sound_group_gen2(int field_count) : base(field_count) { }

		public abstract int GetCodecValue();
	};
	#endregion

	#region sound_gestalt
	#region sound_gestalt_import_names_block
	[TI.Definition(1, 4)]
	public  class sound_gestalt_import_names_block : TI.Definition
	{
		public TI.StringId ImportName;

		public sound_gestalt_import_names_block() : base(1)
		{
			Add(ImportName = new TI.StringId());
		}
	}
	#endregion


	#region sound_gestalt_pitch_ranges_block
	public abstract class sound_gestalt_pitch_ranges_block : TI.Definition
	{
		public TI.BlockIndex Name;
		public TI.BlockIndex Parameters;
		public TI.ShortInteger EncodedPermutationData;
		public TI.ShortInteger FirstRuntimePermutationFlagIndex;
		public TI.BlockIndex FirstPermutation;

		protected sound_gestalt_pitch_ranges_block(int field_count) : base(field_count) { }

		public abstract int GetPermutationCount();
	};
	#endregion

	#region sound_gestalt_permutations_block
	public abstract class sound_gestalt_permutations_block : TI.Definition
	{
		public TI.BlockIndex Name;
		public TI.ShortInteger EncodedSkipFraction;
		public TI.ByteInteger EncodedGain;
		public TI.ByteInteger PermutationInfoIndex;
		public TI.ShortInteger LanguageNeutralTime;

		public TI.BlockIndex FirstChunk;
		public TI.ShortInteger ChunkCount;

		protected sound_gestalt_permutations_block(int field_count) : base(field_count) { }
	};
	#endregion


	#region sound_gestalt_runtime_permutation_bit_vector_block
	[TI.Definition(1, 1)]
	public class sound_gestalt_runtime_permutation_bit_vector_block : TI.Definition
	{
		public sound_gestalt_runtime_permutation_bit_vector_block() : base(1)
		{
			Add(/* = */ new TI.ByteInteger());
		}
	}
	#endregion


	#region sound_gestalt_extra_info_block
	public abstract class sound_gestalt_extra_info_block : TI.Definition
	{
		protected sound_gestalt_extra_info_block(int field_count) : base(field_count) { }

		public abstract sound_encoded_dialogue_section_block GetEncodedPermutation(int element);
	};
	#endregion

	#region sound_cache_file_gestalt
	public abstract class sound_cache_file_gestalt_group_gen2 : TI.Definition
	{
		public TI.Block<Blam.Cache.Tags.sound_gestalt_import_names_block> ImportNames;
		public TI.Block<Blam.Cache.Tags.sound_gestalt_runtime_permutation_bit_vector_block> RuntimePermutationFlags;

		protected sound_cache_file_gestalt_group_gen2(int field_count) : base(field_count) { }

		public abstract sound_gestalt_pitch_ranges_block GetPitchRange(int element);
		public abstract sound_gestalt_permutations_block GetPermutation(int element);
		public abstract sound_permutation_chunk_block_gen2 GetPermutationChunk(int element);
		public abstract sound_gestalt_extra_info_block GetExtraInfo(int element);
	};
	#endregion
	#endregion
}