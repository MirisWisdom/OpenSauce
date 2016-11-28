/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region sound
	public partial class sound_group
	{
		#region sound_pitch_range_block
		public partial class sound_pitch_range_block
		{
			#region sound_permutations_block
			public partial class sound_permutations_block
			{
				public sound_permutations_block() : base(9)
				{
					Add(Name = new TI.String());
					Add(SkipFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(Gain = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(Compression = new TI.Enum());
					Add(NextPermutationIndex = new TI.ShortInteger());
					Add(new TI.Pad(20));
					Add(Samples = new TI.Data(this, BlamLib.TagInterface.DataType.Sound));
					Add(Mouth = new TI.Data(this));
					Add(SubtitleData = new TI.Data(this));
				}
			};
			#endregion

			public sound_pitch_range_block() : base(6)
			{
				Add(Name = new TI.String());
				Add(NaturalPitch = new TI.Real());
				Add(BendBounds = new TI.RealBounds());
				Add(ActualPermutationCount = new TI.ShortInteger());
				Add(new TI.Pad(2 + 12));
				Add(Permutations = new TI.Block<sound_permutations_block>(this, 256));
			}
		};
		#endregion

		public sound_group() : base(26)
		{
			Add(Flags = new TI.Flags());
			Add(Class = new TI.Enum());
			Add(SampleRate = new TI.Enum());
			Add(Distance = new TI.RealBounds());
			Add(SkipFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(RandomPitchBounds = new TI.RealBounds());
			Add(InnerConeAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(OuterConeAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(OuterConeGain = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(GainModifier = new TI.Real());
			Add(MaxBendPerSecond = new TI.Real());
			Add(new TI.Pad(12));
			Add(SkipFractionModifier1 = new TI.Real());
			Add(GainModifier1 = new TI.Real());
			Add(PitchModifier1 = new TI.Real());
			Add(new TI.Pad(12));
			Add(SkipFractionModifier2 = new TI.Real());
			Add(GainModifier2 = new TI.Real());
			Add(PitchModifier2 = new TI.Real());
			Add(new TI.Pad(12));
			Add(Encoding = new TI.Enum());
			Add(Compression = new TI.Enum());
			Add(PromotionSound = new TI.TagReference(this, TagGroups.snd_));
			Add(PromotionCount = new TI.ShortInteger());
			Add(new TI.Pad(2 + 20));
			Add(PitchRanges = new TI.Block<sound_pitch_range_block>(this, 8));
		}
	};
	#endregion

	#region sound_environment
	public partial class sound_environment_group
	{
		public sound_environment_group() : base(16)
		{
			Add(new TI.Pad(4));
			Add(Priority = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(RoomIntensity = new TI.Real());
			Add(RoomIntensityHf = new TI.Real());
			Add(RoomRollOff = new TI.Real());
			Add(DecayTime = new TI.Real());
			Add(DecayHfRatio = new TI.Real());
			Add(ReflectionsIntensity = new TI.Real());
			Add(ReflectionsDelay = new TI.Real());
			Add(ReverbIntensity = new TI.Real());
			Add(ReverbDelay = new TI.Real());
			Add(Diffusion = new TI.Real());
			Add(Denstiy = new TI.Real());
			Add(HfReference = new TI.Real());
			Add(new TI.Pad(16));
		}
	};
	#endregion

	#region sound_looping
	public partial class sound_looping_group
	{
		#region looping_sound_track_block
		public partial class looping_sound_track_block
		{
			public looping_sound_track_block() : base(11)
			{
				Add(Flags = new TI.Flags());
				Add(Gain = new TI.Real());
				Add(FadeInDuration = new TI.Real());
				Add(FadeOutDuration = new TI.Real());
				Add(new TI.Pad(16*2));
				Add(Start = new TI.TagReference(this, TagGroups.snd_));
				Add(Loop = new TI.TagReference(this, TagGroups.snd_));
				Add(End = new TI.TagReference(this, TagGroups.snd_));
				Add(new TI.Pad(16*2));
				Add(AltLoop = new TI.TagReference(this, TagGroups.snd_));
				Add(AltEnd = new TI.TagReference(this, TagGroups.snd_));
			}
		};
		#endregion

		#region looping_sound_detail_block
		public partial class looping_sound_detail_block
		{
			public looping_sound_detail_block() : base(8)
			{
				Add(Sound = new TI.TagReference(this, TagGroups.snd_));
				Add(RandomPeriodBounds = new TI.RealBounds());
				Add(Gain = new TI.Real());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(48));
				Add(YawBounds = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
				Add(PitchBounds = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
				Add(DistBounds = new TI.RealBounds());
			}
		};
		#endregion

		public sound_looping_group() : base(8)
		{
			Add(Flags = new TI.Flags());
			Add(DetailSoundPeriodWhenZero = new TI.Real());
			Add(new TI.Pad(8));
			Add(DetailSoundPeriodWhenOne = new TI.Real());
			Add(new TI.Pad(8 + 16));
			Add(ContinuousDamageEffect = new TI.TagReference(this, TagGroups.cdmg));
			Add(Tracks = new TI.Block<looping_sound_track_block>(this, 4));
			Add(DetailSounds = new TI.Block<looping_sound_detail_block>(this, 32));
		}
	};
	#endregion
}