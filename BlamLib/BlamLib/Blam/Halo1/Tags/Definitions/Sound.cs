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
	[TI.TagGroup((int)TagGroups.Enumerated.snd_, 4, 164)]
	public partial class sound_group : TI.Definition
	{
		#region sound_pitch_range_block
		[TI.Definition(-1, 72)]
		public partial class sound_pitch_range_block : TI.Definition
		{
			#region sound_permutations_block
			[TI.Definition(-1, 124)]
			public partial class sound_permutations_block : TI.Definition
			{
				public TI.String Name;
				public TI.Real SkipFraction;
				public TI.Real Gain;
				public TI.Enum Compression;
				public TI.ShortInteger NextPermutationIndex;
				public TI.Data Samples;
				public TI.Data Mouth;
				public TI.Data SubtitleData;
			};
			#endregion

			public TI.String Name;
			public TI.Real NaturalPitch;
			public TI.RealBounds BendBounds;
			public TI.ShortInteger ActualPermutationCount;
			public TI.Block<sound_permutations_block> Permutations;
		};
		#endregion

		public TI.Flags Flags;
		public TI.Enum Class;
		public TI.Enum SampleRate;
		public TI.RealBounds Distance;
		public TI.Real SkipFraction;
		public TI.RealBounds RandomPitchBounds;
		public TI.Real InnerConeAngle;
		public TI.Real OuterConeAngle;
		public TI.Real OuterConeGain;
		public TI.Real GainModifier;
		public TI.Real MaxBendPerSecond;

		public TI.Real SkipFractionModifier1;
		public TI.Real GainModifier1;
		public TI.Real PitchModifier1;

		public TI.Real SkipFractionModifier2;
		public TI.Real GainModifier2;
		public TI.Real PitchModifier2;

		public TI.Enum Encoding;
		public TI.Enum Compression;
		public TI.TagReference PromotionSound;
		public TI.ShortInteger PromotionCount;

		public TI.Block<sound_pitch_range_block> PitchRanges;
	};
	#endregion

	#region sound_environment
	[TI.TagGroup((int)TagGroups.Enumerated.snde, 1, 72)]
	public partial class sound_environment_group : TI.Definition
	{
		public TI.ShortInteger Priority;
		public TI.Real RoomIntensity, RoomIntensityHf, RoomRollOff, DecayTime, DecayHfRatio, ReflectionsIntensity,
			ReflectionsDelay, ReverbIntensity, ReverbDelay, Diffusion, Denstiy, HfReference;
	};
	#endregion

	#region sound_looping
	[TI.TagGroup((int)TagGroups.Enumerated.lsnd, 3, 84)]
	public partial class sound_looping_group : TI.Definition
	{
		#region looping_sound_track_block
		[TI.Definition(-1, 160)]
		public partial class looping_sound_track_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.Real Gain;
			public TI.Real FadeInDuration, FadeOutDuration;
			public TI.TagReference Start, Loop, End, AltLoop, AltEnd;
		};
		#endregion

		#region looping_sound_detail_block
		[TI.Definition(-1, 104)]
		public partial class looping_sound_detail_block : TI.Definition
		{
			public TI.TagReference Sound;
			public TI.RealBounds RandomPeriodBounds;
			public TI.Real Gain;
			public TI.Flags Flags;
			public TI.RealBounds YawBounds, PitchBounds, DistBounds;
		};
		#endregion

		public TI.Flags Flags;
		public TI.Real DetailSoundPeriodWhenZero, DetailSoundPeriodWhenOne;
		public TI.TagReference ContinuousDamageEffect;
		public TI.Block<looping_sound_track_block> Tracks;
		public TI.Block<looping_sound_detail_block> DetailSounds;
	};
	#endregion
}