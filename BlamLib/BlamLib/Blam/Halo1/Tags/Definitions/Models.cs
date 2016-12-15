/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region model
	[TI.TagGroup((int)TagGroups.Enumerated.mode, 4, 232)]
	public partial class model_group : TI.Definition
	{
		public const int kSizeOf_rasterizer_triangle_buffer = 0x10;
		public const int kSizeOf_rasterizer_vertex_buffer = 0x14;

		#region model_markers_block
		[TI.Definition(-1, 64)]
		public partial class model_markers_block : TI.Definition
		{
			#region model_marker_instance_block
			[TI.Definition(-1, 32)]
			public partial class model_marker_instance_block : TI.Definition
			{
				public TI.BlockIndex RegionIndex;
				public TI.BlockIndex PermutationIndex;
				public TI.BlockIndex NodeIndex;
				public TI.RealPoint3D Translation;
				public TI.RealQuaternion Rotation;
			};
			#endregion

			public TI.String Name;
			public TI.ShortInteger MagicIdentifer;
			public TI.Block<model_marker_instance_block> Instances;
		};
		#endregion

		#region model_node_block
		[TI.Definition(-1, 156)]
		public partial class model_node_block : TI.Definition
		{
			public TI.String Name;
			public TI.BlockIndex NextSiblingNode;
			public TI.BlockIndex FirstChildNode;
			public TI.BlockIndex ParentNode;
			public TI.RealPoint3D DefaultTranslation;
			public TI.RealQuaternion DefaultRotation;
			public TI.Real NodeDistFromParent;
		};
		#endregion

		#region model_region_block
		[TI.Definition(-1, 76)]
		public partial class model_region_block : TI.Definition
		{
			#region model_region_permutation_block
			[TI.Definition(-1, 88)]
			public partial class model_region_permutation_block : TI.Definition
			{
				#region model_region_permutation_marker_block
				[TI.Definition(-1, 80)]
				public partial class model_region_permutation_marker_block : TI.Definition
				{
					public TI.String Name;
					public TI.BlockIndex NodeIndex;
					public TI.RealPoint3D Translation;
					public TI.RealQuaternion Rotation;
				};
				#endregion

				public TI.String Name;
				public TI.Flags Flags;
				public TI.BlockIndex SuperLow, Low, Medium, High, SuperHigh;
				public TI.Block<model_region_permutation_marker_block> Markers;
			};
			#endregion

			public TI.String Name;
			public TI.Block<model_region_permutation_block> Permutations;
		};
		#endregion

		#region model_geometry_block
		[TI.Definition(-1, 48)]
		public partial class model_geometry_block : TI.Definition
		{
			#region model_geometry_part_block
			[TI.Definition(-1, 104)]
			public partial class model_geometry_part_block : TI.Definition
			{
				#region model_vertex_uncompressed_block
				[TI.Definition(-1, 68)]
				public partial class model_vertex_uncompressed_block : TI.Definition
				{
					public TI.RealPoint3D Position;
					public TI.RealVector3D Normal;
					public TI.RealVector3D Binormal;
					public TI.RealVector3D Tangent;
					public TI.RealPoint2D TextureCoords;
					public TI.ShortInteger NodeIndex1;
					public TI.ShortInteger NodeIndex2;
					public TI.Real NodeWeight1;
					public TI.Real NodeWeight2;
				};
				#endregion

				#region model_vertex_compressed_block
				[TI.Definition(-1, 32)]
				public partial class model_vertex_compressed_block : TI.Definition
				{
					public TI.RealPoint3D Position;
					public TI.LongInteger Normal;
					public TI.LongInteger Binormal;
					public TI.LongInteger Tangent;
					public TI.Point2D TextureCoords;
					public TI.ByteInteger NodeIndex1;
					public TI.ByteInteger NodeIndex2;
					public TI.ShortInteger NodeWeight0;
				};
				#endregion

				#region model_triangle_block
				[TI.Definition(-1, 6)]
				public partial class model_triangle_block : TI.Definition
				{
					public TI.ShortInteger VertexIndex0;
					public TI.ShortInteger VertexIndex1;
					public TI.ShortInteger VertexIndex2;
				};
				#endregion

				public TI.Flags Flags;
				public TI.BlockIndex ShaderIndex;
				public TI.ByteInteger PrevFilthyPartIndex;
				public TI.ByteInteger NextFilthyPartIndex;
				public TI.ShortInteger CentroidPrimaryNode;
				public TI.ShortInteger CentroidSecondaryNode;
				public TI.Real CentroidPrimaryWeight;
				public TI.Real CentroidSecondaryWeight;
				public TI.RealPoint3D Centroid;
				public TI.Block<model_vertex_uncompressed_block> UncompressedVertices;
				public TI.Block<model_vertex_compressed_block> CompressedVertices;
				public TI.Block<model_triangle_block> Triangles;
			};
			#endregion

			public TI.Flags Flags;
			public TI.Block<model_geometry_part_block> Parts;
		};
		#endregion

		#region model_shader_reference_block
		[TI.Definition(-1, 32)]
		public partial class model_shader_reference_block : TI.Definition
		{
			public TI.TagReference Shader;
			public TI.ShortInteger Permutation;
		};
		#endregion

		public TI.Flags Flags;
		public TI.LongInteger NodeListChecksum;
		public TI.Real SuperHighDetailCutoff, HighDetailCutoff, MediumDetailCutoff, LowDetailCutoff, SuperLowDetailCutoff;
		public TI.ShortInteger SuperHighDetailNodeCount, HighDetailNodeCount, MediumDetailNodeCount, LowDetailNodeCount, SuperLowDetailNodeCount;
		public TI.Real BaseMapUScale, BaseMapVScale;
		public TI.Block<model_markers_block> Markers;
		public TI.Block<model_node_block> Nodes;
		public TI.Block<model_region_block> Regions;
		public TI.Block<model_geometry_block> Geometries;
		public TI.Block<model_shader_reference_block> Shaders;
	};
	#endregion

	#region gbxmodel
	[TI.TagGroup((int)TagGroups.Enumerated.mod2, 5, 232)]
	public partial class gbxmodel_group : TI.Definition
	{
		#region model_geometry_block
		[TI.Definition(-1, 48)]
		public partial class model_geometry_block : TI.Definition
		{
			#region model_geometry_part_block
			[TI.Definition(-1, 132)]
			public partial class model_geometry_part_block : TI.Definition
			{
				public TI.Flags Flags;
				public TI.BlockIndex ShaderIndex;
				public TI.ByteInteger PrevFilthyPartIndex;
				public TI.ByteInteger NextFilthyPartIndex;
				public TI.ShortInteger CentroidPrimaryNode;
				public TI.ShortInteger CentroidSecondaryNode;
				public TI.Real CentroidPrimaryWeight;
				public TI.Real CentroidSecondaryWeight;
				public TI.RealPoint3D Centroid;
				public TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_vertex_uncompressed_block> UncompressedVertices;
				public TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_vertex_compressed_block> CompressedVertices;
				public TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_triangle_block> Triangles;
				public TI.ByteInteger NodeMapCount;
				public TI.ByteInteger[] NodeMap = new TI.ByteInteger[22];
			};
			#endregion

			public TI.Flags Flags;
			public TI.Block<model_geometry_part_block> Parts;
		};
		#endregion

		public TI.Flags Flags;
		public TI.LongInteger NodeListChecksum;
		public TI.Real SuperHighDetailCutoff, HighDetailCutoff, MediumDetailCutoff, LowDetailCutoff, SuperLowDetailCutoff;
		public TI.ShortInteger SuperHighDetailNodeCount, HighDetailNodeCount, MediumDetailNodeCount, LowDetailNodeCount, SuperLowDetailNodeCount;
		public TI.Real BaseMapUScale, BaseMapVScale;
		public TI.Block<model_group.model_markers_block> Markers;
		public TI.Block<model_group.model_node_block> Nodes;
		public TI.Block<model_group.model_region_block> Regions;
		public TI.Block<model_geometry_block> Geometries;
		public TI.Block<model_group.model_shader_reference_block> Shaders;
	};
	#endregion

	#region model_collision
	[TI.TagGroup((int)TagGroups.Enumerated.coll, 10, 664)]
	public partial class model_collision_group : TI.Definition
	{
		#region damage_materials_block
		[TI.Definition(-1, 72)]
		public partial class damage_materials_block : TI.Definition
		{
			public TI.String Name;
			public TI.Flags Flags;
			public TI.Enum MaterialType;
			public TI.Real ShieldLeakPercentage;
			public TI.Real ShieldDamagedMultiplier;
			public TI.Real BodyDamagedMultiplier;
		};
		#endregion

		#region damage_regions_block
		[TI.Definition(-1, 84)]
		public partial class damage_regions_block : TI.Definition
		{
			#region damage_permutations_block
			[TI.Definition(-1, 32)]
			public partial class damage_permutations_block : TI.Definition
			{
				public TI.String Name;
			};
			#endregion

			public TI.String Name;
			public TI.Flags Flags;
			public TI.Real DamageThreshold;
			public TI.TagReference DestroyedEffect;
			public TI.Block<damage_permutations_block> Permutations;
		};
		#endregion

		// unused
		#region damage_modifiers_block
		[TI.Definition(-1, 52)]
		public partial class damage_modifiers_block : TI.Definition
		{
			public damage_modifiers_block() : base(1)
			{
				Add(new TI.Skip(52));
			}
		};
		#endregion

		#region pathfinding_spheres_block
		[TI.Definition(-1, 32)]
		public partial class pathfinding_spheres_block : TI.Definition
		{
			public TI.BlockIndex Node;
			public TI.RealPoint3D Center;
			public TI.Real Radius;
		};
		#endregion

		#region collision_nodes_block
		[TI.Definition(-1, 64)]
		public partial class collision_nodes_block : TI.Definition
		{
			public TI.String Name;
			public TI.BlockIndex Region;
			public TI.BlockIndex ParentNode;
			public TI.BlockIndex NextSiblingNode;
			public TI.BlockIndex FirstChildNode;
			public TI.Block<structure_bsp_group.collision_bsp_block> Bsp;
		};
		#endregion

		public TI.Flags Flags;
		public TI.BlockIndex IndirectDamageMaterial;
		public TI.Real MaximumBodyVitality;
		public TI.Real BodySystemShock;
		public TI.Real FriendlyDamageResistance;

		public TI.TagReference LocalizedDamageEffect;

		public TI.Real AreaDamageEffectThreshold;
		public TI.TagReference AreaDamageEffect;

		public TI.Real BodyDamagedThreshold;
		public TI.TagReference BodyDamagedEffect;
		public TI.TagReference BodyDepletedEffect;
		public TI.Real BodyDestroyedThreshold;
		public TI.TagReference BodyDestroyedEffect;

		public TI.Real MaximumShieldVitality;
		public TI.Enum ShieldMaterialType;
		public TI.Enum ShieldFailureFunction;
		public TI.Real ShieldFailureThreshold;
		public TI.Real FailingShieldLeakFraction;
		public TI.Real MinimumStunDamage;
		public TI.Real StunTime;
		public TI.Real RechargeTime;
		public TI.Real ShieldDamagedThreshold;
		public TI.TagReference ShieldDamagedEffect;
		public TI.TagReference ShieldDepletedEffect;
		public TI.TagReference ShieldRechargingEffect;

		public TI.Block<damage_materials_block> Materials;
		public TI.Block<damage_regions_block> Regions;
		//public TI.Block<damage_modifiers_block> Modifiers;

		public TI.RealBounds X;
		public TI.RealBounds Y;
		public TI.RealBounds Z;
		public TI.Block<pathfinding_spheres_block> PathfindingSpheres;
		public TI.Block<collision_nodes_block> Nodes;
	};
	#endregion

	#region model_animation
	[TI.TagGroup((int)TagGroups.Enumerated.antr, 4, 128)]
	public partial class model_animation_group : TI.Definition
	{
		#region animation_indexer_block
		[TI.Definition(-1, 2)]
		public partial class animation_indexer_block : TI.Definition
		{
			public TI.BlockIndex Animation;
		};
		#endregion

		#region animation_graph_object_overlay
		[TI.Definition(-1, 20)]
		public partial class animation_graph_object_overlay : TI.Definition
		{
			public TI.BlockIndex Animation;
			public TI.Enum Function;
			public TI.Enum FunctionControls;
		};
		#endregion

		#region animation_graph_unit_seat_block
		[TI.Definition(-1, 100)]
		public partial class animation_graph_unit_seat_block : TI.Definition
		{
			#region animation_graph_unit_seat_ik_point
			[TI.Definition(-1, 64)]
			public partial class animation_graph_unit_seat_ik_point : TI.Definition
			{
				public TI.String Marker;
				public TI.String AttachToMarker;
			};
			#endregion

			#region animation_graph_weapon_block
			[TI.Definition(-1, 188)]
			public partial class animation_graph_weapon_block : TI.Definition
			{
				#region animation_graph_weapon_type_block
				[TI.Definition(-1, 60)]
				public partial class animation_graph_weapon_type_block : TI.Definition
				{
					public TI.String Label;
					public TI.Block<animation_indexer_block> Animations;
				};
				#endregion

				public TI.String Name;
				public TI.String GripMarker;
				public TI.String HandMarker;
				public TI.Real RightYawPerFrame;
				public TI.Real LeftYawPerFrame;
				public TI.ShortInteger RightFrameCount;
				public TI.ShortInteger LeftFrameCount;
				public TI.Real DownYawPerFrame;
				public TI.Real UpYawPerFrame;
				public TI.ShortInteger DownFrameCount;
				public TI.ShortInteger UpFrameCount;
				public TI.Block<animation_indexer_block> Animations;
				public TI.Block<animation_graph_unit_seat_ik_point> IKPoints;
				public TI.Block<animation_graph_weapon_type_block> WeaponsTypes;
			};
			#endregion

			public TI.String Name;
			public TI.Real RightYawPerFrame;
			public TI.Real LeftYawPerFrame;
			public TI.ShortInteger RightFrameCount;
			public TI.ShortInteger LeftFrameCount;
			public TI.Real DownYawPerFrame;
			public TI.Real UpYawPerFrame;
			public TI.ShortInteger DownFrameCount;
			public TI.ShortInteger UpFrameCount;
			public TI.Block<animation_indexer_block> Animations;
			public TI.Block<animation_graph_unit_seat_ik_point> IKPoints;
			public TI.Block<animation_graph_weapon_block> WeaponsTypes;
		};
		#endregion

		#region animation_graph_weapon_animations_block
		[TI.Definition(-1, 28)]
		public partial class animation_graph_weapon_animations_block : TI.Definition
		{
			public TI.Block<animation_indexer_block> Animations;
		};
		#endregion

		#region animation_graph_vehicle_animations_block
		[TI.Definition(-1, 116)]
		public partial class animation_graph_vehicle_animations_block : TI.Definition
		{
			#region suspension_animation_block
			[TI.Definition(-1, 20)]
			public partial class suspension_animation_block : TI.Definition
			{
				public TI.ShortInteger MassPointIndex;
				public TI.BlockIndex Animation;
				public TI.Real FullExtensionGroundDepth;
				public TI.Real FullCompressionGroundDepth;
			};
			#endregion

			public TI.String Name;
			public TI.Real RightYawPerFrame;
			public TI.Real LeftYawPerFrame;
			public TI.ShortInteger RightFrameCount;
			public TI.ShortInteger LeftFrameCount;
			public TI.Real DownYawPerFrame;
			public TI.Real UpYawPerFrame;
			public TI.ShortInteger DownFrameCount;
			public TI.ShortInteger UpFrameCount;
			public TI.Block<animation_indexer_block> Animations;
			public TI.Block<suspension_animation_block> SuspensionAnimations;
		};
		#endregion

		#region device_animations
		[TI.Definition(-1, 96)]
		public partial class device_animations : TI.Definition
		{
			public TI.Block<animation_indexer_block> Animations;
		};
		#endregion

		#region animation_graph_first_person_weapon_animations_block
		[TI.Definition(-1, 28)]
		public partial class animation_graph_first_person_weapon_animations_block : TI.Definition
		{
			public TI.Block<animation_indexer_block> Animations;
		};
		#endregion

		#region animation_graph_sound_reference_block
		[TI.Definition(-1, 20)]
		public partial class animation_graph_sound_reference_block : TI.Definition
		{
			public TI.TagReference Sound;
		};
		#endregion

		#region animation_graph_node_block
		[TI.Definition(-1, 64)]
		public partial class animation_graph_node_block : TI.Definition
		{
			public TI.String Name;
			public TI.BlockIndex NextSiblingNodeIndex;
			public TI.BlockIndex FirstChildNodeIndex;
			public TI.BlockIndex ParentNodeIndex;
			public TI.Flags NodeJointFlags;
			public TI.RealVector3D BaseVector;
			public TI.Real VectorRange;
		};
		#endregion

		#region animation_block
		[TI.Definition(-1, 180)]
		public partial class animation_block : TI.Definition
		{
			public TI.String Name;
			public TI.Enum Type;
			public TI.ShortInteger FrameCount;
			public TI.ShortInteger FrameSize;
			public TI.Enum FrameInfoType;
			public TI.LongInteger NodeListChecksum;
			public TI.ShortInteger NodeCount;
			public TI.ShortInteger LoopFrameIndex;
			public TI.Real Weight;
			public TI.ShortInteger KeyFrameIndex;
			public TI.ShortInteger SecondKeyFrameIndex;
			public TI.BlockIndex NextAnimation;
			public TI.Flags Flags;
			public TI.BlockIndex Sound;
			public TI.ShortInteger SoundFrameIndex;
			public TI.ByteInteger LeftFootFrameIndex;
			public TI.ByteInteger RightFootFrameIndex;
			public TI.Data FrameInfo;

			public TI.LongInteger NodeTransFlags1;
			public TI.LongInteger NodeTransFlags2;

			public TI.LongInteger NodeRotationFlags1;
			public TI.LongInteger NodeRotationFlags2;

			public TI.LongInteger NodeScaleFlags1;
			public TI.LongInteger NodeScaleFlags2;

			public TI.LongInteger OffsetToCompressedData;
			public TI.Data DefaultData;
			public TI.Data FrameData;
		};
		#endregion

		public TI.Block<animation_graph_object_overlay> Objects;
		public TI.Block<animation_graph_unit_seat_block> Units;
		public TI.Block<animation_graph_weapon_animations_block> Weapons;
		public TI.Block<animation_graph_vehicle_animations_block> Vehicles;
		public TI.Block<device_animations> Devices;
		public TI.Block<animation_indexer_block> UnitDamage;
		public TI.Block<animation_graph_first_person_weapon_animations_block> FirstPersonWeapons;
		public TI.Block<animation_graph_sound_reference_block> SoundReferences;
		public TI.Real LimpBodyNodeRadius;
		public TI.Flags Flags;
		public TI.Block<animation_graph_node_block> Nodes;
		public TI.Block<animation_block> Animations;
	};
	#endregion
}