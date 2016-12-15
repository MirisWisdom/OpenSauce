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
	public partial class antenna_group
	{
		#region antenna_vertex_block
		public partial class antenna_vertex_block
		{
			public antenna_vertex_block() : base(9)
			{
				Add(SpringStrengthCoefficent = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(new TI.Pad(24));
				Add(Angles = new TI.RealEulerAngles2D());
				Add(Length = new TI.Real());
				Add(SequenceIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(Color = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(LodColor = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(new TI.Pad(40 + 12));
			}
		};
		#endregion

		public antenna_group() : base(9)
		{
			Add(AttachmentMarkerName = new TI.String());
			Add(Bitmaps = new TI.TagReference(this, TagGroups.bitm));
			Add(Physics = new TI.TagReference(this, TagGroups.phys));
			Add(new TI.Pad(80));
			Add(SpringStrenthCoefficent = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(FalloffPixels = new TI.Real());
			Add(CutoffPixels = new TI.Real());
			Add(new TI.Pad(40));
			Add(Vertices = new TI.Block<antenna_vertex_block>(this, 20));
		}
	};
	#endregion

	#region contrail
	public partial class contrail_group
	{
		#region contrail_point_states_block
		public partial class contrail_point_states_block
		{
			public contrail_point_states_block() : base(8)
			{
				Add(Duration = new TI.RealBounds());
				Add(TransitionDuration = new TI.RealBounds());
				Add(Physics = new TI.TagReference(this, TagGroups.phys));
				Add(new TI.Pad(32));
				Add(Width = new TI.Real());
				Add(ColorLowerBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(ColorUpperBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(ScaleFlags = new TI.Flags());
			}
		};
		#endregion

		public contrail_group() : base(19)
		{
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(ScaleFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));

			Add(PointGenerationRate = new TI.Real());
			Add(PointVelocity = new TI.RealBounds());
			Add(PointVelocityConeAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(InheritedVelocityFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(RenderType = new TI.Enum());
			Add(new TI.Pad(2));
			Add(TextureRepeatsU = new TI.Real());
			Add(TextureRepeatsV = new TI.Real());
			Add(TextureAnimationU = new TI.Real());
			Add(TextureAnimationV = new TI.Real());
			Add(AnimationRate = new TI.Real());
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(FirstSequenceIndex = new TI.ShortInteger());
			Add(SequenceCount = new TI.ShortInteger());
			Add(new TI.Pad(64));

			// Map is 'Secondary Map'
			Add(ShaderMap = new TI.Struct<shader_map_struct>(this));
			Add(PointStates = new TI.Block<contrail_point_states_block>(this, 16));
		}
	};
	#endregion

	#region flag
	public partial class flag_group
	{
		#region flag_attachment_point_block
		public partial class flag_attachment_point_block
		{
			public flag_attachment_point_block() : base(3)
			{
				Add(HeightToNextAttachment = new TI.ShortInteger());
				Add(new TI.Pad(2 + 16));
				Add(MarkerName = new TI.String());
			}
		};
		#endregion

		public flag_group() : base(15)
		{
			Add(Flags = new TI.Flags());
			Add(TrailingEdgeShape = new TI.Enum());
			Add(TrailingEdgeShapeOffset = new TI.ShortInteger());
			Add(TrailingEdgeShape = new TI.Enum());
			Add(new TI.Pad(2));
			Add(Width = new TI.ShortInteger());
			Add(Height = new TI.ShortInteger());
			Add(CellWidth = new TI.Real());
			Add(CellHeight = new TI.Real());
			Add(RedFlagShader = new TI.TagReference(this, TagGroups.shdr));
			Add(Physics = new TI.TagReference(this, TagGroups.phys));
			Add(WindNoise = new TI.Real());
			Add(new TI.Pad(8));
			Add(BlueFlagShader = new TI.TagReference(this, TagGroups.shdr));
			Add(AttachmentPoints = new TI.Block<flag_attachment_point_block>(this, 5));
		}
	};
	#endregion

	#region glow
	public partial class glow_group
	{
		public glow_group() : base(42)
		{
			Add(AttachmentMarker = new TI.String());
			Add(NumberOfParticles = new TI.ShortInteger());
			Add(BoundaryEffect = new TI.Enum());
			Add(NormalParticleDistribution = new TI.Enum());
			Add(TrailingParticleDistribution = new TI.Enum());
			Add(Flags = new TI.Flags());
			Add(new TI.Pad(28 + 2 + 2 + 4));

			Add(ParticleRotAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(ParticleRotVelocity = new TI.Real());
			Add(EffectRotVelocityMultiplier = new TI.RealBounds());

			Add(EffectRotAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(EffectRotVelocity = new TI.Real());
			Add(ParticleRotVelocityMultiplier = new TI.RealBounds());

			Add(EffectTransAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(EffectTransVelocity = new TI.Real());
			Add(EffectTransVelocityMultiplier = new TI.RealBounds());

			Add(DistToObjectAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(DistToObject = new TI.RealBounds());
			Add(DistToObjectMultiplier = new TI.RealBounds());
			Add(new TI.Pad(8));

			Add(ParticleSizeAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(ParticleSizeBounds = new TI.RealBounds());
			Add(ParticleSizeAttachmentMultiplier = new TI.RealBounds());

			Add(ColorAttachment = new TI.Enum());
			Add(new TI.Pad(2));
			Add(ColorBound0 = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(ColorBound1 = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(ColorScale0 = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(ColorScale1 = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(ColorRateOfChange = new TI.Real());
			Add(FadingPercentageOfGlow = new TI.Real());
			Add(ParticleGenerationFreq = new TI.Real());
			Add(LifetimeOfTrailingParticles = new TI.Real());
			Add(VelocityOfTrailingParticles = new TI.Real());
			Add(TrailingParticleT = new TI.RealBounds());
			Add(new TI.Pad(52));
			Add(Texture = new TI.TagReference(this, TagGroups.bitm));
		}
	};
	#endregion

	#region light
	public partial class light_group
	{
		public light_group() : base(35)
		{
			Add(Flags = new TI.Flags());
			
			Add(ShapeRadius = new TI.Real());
			Add(ShapeRadiusModifier = new TI.RealBounds());
			Add(FalloffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CuttoffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(LensFlareOnlyRadius = new TI.Real());
			Add(new TI.Pad(24));

			Add(InterpolationFlags = new TI.Flags());
			Add(ColorLowerBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(ColorUpperBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
			Add(new TI.Pad(12));

			Add(PrimaryCubeMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(2));
			Add(TextureAnimationFunction = new TI.Enum());
			Add(TextureAnimationPeriod = new TI.Real());
			Add(SecondaryCubeMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(2));
			Add(YawFunction = new TI.Enum());
			Add(YawPeriod = new TI.Real());
			Add(new TI.Pad(2));
			Add(RollFunction = new TI.Enum());
			Add(RollPeriod = new TI.Real());
			Add(new TI.Pad(2));
			Add(PitchFunction = new TI.Enum());
			Add(PitchPeriod = new TI.Real());
			Add(new TI.Pad(8));

			Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			Add(new TI.Pad(24));
			
			Add(RadiosityIntensity = new TI.Real());
			Add(RadiosityColor = new TI.RealColor());
			Add(new TI.Pad(16));
			
			Add(Duration = new TI.Real());
			Add(new TI.Pad(2));

			Add(FalloffFunction = new TI.Enum());
			Add(new TI.Pad(8 + 92));
		}
	};
	#endregion

	#region light_volume
	public partial class light_volume_group
	{
		#region light_volume_frame_block
		public partial class light_volume_frame_block
		{
			public light_volume_frame_block() : base(14)
			{
				Add(new TI.Pad(16));
				Add(OffsetFromMarker = new TI.Real());
				Add(OffsetExponent = new TI.Real());
				Add(Length = new TI.Real());
				Add(new TI.Pad(32));
				Add(RadiusHither = new TI.Real());
				Add(RadiusYon = new TI.Real());
				Add(RadiusExponent = new TI.Real());
				Add(new TI.Pad(32));
				Add(TintColorHither = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(TintColorYon = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(TintColorExponent = new TI.Real());
				Add(BrightnessExponent = new TI.Real());
				Add(new TI.Pad(32));
			}
		};
		#endregion

		public light_volume_group() : base(18)
		{
			Add(AttachmentMarker = new TI.String());
			Add(new TI.Pad(2));
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(16));
			Add(NearFadeDist = new TI.Real());
			Add(FarFadeDist = new TI.Real());
			Add(PerpendicularBrightnessScale = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(ParallelBrightnessScale = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(BrightnessScaleSource = new TI.Enum());
			Add(new TI.Pad(2 + 20));
			Add(Map = new TI.TagReference(this, TagGroups.bitm));
			Add(SequenceIndex = new TI.ShortInteger());
			Add(DrawCount = new TI.ShortInteger());
			Add(new TI.Pad(72));
			Add(FrameAnimationSource = new TI.Enum());
			Add(new TI.Pad(2 + 36 + 64));
			Add(Frames = new TI.Block<light_volume_frame_block>(this, 2));
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region lightning
	public partial class lightning_group
	{
		#region lightning_marker_block
		public partial class lightning_marker_block
		{
			public lightning_marker_block() : base(10)
			{
				Add(AttachmentMarker = new TI.String());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(OctavesToNextMarker = new TI.ShortInteger());
				Add(new TI.Pad(2 + 76));
				Add(RandomPositionBounds = new TI.RealVector3D());
				Add(RandomJitter = new TI.Real());
				Add(Thickness = new TI.Real());
				Add(Tint = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(new TI.Pad(76));
			}
		};
		#endregion

		// TODO: this is really a shader_map_struct in disguise FYI
		#region lightning_shader_block
		public partial class lightning_shader_block
		{
			public lightning_shader_block() : base(6)
			{
				Add(new TI.Pad(40));
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(FrameBufferBlendFunction = new TI.Enum());
				Add(FrameBufferFadeMode = new TI.Enum());
				Add(MapFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(28 + 16 + 2 + 2 + 56 + 28));
			}
		};
		#endregion

		public lightning_group() : base(15)
		{
			Add(new TI.Pad(2));
			Add(DrawCount = new TI.ShortInteger());
			Add(new TI.Pad(16));
			Add(NearFadeDist = new TI.Real());
			Add(FarFadeDist = new TI.Real());
			Add(new TI.Pad(16));
			Add(JitterScaleSource = new TI.Enum());
			Add(ThicknessScaleSource = new TI.Enum());
			Add(TintModulationSource = new TI.Enum());
			Add(BrightnessScaleSource = new TI.Enum());
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(84));
			Add(Markers = new TI.Block<lightning_marker_block>(this, 16));
			Add(Shader = new TI.Block<lightning_shader_block>(this, 1));
			Add(new TI.Pad(88));
		}
	};
	#endregion
}