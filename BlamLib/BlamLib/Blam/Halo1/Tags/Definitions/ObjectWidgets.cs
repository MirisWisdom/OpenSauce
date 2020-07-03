/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region antenna
	[TI.TagGroup((int)TagGroups.Enumerated.ant_, 1, 208)]
	public partial class antenna_group : TI.Definition
	{
		#region antenna_vertex_block
		[TI.Definition(-1, 128)]
		public partial class antenna_vertex_block : TI.Definition
		{
			public TI.Real SpringStrengthCoefficent;
			public TI.RealEulerAngles2D Angles;
			public TI.Real Length;
			public TI.ShortInteger SequenceIndex;
			public TI.RealColor Color;
			public TI.RealColor LodColor;
		};
		#endregion

		public TI.String AttachmentMarkerName;
		public TI.TagReference Bitmaps;
		public TI.TagReference Physics;
		public TI.Real SpringStrenthCoefficent;
		public TI.Real FalloffPixels;
		public TI.Real CutoffPixels;
		public TI.Block<antenna_vertex_block> Vertices;
	};
	#endregion

	#region contrail
	[TI.TagGroup((int)TagGroups.Enumerated.cont, 3, 324)]
	public partial class contrail_group : TI.Definition
	{
		#region contrail_point_states_block
		[TI.Definition(-1, 104)]
		public partial class contrail_point_states_block : TI.Definition
		{
			public TI.RealBounds Duration;
			public TI.RealBounds TransitionDuration;
			public TI.TagReference Physics;
			public TI.Real Width;
			public TI.RealColor ColorLowerBound;
			public TI.RealColor ColorUpperBound;
			public TI.Flags ScaleFlags;
		};
		#endregion

		public TI.Flags Flags;
		public TI.Flags ScaleFlags;

		public TI.Real PointGenerationRate;
		public TI.RealBounds PointVelocity;
		public TI.Real PointVelocityConeAngle;
		public TI.Real InheritedVelocityFraction;

		public TI.Enum RenderType;
		public TI.Real TextureRepeatsU, TextureRepeatsV;
		public TI.Real TextureAnimationU, TextureAnimationV;
		public TI.Real AnimationRate;
		public TI.TagReference Bitmap;
		public TI.ShortInteger FirstSequenceIndex;
		public TI.ShortInteger SequenceCount;

		public TI.Struct<shader_map_struct> ShaderMap;
		public TI.Block<contrail_point_states_block> PointStates;
	};
	#endregion

	#region flag
	[TI.TagGroup((int)TagGroups.Enumerated.flag, 1, 96)]
	public partial class flag_group : TI.Definition
	{
		#region flag_attachment_point_block
		[TI.Definition(-1, 52)]
		public partial class flag_attachment_point_block : TI.Definition
		{
			public TI.ShortInteger HeightToNextAttachment;
			public TI.String MarkerName;
		};
		#endregion

		public TI.Flags Flags;
		public TI.Enum TrailingEdgeShape;
		public TI.ShortInteger TrailingEdgeShapeOffset;
		public TI.Enum AttachedEdgeShape;
		public TI.ShortInteger Width, Height;
		public TI.Real CellWidth, CellHeight;
		public TI.TagReference RedFlagShader;
		public TI.TagReference Physics;
		public TI.Real WindNoise;
		public TI.TagReference BlueFlagShader;
		public TI.Block<flag_attachment_point_block> AttachmentPoints;
	};
	#endregion

	#region glow
	[TI.TagGroup((int)TagGroups.Enumerated.glw_, 1, 340)]
	public partial class glow_group : TI.Definition
	{
		public TI.String AttachmentMarker;
		public TI.ShortInteger NumberOfParticles;
		public TI.Enum BoundaryEffect;
		public TI.Enum NormalParticleDistribution;
		public TI.Enum TrailingParticleDistribution;
		public TI.Flags Flags;
		
		public TI.Enum ParticleRotAttachment;
		public TI.Real ParticleRotVelocity;
		public TI.RealBounds ParticleRotVelocityMultiplier;
		
		public TI.Enum EffectRotAttachment;
		public TI.Real EffectRotVelocity;
		public TI.RealBounds EffectRotVelocityMultiplier;
		
		public TI.Enum EffectTransAttachment;
		public TI.Real EffectTransVelocity;
		public TI.RealBounds EffectTransVelocityMultiplier;

		public TI.Enum DistToObjectAttachment;
		public TI.RealBounds DistToObject, DistToObjectMultiplier;

		public TI.Enum ParticleSizeAttachment;
		public TI.RealBounds ParticleSizeBounds, ParticleSizeAttachmentMultiplier;

		public TI.Enum ColorAttachment;
		public TI.RealColor ColorBound0, ColorBound1;
		public TI.RealColor ColorScale0, ColorScale1;
		public TI.Real ColorRateOfChange;
		public TI.Real FadingPercentageOfGlow;
		public TI.Real ParticleGenerationFreq;
		public TI.Real LifetimeOfTrailingParticles;
		public TI.Real VelocityOfTrailingParticles;
		public TI.RealBounds TrailingParticleT;
		public TI.TagReference Texture;
	};
	#endregion

	#region light
	[TI.TagGroup((int)TagGroups.Enumerated.ligh, 3, 352)]
	public partial class light_group : TI.Definition
	{
		public TI.Flags Flags;
		
		public TI.Real ShapeRadius;
		public TI.RealBounds ShapeRadiusModifier;
		public TI.Real FalloffAngle;
		public TI.Real CuttoffAngle;
		public TI.Real LensFlareOnlyRadius;
		
		public TI.Flags InterpolationFlags;
		public TI.RealColor ColorLowerBound;
		public TI.RealColor ColorUpperBound;

		public TI.TagReference PrimaryCubeMap;
		public TI.Enum TextureAnimationFunction;
		public TI.Real TextureAnimationPeriod;
		public TI.TagReference SecondaryCubeMap;
		public TI.Enum YawFunction;
		public TI.Real YawPeriod;
		public TI.Enum RollFunction;
		public TI.Real RollPeriod;
		public TI.Enum PitchFunction;
		public TI.Real PitchPeriod;

		public TI.TagReference LensFlare;

		public TI.Real RadiosityIntensity;
		public TI.RealColor RadiosityColor;

		public TI.Real Duration;
		public TI.Enum FalloffFunction;
	};
	#endregion

	#region light_volume
	[TI.TagGroup((int)TagGroups.Enumerated.mgs2, 1, 332)]
	public partial class light_volume_group : TI.Definition
	{
		#region light_volume_frame_block
		[TI.Definition(-1, 176)]
		public partial class light_volume_frame_block : TI.Definition
		{
			public TI.Real OffsetFromMarker;
			public TI.Real OffsetExponent;
			public TI.Real Length;

			public TI.Real RadiusHither;
			public TI.Real RadiusYon;
			public TI.Real RadiusExponent;

			public TI.RealColor TintColorHither;
			public TI.RealColor TintColorYon;
			public TI.Real TintColorExponent;
			public TI.Real BrightnessExponent;
		};
		#endregion

		public TI.String AttachmentMarker;
		public TI.Flags Flags;

		public TI.Real NearFadeDist;
		public TI.Real FarFadeDist;
		public TI.Real PerpendicularBrightnessScale;
		public TI.Real ParallelBrightnessScale;
		public TI.Enum BrightnessScaleSource;

		public TI.TagReference Map;
		public TI.ShortInteger SequenceIndex;
		public TI.ShortInteger DrawCount;

		public TI.Enum FrameAnimationSource;
		public TI.Block<light_volume_frame_block> Frames;
	};
	#endregion

	#region lightning
	[TI.TagGroup((int)TagGroups.Enumerated.elec, 1, 264)]
	public partial class lightning_group : TI.Definition
	{
		#region lightning_marker_block
		[TI.Definition(-1, 228)]
		public partial class lightning_marker_block : TI.Definition
		{
			public TI.String AttachmentMarker;
			public TI.Flags Flags;
			public TI.ShortInteger OctavesToNextMarker;
			public TI.RealVector3D RandomPositionBounds;
			public TI.Real RandomJitter;
			public TI.Real Thickness;
			public TI.RealColor Tint;
		};
		#endregion

		#region lightning_shader_block
		[TI.Definition(-1, 180)]
		public partial class lightning_shader_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.Enum FrameBufferBlendFunction;
			public TI.Enum FrameBufferFadeMode;
			public TI.Flags MapFlags;
		};
		#endregion

		public TI.ShortInteger DrawCount;
		public TI.Real NearFadeDist;
		public TI.Real FarFadeDist;
		public TI.Enum JitterScaleSource;
		public TI.Enum ThicknessScaleSource;
		public TI.Enum TintModulationSource;
		public TI.Enum BrightnessScaleSource;
		public TI.TagReference Bitmap;
		public TI.Block<lightning_marker_block> Markers;
		public TI.Block<lightning_shader_block> Shader;
	};
	#endregion
}