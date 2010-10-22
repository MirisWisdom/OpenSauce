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

namespace BlamLib.Blam.Halo2.Tags
{
	#region sound
	#region sound_playback_parameters_struct
	[TI.Struct((int)StructGroups.Enumerated.snpl, 1, 56)]
	public partial class sound_playback_parameters_struct : TI.Definition
	{
		#region Fields
		public TI.Real MinDist;
		public TI.Real MaxDist;
		public TI.Real SkipFraction;
		public TI.Real MaxBendPerSec;
		public TI.Real GainBase;
		public TI.Real GainVariance;
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
		public sound_playback_parameters_struct() : base(14)
		{
			Add(MinDist = new TI.Real());
			Add(MaxDist = new TI.Real());
			Add(SkipFraction = new TI.Real(TI.FieldType.RealFraction));
			Add(MaxBendPerSec = new TI.Real());
			Add(GainBase = new TI.Real());
			Add(GainVariance = new TI.Real());
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
	};
	#endregion

	#region sound_scale_modifiers_struct
	[TI.Struct((int)StructGroups.Enumerated.snsc, 1, 20)]
	public partial class sound_scale_modifiers_struct : TI.Definition
	{
		#region Fields
		public TI.RealBounds GainModifier;
		public TI.ShortIntegerBounds PitchModifier;
		public TI.RealBounds SkipFractionModifier;
		#endregion

		#region Ctor
		public sound_scale_modifiers_struct() : base(3)
		{
			Add(GainModifier = new TI.RealBounds());
			Add(PitchModifier = new TI.ShortIntegerBounds());
			Add(SkipFractionModifier = new TI.RealBounds(TI.FieldType.RealFractionBounds));
		}
		#endregion
	};
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
		#region Fields
		#endregion

		#region Ctor
		public sound_playback_parameter_definition() : base(2)
		{
			Add(/*scale bounds = */ new TI.RealBounds());
			Add(/*random base and variance = */ new TI.RealBounds());
		}
		#endregion
	}
	#endregion


	#region sound_effect_component_block
	[TI.Definition(1, 24)]
	public class sound_effect_component_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_effect_component_block() : base(3)
		{
			Add(/*sound = */ new TI.TagReference(this)); // lsnd,snd!,
			Add(/*gain = */ new TI.Real());
			Add(/*flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region sound_effect_override_parameters_block
	[TI.Definition(2, 36)]
	public class sound_effect_override_parameters_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_effect_override_parameters_block() : base(7)
		{
			Add(/*name = */ new TI.StringId());
			Add(/*input = */ new TI.StringId());
			Add(/*range = */ new TI.StringId());
			Add(/*time period = */ new TI.Real());
			Add(/*integer value = */ new TI.LongInteger());
			Add(/*real value = */ new TI.Real());
			// Custom: fned
			Add(/*function value = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region sound_effect_overrides_block
	[TI.Definition(1, 16)]
	public class sound_effect_overrides_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_effect_overrides_block() : base(2)
		{
			Add(/*name = */ new TI.StringId());
			Add(/*overrides = */ new TI.Block<sound_effect_override_parameters_block>(this, 128));
		}
		#endregion
	}
	#endregion


	#region platform_sound_effect_function_block
	[TI.Definition(1, 20)]
	public class platform_sound_effect_function_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_effect_function_block() : base(4)
		{
			Add(/*input = */ new TI.Enum());
			Add(/*range = */ new TI.Enum());
			Add(/*function = */ new TI.Struct<mapping_function>(this));
			Add(/*time period = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region platform_sound_effect_constant_block
	[TI.Definition(1, 4)]
	public class platform_sound_effect_constant_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_effect_constant_block() : base(1)
		{
			Add(/*constant value = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region platform_sound_effect_override_descriptor_block
	[TI.Definition(1, 1)]
	public class platform_sound_effect_override_descriptor_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_effect_override_descriptor_block() : base(1)
		{
			Add(/*override descriptor = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region platform_sound_effect_block
	[TI.Definition(1, 40)]
	public class platform_sound_effect_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_effect_block() : base(4)
		{
			Add(/*function inputs = */ new TI.Block<platform_sound_effect_function_block>(this, 16));
			Add(/*constant inputs = */ new TI.Block<platform_sound_effect_constant_block>(this, 16));
			Add(/*template override descriptors = */ new TI.Block<platform_sound_effect_override_descriptor_block>(this, 16));
			Add(/*input overrides = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region platform_sound_effect_collection_block
	[TI.Definition(1, 28)]
	public class platform_sound_effect_collection_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_effect_collection_block() : base(3)
		{
			Add(/*sound effects = */ new TI.Block<platform_sound_effect_block>(this, 8));
			Add(/*low frequency input = */ new TI.Block<platform_sound_effect_function_block>(this, 16));
			Add(/*sound effect overrides = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region sound_effect_struct_definition
	[TI.Struct((int)StructGroups.Enumerated.ssfx, 1, 72)]
	public class sound_effect_struct_definition : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_effect_struct_definition() : base(5)
		{
			Add(/* = */ new TI.TagReference(this, TagGroups._fx_));
			Add(/*components = */ new TI.Block<sound_effect_component_block>(this, 16));
			Add(/* = */ new TI.Block<sound_effect_overrides_block>(this, 128));
			Add(/* = */ new TI.Data(this));
			Add(/* = */ new TI.Block<platform_sound_effect_collection_block>(this, 1));
		}
		#endregion
	}
	#endregion


	#region platform_sound_override_mixbins_block
	[TI.Definition(1, 8)]
	public class platform_sound_override_mixbins_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_override_mixbins_block() : base(2)
		{
			Add(/*mixbin = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*gain = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region platform_sound_filter_block
	[TI.Definition(1, 72)]
	public class platform_sound_filter_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_filter_block() : base(6)
		{
			Add(/*filter type = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*filter width = */ new TI.LongInteger());
			Add(/*left filter frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*left filter gain = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*right filter frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*right filter gain = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
		#endregion
	}
	#endregion

	#region platform_sound_pitch_lfo_block
	[TI.Definition(1, 48)]
	public class platform_sound_pitch_lfo_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_pitch_lfo_block() : base(3)
		{
			Add(/*delay = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*pitch modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
		#endregion
	}
	#endregion

	#region platform_sound_filter_lfo_block
	[TI.Definition(1, 64)]
	public class platform_sound_filter_lfo_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_filter_lfo_block() : base(4)
		{
			Add(/*delay = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*frequency = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*cutoff modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
			Add(/*gain modulation = */ new TI.Struct<sound_playback_parameter_definition>(this));
		}
		#endregion
	}
	#endregion

	#region sound_effect_playback_block
	[TI.Definition(1, 72)]
	public class sound_effect_playback_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_effect_playback_block() : base(1)
		{
			Add(/*sound effect struct = */ new TI.Struct<sound_effect_struct_definition>(this));
		}
		#endregion
	}
	#endregion

	#region simple_platform_sound_playback_struct
	[TI.Struct((int)StructGroups.Enumerated.plsn_simple, 1, 72)]
	public class simple_platform_sound_playback_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public simple_platform_sound_playback_struct() : base(7)
		{
			Add(/*override mixbins = */ new TI.Block<platform_sound_override_mixbins_block>(this, 8));
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(8));
			Add(/*filter = */ new TI.Block<platform_sound_filter_block>(this, 1));
			Add(/*pitch lfo = */ new TI.Block<platform_sound_pitch_lfo_block>(this, 1));
			Add(/*filter lfo = */ new TI.Block<platform_sound_filter_lfo_block>(this, 1));
			Add(/*sound effect = */ new TI.Block<sound_effect_playback_block>(this, 1));
		}
		#endregion
	}
	#endregion


	#region sound_platform_sound_playback_block
	[TI.Definition(1, 84)]
	public class sound_platform_sound_playback_block : TI.Definition
	{
		public TI.Struct<simple_platform_sound_playback_struct> PlaybackDefinition;

		public sound_platform_sound_playback_block() : base(2)
		{
			Add(PlaybackDefinition = new TI.Struct<simple_platform_sound_playback_struct>(this));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
		}
	}
	#endregion


	#region sound_permutation_chunk_block
	[TI.Definition(2, 12)]
	public partial class sound_permutation_chunk_block : TI.Definition
	{
		#region Fields
		public TI.LongInteger FileOffset;
		public TI.LongInteger SizeFlags;
		public TI.LongInteger RuntimeIndex;
		#endregion

		#region Ctor
		public sound_permutation_chunk_block() : base(3)
		{
			Add(FileOffset = new TI.LongInteger());
			Add(SizeFlags = new TI.LongInteger());
			Add(RuntimeIndex = new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region sound_permutations_block
	[TI.Definition(1, 32)]
	public partial class sound_permutations_block : TI.Definition
	{
		#region Fields
		public TI.StringId Name;
		public TI.Real SkipFraction;
		public TI.Real Gain;
		public TI.LongInteger SamplesSize;
		public TI.BlockIndex RawInfo;
		public TI.ShortInteger LanguageNeutralMilliseconds;
		public TI.Block<sound_permutation_chunk_block> Chunks;
		#endregion

		#region Ctor
		public sound_permutations_block() : base(7)
		{
			Add(Name = new TI.StringId());
			Add(SkipFraction = new TI.Real(TI.FieldType.RealFraction));
			Add(Gain = new TI.Real());
			Add(SamplesSize = new TI.LongInteger());
			Add(RawInfo = new TI.BlockIndex());
			Add(LanguageNeutralMilliseconds = new TI.ShortInteger());
			Add(Chunks = new TI.Block<sound_permutation_chunk_block>(this, 32767));
		}
		#endregion
	};
	#endregion

	#region sound_pitch_range_block
	[TI.Definition(1, 32)]
	public partial class sound_pitch_range_block : TI.Definition
	{
		#region Fields
		public TI.StringId Name;
		public TI.ShortInteger NaturalPitch;
		public TI.ShortIntegerBounds BendBounds;
		public TI.ShortIntegerBounds PitchBounds;
		public TI.Block<sound_permutations_block> Permutations;
		#endregion

		#region Ctor
		public sound_pitch_range_block() : base(7)
		{
			Add(Name = new TI.StringId());
			Add(NaturalPitch = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(BendBounds = new TI.ShortIntegerBounds());
			Add(PitchBounds = new TI.ShortIntegerBounds());
			Add(new TI.Pad(4));
			Add(Permutations = new TI.Block<sound_permutations_block>(this, 32));
		}
		#endregion
	};
	#endregion


	#region sound_permutation_marker_block
	[TI.Definition(1, 12)]
	public class sound_permutation_marker_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_permutation_marker_block() : base(3)
		{
			Add(/*marker id = */ new TI.LongInteger());
			Add(/*name = */ new TI.StringId());
			Add(/*sample offset = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region sound_permutation_raw_info_block
	[TI.Definition(1, 80)]
	public partial class sound_permutation_raw_info_block : TI.Definition
	{
		#region Fields
		public TI.StringId SkipFractionName;
		public TI.Data Samples;
		public TI.Data MouthData;
		public TI.Data LipsyncData;
		public TI.Block<sound_permutation_marker_block> Markers;
		public TI.Enum Compression;
		public TI.Enum Language;
		#endregion

		#region Ctor
		public sound_permutation_raw_info_block() : base(8)
		{
			Add(SkipFractionName = new TI.StringId());
			Add(Samples = new TI.Data(this));
			Add(MouthData = new TI.Data(this));
			Add(LipsyncData = new TI.Data(this));
			Add(Markers = new TI.Block<sound_permutation_marker_block>(this, 65535));
			Add(Compression = new TI.Enum());
			Add(Language = new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
		}
		#endregion
	};
	#endregion

	#region sound_definition_language_permutation_info_block
	[TI.Definition(2, 12)]
	public partial class sound_definition_language_permutation_info_block : TI.Definition
	{
		public TI.Block<sound_permutation_raw_info_block> RawInfoBlock;

		public sound_definition_language_permutation_info_block() : base(1)
		{
			Add(RawInfoBlock = new TI.Block<sound_permutation_raw_info_block>(this, 18));
		}
	};
	#endregion

	#region sound_encoded_dialogue_section_block
	[TI.Definition(1, 32)]
	public partial class sound_encoded_dialogue_section_block : TI.Definition
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
		const int OffsetEncodedData = 0;
		const int OffsetSoundDialogueInfo = 8;

		public TI.Data EncodedData;
		public TI.Block<sound_permutation_dialogue_info_block> SoundDialogueInfo;
		#endregion

		#region Ctor
		public sound_encoded_dialogue_section_block() : base(2)
		{
			Add(EncodedData = new TI.Data(this));
			Add(SoundDialogueInfo = new TI.Block<sound_permutation_dialogue_info_block>(this, 288));
		}
		#endregion
	};
	#endregion

	#region sound_extra_info_block
	[TI.Definition(1, 64)]
	public partial class sound_extra_info_block : TI.Definition
	{
		public TI.Block<sound_definition_language_permutation_info_block> LanguagePermutationInfo;
		public TI.Block<sound_encoded_dialogue_section_block> EncodedPermutationSection;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;

		public sound_extra_info_block() : base(3)
		{
			Add(LanguagePermutationInfo = new TI.Block<sound_definition_language_permutation_info_block>(this, 576));
			Add(EncodedPermutationSection = new TI.Block<sound_encoded_dialogue_section_block>(this, 1));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
		}
	};
	#endregion

	#region sound
	[TI.TagGroup((int)TagGroups.Enumerated.snd_, 4, 6, 172)]
	public partial class sound_group : TI.Definition
	{
		#region Fields
		public TI.Flags Flags;
		public TI.Enum Class;
		public TI.Enum SampleRate;
		public TI.Enum OutputEffect;
		public TI.Enum ImportType;
		public TI.Struct<sound_playback_parameters_struct> Playback;
		public TI.Struct<sound_scale_modifiers_struct> Scale;
		public TI.Enum Encoding;
		public TI.Enum Compression;
		public TI.Struct<sound_promotion_parameters_struct> Promotion;
		public TI.Block<sound_pitch_range_block> PitchRanges;
		public TI.Block<sound_platform_sound_playback_block> PlatformParameters;
		public TI.Block<sound_extra_info_block> ExtraInfo;
		#endregion

		#region Ctor
		public sound_group() : base(15)
		{
			Add(Flags = new TI.Flags());
			Add(Class = new TI.Enum(TI.FieldType.ByteEnum));
			Add(SampleRate = new TI.Enum(TI.FieldType.ByteEnum));
			Add(OutputEffect = new TI.Enum(TI.FieldType.ByteEnum));
			Add(ImportType = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Playback = new TI.Struct<sound_playback_parameters_struct>(this));
			Add(Scale = new TI.Struct<sound_scale_modifiers_struct>(this));
			Add(new TI.Pad(2));
			Add(Encoding = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Compression = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Promotion = new TI.Struct<sound_promotion_parameters_struct>(this));
			Add(new TI.Pad(12));
			Add(PitchRanges = new TI.Block<sound_pitch_range_block>(this, 9));
			Add(PlatformParameters = new TI.Block<sound_platform_sound_playback_block>(this, 1));
			Add(ExtraInfo = new TI.Block<sound_extra_info_block>(this, 1));
		}
		#endregion
	};
	#endregion
	#endregion

	#region cache_file_sound
	[TI.TagGroup((int)TagGroups.Enumerated.shit, 1, 20)]
	public class cache_file_sound_group : TI.Definition
	{
		#region Fields
		public TI.Flags Flags;
		public TI.Enum SoundClass;
		public TI.Enum SampleRate;
		public TI.Enum Encoding;
		public TI.Enum Compression;
		public TI.ShortInteger PlaybackIndex;
		public TI.ShortInteger FirstPitchRangeIndex;
		public TI.ByteInteger PitchRangeIndex;
		public TI.ByteInteger ScaleIndex;
		public TI.ByteInteger PromotionIndex;
		public TI.ByteInteger CustomPlaybackIndex;
		public TI.ShortInteger ExtraInfoIndex;
		public TI.LongInteger MaximumPlayTime;
		#endregion

		#region Ctor
		public cache_file_sound_group() : base(13)
		{
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(SoundClass = new TI.Enum(TI.FieldType.ByteEnum));
			Add(SampleRate = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Encoding = new TI.Enum(TI.FieldType.ByteEnum));
			Add(Compression = new TI.Enum(TI.FieldType.ByteEnum));
			Add(PlaybackIndex = new TI.ShortInteger());
			Add(FirstPitchRangeIndex = new TI.ShortInteger());
			Add(PitchRangeIndex = new TI.ByteInteger());
			Add(ScaleIndex = new TI.ByteInteger());
			Add(PromotionIndex = new TI.ByteInteger());
			Add(CustomPlaybackIndex = new TI.ByteInteger());
			Add(ExtraInfoIndex = new TI.ShortInteger());
			Add(MaximumPlayTime = new TI.LongInteger());
		}
		#endregion
	};
	#endregion

	#region sound_gestalt
	#region sound_gestalt_playback_block
	[TI.Definition(1, 56)]
	public class sound_gestalt_playback_block : TI.Definition
	{
		#region Fields
		public TI.Struct<sound_playback_parameters_struct> Playback;
		#endregion

		#region Ctor
		public sound_gestalt_playback_block() : base(1)
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
		public sound_gestalt_scale_block() : base(1)
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
		public TI.StringId ImportName;
		#endregion

		#region Ctor
		public sound_gestalt_import_names_block() : base(1)
		{
			Add(ImportName = new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region sound_gestalt_pitch_range_parameters_block
	[TI.Definition(1, 10)]
	public class sound_gestalt_pitch_range_parameters_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger NaturalPitch;
		public TI.ShortIntegerBounds BendBounds;
		public TI.ShortIntegerBounds MaxGainPitchBounds;
		#endregion

		#region Ctor
		public sound_gestalt_pitch_range_parameters_block() : base(3)
		{
			Add(NaturalPitch = new TI.ShortInteger());
			Add(BendBounds = new TI.ShortIntegerBounds());
			Add(MaxGainPitchBounds = new TI.ShortIntegerBounds());
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
		public sound_gestalt_pitch_ranges_block() : base(6)
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
		public sound_gestalt_permutations_block() : base(8)
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

	#region sound_gestalt_custom_playback_block
	[TI.Definition(1, 72)]
	public class sound_gestalt_custom_playback_block : TI.Definition
	{
		#region Fields
		public TI.Struct<simple_platform_sound_playback_struct> PlaybackDefinition;
		#endregion

		#region Ctor
		public sound_gestalt_custom_playback_block() : base(1)
		{
			Add(PlaybackDefinition = new TI.Struct<simple_platform_sound_playback_struct>(this));
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
		public sound_gestalt_runtime_permutation_bit_vector_block() : base(1)
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
	[TI.Definition(1, 52)]
	public partial class sound_gestalt_extra_info_block : TI.Definition
	{
		public TI.Block<sound_encoded_dialogue_section_block> EncodedPermutationSection;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;

		public sound_gestalt_extra_info_block() : base(2)
		{
			Add(EncodedPermutationSection = new TI.Block<sound_encoded_dialogue_section_block>(this, 1));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
		}
	}
	#endregion

	#region sound_cache_file_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.ugh_, 1, 132)]
	public class sound_cache_file_gestalt_group : TI.Definition
	{
		#region Fields
		public TI.Block<sound_gestalt_playback_block> Playbacks;
		public TI.Block<sound_gestalt_scale_block> Scales;
		public TI.Block<sound_gestalt_import_names_block> ImportNames;
		public TI.Block<sound_gestalt_pitch_range_parameters_block> PitchRangeParameters;
		public TI.Block<sound_gestalt_pitch_ranges_block> PitchRanges;
		public TI.Block<sound_gestalt_permutations_block> Permutations;
		public TI.Block<sound_gestalt_custom_playback_block> CustomPlaybacks;
		public TI.Block<sound_gestalt_runtime_permutation_bit_vector_block> RuntimePermutationFlags;
		public TI.Block<sound_permutation_chunk_block> Chunks;
		public TI.Block<sound_gestalt_promotions_block> Promotions;
		public TI.Block<sound_gestalt_extra_info_block> ExtraInfos;
		#endregion

		public sound_cache_file_gestalt_group() : base(11)
		{
			Add(Playbacks = new TI.Block<sound_gestalt_playback_block>(this, 32767));
			Add(Scales = new TI.Block<sound_gestalt_scale_block>(this, 32767));
			Add(ImportNames = new TI.Block<sound_gestalt_import_names_block>(this, 32767));
			Add(PitchRangeParameters = new TI.Block<sound_gestalt_pitch_range_parameters_block>(this, 32767));
			Add(PitchRanges = new TI.Block<sound_gestalt_pitch_ranges_block>(this, 32767));
			Add(Permutations = new TI.Block<sound_gestalt_permutations_block>(this, 32767));
			Add(CustomPlaybacks = new TI.Block<sound_gestalt_custom_playback_block>(this, 32767));
			Add(RuntimePermutationFlags = new TI.Block<sound_gestalt_runtime_permutation_bit_vector_block>(this, 32767));
			Add(Chunks = new TI.Block<sound_permutation_chunk_block>(this, 32767));
			Add(Promotions = new TI.Block<sound_gestalt_promotions_block>(this, 32767));
			Add(ExtraInfos = new TI.Block<sound_gestalt_extra_info_block>(this, 32767));
		}
	};
	#endregion
	#endregion

	#region sound_classes
	[TI.TagGroup((int)TagGroups.Enumerated.sncl, 1, 2, 12)]
	public class sound_classes_group : TI.Definition
	{
		#region sound_class_block
		[TI.Definition(1, 92)]
		public class sound_class_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_class_block() : base(25)
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
				Add(/*doppler factor = */ new TI.Real());
				Add(/*stereo playback type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(3));
				Add(/*transmission multiplier = */ new TI.Real());
				Add(/*obstruction max bend = */ new TI.Real());
				Add(/*occlusion max bend = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sound_classes_group() : base(1)
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
		public sound_dialogue_constants_group() : base(5)
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

	#region platform_sound_playback_struct
	[TI.Struct((int)StructGroups.Enumerated.plsn, 1, 72)]
	public class platform_sound_playback_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public platform_sound_playback_struct() : base(7)
		{
			Add(/*override mixbins = */ new TI.Block<platform_sound_override_mixbins_block>(this, 8));
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(8));
			Add(/*filter = */ new TI.Block<platform_sound_filter_block>(this, 1));
			Add(/*pitch lfo = */ new TI.Block<platform_sound_pitch_lfo_block>(this, 1));
			Add(/*filter lfo = */ new TI.Block<platform_sound_filter_lfo_block>(this, 1));
			Add(/*sound effect = */ new TI.Block<sound_effect_playback_block>(this, 1));
		}
		#endregion
	}
	#endregion

	#region sound_effect_collection
	[TI.TagGroup((int)TagGroups.Enumerated.sfx_, 1, 12)]
	public class sound_effect_collection_group : TI.Definition
	{
		#region platform_sound_playback_block
		[TI.Definition(1, 76)]
		public class platform_sound_playback_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public platform_sound_playback_block() : base(2)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*playback = */ new TI.Struct<platform_sound_playback_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sound_effect_collection_group() : base(1)
		{
			Add(/*sound effects = */ new TI.Block<platform_sound_playback_block>(this, 128));
		}
		#endregion
	};
	#endregion

	#region sound_effect_template
	[TI.TagGroup((int)TagGroups.Enumerated._fx_, 1, 2, 40)]
	public class sound_effect_template_group : TI.Definition
	{
		#region sound_effect_templates_block
		[TI.Definition(1, 44)]
		public class sound_effect_templates_block : TI.Definition
		{
			#region sound_effect_template_parameter_block
			[TI.Definition(1, 40)]
			public class sound_effect_template_parameter_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public sound_effect_template_parameter_block() : base(9)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*type = */ new TI.Enum());
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*hardware offset = */ new TI.LongInteger());
					Add(/*default enum integer value = */ new TI.LongInteger());
					Add(/*default scalar value = */ new TI.Real());
					Add(/*default function = */ new TI.Struct<mapping_function>(this));
					Add(/*minimum scalar value = */ new TI.Real());
					Add(/*maximum scalar value = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public sound_effect_templates_block() : base(6)
			{
				Add(/*dsp effect = */ new TI.StringId());
				Add(/*explanation = */ new TI.Data(this));
				Add(/*flags = */ new TI.Flags());
				Add(/*dsp state offset = */ new TI.ShortInteger());
				Add(/*dsp state size = */ new TI.ShortInteger());
				Add(/*parameters = */ new TI.Block<sound_effect_template_parameter_block>(this, 128));
			}
			#endregion
		}
		#endregion

		#region sound_effect_template_additional_sound_input_block
		[TI.Definition(1, 20)]
		public class sound_effect_template_additional_sound_input_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_effect_template_additional_sound_input_block() : base(3)
			{
				Add(/*dsp effect = */ new TI.StringId());
				Add(/*low frequency sound = */ new TI.Struct<mapping_function>(this));
				Add(/*time period = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region platform_sound_effect_template_collection_block
		[TI.Definition(1, 16)]
		public class platform_sound_effect_template_collection_block : TI.Definition
		{
			#region platform_sound_effect_template_block
			[TI.Definition(1, 28)]
			public class platform_sound_effect_template_block : TI.Definition
			{
				#region platform_sound_effect_template_component_block
				[TI.Definition(1, 16)]
				public class platform_sound_effect_template_component_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public platform_sound_effect_template_component_block() : base(4)
					{
						Add(/*value type = */ new TI.Enum(TI.FieldType.LongEnum));
						Add(/*default value = */ new TI.Real());
						Add(/*minimum value = */ new TI.Real());
						Add(/*maximum value = */ new TI.Real());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public platform_sound_effect_template_block() : base(3)
				{
					Add(/*input dsp effect name = */ new TI.StringId());
					Add(new TI.Pad(12));
					Add(/*components = */ new TI.Block<platform_sound_effect_template_component_block>(this, 16));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public platform_sound_effect_template_collection_block() : base(2)
			{
				Add(/*platform effect templates = */ new TI.Block<platform_sound_effect_template_block>(this, 8));
				Add(/*input dsp effect name = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sound_effect_template_group() : base(4)
		{
			Add(/*template collection = */ new TI.Block<sound_effect_templates_block>(this, 8));
			Add(/*input effect name = */ new TI.StringId());
			Add(/*additional sound inputs = */ new TI.Block<sound_effect_template_additional_sound_input_block>(this, 1));
			Add(/*platform template collection = */ new TI.Block<platform_sound_effect_template_collection_block>(this, 1));
		}
		#endregion
	};
	#endregion

	#region sound_environment
	[TI.TagGroup((int)TagGroups.Enumerated.snde, 1, 2, 72)]
	public class sound_environment_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_environment_group() : base(16)
		{
			Add(new TI.Pad(4));
			Add(/*priority = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*room intensity = */ new TI.Real());
			Add(/*room intensity hf = */ new TI.Real());
			Add(/*room rolloff (0 to 10) = */ new TI.Real());
			Add(/*decay time (.1 to 20) = */ new TI.Real());
			Add(/*decay hf ratio (.1 to 2) = */ new TI.Real());
			Add(/*reflections intensity = */ new TI.Real());
			Add(/*reflections delay (0 to .3) = */ new TI.Real());
			Add(/*reverb intensity = */ new TI.Real());
			Add(/*reverb delay (0 to .1) = */ new TI.Real());
			Add(/*diffusion = */ new TI.Real());
			Add(/*density = */ new TI.Real());
			Add(/*hf reference(20 to 20,000) = */ new TI.Real());
			Add(new TI.Pad(16));
		}
		#endregion
	};
	#endregion

	#region sound_looping
	[TI.TagGroup((int)TagGroups.Enumerated.lsnd, 3, 3, 60)]
	public class sound_looping_group : TI.Definition
	{
		#region looping_sound_track_block
		[TI.Definition(2, 144)]
		public class looping_sound_track_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public looping_sound_track_block() : base(17)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags());
				Add(/*gain = */ new TI.Real());
				Add(/*fade in duration = */ new TI.Real());
				Add(/*fade out duration = */ new TI.Real());
				Add(/*in = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*loop = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*out = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*alt loop = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*alt out = */ new TI.TagReference(this, TagGroups.snd_));
				Add(new TI.UselessPad(12));
				Add(/*output effect = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*alt trans in = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*alt trans out = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*alt crossfade duration = */ new TI.Real());
				Add(/*alt fade out duration = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region looping_sound_detail_block
		[TI.Definition(2, 60)]
		public class looping_sound_detail_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public looping_sound_detail_block() : base(8)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*random period bounds = */ new TI.RealBounds());
				Add(/* = */ new TI.Real());
				Add(/*flags = */ new TI.Flags());
				Add(/*yaw bounds = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*pitch bounds = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*distance bounds = */ new TI.RealBounds());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sound_looping_group() : base(7)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*marty's music time = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(new TI.Pad(8));
			Add(/* = */ new TI.TagReference(this));
			Add(/*tracks = */ new TI.Block<looping_sound_track_block>(this, 3));
			Add(/*detail sounds = */ new TI.Block<looping_sound_detail_block>(this, 12));
		}
		#endregion
	};
	#endregion

	#region sound_global_mix_struct
	[TI.Struct((int)StructGroups.Enumerated.sngl, 1, 48)]
	public class sound_global_mix_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_global_mix_struct() : base(12)
		{
			Add(/*mono unspatialized gain = */ new TI.Real());
			Add(/*stereo to 3d gain = */ new TI.Real());
			Add(/*rear surround to front stereo gain = */ new TI.Real());
			Add(/*front speaker gain = */ new TI.Real());
			Add(/*center speaker gain = */ new TI.Real());
			Add(/*front speaker gain = */ new TI.Real());
			Add(/*center speaker gain = */ new TI.Real());
			Add(/*stereo unspatialized gain = */ new TI.Real());
			Add(/*solo player fade out delay = */ new TI.Real());
			Add(/*solo player fade out time = */ new TI.Real());
			Add(/*solo player fade in time = */ new TI.Real());
			Add(/*game music fade out time = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region sound_mix
	[TI.TagGroup((int)TagGroups.Enumerated.snmx, 1, 88)]
	public class sound_mix_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_mix_group() : base(11)
		{
			Add(/*left stereo gain = */ new TI.Real());
			Add(/*right stereo gain = */ new TI.Real());
			Add(/*left stereo gain = */ new TI.Real());
			Add(/*right stereo gain = */ new TI.Real());
			Add(/*left stereo gain = */ new TI.Real());
			Add(/*right stereo gain = */ new TI.Real());
			Add(/*front speaker gain = */ new TI.Real());
			Add(/*rear speaker gain = */ new TI.Real());
			Add(/*front speaker gain = */ new TI.Real());
			Add(/*rear speaker gain = */ new TI.Real());
			Add(/*global mix = */ new TI.Struct<sound_global_mix_struct>(this));
		}
		#endregion
	};
	#endregion

	#region stereo_system
	[TI.TagGroup((int)TagGroups.Enumerated.BooM, 1, 4)]
	public class stereo_system_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public stereo_system_group() : base(1)
		{
			Add(/*unused = */ new TI.LongInteger());
		}
		#endregion
	};
	#endregion
}