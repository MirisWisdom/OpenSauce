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
	public partial class model_group
	{
		#region model_markers_block
		public partial class model_markers_block
		{
			#region model_marker_instance_block
			public partial class model_marker_instance_block
			{
				public model_marker_instance_block() : base(6)
				{
					Add(RegionIndex = new TI.BlockIndex(BlamLib.TagInterface.FieldType.ByteBlockIndex));
					Add(PermutationIndex = new TI.BlockIndex(BlamLib.TagInterface.FieldType.ByteBlockIndex));
					Add(NodeIndex = new TI.BlockIndex(BlamLib.TagInterface.FieldType.ByteBlockIndex));
					Add(new TI.Pad(1));
					Add(Translation = new TI.RealPoint3D());
					Add(Rotation = new TI.RealQuaternion());
				}
			};
			#endregion

			public model_markers_block() : base(4)
			{
				Add(Name = new TI.String());
				Add(MagicIdentifer = new TI.ShortInteger());
				Add(new TI.Pad(2 + 16));
				Add(Instances = new TI.Block<model_marker_instance_block>(this, 32));
			}
		};
		#endregion

		#region model_node_block
		public partial class model_node_block
		{
			public model_node_block() : base(9)
			{
				Add(Name = new TI.String());
				Add(NextSiblingNode = new TI.BlockIndex());
				Add(FirstChildNode = new TI.BlockIndex());
				Add(ParentNode = new TI.BlockIndex());
				Add(new TI.Pad(2));
				Add(DefaultTranslation = new TI.RealPoint3D());
				Add(DefaultRotation = new TI.RealQuaternion());
				Add(NodeDistFromParent = new TI.Real());
				Add(new TI.Pad(32 + 52));
			}
		};
		#endregion

		#region model_region_block
		public partial class model_region_block
		{
			#region model_region_permutation_block
			public partial class model_region_permutation_block
			{
				#region model_region_permutation_marker_block
				public partial class model_region_permutation_marker_block
				{
					public model_region_permutation_marker_block() : base(6)
					{
						Add(Name = new TI.String());
						Add(NodeIndex = new TI.BlockIndex());
						Add(new TI.Pad(2));
						Add(Translation = new TI.RealPoint3D());
						Add(Rotation = new TI.RealQuaternion());
						Add(new TI.Pad(16));
					}
				};
				#endregion

				public model_region_permutation_block() : base(10)
				{
					Add(Name = new TI.String());
					Add(Flags = new TI.Flags());
					Add(new TI.Pad(28)); // TODO: first 16-bits is the variant id
					Add(SuperLow = new TI.BlockIndex());
					Add(Low = new TI.BlockIndex());
					Add(Medium = new TI.BlockIndex());
					Add(High = new TI.BlockIndex());
					Add(SuperHigh = new TI.BlockIndex());
					Add(new TI.Pad(2));
					Add(Markers = new TI.Block<model_region_permutation_marker_block>(this, 64));
				}
			};
			#endregion

			public model_region_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(new TI.Pad(32));
				Add(Permutations = new TI.Block<model_region_permutation_block>(this, 32));
			}
		};
		#endregion

		#region model_geometry_block
		public partial class model_geometry_block
		{
			#region model_geometry_part_block
			public partial class model_geometry_part_block
			{
				#region model_vertex_uncompressed_block
				public partial class model_vertex_uncompressed_block
				{
					public model_vertex_uncompressed_block() : base(9)
					{
						Add(Position = new TI.RealPoint3D());
						Add(Normal = new TI.RealVector3D());
						Add(Binormal = new TI.RealVector3D());
						Add(Tangent = new TI.RealVector3D());
						Add(TextureCoords = new TI.RealPoint2D());
						Add(NodeIndex1 = new TI.ShortInteger());
						Add(NodeIndex2 = new TI.ShortInteger());
						Add(NodeWeight1 = new TI.Real());
						Add(NodeWeight2 = new TI.Real());
					}
				};
				#endregion

				#region model_vertex_compressed_block
				public partial class model_vertex_compressed_block
				{
					public model_vertex_compressed_block() : base(8)
					{
						Add(Position = new TI.RealPoint3D());
						Add(Normal = new TI.LongInteger());
						Add(Binormal = new TI.LongInteger());
						Add(Tangent = new TI.LongInteger());
						Add(TextureCoords = new TI.Point2D());
						Add(NodeIndex1 = new TI.ByteInteger());
						Add(NodeIndex2 = new TI.ByteInteger());
						Add(NodeWeight0 = new TI.ShortInteger());
					}
				};
				#endregion

				#region model_triangle_block
				public partial class model_triangle_block
				{
					public model_triangle_block() : base(3)
					{
						Add(VertexIndex0 = new TI.ShortInteger());
						Add(VertexIndex1 = new TI.ShortInteger());
						Add(VertexIndex2 = new TI.ShortInteger());
					}
				};
				#endregion

				public model_geometry_part_block() : base(13)
				{
					Add(Flags = new TI.Flags());
					Add(ShaderIndex = new TI.BlockIndex());
					Add(PrevFilthyPartIndex = new TI.ByteInteger());
					Add(NextFilthyPartIndex = new TI.ByteInteger());
					Add(CentroidPrimaryNode = new TI.ShortInteger());
					Add(CentroidSecondaryNode = new TI.ShortInteger());
					Add(CentroidPrimaryWeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(CentroidSecondaryWeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(Centroid = new TI.RealPoint3D());
					Add(UncompressedVertices = new TI.Block<model_vertex_uncompressed_block>(this, 65535));
					Add(CompressedVertices = new TI.Block<model_vertex_compressed_block>(this, 65535));
					Add(Triangles = new TI.Block<model_triangle_block>(this, 65535));
					Add(new TI.Pad(kSizeOf_rasterizer_triangle_buffer + kSizeOf_rasterizer_vertex_buffer));
				}
			};
			#endregion

			public model_geometry_block() : base(3)
			{
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(32));
				Add(Parts = new TI.Block<model_geometry_part_block>(this, 32));
			}
		};
		#endregion

		#region model_shader_reference_block
		public partial class model_shader_reference_block
		{
			public model_shader_reference_block() : base(3)
			{
				Add(Shader = new TI.TagReference(this, TagGroups.shdr));
				Add(Permutation = new TI.ShortInteger());
				Add(new TI.Pad(2 + 12));
			}
		};
		#endregion

		public model_group() : base(21)
		{
			Add(Flags = new TI.Flags());
			Add(NodeListChecksum = new TI.LongInteger());

			Add(SuperHighDetailCutoff = new TI.Real());
			Add(HighDetailCutoff = new TI.Real());
			Add(MediumDetailCutoff = new TI.Real());
			Add(LowDetailCutoff = new TI.Real());
			Add(SuperLowDetailCutoff = new TI.Real());

			Add(SuperHighDetailNodeCount = new TI.ShortInteger());
			Add(HighDetailNodeCount = new TI.ShortInteger());
			Add(MediumDetailNodeCount = new TI.ShortInteger());
			Add(LowDetailNodeCount = new TI.ShortInteger());
			Add(SuperLowDetailNodeCount = new TI.ShortInteger());

			Add(new TI.Pad(2 + 8));
			Add(BaseMapUScale = new TI.Real());
			Add(BaseMapVScale = new TI.Real());
			Add(new TI.Pad(116));

			Add(Markers = new TI.Block<model_markers_block>(this, 256));
			Add(Nodes = new TI.Block<model_node_block>(this, 64));
			Add(Regions = new TI.Block<model_region_block>(this, 32));
			Add(Geometries = new TI.Block<model_geometry_block>(this, 256));
			Add(Shaders = new TI.Block<model_shader_reference_block>(this, 32));
		}
	};
	#endregion

	#region gbxmodel
	public partial class gbxmodel_group
	{
		#region model_geometry_block
		public partial class model_geometry_block
		{
			#region model_geometry_part_block
			public partial class model_geometry_part_block
			{
				public model_geometry_part_block() : base(37)
				{
					Add(Flags = new TI.Flags());
					Add(ShaderIndex = new TI.BlockIndex());
					Add(PrevFilthyPartIndex = new TI.ByteInteger());
					Add(NextFilthyPartIndex = new TI.ByteInteger());
					Add(CentroidPrimaryNode = new TI.ShortInteger());
					Add(CentroidSecondaryNode = new TI.ShortInteger());
					Add(CentroidPrimaryWeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(CentroidSecondaryWeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
					Add(Centroid = new TI.RealPoint3D());
					Add(UncompressedVertices = new TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_vertex_uncompressed_block>(this, 65535));
					Add(CompressedVertices = new TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_vertex_compressed_block>(this, 65535));
					Add(Triangles = new TI.Block<model_group.model_geometry_block.model_geometry_part_block.model_triangle_block>(this, 65535));
					Add(new TI.Skip(model_group.kSizeOf_rasterizer_triangle_buffer + model_group.kSizeOf_rasterizer_vertex_buffer
						+ 1 + 1 + 1));
					Add(NodeMapCount = new BlamLib.TagInterface.ByteInteger());
					for (int x = 0; x < NodeMap.Length; x++)
						Add(NodeMap[x] = new TI.ByteInteger());
					Add(TI.Pad.Word);
				}
			};
			#endregion

			public model_geometry_block() : base(3)
			{
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(32));
				Add(Parts = new TI.Block<model_geometry_part_block>(this, 32));
			}
		};
		#endregion

		public gbxmodel_group() : base(21)
		{
			Add(Flags = new TI.Flags());
			Add(NodeListChecksum = new TI.LongInteger());

			Add(SuperHighDetailCutoff = new TI.Real());
			Add(HighDetailCutoff = new TI.Real());
			Add(MediumDetailCutoff = new TI.Real());
			Add(LowDetailCutoff = new TI.Real());
			Add(SuperLowDetailCutoff = new TI.Real());

			Add(SuperHighDetailNodeCount = new TI.ShortInteger());
			Add(HighDetailNodeCount = new TI.ShortInteger());
			Add(MediumDetailNodeCount = new TI.ShortInteger());
			Add(LowDetailNodeCount = new TI.ShortInteger());
			Add(SuperLowDetailNodeCount = new TI.ShortInteger());

			Add(new TI.Pad(2 + 8));
			Add(BaseMapUScale = new TI.Real());
			Add(BaseMapVScale = new TI.Real());
			Add(new TI.Pad(116));

			Add(Markers = new TI.Block<model_group.model_markers_block>(this, 256));
			Add(Nodes = new TI.Block<model_group.model_node_block>(this, 64));
			Add(Regions = new TI.Block<model_group.model_region_block>(this, 32));
			Add(Geometries = new TI.Block<model_geometry_block>(this, 256));
			Add(Shaders = new TI.Block<model_group.model_shader_reference_block>(this, 32));
		}
	};
	#endregion

	#region model_collision
	public partial class model_collision_group
	{
		#region damage_materials_block
		public partial class damage_materials_block
		{
			public damage_materials_block() : base(9)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(MaterialType = new TI.Enum());
				Add(new TI.Pad(2));
				Add(ShieldLeakPercentage = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(ShieldDamagedMultiplier = new TI.Real());
				Add(new TI.Pad(12));
				Add(BodyDamagedMultiplier = new TI.Real());
				Add(new TI.Pad(8));
			}
		};
		#endregion

		#region damage_regions_block
		public partial class damage_regions_block
		{
			#region damage_permutations_block
			public partial class damage_permutations_block
			{
				public damage_permutations_block() : base(1)
				{
					Add(Name = new TI.String());
				}
			};
			#endregion

			public damage_regions_block() : base(7)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(4));
				Add(DamageThreshold = new TI.Real());
				Add(new TI.Pad(12));
				Add(DestroyedEffect = new TI.TagReference(this, TagGroups.effe));
				Add(Permutations = new TI.Block<damage_permutations_block>(this, 32));
			}
		};
		#endregion

		#region pathfinding_spheres_block
		public partial class pathfinding_spheres_block
		{
			public pathfinding_spheres_block() : base(4)
			{
				Add(Node = new TI.BlockIndex());
				Add(new TI.Pad(2 + 12));
				Add(Center = new TI.RealPoint3D());
				Add(Radius = new TI.Real());
			}
		};
		#endregion

		#region collision_nodes_block
		public partial class collision_nodes_block
		{
			public collision_nodes_block() : base(7)
			{
				Add(Name = new TI.String());
				Add(Region = new TI.BlockIndex());
				Add(ParentNode = new TI.BlockIndex());
				Add(NextSiblingNode = new TI.BlockIndex());
				Add(FirstChildNode = new TI.BlockIndex());
				Add(new TI.Pad(12));
				Add(Bsp = new TI.Block<structure_bsp_group.collision_bsp_block>(this, 32));
			}
		};
		#endregion

		public model_collision_group() : base(42)
		{
			Add(Flags = new TI.Flags());
			Add(IndirectDamageMaterial = new TI.BlockIndex());
			Add(new TI.Pad(2));
			Add(MaximumBodyVitality = new TI.Real());
			Add(BodySystemShock = new TI.Real());
			Add(new TI.Pad(24 + 28));
			Add(FriendlyDamageResistance = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(8 + 32));
			Add(LocalizedDamageEffect = new TI.TagReference(this, TagGroups.effe));
			Add(AreaDamageEffectThreshold = new TI.Real());
			Add(AreaDamageEffect = new TI.TagReference(this, TagGroups.effe));
			Add(BodyDamagedThreshold = new TI.Real());
			Add(BodyDamagedEffect = new TI.TagReference(this, TagGroups.effe));
			Add(BodyDepletedEffect = new TI.TagReference(this, TagGroups.effe));
			Add(BodyDestroyedThreshold = new TI.Real());
			Add(BodyDestroyedEffect = new TI.TagReference(this, TagGroups.effe));
			Add(MaximumShieldVitality = new TI.Real());
			Add(new TI.Pad(2));
			Add(ShieldMaterialType = new TI.Enum());
			Add(new TI.Pad(24));
			Add(ShieldFailureFunction = new TI.Enum());
			Add(new TI.Pad(2));
			Add(ShieldFailureThreshold = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(FailingShieldLeakFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(16));
			Add(MinimumStunDamage = new TI.Real());
			Add(StunTime = new TI.Real());
			Add(RechargeTime = new TI.Real());
			Add(new TI.Pad(16 + 96));
			Add(ShieldDamagedThreshold = new TI.Real());
			Add(ShieldDamagedEffect = new TI.TagReference(this, TagGroups.effe));
			Add(ShieldDepletedEffect = new TI.TagReference(this, TagGroups.effe));
			Add(ShieldRechargingEffect = new TI.TagReference(this, TagGroups.effe));
			Add(new TI.Pad(12 + 112));
			Add(Materials = new TI.Block<damage_materials_block>(this, 32));
			Add(Regions = new TI.Block<damage_regions_block>(this, 8));
			Add(new TI.Pad(12 + 16)); // Modifiers isn't used
			Add(X = new TI.RealBounds());
			Add(Y = new TI.RealBounds());
			Add(Z = new TI.RealBounds());
			Add(PathfindingSpheres = new TI.Block<pathfinding_spheres_block>(this, 32));
			Add(Nodes = new TI.Block<collision_nodes_block>(this, 64));
		}
	};
	#endregion

	#region model_animation
	public partial class model_animation_group
	{
		#region animation_indexer_block
		public partial class animation_indexer_block
		{
			public animation_indexer_block() : base(1)
			{
				Add(Animation = new TI.BlockIndex());
			}
		};
		#endregion

		#region animation_graph_object_overlay
		public partial class animation_graph_object_overlay
		{
			public animation_graph_object_overlay() : base(4)
			{
				Add(Animation = new TI.BlockIndex());
				Add(Function = new TI.Enum());
				Add(FunctionControls = new TI.Enum());
				Add(new TI.Pad(2 + 12));
			}
		};
		#endregion

		#region animation_graph_unit_seat_block
		public partial class animation_graph_unit_seat_block
		{
			#region animation_graph_unit_seat_ik_point
			public partial class animation_graph_unit_seat_ik_point
			{
				public animation_graph_unit_seat_ik_point() : base(2)
				{
					Add(Marker = new TI.String());
					Add(AttachToMarker = new TI.String());
				}
			};
			#endregion

			#region animation_graph_weapon_block
			public partial class animation_graph_weapon_block
			{
				#region animation_graph_weapon_type_block
				public partial class animation_graph_weapon_type_block
				{
					public animation_graph_weapon_type_block() : base(3)
					{
						Add(Label = new TI.String());
						Add(new TI.Pad(16));
						Add(Animations = new TI.Block<animation_indexer_block>(this, 10)); // weapon_type_animation_block
					}
				};
				#endregion

				public animation_graph_weapon_block() : base(15)
				{
					Add(Name = new TI.String());
					Add(GripMarker = new TI.String());
					Add(HandMarker = new TI.String());
					Add(RightYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
					Add(LeftYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
					Add(RightFrameCount = new TI.ShortInteger());
					Add(LeftFrameCount = new TI.ShortInteger());
					Add(DownYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
					Add(UpYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
					Add(DownFrameCount = new TI.ShortInteger());
					Add(UpFrameCount = new TI.ShortInteger());
					Add(new TI.Pad(32));
					Add(Animations = new TI.Block<animation_indexer_block>(this, 55)); // weapon_class_animation_block
					Add(IKPoints = new TI.Block<animation_graph_unit_seat_ik_point>(this, 4));
					Add(WeaponsTypes = new TI.Block<animation_graph_weapon_type_block>(this, 16));
				}
			};
			#endregion

			public animation_graph_unit_seat_block() : base(13)
			{
				Add(Name = new TI.String());
				Add(RightYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(LeftYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(RightFrameCount = new TI.ShortInteger());
				Add(LeftFrameCount = new TI.ShortInteger());
				Add(DownYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(UpYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(DownFrameCount = new TI.ShortInteger());
				Add(UpFrameCount = new TI.ShortInteger());
				Add(new TI.Pad(8));
				Add(Animations = new TI.Block<animation_indexer_block>(this, 30)); // unit_seat_animation_block
				Add(IKPoints = new TI.Block<animation_graph_unit_seat_ik_point>(this, 4));
				Add(WeaponsTypes = new TI.Block<animation_graph_weapon_block>(this, 16));
			}
		};
		#endregion

		#region animation_graph_weapon_animations_block
		public partial class animation_graph_weapon_animations_block
		{
			public animation_graph_weapon_animations_block() : base(2)
			{
				Add(new TI.Pad(16));
				Add(Animations = new TI.Block<animation_indexer_block>(this, 11)); // weapon_animation_block
			}
		};
		#endregion

		#region animation_graph_vehicle_animations_block
		public partial class animation_graph_vehicle_animations_block
		{
			#region suspension_animation_block
			public partial class suspension_animation_block
			{
				public suspension_animation_block() : base(5)
				{
					Add(MassPointIndex = new TI.ShortInteger());
					Add(Animation = new TI.BlockIndex());
					Add(FullExtensionGroundDepth = new TI.Real());
					Add(FullCompressionGroundDepth = new TI.Real());
					Add(new TI.Pad(8));
				}
			};
			#endregion

			public animation_graph_vehicle_animations_block() : base(11)
			{
				Add(RightYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(LeftYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(RightFrameCount = new TI.ShortInteger());
				Add(LeftFrameCount = new TI.ShortInteger());
				Add(DownYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(UpYawPerFrame = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(DownFrameCount = new TI.ShortInteger());
				Add(UpFrameCount = new TI.ShortInteger());
				Add(new TI.Pad(68));
				Add(Animations = new TI.Block<animation_indexer_block>(this, 8)); // vehicle_animation_block
				Add(SuspensionAnimations = new TI.Block<suspension_animation_block>(this, 8));
			}
		};
		#endregion

		#region device_animations
		public partial class device_animations
		{
			public device_animations() : base(2)
			{
				Add(new TI.Pad(84));
				Add(Animations = new TI.Block<animation_indexer_block>(this, 2)); // device_animation_block
			}
		};
		#endregion

		#region animation_graph_first_person_weapon_animations_block
		public partial class animation_graph_first_person_weapon_animations_block
		{
			public animation_graph_first_person_weapon_animations_block() : base(2)
			{
				Add(new TI.Pad(16));
				Add(Animations = new TI.Block<animation_indexer_block>(this, 28)); // first_person_weapon_block
			}
		};
		#endregion

		#region animation_graph_sound_reference_block
		public partial class animation_graph_sound_reference_block
		{
			public animation_graph_sound_reference_block() : base(2)
			{
				Add(Sound = new TI.TagReference(this, TagGroups.snd_));
				Add(new TI.Pad(4));
			}
		};
		#endregion

		#region animation_graph_node_block
		public partial class animation_graph_node_block
		{
			public animation_graph_node_block() : base(9)
			{
				Add(Name = new TI.String());
				Add(NextSiblingNodeIndex = new TI.BlockIndex());
				Add(FirstChildNodeIndex = new TI.BlockIndex());
				Add(ParentNodeIndex = new TI.BlockIndex());
				Add(new TI.Pad(2));
				Add(NodeJointFlags = new TI.Flags());
				Add(BaseVector = new TI.RealVector3D());
				Add(VectorRange = new TI.Real());
				Add(new TI.Pad(4));
			}
		};
		#endregion

		#region animation_block
		public partial class animation_block
		{
			public animation_block() : base(31)
			{
				Add(Name = new TI.String());
				Add(Type = new TI.Enum());
				Add(FrameCount = new TI.ShortInteger());
				Add(FrameSize = new TI.ShortInteger());
				Add(FrameInfoType = new TI.Enum());
				Add(NodeListChecksum = new TI.LongInteger());
				Add(NodeCount = new TI.ShortInteger());
				Add(LoopFrameIndex = new TI.ShortInteger());
				Add(Weight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(KeyFrameIndex = new TI.ShortInteger());
				Add(SecondKeyFrameIndex = new TI.ShortInteger());
				Add(NextAnimation = new TI.BlockIndex());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(Sound = new TI.BlockIndex());
				Add(SoundFrameIndex = new TI.ShortInteger());
				Add(LeftFootFrameIndex = new TI.ByteInteger());
				Add(RightFootFrameIndex = new TI.ByteInteger());
				Add(new TI.Pad(2 + 4));
				Add(FrameInfo = new TI.Data(this));
				Add(NodeTransFlags1 = new TI.LongInteger());
				Add(NodeTransFlags2 = new TI.LongInteger());
				Add(new TI.Pad(8));
				Add(NodeRotationFlags1 = new TI.LongInteger());
				Add(NodeRotationFlags2 = new TI.LongInteger());
				Add(new TI.Pad(8));
				Add(NodeScaleFlags1 = new TI.LongInteger());
				Add(NodeScaleFlags2 = new TI.LongInteger());
				Add(new TI.Pad(4));
				Add(OffsetToCompressedData = new TI.LongInteger());
				Add(DefaultData = new TI.Data(this));
				Add(FrameData = new TI.Data(this));
			}
		};
		#endregion

		public model_animation_group() : base(13)
		{
			Add(Objects = new TI.Block<animation_graph_object_overlay>(this, 4));
			Add(Units = new TI.Block<animation_graph_unit_seat_block>(this, 32));
			Add(Weapons = new TI.Block<animation_graph_weapon_animations_block>(this, 1));
			Add(Vehicles = new TI.Block<animation_graph_vehicle_animations_block>(this, 1));
			Add(Devices = new TI.Block<device_animations>(this, 1));
			Add(UnitDamage = new TI.Block<animation_indexer_block>(this, 176)); // unit_damage_animations
			Add(FirstPersonWeapons = new TI.Block<animation_graph_first_person_weapon_animations_block>(this, 1));
			Add(SoundReferences = new TI.Block<animation_graph_sound_reference_block>(this, 257));
			Add(LimpBodyNodeRadius = new TI.Real());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(Nodes = new TI.Block<animation_graph_node_block>(this, 64));
			Add(Animations = new TI.Block<animation_block>(this, 256));
		}
	};
	#endregion
}