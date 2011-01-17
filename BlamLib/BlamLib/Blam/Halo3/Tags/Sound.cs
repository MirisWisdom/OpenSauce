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
	#region sound_resource_definition
	[TI.Definition(1, sound_resource_definition.kSizeOf)]
	public class sound_resource_definition : TI.Definition
	{
		public const int kSizeOf = 20;

		public TI.Data Data;

		public sound_resource_definition() : base(1)
		{
			Add(Data = new TI.Data(this, TI.DataType.Sound));
		}

		internal static void InsertDataSizeIntoFauxDefinitionData(byte[] definition_data, int offset, uint data_size)
		{
			IO.ByteSwap.SwapUDWordAndInsert(data_size, definition_data, offset);
		}
	};
	#endregion

	#region cache_file_sound
	[TI.TagGroup((int)TagGroups.Enumerated.shit, 2, 32)]
	public partial class cache_file_sound_group : TI.Definition
	{
		#region Fields
		public TI.Flags Flags;
		public TI.Enum SoundClass;
		public TI.Enum SampleRate;
		public TI.Enum Encoding;
		public TI.ByteInteger CodecIndex;
		public TI.ShortInteger PlaybackIndex;
		public TI.ShortInteger Unknown08;
		public TI.ShortInteger Unknown0A;
		public TI.ShortInteger FirstPitchRangeIndex;
		public TI.ByteInteger PitchRangeIndex;
		public TI.ByteInteger ScaleIndex;
		public TI.ByteInteger PromotionIndex;
		public TI.ByteInteger CustomPlaybackIndex;
		public TI.ShortInteger ExtraInfoIndex;
		public TI.LongInteger Unknown14;
		public TI.LongInteger ResourceIndex;
		public TI.LongInteger MaximumPlayTime;
		#endregion

		#region Ctor
		public cache_file_sound_group()
		{
			// 0x0
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(SoundClass = new TI.Enum(TI.FieldType.ByteEnum));
			Add(SampleRate = new TI.Enum(TI.FieldType.ByteEnum));
			// 0x4
			Add(Encoding = new TI.Enum(TI.FieldType.ByteEnum));
			Add(CodecIndex = new TI.ByteInteger());
			Add(PlaybackIndex = new TI.ShortInteger());
			// 0x8
			Add(Unknown08 = new TI.ShortInteger()); // TODO: UNKNOWN FIELD, index to something, but doesn't seem to be in the sound gestalt
			Add(Unknown0A = new TI.ShortInteger()); // TODO: UNKNOWN FIELD, maybe an index to the first block in the sound gestalt? or not a index...
			// 0xC
			Add(FirstPitchRangeIndex = new TI.ShortInteger());
			Add(PitchRangeIndex = new TI.ByteInteger());
			Add(ScaleIndex = new TI.ByteInteger());
			// 0x10
			Add(PromotionIndex = new TI.ByteInteger());
			Add(CustomPlaybackIndex = new TI.ByteInteger());
			Add(ExtraInfoIndex = new TI.ShortInteger());
			// 0x14
			Add(Unknown14 = new TI.LongInteger()); // TODO: UNKNOWN FIELD
			// 0x18
			Add(ResourceIndex = new TI.LongInteger());
			// 0x1C
			Add(MaximumPlayTime = new TI.LongInteger());
		}
		#endregion

		public DatumIndex ResourceDatumIndex { get { return ResourceIndex.Value; } }
	};
	#endregion

	#region sound
	#region sound_playback_parameters_struct
	[TI.Struct((int)StructGroups.Enumerated.snpl, 2, 68)]
	public class sound_playback_parameters_struct : TI.Definition
	{
		#region Fields
		//
		public TI.Real MinDist;
		public TI.Real MaxDist;
		public TI.Real SkipFraction;
		public TI.Real MaxBendPerSec;
		public TI.Real GainBase;
		public TI.Real GainVariance;
		//
		//
		public TI.ShortIntegerBounds RandomPitchBounds;
		public TI.Real InnerConeAngle;
		public TI.Real OuterConeAngle;
		public TI.Real OuterConeGain;
		public TI.Flags Flags;
		public TI.Real Azimuth;
		public TI.Real PositionalGain;
		public TI.Real FirstPersonGain;
		#endregion

		#region Ctor
		public sound_playback_parameters_struct()
		{
			Add(new TI.ShortIntegerBounds()); // TODO: UNKNOWN FIELD

			Add(MinDist = new TI.Real());
			Add(MaxDist = new TI.Real());
			Add(SkipFraction = new TI.Real(TI.FieldType.RealFraction));
			Add(MaxBendPerSec = new TI.Real());
			Add(GainBase = new TI.Real());
			Add(GainVariance = new TI.Real());

			Add(new TI.Real()); // TODO: UNKNOWN FIELD
			Add(new TI.Real()); // TODO: UNKNOWN FIELD

			Add(RandomPitchBounds = new TI.ShortIntegerBounds());
			Add(InnerConeAngle = new TI.Real(TI.FieldType.Angle));
			Add(OuterConeAngle = new TI.Real(TI.FieldType.Angle));
			Add(OuterConeGain = new TI.Real());
			Add(Flags = new TI.Flags());
			Add(Azimuth = new TI.Real(TI.FieldType.Angle));
			Add(PositionalGain = new TI.Real());
			Add(FirstPersonGain = new TI.Real());
		}
		#endregion
	}
	#endregion

	#region sound_scale_modifiers_struct
	[TI.Struct((int)StructGroups.Enumerated.snsc, 1, 20)]
	public class sound_scale_modifiers_struct : TI.Definition
	{
		#region Fields
		public TI.RealBounds GainModifier;
		public TI.ShortIntegerBounds PitchModifier;
		public TI.RealBounds SkipFractionModifier;
		#endregion

		#region Ctor
		public sound_scale_modifiers_struct()
		{
			Add(GainModifier = new TI.RealBounds());
			Add(PitchModifier = new TI.ShortIntegerBounds());
			Add(SkipFractionModifier = new TI.RealBounds(TI.FieldType.RealFractionBounds));
		}
		#endregion
	}
	#endregion


	#region sound_promotion_rule_block
	[TI.Definition(1, 16)]
	public class sound_promotion_rule_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_promotion_rule_block() : base(4)
		{
			Add(/*pitch range = */ new TI.BlockIndex()); // 1 sound_pitch_range_block
			Add(/*maximum playing count = */ new TI.ShortInteger());
			Add(/*suppression time = */ new TI.Real());
			Add(new TI.Pad(8));
		}
		#endregion
	}
	#endregion

	#region sound_promotion_runtime_timer_block
	[TI.Definition(1, 4)]
	public class sound_promotion_runtime_timer_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_promotion_runtime_timer_block() : base(1)
		{
			Add(/* = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region sound_promotion_parameters_struct
	[TI.Struct((int)StructGroups.Enumerated.snpr, 2, 36)]
	public class sound_promotion_parameters_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_promotion_parameters_struct() : base(3)
		{
			Add(/*promotion rules = */ new TI.Block<sound_promotion_rule_block>(this, 9));
			Add(/*runtime timers = */ new TI.Block<sound_promotion_runtime_timer_block>(this, 9));
			Add(new TI.Pad(12));
		}
		#endregion
	}
	#endregion

	#region sound_playback_parameter_definition
	[TI.Struct((int)StructGroups.Enumerated.spl1, 1, 16)]
	public class sound_playback_parameter_definition : TI.Definition
	{

		public sound_playback_parameter_definition() : base(2)
		{
			Add(/*scale bounds = */ new TI.RealBounds());
			Add(/*random base and variance = */ new TI.RealBounds());
		}
	}
	#endregion


	#region platform_sound_override_mixbins_block
	[TI.Definition(1, 8)]
	public class platform_sound_override_mixbins_block : TI.Definition
	{

		public platform_sound_override_mixbins_block() : base(2)
		{
			Add(/*mixbin = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*gain = */ new TI.Real());
		}
	}
	#endregion

	#region platform_sound_filter_block
	[TI.Definition(1, 72)]
	public class platform_sound_filter_block : TI.Definition
	{

		public platform_sound_filter_block() : base(6)
		{
			Add(/*filter type = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*filter width = */ new TI.LongInteger());
			Add(/*left filter frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*left filter gain = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*right filter frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*right filter gain = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
	}
	#endregion

	#region platform_sound_pitch_lfo_block
	[TI.Definition(1, 48)]
	public class platform_sound_pitch_lfo_block : TI.Definition
	{

		public platform_sound_pitch_lfo_block() : base(3)
		{
			Add(/*delay = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*pitch modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
	}
	#endregion

	#region platform_sound_filter_lfo_block
	[TI.Definition(1, 64)]
	public class platform_sound_filter_lfo_block : TI.Definition
	{

		public platform_sound_filter_lfo_block() : base(4)
		{
			Add(/*delay = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*cutoff modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*gain modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
	}
	#endregion

	#region sound_effect_playback_block
	[TI.Definition(1, 72)]
	public class sound_effect_playback_block : TI.Definition
	{

		public sound_effect_playback_block() : base(1)
		{
			Add(new TI.Pad(72));//Add(/*sound effect struct = */ new TI.Struct<sound_effect_struct_definition>(this));
		}
	}
	#endregion

	#region simple_platform_sound_playback_struct
	[TI.Struct((int)StructGroups.Enumerated.plsn_simple, 1, 72)]
	public class simple_platform_sound_playback_struct : TI.Definition
	{

		public simple_platform_sound_playback_struct() : base(7)
		{
			// TODO: BLOCK STRUCTURE VERIFICATION.
			Add(TI.Pad.BlockHalo3);//Add(/*override mixbins = */ new TI.Block<platform_sound_override_mixbins_block>(this, 8));
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(8));
			Add(/*filter = */ new TI.Block<platform_sound_filter_block>(this, 1));
			// TODO: BLOCK STRUCTURE VERIFICATION.
			Add(TI.Pad.BlockHalo3);//Add(/*pitch lfo = */ new TI.Block<platform_sound_pitch_lfo_block>(this, 1));
			// TODO: BLOCK STRUCTURE VERIFICATION.
			Add(TI.Pad.BlockHalo3);//Add(/*filter lfo = */ new TI.Block<platform_sound_filter_lfo_block>(this, 1));
			// TODO: BLOCK STRUCTURE VERIFICATION.
			Add(TI.Pad.BlockHalo3);//Add(/*sound effect = */ new TI.Block<sound_effect_playback_block>(this, 1));
		}
	}
	#endregion


	#region sound_permutation_chunk_block
	[TI.Definition(3, 20)]
	public partial class sound_permutation_chunk_block : TI.Definition
	{
		public TI.LongInteger FileOffset;
		public TI.LongInteger SizeFlags;
		public TI.LongInteger RuntimeIndex;
		public TI.LongInteger Unknown0C, Unknown10;

		public sound_permutation_chunk_block() : base(5)
		{
			Add(FileOffset = new TI.LongInteger());
			Add(SizeFlags = new TI.LongInteger());
			Add(RuntimeIndex = new TI.LongInteger());
			Add(Unknown0C = new TI.LongInteger()); // header size?
			Add(Unknown10 = new TI.LongInteger());
		}

		public int GetSize() { return SizeFlags.Value & 0xFFFF; } // pretty sure size still takes up only the lower 16-bits on the 360
	}
	#endregion


	#region sound_encoded_dialogue_section_block
	[TI.Definition(2, 44)]
	public class sound_encoded_dialogue_section_block : TI.Definition
	{
		#region sound_permutation_dialogue_info_block
		[TI.Definition(1, 16)]
		public class sound_permutation_dialogue_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_permutation_dialogue_info_block() : base(4)
			{
				Add(/*mouth data offset = */ new TI.LongInteger());
				Add(/*mouth data length = */ new TI.LongInteger());
				Add(/*lipsync data offset = */ new TI.LongInteger());
				Add(/*lipsync data length = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Data EncodedData;
		public TI.Block<sound_permutation_dialogue_info_block> SoundDialogueInfo;
		#endregion

		#region Ctor
		public sound_encoded_dialogue_section_block() : base(3)
		{
			Add(EncodedData = new TI.Data(this));
			Add(SoundDialogueInfo = new TI.Block<sound_permutation_dialogue_info_block>(this, 288));
			Add(TI.Pad.BlockHalo3); // will have the same count as SoundDialogueInfo
				// tag block [0x28]
					// real[4]
					// dword[3] // probably some more reals? only seen zeros here
					// tag block [0x8]
						// word
						// word
						// word
						// word
		}
		#endregion
	}
	#endregion
	#endregion

	#region sound_gestalt
	#region sound_gestalt_platform_codec_block
	[TI.Definition(1, 3)]
	public partial class sound_gestalt_platform_codec_block : TI.Definition
	{
		public TI.Enum Unknown00;
		public TI.Enum Type;
		public TI.Flags Flags; // Channel mask?

		public sound_gestalt_platform_codec_block() : base(3)
		{
			Add(Unknown00 = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
		}
	}
	#endregion

	#region sound_gestalt_playback_block
	[TI.Definition(1, 68)]
	public class sound_gestalt_playback_block : TI.Definition
	{
		#region Fields
		public TI.Struct<sound_playback_parameters_struct> Playback;
		#endregion

		#region Ctor
		public sound_gestalt_playback_block()
		{
			Add(Playback = new TI.Struct<sound_playback_parameters_struct>(this));
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_scale_block
	[TI.Definition(1, 20)]
	public class sound_gestalt_scale_block : TI.Definition
	{
		#region Fields
		public TI.Struct<sound_scale_modifiers_struct> Scale;
		#endregion

		#region Ctor
		public sound_gestalt_scale_block()
		{
			Add(Scale = new TI.Struct<sound_scale_modifiers_struct>(this));
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_import_names_block
	[TI.Definition(1, 4)]
	public class sound_gestalt_import_names_block : TI.Definition
	{
		public TI.StringId ImportName;

		public sound_gestalt_import_names_block() : base(1)
		{
			Add(ImportName = new TI.StringId());
		}
	}
	#endregion

	#region sound_gestalt_pitch_range_parameters_block
	[TI.Definition(2, 14)]
	public class sound_gestalt_pitch_range_parameters_block : TI.Definition
	{
		public TI.ShortInteger NaturalPitch;
		public TI.ShortIntegerBounds BendBounds;
		public TI.ShortIntegerBounds MaxGainPitchBounds;
		public TI.ShortIntegerBounds Unknown0A;

		public sound_gestalt_pitch_range_parameters_block() : base(4)
		{
			Add(NaturalPitch = new TI.ShortInteger());
			Add(BendBounds = new TI.ShortIntegerBounds());
			Add(MaxGainPitchBounds = new TI.ShortIntegerBounds());
			Add(Unknown0A = new TI.ShortIntegerBounds()); // TODO: UNKNOWN FIELD
		}
	}
	#endregion

	#region sound_gestalt_pitch_ranges_block
	[TI.Definition(2, 12)]
	public partial class sound_gestalt_pitch_ranges_block : TI.Definition
	{
		public TI.BlockIndex Name;
		public TI.BlockIndex Parameters;
		public TI.BlockIndex Unknown04;
		public TI.ShortInteger FirstRuntimePermutationFlagIndex;
		public TI.ShortInteger EncodedPermutationData;
		public TI.BlockIndex FirstPermutation;

		public sound_gestalt_pitch_ranges_block() : base(6)
		{
			Add(Name = new TI.BlockIndex()); // 1 sound_gestalt_import_names_block
			Add(Parameters = new TI.BlockIndex()); // 1 sound_gestalt_pitch_range_parameters_block
			Add(Unknown04 = new TI.BlockIndex()); // I think this may be a block index to sound_gestalt_60_block
			Add(FirstRuntimePermutationFlagIndex = new TI.ShortInteger());
			Add(EncodedPermutationData = new TI.ShortInteger());
			Add(FirstPermutation = new TI.BlockIndex()); // 1 sound_gestalt_permutations_block
		}

		public int GetPermutationCount()
		{
			const int k_count_shift = 4;
			const int k_count_mask = 0x3F;

			int count = EncodedPermutationData.Value;
			count >>= k_count_shift;
			count &= k_count_mask;

			return count;
		}
	}
	#endregion

	#region sound_gestalt_permutations_block
	[TI.Definition(2, 16)]
	public partial class sound_gestalt_permutations_block : TI.Definition
	{
		public TI.BlockIndex Name;
		public TI.ShortInteger EncodedSkipFraction;
		public TI.ByteInteger EncodedGain;
		public TI.ByteInteger PermutationInfoIndex;
		public TI.ShortInteger LanguageNeutralTime;
		public TI.BlockIndex FirstChunk;
		public TI.ShortInteger ChunkCount;
		public TI.ShortInteger EncodedPermutationIndex; // The permutation index of this permutation definition

		public sound_gestalt_permutations_block() : base(8)
		{
			Add(Name = new TI.BlockIndex()); // 1 sound_gestalt_import_names_block
			Add(EncodedSkipFraction = new TI.ShortInteger());
			Add(EncodedGain = new TI.ByteInteger());
			Add(PermutationInfoIndex = new TI.ByteInteger());
			Add(LanguageNeutralTime = new TI.ShortInteger());
			Add(FirstChunk = new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // 1 sound_permutation_chunk_block
			Add(ChunkCount = new TI.ShortInteger());
			Add(EncodedPermutationIndex = new TI.ShortInteger());
		}

		public int GetPermutationIndex()
		{
			const int k_perm_index_mask = 0xFF;

			return EncodedPermutationIndex.Value & k_perm_index_mask;
		}
	}
	#endregion

	#region sound_gestalt_custom_playback_block
	[TI.Definition(1, 72)]
	public class sound_gestalt_custom_playback_block : TI.Definition
	{
		public TI.Struct<simple_platform_sound_playback_struct> PlaybackDefinition;

		public sound_gestalt_custom_playback_block() : base(1)
		{
			Add(PlaybackDefinition = new TI.Struct<simple_platform_sound_playback_struct>(this));
		}
	}
	#endregion

	#region sound_gestalt_runtime_permutation_bit_vector_block
	[TI.Definition(1, 1)]
	public class sound_gestalt_runtime_permutation_bit_vector_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_gestalt_runtime_permutation_bit_vector_block()
		{
			Add(/* = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_promotions_block
	[TI.Definition(1, 36)]
	public class sound_gestalt_promotions_block : TI.Definition
	{
		public TI.Struct<sound_promotion_parameters_struct> Promotion;

		public sound_gestalt_promotions_block() : base(1)
		{
			Add(Promotion = new TI.Struct<sound_promotion_parameters_struct>(this));
		}
	}
	#endregion

	#region sound_gestalt_extra_info_block
	[TI.Definition(2, 12)]
	public class sound_gestalt_extra_info_block : TI.Definition
	{
		public TI.Block<sound_encoded_dialogue_section_block> EncodedPermutationSection;

		public sound_gestalt_extra_info_block() : base(1)
		{
			Add(EncodedPermutationSection = new TI.Block<sound_encoded_dialogue_section_block>(this, 1));
		}
	}
	#endregion

	#region sound_cache_file_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.ugh_, 2, 184)]
	public partial class sound_cache_file_gestalt_group : TI.Definition
	{
		#region sound_gestalt_60_block
		[TI.Definition(1, 28)]
		public partial class sound_gestalt_60_block : TI.Definition
		{
			#region block_10
			[TI.Definition(1, 4)]
			public partial class block_10 : TI.Definition
			{
				public TI.ShortInteger Start, Length;

				public block_10() : base(2)
				{
					Add(Start = new TI.ShortInteger());
					Add(Length = new TI.ShortInteger());
				}
			}
			#endregion

			public TI.LongInteger Unknown00;
			public TI.Block<field_block<TI.ShortInteger>> Unknown04;
			public TI.Block<block_10> Unknown10;

			public sound_gestalt_60_block() : base(3)
			{
				Add(Unknown00 = new TI.LongInteger()); // self referencing to the element's position
				Add(Unknown04 = new TI.Block<field_block<TI.ShortInteger>>(this));
				Add(Unknown10 = new TI.Block<block_10>(this));
			}
		}
		#endregion

		#region Fields
		public TI.Block<sound_gestalt_platform_codec_block> PlatformCodecs;
 		public TI.Block<sound_gestalt_playback_block> Playbacks;
 		public TI.Block<sound_gestalt_scale_block> Scales;
 		public TI.Block<sound_gestalt_import_names_block> ImportNames;
 		public TI.Block<sound_gestalt_pitch_range_parameters_block> PitchRangeParameters;
 		public TI.Block<sound_gestalt_pitch_ranges_block> PitchRanges;
 		public TI.Block<sound_gestalt_permutations_block> Permutations;
 		public TI.Block<sound_gestalt_custom_playback_block> CustomPlaybacks;
		public TI.Block<sound_gestalt_60_block> Unknown60;
 		public TI.Block<sound_gestalt_runtime_permutation_bit_vector_block> RuntimePermutationFlags;
 		public TI.Block<sound_permutation_chunk_block> Chunks;
 		public TI.Block<sound_gestalt_promotions_block> Promotions;
		public TI.Block<sound_gestalt_extra_info_block> ExtraInfos;
		#endregion

		#region Ctor
		public sound_cache_file_gestalt_group()
		{
			Add(PlatformCodecs = new TI.Block<sound_gestalt_platform_codec_block>(this, 0));
 			Add(Playbacks = new TI.Block<sound_gestalt_playback_block>(this, 0));
 			Add(Scales = new TI.Block<sound_gestalt_scale_block>(this, 0));
 			Add(ImportNames = new TI.Block<sound_gestalt_import_names_block>(this, 0));

			// in ODST, a tag block was added here. only ever see it with zero byte data, and 
			// other tag memory whose data has the same memory hash will reference this same memory
			// tag block [0x?] At most: 0x10. best estimate size, only ever see this with one element.

 			Add(PitchRangeParameters = new TI.Block<sound_gestalt_pitch_range_parameters_block>(this, 0));
 			Add(PitchRanges = new TI.Block<sound_gestalt_pitch_ranges_block>(this, 0));
 			Add(Permutations = new TI.Block<sound_gestalt_permutations_block>(this, 0));
			Add(CustomPlaybacks = new TI.Block<sound_gestalt_custom_playback_block>(this, 0));
			Add(Unknown60 = new TI.Block<sound_gestalt_60_block>(this));
 			Add(RuntimePermutationFlags = new TI.Block<sound_gestalt_runtime_permutation_bit_vector_block>(this, 0));

			// All Halo 3 maps have had this all zeros
			// If this is ever used, I'm sure there is a tag data field somewhere in it...
			Add(new TI.Pad(12 + 12 + 4));

			Add(Chunks = new TI.Block<sound_permutation_chunk_block>(this, 0));
			Add(Promotions = new TI.Block<sound_gestalt_promotions_block>(this, 0));
			Add(ExtraInfos = new TI.Block<sound_gestalt_extra_info_block>(this, 0));
		}
		#endregion
	};
	#endregion
	#endregion

	#region sound_classes
	[TI.TagGroup((int)TagGroups.Enumerated.sncl, 1, 2, 12)]
	public class sound_classes_group : TI.Definition
	{
		#region sound_class_block
		[TI.Definition(2, 152)]
		public class sound_class_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_class_block()
			{
				Add(/*max sounds per tag [1,16] = */ new TI.ShortInteger());
				Add(/*max sounds per object [1,16] = */ new TI.ShortInteger());
				Add(/*preemption time = */ new TI.LongInteger());
				Add(/*internal flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*priority = */ new TI.ShortInteger());
				Add(/*cache miss mode = */ new TI.Enum());
				Add(/*reverb gain = */ new TI.Real());
				Add(/*override speaker gain = */ new TI.Real());
				Add(new TI.Pad(36)); // TODO: UNKNOWN FIELDS
				Add(/*distance bounds = */ new TI.RealBounds());
				Add(/*gain bounds = */ new TI.RealBounds());
				Add(/*cutscene ducking = */ new TI.Real());
				Add(/*cutscene ducking fade in time = */ new TI.Real());
				Add(/*cutscene ducking sustain time = */ new TI.Real());
				Add(/*cutscene ducking fade out time = */ new TI.Real());
				Add(/*scripted dialog ducking = */ new TI.Real());
				Add(/*scripted dialog ducking fade in time = */ new TI.Real());
				Add(/*scripted dialog ducking sustain time = */ new TI.Real());
				Add(/*scripted dialog ducking fade out time = */ new TI.Real());
				Add(new TI.Real()); // TODO: UNKNOWN FIELD
				Add(/*doppler factor = */ new TI.Real());
				Add(/*stereo playback type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(3));
				Add(new TI.Real()); // TODO: UNKNOWN FIELD
				Add(new TI.Real()); // TODO: UNKNOWN FIELD
				Add(/*transmission multiplier = */ new TI.Real());
				Add(new TI.Real()); // TODO: UNKNOWN FIELD
				Add(new TI.Real()); // TODO: UNKNOWN FIELD
				Add(/*obstruction max bend = */ new TI.Real());
				Add(/*occlusion max bend = */ new TI.Real());
				Add(new TI.LongInteger()); // TODO: UNKNOWN FIELD
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sound_classes_group()
		{
			Add(/*sound classes = */ new TI.Block<sound_class_block>(this, 54));
		}
		#endregion
	};
	#endregion

	#region sound_dialogue_constants
	[TI.TagGroup((int)TagGroups.Enumerated.spk_, 1, 40)]
	public class sound_dialogue_constants_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_dialogue_constants_group()
		{
			Add(/*almost never = */ new TI.Real());
			Add(/*rarely = */ new TI.Real());
			Add(/*somewhat = */ new TI.Real());
			Add(/*often = */ new TI.Real());
			Add(new TI.Pad(24));
		}
		#endregion
	};
	#endregion
}