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
	#region cache_file_sound
	[TI.TagGroup((int)TagGroups.Enumerated.shit, 2, 32)]
	public class cache_file_sound_group : TI.Definition
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
		public TI.ShortInteger Unknown12;
		public TI.LongInteger Unknown14;
		//public TI.LongInteger ExtraInfoIndex;
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
			Add(Unknown12 = new TI.ShortInteger()); // TODO: UNKNOWN FIELD, index to the 15th block in the sound_cache_file_gestalt
			// 0x14
			//Add(ExtraInfoIndex = new TI.LongInteger());
			Add(Unknown14 = new TI.LongInteger()); // TODO: UNKNOWN FIELD
			// 0x18
			Add(ResourceIndex = new TI.LongInteger());
			// 0x1C
			Add(MaximumPlayTime = new TI.LongInteger());
		}
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		public static void Output(System.IO.StreamWriter s, cache_file_sound_group def)
		{
			s.WriteLine(
				"\t\t{0}\t{1}\t{2}" + Program.NewLine + // Flags,SoundClass,SampleRate
				"\t\t{3}\t{4}\t{5}" + Program.NewLine + // Encoding,CodecIndex,PlaybackIndex
				"\t\t{6}\t{7}" + Program.NewLine + // 08,0A
				"\t\t{8}\t{9}\t{10}" + Program.NewLine + // FirstPitchRangeIndex,PitchRangeIndex,ScaleIndex
				"\t\t{11}\t{12}\t{13}" + Program.NewLine + // PromotionIndex,CustomPlaybackIndex,12
				"\t\t{14}\t{15}\t{16}", // 14,ResourceIndex,MaximumPlayTime
				def.Flags.Value.ToString("X4"),def.SoundClass.Value.ToString("X2"), def.SampleRate.Value.ToString("X2"),
				def.Encoding.Value.ToString("X2"), def.CodecIndex.Value.ToString("X2"), def.PlaybackIndex.Value.ToString("X4"),
				def.Unknown08.Value.ToString("X4"), def.Unknown0A.Value.ToString("X4"),
				def.FirstPitchRangeIndex.Value.ToString("X4"), def.PitchRangeIndex.Value.ToString("X2"), def.ScaleIndex.Value.ToString("X2"),
				def.PromotionIndex.Value.ToString("X2"), def.CustomPlaybackIndex.Value.ToString("X2"), def.Unknown12.Value.ToString("X4"),
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
				"\t\t\t{11}\t{12}\t{13}" + Program.NewLine + // PromotionIndex,CustomPlaybackIndex,12
				"\t\t\t{14}\t{15}\t{16}", // 14,ResourceIndex,MaximumPlayTime
				def.Flags.Value.ToString("X4"),def.SoundClass.Value.ToString("X2"), def.SampleRate.Value.ToString("X2"),
				def.Encoding.Value.ToString("X2"), def.CodecIndex.Value.ToString("X2"), def.PlaybackIndex.Value.ToString("X4"),
				def.Unknown08.Value.ToString("X4"), def.Unknown0A.Value.ToString("X4"),
				def.FirstPitchRangeIndex.Value.ToString("X4"), def.PitchRangeIndex.Value.ToString("X2"), def.ScaleIndex.Value.ToString("X2"),
				def.PromotionIndex.Value.ToString("X2"), def.CustomPlaybackIndex.Value.ToString("X2"), def.Unknown12.Value.ToString("X4"),
				def.Unknown14.Value.ToString("X8"), def.ResourceIndex.Value.ToString("X8"), def.MaximumPlayTime.Value.ToString("X8")
				);
			dic[def.CodecIndex.Value].Add(sb.ToString());
			//dic[def.SampleRate.Value].Add(sb.ToString());
		}

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


	#region sound_permutation_chunk_block
	[TI.Definition(3, 20)]
	public class sound_permutation_chunk_block : TI.Definition
	{
		#region Fields
		public TI.LongInteger FileOffset;
		public TI.LongInteger SizeFlags;
		public TI.LongInteger RuntimeIndex;
		public TI.LongInteger Unknown0C, Unknown10;
		#endregion

		#region Ctor
		public sound_permutation_chunk_block() : base(3)
		{
			Add(FileOffset = new TI.LongInteger());
			Add(SizeFlags = new TI.LongInteger());
			Add(RuntimeIndex = new TI.LongInteger());
			Add(Unknown0C = new TI.LongInteger()); // header size?
			Add(Unknown10 = new TI.LongInteger());
		}
		#endregion

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
	public class sound_gestalt_platform_codec_block : TI.Definition
	{
		public TI.Enum Unknown01;
		public TI.Enum Type;
		public TI.Flags Flags; // Channel mask?

		#region Ctor
		public sound_gestalt_platform_codec_block() : base(3)
		{
			Add(Unknown01 = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
		}
		#endregion
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
		#region Fields
		//public TI.StringId ImportName;
		#endregion

		#region Ctor
		public sound_gestalt_import_names_block()
		{
			//Add(ImportName = new TI.StringId());
			Add(new TI.Pad(TI.Pad.DWord)); // Something is fishy...
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_pitch_range_parameters_block
	[TI.Definition(1, 14)]
	public class sound_gestalt_pitch_range_parameters_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger NaturalPitch;
		public TI.ShortIntegerBounds BendBounds;
		public TI.ShortIntegerBounds MaxGainPitchBounds;
		//
		#endregion

		#region Ctor
		public sound_gestalt_pitch_range_parameters_block()
		{
			Add(NaturalPitch = new TI.ShortInteger());
			Add(BendBounds = new TI.ShortIntegerBounds());
			Add(MaxGainPitchBounds = new TI.ShortIntegerBounds());
			Add(new TI.ShortIntegerBounds()); // TODO: UNKNOWN FIELD
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_pitch_ranges_block
	[TI.Definition(1, 12)]
	public class sound_gestalt_pitch_ranges_block : TI.Definition
	{
		#region Fields
		public TI.BlockIndex Name;
		public TI.BlockIndex Parameters;
		public TI.ShortInteger EncodedPermutationData;
		public TI.ShortInteger FirstRuntimePermutationFlagIndex;
		public TI.BlockIndex FirstPermutation;
		public TI.ShortInteger PermutationCount;
		#endregion

		#region Ctor
		public sound_gestalt_pitch_ranges_block()
		{
			Add(Name = new TI.BlockIndex()); // 1 sound_gestalt_import_names_block
			Add(Parameters = new TI.BlockIndex()); // 1 sound_gestalt_pitch_range_parameters_block
			Add(EncodedPermutationData = new TI.ShortInteger());
			Add(FirstRuntimePermutationFlagIndex = new TI.ShortInteger());
			Add(FirstPermutation = new TI.BlockIndex()); // 1 sound_gestalt_permutations_block
			Add(PermutationCount = new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_permutations_block
	[TI.Definition(1, 16)]
	public class sound_gestalt_permutations_block : TI.Definition
	{
		#region Fields
		public TI.BlockIndex Name;
		public TI.ShortInteger EncodedSkipFraction;
		public TI.ByteInteger EncodedGain;
		public TI.ByteInteger PermutationInfoIndex;
		public TI.ShortInteger LanguageNeutralTime;
		public TI.LongInteger SampleSize;
		public TI.BlockIndex FirstChunk;
		public TI.ShortInteger ChunkCount;
		#endregion

		#region Ctor
		public sound_gestalt_permutations_block()
		{
			Add(Name = new TI.BlockIndex()); // 1 sound_gestalt_import_names_block
			Add(EncodedSkipFraction = new TI.ShortInteger());
			Add(EncodedGain = new TI.ByteInteger());
			Add(PermutationInfoIndex = new TI.ByteInteger());
			Add(LanguageNeutralTime = new TI.ShortInteger());
			Add(SampleSize = new TI.LongInteger());
			Add(FirstChunk = new TI.BlockIndex()); // 1 sound_permutation_chunk_block
			Add(ChunkCount = new TI.ShortInteger());
		}
		#endregion
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
		#region Fields
		public TI.Struct<sound_promotion_parameters_struct> Promotion;
		#endregion

		#region Ctor
		public sound_gestalt_promotions_block() : base(1)
		{
			Add(Promotion = new TI.Struct<sound_promotion_parameters_struct>(this));
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_extra_info_block
	[TI.Definition(2, 12)]
	public class sound_gestalt_extra_info_block : TI.Definition
	{
		#region Fields
		public TI.Block<sound_encoded_dialogue_section_block> EncodedPermutationSection;
		#endregion

		#region Ctor
		public sound_gestalt_extra_info_block() : base(1)
		{
			Add(EncodedPermutationSection = new TI.Block<sound_encoded_dialogue_section_block>(this, 1));
		}
		#endregion
	}
	#endregion

	#region sound_cache_file_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.ugh_, 2, 184)]
	public class sound_cache_file_gestalt_group : TI.Definition
	{
		#region Fields
		public TI.Block<sound_gestalt_platform_codec_block> PlatformCodecs;
 		public TI.Block<sound_gestalt_playback_block> Playbacks;
 		public TI.Block<sound_gestalt_scale_block> Scales;
 		public TI.Block<sound_gestalt_import_names_block> ImportNames;
 		public TI.Block<sound_gestalt_pitch_range_parameters_block> PitchRangeParameters;
 		public TI.Block<sound_gestalt_pitch_ranges_block> PitchRanges;
 		public TI.Block<sound_gestalt_permutations_block> Permutations;
 		//public TI.Block<sound_gestalt_custom_playback_block> CustomPlaybacks;
 		public TI.Block<sound_gestalt_runtime_permutation_bit_vector_block> RuntimePermutationFlags;
 		public TI.Block<sound_permutation_chunk_block> Chunks;
 		public TI.Block<sound_gestalt_promotions_block> Promotions;
		public TI.Block<sound_gestalt_extra_info_block> ExtraInfos;
		#endregion

		#region Ctor
		public sound_cache_file_gestalt_group()
		{
			Add(PlatformCodecs = new TI.Block<sound_gestalt_platform_codec_block>(this, 0));
 			Add(Playbacks = new TI.Block<sound_gestalt_playback_block>(this, 32767));
 			Add(Scales = new TI.Block<sound_gestalt_scale_block>(this, 32767));
 			Add(ImportNames = new TI.Block<sound_gestalt_import_names_block>(this, 32767));

			// in ODST, a tag block was added here. only ever see it with zero byte data, and 
			// other tag memory whose data has the same memory hash will reference this same memory
			// tag block [0x?] At most: 0x10. best estimate size, only ever see this with one element.

 			Add(PitchRangeParameters = new TI.Block<sound_gestalt_pitch_range_parameters_block>(this, 32767));
 			Add(PitchRanges = new TI.Block<sound_gestalt_pitch_ranges_block>(this, 32767));
 			Add(Permutations = new TI.Block<sound_gestalt_permutations_block>(this, 32767));
			// TODO: BLOCK STRUCTURE VERIFICATION. I actually don't know if this is custom playbacks or not
			Add(TI.Pad.BlockHalo3);//Add(CustomPlaybacks = new TI.Block<sound_gestalt_custom_playback_block>(this, 32767));
			Add(TI.Pad.BlockHalo3);
				// long // index, self referencing to the element's position
				// tag block [0x2] [short]
				// tag block [0x4] [short start, short length]
 			Add(RuntimePermutationFlags = new TI.Block<sound_gestalt_runtime_permutation_bit_vector_block>(this, 32767));

			// I'm sure there is a tag data field somewhere here...
			Add(TI.Pad.BlockHalo3); // ?
			Add(TI.Pad.BlockHalo3); // ?
			Add(TI.Pad.DWord);

			Add(Chunks = new TI.Block<sound_permutation_chunk_block>(this, 32767));
			Add(Promotions = new TI.Block<sound_gestalt_promotions_block>(this, 32767));
			Add(ExtraInfos = new TI.Block<sound_gestalt_extra_info_block>(this, 32767));
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