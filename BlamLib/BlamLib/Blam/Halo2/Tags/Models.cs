/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region model collision
	#region bsp3d_nodes_block
	[TI.Definition(1, 8)]
	public class bsp3d_nodes_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger Plane;
		public TI.ShortInteger BackChild;
		public TI.ShortInteger FrontChild;
		#endregion

		#region Ctor
		public bsp3d_nodes_block() : base(4)
		{
			Add(Plane = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(BackChild = new TI.ShortInteger());
			Add(FrontChild = new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region planes_block
	[TI.Definition(1, 16)]
	public class planes_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public planes_block() : base(1)
		{
			Add(/*Plane = */ new TI.RealPlane3D());
		}
		#endregion
	}
	#endregion

	#region leaves_block
	[TI.Definition(1, 4)]
	public class leaves_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public leaves_block() : base(3)
		{
			Add(/*Flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*BSP 2D Reference Count = */ new TI.ByteInteger());
			Add(/*First BSP 2D Reference = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region bsp2d_references_block
	[TI.Definition(1, 4)]
	public class bsp2d_references_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public bsp2d_references_block() : base(2)
		{
			Add(/*Plane = */ new TI.ShortInteger());
			Add(/*BSP 2D Node = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region bsp2d_nodes_block
	[TI.Definition(1, 16)]
	public class bsp2d_nodes_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public bsp2d_nodes_block() : base(3)
		{
			Add(/*Plane = */ new TI.RealPlane2D());
			Add(/*Left Child = */ new TI.ShortInteger());
			Add(/*Right Child = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region surfaces_block
	[TI.Definition(1, 8)]
	public class surfaces_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public surfaces_block() : base(5)
		{
			Add(/*Plane = */ new TI.ShortInteger());
			Add(/*First Edge = */ new TI.ShortInteger());
			Add(/*Flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*Breakable Surface = */ new TI.ByteInteger());
			Add(/*Material = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region edges_block
	[TI.Definition(2, 12)]
	public class edges_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public edges_block() : base(6)
		{
			Add(/*Start Vertex = */ new TI.ShortInteger());
			Add(/*End Vertex = */ new TI.ShortInteger());
			Add(/*Forward Edge = */ new TI.ShortInteger());
			Add(/*Reverse Edge = */ new TI.ShortInteger());
			Add(/*Left Surface = */ new TI.ShortInteger());
			Add(/*Right Surface = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region vertices_block
	[TI.Definition(1, 16)]
	public class vertices_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public vertices_block() : base(3)
		{
			Add(/*Point = */ new TI.RealPoint3D());
			Add(/*First Edge = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region global_collision_bsp_struct
	[TI.Struct((int)StructGroups.Enumerated.cbsp, 3, 96)]
	public class global_collision_bsp_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_collision_bsp_struct() : base(8)
		{
			Add(/*BSP 3D Nodes = */ new TI.Block<bsp3d_nodes_block>(this, 131072));
			Add(/*Planes = */ new TI.Block<planes_block>(this, 65536));
			Add(/*Leaves = */ new TI.Block<leaves_block>(this, 65536));
			Add(/*BSP 2D References = */ new TI.Block<bsp2d_references_block>(this, 131072));
			Add(/*BSP 2D Nodes = */ new TI.Block<bsp2d_nodes_block>(this, 131072));
			Add(/*Surfaces = */ new TI.Block<surfaces_block>(this, 131072));
			Add(/*Edges = */ new TI.Block<edges_block>(this, 262144));
			Add(/*Vertices = */ new TI.Block<vertices_block>(this, 131072));
		}
		#endregion
	}
	#endregion

	#region global_collision_bsp_block
	[TI.Definition(3, 96)]
	public class global_collision_bsp_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_collision_bsp_block() : base(8)
		{
			Add(/*BSP 3D Nodes = */ new TI.Block<bsp3d_nodes_block>(this, 131072));
			Add(/*Planes = */ new TI.Block<planes_block>(this, 65536));
			Add(/*Leaves = */ new TI.Block<leaves_block>(this, 65536));
			Add(/*BSP 2D References = */ new TI.Block<bsp2d_references_block>(this, 131072));
			Add(/*BSP 2D Nodes = */ new TI.Block<bsp2d_nodes_block>(this, 131072));
			Add(/*Surfaces = */ new TI.Block<surfaces_block>(this, 131072));
			Add(/*Edges = */ new TI.Block<edges_block>(this, 262144));
			Add(/*Vertices = */ new TI.Block<vertices_block>(this, 131072));
		}
		#endregion
	}
	#endregion

	#region collision_bsp_physics_block
	[TI.Definition(1, 128)]
	public class collision_bsp_physics_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public collision_bsp_physics_block() : base(19)
		{
			Add(new TI.Skip(4));
			Add(/*Size = */ new TI.ShortInteger());
			Add(/*Count = */ new TI.ShortInteger());
			Add(new TI.Skip(4));
			Add(new TI.Pad(4));
			Add(new TI.Skip(32));
			Add(new TI.Pad(16));

			Add(new TI.Skip(4));
			Add(/*Size = */ new TI.ShortInteger());
			Add(/*Count = */ new TI.ShortInteger());
			Add(new TI.Skip(4));
			Add(new TI.Pad(4));

			Add(new TI.Skip(4));
			Add(/*Size = */ new TI.ShortInteger());
			Add(/*Count = */ new TI.ShortInteger());
			Add(new TI.Skip(4));
			Add(new TI.Pad(8));
			Add(/*mopp Code Data = */ new TI.Data(this));
			Add(new TI.Pad(8)); // algn
		}
		#endregion
	}
	#endregion

	#region collision_model
	[TI.TagGroup((int)TagGroups.Enumerated.coll, 10, 2, 76)]
	public partial class collision_model_group : TI.Definition, ITagImportInfo
	{
		#region collision_model_material_block
		[TI.Definition(1, 4)]
		public class collision_model_material_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public collision_model_material_block() : base(1)
			{
				Add(/*name = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region collision_model_region_block
		[TI.Definition(1, 16)]
		public class collision_model_region_block : TI.Definition
		{
			#region collision_model_permutation_block
			[TI.Definition(2, 28)]
			public class collision_model_permutation_block : TI.Definition
			{
				#region collision_model_bsp_block
				[TI.Definition(1, 100)]
				public class collision_model_bsp_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public collision_model_bsp_block() : base(4)
					{
						Add(/*node index = */ new TI.ShortInteger());
						Add(new TI.Pad(2));
						Add(new TI.UselessPad(16));
						Add(/*bsp = */ new TI.Struct<global_collision_bsp_struct>(this));
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public collision_model_permutation_block() : base(3)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*bsps = */ new TI.Block<collision_model_bsp_block>(this, 64));
					Add(/*bsp_physics = */ new TI.Block<collision_bsp_physics_block>(this, 1024));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public collision_model_region_block() : base(2)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*permutations = */ new TI.Block<collision_model_permutation_block>(this, 32));
			}
			#endregion
		}
		#endregion

		#region collision_model_pathfinding_sphere_block
		[TI.Definition(1, 20)]
		public class collision_model_pathfinding_sphere_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public collision_model_pathfinding_sphere_block() : base(5)
			{
				Add(/*node = */ new TI.BlockIndex()); // 1 collision_model_node_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.UselessPad(12));
				Add(/*center = */ new TI.RealPoint3D());
				Add(/*radius = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region collision_model_node_block
		[TI.Definition(2, 12)]
		public class collision_model_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public collision_model_node_block() : base(5)
			{
				Add(/*name = */ new TI.StringId());
				Add(new TI.Pad(2));
				Add(/*parent node = */ new TI.BlockIndex()); // 1 collision_model_node_block
				Add(/*next sibling node = */ new TI.BlockIndex()); // 1 collision_model_node_block
				Add(/*first child node = */ new TI.BlockIndex()); // 1 collision_model_node_block
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<global_tag_import_info_block> ImportInfo;
		public TI.Block<global_error_report_categories_block> Errors;
		#endregion

		#region Ctor
		public collision_model_group() : base(8)
		{
			Add(ImportInfo = new TI.Block<global_tag_import_info_block>(this, 1));
			Add(Errors = new TI.Block<global_error_report_categories_block>(this, 64));
			Add(/*flags = */ TI.Flags.Long);
			Add(new TI.UselessPad(124));
			Add(/*materials = */ new TI.Block<collision_model_material_block>(this, 32));
			Add(/*regions = */ new TI.Block<collision_model_region_block>(this, 16));
			Add(/*pathfinding spheres = */ new TI.Block<collision_model_pathfinding_sphere_block>(this, 32));
			Add(/*nodes = */ new TI.Block<collision_model_node_block>(this, 255));
		}
		#endregion
	};
	#endregion
	#endregion


	#region model animations
	#region packed_data_sizes_struct
	[TI.Struct((int)StructGroups.Enumerated.apds, 1, 16)]
	public class packed_data_sizes_struct : TI.Definition
	{
		#region Fields
		public TI.ByteInteger StaticNodeFlags;
		public TI.ByteInteger AnimatedNodeFlags;
		public TI.ShortInteger MovementData;
		public TI.ShortInteger PillOffsetData;
		public TI.ShortInteger DefaultData;
		public TI.LongInteger UncompressedData;
		public TI.LongInteger CompressedData;
		#endregion

		#region Ctor
		public packed_data_sizes_struct() : base(7)
		{
			Add(StaticNodeFlags = new TI.ByteInteger());
			Add(AnimatedNodeFlags = new TI.ByteInteger());
			Add(MovementData = new TI.ShortInteger());
			Add(PillOffsetData = new TI.ShortInteger());
			Add(DefaultData = new TI.ShortInteger());
			Add(UncompressedData = new TI.LongInteger());
			Add(CompressedData = new TI.LongInteger());
		}
		#endregion

		public int GetTotalSize()
		{
			return MovementData.Value +
				PillOffsetData.Value +
				DefaultData.Value +
				UncompressedData.Value +
				CompressedData.Value;
		}
	}
	#endregion

	#region quantized_orientation_struct
	[TI.Struct((int)StructGroups.Enumerated.qoSS, 1, 24)]
	public class quantized_orientation_struct : TI.Definition
	{
		#region Fields
		public TI.ShortInteger RotationX, RotationY, RotationZ, RotationW;
		public TI.RealPoint3D DefaultTranslation;
		public TI.Real DefaultScale;
		#endregion

		#region Ctor
		public quantized_orientation_struct() : base(6)
		{
			Add(RotationX = new TI.ShortInteger());
			Add(RotationY = new TI.ShortInteger());
			Add(RotationZ = new TI.ShortInteger());
			Add(RotationW = new TI.ShortInteger());
			Add(DefaultTranslation = new TI.RealPoint3D());
			Add(DefaultScale = new TI.Real());
		}
		#endregion
	}
	#endregion

	#region animation_aiming_screen_struct
	[TI.Struct((int)StructGroups.Enumerated.aaim, 1, 24)]
	public class animation_aiming_screen_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_aiming_screen_struct() : base(8)
		{
			Add(/*right yaw per frame = */ new TI.Real(TI.FieldType.Angle));
			Add(/*left yaw per frame = */ new TI.Real(TI.FieldType.Angle));
			Add(/*right frame count = */ new TI.ShortInteger());
			Add(/*left frame count = */ new TI.ShortInteger());
			Add(/*down pitch per frame = */ new TI.Real(TI.FieldType.Angle));
			Add(/*up pitch per frame = */ new TI.Real(TI.FieldType.Angle));
			Add(/*down pitch frame count = */ new TI.ShortInteger());
			Add(/*up pitch frame count = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion

	#region animation_destination_state_struct
	[TI.Struct((int)StructGroups.Enumerated.ATSS_dest, 1, 8)]
	public class animation_destination_state_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_destination_state_struct() : base(5)
		{
			Add(/*state name = */ new TI.StringId());
			Add(/*frame event link = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
			Add(/*index a = */ new TI.ByteInteger());
			Add(/*index b = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region animation_transition_state_struct
	[TI.Struct((int)StructGroups.Enumerated.ATSS_trans, 1, 8)]
	public class animation_transition_state_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_transition_state_struct() : base(4)
		{
			Add(/*state name = */ new TI.StringId());
			Add(new TI.Pad(2));
			Add(/*index a = */ new TI.ByteInteger());
			Add(/*index b = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region animation_graph_resources_struct
	[TI.Struct((int)StructGroups.Enumerated.MAgr, 3, 80)]
	public partial class animation_graph_resources_struct : TI.Definition
	{
		#region animation_graph_node_block
		[TI.Definition(2, 32)]
		public class animation_graph_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_graph_node_block() : base(9)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*next sibling node index = */ new TI.BlockIndex()); // 1 animation_graph_node_block
				Add(/*first child node index = */ new TI.BlockIndex()); // 1 animation_graph_node_block
				Add(/*parent node index = */ new TI.BlockIndex()); // 1 animation_graph_node_block
				Add(/*model flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*node joint flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*base vector = */ new TI.RealVector3D());
				Add(/*vector range = */ new TI.Real());
				Add(/*z_pos = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region animation_graph_sound_reference_block
		[TI.Definition(1, 20)]
		public class animation_graph_sound_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_graph_sound_reference_block() : base(3)
			{
				Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region animation_graph_effect_reference_block
		[TI.Definition(1, 20)]
		public class animation_graph_effect_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_graph_effect_reference_block() : base(3)
			{
				Add(/*effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region animation_blend_screen_block
		[TI.Definition(1, 28)]
		public class animation_blend_screen_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_blend_screen_block() : base(2)
			{
				Add(/*label = */ new TI.StringId());
				Add(/*aiming screen = */ new TI.Struct<animation_aiming_screen_struct>(this));
			}
			#endregion
		}
		#endregion

		#region animation_pool_block
		[TI.Definition(6, 124)]
		public class animation_pool_block : TI.Definition
		{
			#region animation_frame_event_block
			[TI.Definition(1, 4)]
			public class animation_frame_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public animation_frame_event_block() : base(2)
				{
					Add(/*type = */ new TI.Enum());
					Add(/*frame = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region animation_sound_event_block
			[TI.Definition(1, 8)]
			public class animation_sound_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public animation_sound_event_block() : base(3)
				{
					Add(/*sound = */ new TI.BlockIndex()); // 1 animation_graph_sound_reference_block
					Add(/*frame = */ new TI.ShortInteger());
					Add(/*marker name = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region animation_effect_event_block
			[TI.Definition(2, 4)]
			public class animation_effect_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public animation_effect_event_block() : base(2)
				{
					Add(/*effect = */ new TI.BlockIndex()); // 1 animation_graph_effect_reference_block
					Add(/*frame = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region object_space_node_data_block
			[TI.Definition(1, 28)]
			public class object_space_node_data_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public object_space_node_data_block() : base(3)
				{
					Add(/*node_index = */ new TI.ShortInteger());
					Add(/*component flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*orientation = */ new TI.Struct<quantized_orientation_struct>(this));
				}
				#endregion
			}
			#endregion

			#region Fields
			public TI.StringId Name;
			public TI.LongInteger NodeListChecksum, ProductionChecksum, ImportChecksum;
			public TI.Enum Type, FrameInfoType;
			public TI.BlockIndex BlendScreen;
			public TI.ByteInteger NodeCount;
			public TI.ShortInteger FrameCount;
			public TI.Flags InternalFlags, ProductionFlags, PlaybackFlags;
			public TI.Enum DesiredCompression, CurrentCompression;
			public TI.Real Weight;
			public TI.LongInteger ParentGraphIndex, ParentGraphBlockIndex, ParentGraphBlockOffset;
			public TI.ShortInteger ParentGraphStartingPoint;
			public TI.ShortInteger LoopFrameIndex;
			public TI.BlockIndex ParentAnimation, NextAnimation;
			public TI.Data AnimationData;

			public TI.LongInteger CacheBlockIndex;
			public TI.Struct<packed_data_sizes_struct> DataSizes;
			#endregion

			#region Ctor
			public animation_pool_block() : base(29)
			{
				Add(Name = new TI.StringId());
				Add(NodeListChecksum = new TI.LongInteger());
				Add(ProductionChecksum = new TI.LongInteger());
				Add(ImportChecksum = new TI.LongInteger());
				Add(Type = new TI.Enum(TI.FieldType.ByteEnum));
				Add(FrameInfoType = new TI.Enum(TI.FieldType.ByteEnum));
				Add(BlendScreen = new TI.BlockIndex(TI.FieldType.ByteBlockIndex)); // 1 animation_blend_screen_block
				Add(NodeCount = new TI.ByteInteger());
				Add(FrameCount = new TI.ShortInteger());
				Add(InternalFlags = new TI.Flags(TI.FieldType.ByteFlags));
				Add(ProductionFlags = new TI.Flags(TI.FieldType.ByteFlags));

				Add(PlaybackFlags = new TI.Flags(TI.FieldType.WordFlags));
				Add(DesiredCompression = new TI.Enum(TI.FieldType.ByteEnum));
				Add(CurrentCompression = new TI.Enum(TI.FieldType.ByteEnum));
				Add(Weight = new TI.Real());

				AddXbox(ParentGraphIndex = new TI.LongInteger());
				AddXbox(ParentGraphBlockIndex = new TI.LongInteger());
				AddXbox(ParentGraphBlockOffset = new TI.LongInteger());
				AddXbox(ParentGraphStartingPoint = new TI.ShortInteger());
				
				Add(LoopFrameIndex = new TI.ShortInteger());
				Add(ParentAnimation = new TI.BlockIndex()); // 1 animation_pool_block
				Add(NextAnimation = new TI.BlockIndex()); // 1 animation_pool_block
				AddPc(new TI.Pad(2));
				Add(AnimationData = new TI.Data(this));

				Add(DataSizes = new TI.Struct<packed_data_sizes_struct>(this));
				Add(/*frame events = */ new TI.Block<animation_frame_event_block>(this, 512));
				Add(/*sound events = */ new TI.Block<animation_sound_event_block>(this, 512));
				Add(/*effect events = */ new TI.Block<animation_effect_event_block>(this, 512));
				Add(/*object-space parent nodes = */ new TI.Block<object_space_node_data_block>(this, 255));
			}
			#endregion

			public int GetTotalSize() { return DataSizes.Value.GetTotalSize(); }
		}
		#endregion

		#region Fields
		public TI.TagReference ParentAnimationGraph;
		public TI.Flags InheritanceFlags;
		public TI.Flags PrivateFlags;
		public TI.ShortInteger AnimationCodecPack;
		public TI.Block<animation_graph_node_block> SkeletonNodes;

		public TI.Block<animation_blend_screen_block> BlendScreens;
		public TI.Block<animation_pool_block> Animations;
		#endregion

		#region Ctor
		public animation_graph_resources_struct() : base(9)
		{
			Add(ParentAnimationGraph = new TI.TagReference(this, TagGroups.jmad));
			Add(InheritanceFlags = new TI.Flags(TI.FieldType.ByteFlags));
			Add(PrivateFlags = new TI.Flags(TI.FieldType.ByteFlags));
			Add(AnimationCodecPack = new TI.ShortInteger());
			Add(SkeletonNodes = new TI.Block<animation_graph_node_block>(this, 255));
			Add(/*sound references = */ new TI.Block<animation_graph_sound_reference_block>(this, 512));
			Add(/*effect references = */ new TI.Block<animation_graph_effect_reference_block>(this, 512));
			Add(BlendScreens = new TI.Block<animation_blend_screen_block>(this, 64));
			Add(Animations = new TI.Block<animation_pool_block>(this, 2048));
		}
		#endregion
	}
	#endregion

	#region animation_index_struct
	[TI.Struct((int)StructGroups.Enumerated.ANII, 1, 4)]
	public class animation_index_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_index_struct() : base(2)
		{
			Add(/*graph index = */ new TI.ShortInteger());
			Add(/*animation = */ new TI.BlockIndex()); // 1 animation_pool_block
		}
		#endregion
	}
	#endregion

	#region animation_graph_contents_struct
	[TI.Struct((int)StructGroups.Enumerated.MAgc, 1, 36)]
	public class animation_graph_contents_struct : TI.Definition
	{
		#region animation_ik_block
		[TI.Definition(1, 8)]
		public class animation_ik_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_ik_block() : base(2)
			{
				Add(/*marker = */ new TI.StringId());
				Add(/*attach to marker = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region animation_mode_block
		[TI.Definition(1, 28)]
		public class animation_mode_block : TI.Definition
		{
			#region weapon_class_block
			[TI.Definition(1, 28)]
			public class weapon_class_block : TI.Definition
			{
				#region weapon_type_block
				[TI.Definition(1, 76)]
				public class weapon_type_block : TI.Definition
				{
					#region animation_entry_block
					[TI.Definition(1, 8)]
					public class animation_entry_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public animation_entry_block() : base(2)
						{
							Add(/*label = */ new TI.StringId());
							Add(/*animation = */ new TI.Struct<animation_index_struct>(this));
						}
						#endregion
					}
					#endregion

					#region damage_animation_block
					[TI.Definition(1, 16)]
					public class damage_animation_block : TI.Definition
					{
						#region damage_direction_block
						[TI.Definition(1, 12)]
						public class damage_direction_block : TI.Definition
						{
							#region damage_region_block
							[TI.Definition(1, 4)]
							public class damage_region_block : TI.Definition
							{
								#region Fields
								#endregion

								#region Ctor
								public damage_region_block() : base(1)
								{
									Add(/*animation = */ new TI.Struct<animation_index_struct>(this));
								}
								#endregion
							}
							#endregion

							#region Fields
							#endregion

							#region Ctor
							public damage_direction_block() : base(1)
							{
								Add(/*regions = */ new TI.Block<damage_region_block>(this, 11));
							}
							#endregion
						}
						#endregion

						#region Fields
						#endregion

						#region Ctor
						public damage_animation_block() : base(2)
						{
							Add(/*label = */ new TI.StringId());
							Add(/*directions = */ new TI.Block<damage_direction_block>(this, 4));
						}
						#endregion
					}
					#endregion

					#region animation_transition_destination_block
					[TI.Definition(1, 20)]
					public class animation_transition_destination_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public animation_transition_destination_block() : base(4)
						{
							Add(/*full name = */ new TI.StringId());
							Add(/*mode = */ new TI.StringId());
							Add(/*state info = */ new TI.Struct<animation_destination_state_struct>(this));
							Add(/*animation = */ new TI.Struct<animation_index_struct>(this));
						}
						#endregion
					}
					#endregion

					#region animation_transition_block
					[TI.Definition(1, 24)]
					public class animation_transition_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public animation_transition_block() : base(3)
						{
							Add(/*full name = */ new TI.StringId());
							Add(/*state info = */ new TI.Struct<animation_transition_state_struct>(this));
							Add(/*destinations = */ new TI.Block<animation_transition_destination_block>(this, 32));
						}
						#endregion
					}
					#endregion

					#region precache_list_block
					[TI.Definition(1, 4)]
					public class precache_list_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public precache_list_block() : base(1)
						{
							Add(/*cache block index = */ new TI.LongInteger());
						}
						#endregion
					}
					#endregion

					#region Fields
					#endregion

					#region Ctor
					public weapon_type_block() : base(7)
					{
						Add(/*label = */ new TI.StringId());
						Add(/*actions = */ new TI.Block<animation_entry_block>(this, 256));
						Add(/*overlays = */ new TI.Block<animation_entry_block>(this, 256));
						Add(/*death and damage = */ new TI.Block<damage_animation_block>(this, 8));
						Add(/*transitions = */ new TI.Block<animation_transition_block>(this, 256));
						Add(/*high precache = */ new TI.Block<precache_list_block>(this, 1024));
						Add(/*low precache = */ new TI.Block<precache_list_block>(this, 1024));
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public weapon_class_block() : base(3)
				{
					Add(/*label = */ new TI.StringId());
					Add(/*weapon type = */ new TI.Block<weapon_type_block>(this, 64));
					Add(/*weapon ik = */ new TI.Block<animation_ik_block>(this, 8));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public animation_mode_block() : base(3)
			{
				Add(/*label = */ new TI.StringId());
				Add(/*weapon class = */ new TI.Block<weapon_class_block>(this, 64));
				Add(/*mode ik = */ new TI.Block<animation_ik_block>(this, 8));
			}
			#endregion
		}
		#endregion

		#region vehicle_suspension_block
		[TI.Definition(1, 40)]
		public class vehicle_suspension_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public vehicle_suspension_block() : base(10)
			{
				Add(/*label = */ new TI.StringId());
				Add(/*animation = */ new TI.Struct<animation_index_struct>(this));
				Add(/*marker name = */ new TI.StringId());
				Add(/*mass point offset = */ new TI.Real());
				Add(/*full extension ground_depth = */ new TI.Real());
				Add(/*full compression ground_depth = */ new TI.Real());
				Add(/*region name = */ new TI.StringId());
				Add(/*destroyed mass point offset = */ new TI.Real());
				Add(/*destroyed full extension ground_depth = */ new TI.Real());
				Add(/*destroyed full compression ground_depth = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region object_animation_block
		[TI.Definition(1, 20)]
		public class object_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_animation_block() : base(6)
			{
				Add(/*label = */ new TI.StringId());
				Add(/*animation = */ new TI.Struct<animation_index_struct>(this));
				Add(new TI.Pad(2));
				Add(/*function controls = */ new TI.Enum());
				Add(/*function = */ new TI.StringId());
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public animation_graph_contents_struct() : base(3)
		{
			Add(/*modes = */ new TI.Block<animation_mode_block>(this, 512));
			Add(/*vehicle suspension = */ new TI.Block<vehicle_suspension_block>(this, 32));
			Add(/*object overlays = */ new TI.Block<object_animation_block>(this, 32));
		}
		#endregion
	}
	#endregion

	#region model_animation_runtime_data_struct
	[TI.Struct((int)StructGroups.Enumerated.MArt, 1, 88)]
	public class model_animation_runtime_data_struct : TI.Definition
	{
		#region inherited_animation_block
		[TI.Definition(1, 48)]
		public class inherited_animation_block : TI.Definition
		{
			#region inherited_animation_node_map_block
			[TI.Definition(1, 2)]
			public class inherited_animation_node_map_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public inherited_animation_node_map_block() : base(1)
				{
					Add(/*local node = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region inherited_animation_node_map_flag_block
			[TI.Definition(1, 4)]
			public class inherited_animation_node_map_flag_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public inherited_animation_node_map_flag_block() : base(1)
				{
					Add(/*local node flags = */ new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public inherited_animation_block() : base(5)
			{
				Add(/*inherited graph = */ new TI.TagReference(this, TagGroups.jmad));
				Add(/*node map = */ new TI.Block<inherited_animation_node_map_block>(this, 255));
				Add(/*node map flags = */ new TI.Block<inherited_animation_node_map_flag_block>(this, 255));
				Add(/*root z offset = */ new TI.Real());
				Add(/*inheritance_flags = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region weapon_class_lookup_block
		[TI.Definition(1, 8)]
		public class weapon_class_lookup_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_class_lookup_block() : base(2)
			{
				Add(/*weapon name = */ new TI.StringId());
				Add(/*weapon class = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public model_animation_runtime_data_struct() : base(4)
		{
			Add(/*inheritence list = */ new TI.Block<inherited_animation_block>(this, 8));
			Add(/*weapon list = */ new TI.Block<weapon_class_lookup_block>(this, 64));
			Add(new TI.Pad(32));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region model_animation_graph
	[TI.TagGroup((int)TagGroups.Enumerated.jmad, 1, 236)]
	public partial class model_animation_graph_group : TI.Definition
	{
		#region additional_node_data_block
		[TI.Definition(1, 60)]
		public class additional_node_data_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public additional_node_data_block() : base(6)
			{
				Add(/*node name = */ new TI.StringId());
				Add(/*default rotation = */ new TI.RealQuaternion());
				Add(/*default translation = */ new TI.RealPoint3D());
				Add(/*default scale = */ new TI.Real());
				Add(/*min bounds = */ new TI.RealPoint3D());
				Add(/*max bounds = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region animation_graph_cache_block
		[TI.Definition(1, 20)]
		public partial class animation_graph_cache_block : TI.Definition
		{
			#region Fields
			public TI.LongInteger OwnerTagIndex;
			public TI.LongInteger BlockSize;
			public TI.LongInteger BlockOffset;
			#endregion

			#region Ctor
			public animation_graph_cache_block() : base(4)
			{
				Add(OwnerTagIndex = new TI.LongInteger());
				Add(BlockSize = new TI.LongInteger());
				Add(BlockOffset = new TI.LongInteger());
				Add(new TI.Pad(2 + 1 + 1 + 4));
			}
			#endregion
		}
		#endregion

		#region animation_graph_cache_unknown_block
		[TI.Definition(1, 24)]
		public class animation_graph_cache_unknown_block : TI.Definition
		{
			#region Fields
			public TI.Skip Data;
			#endregion

			#region Ctor
			public animation_graph_cache_unknown_block() : base(1)
			{
				Add(Data = new TI.Skip(24));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Struct<animation_graph_resources_struct> Resources;
		public TI.Struct<animation_graph_contents_struct> Content;
		public TI.Struct<model_animation_runtime_data_struct> RuntimeData;
		public TI.Data LastImportResults;
		public TI.Block<additional_node_data_block> AdditionalNodeData;
		TI.Block<animation_graph_cache_block> CacheBlocks;
		TI.Block<animation_graph_cache_unknown_block> CacheUnknown;
		#endregion

		#region Ctor
		private void version1_construct()
		{
			Add(Resources = new TI.Struct<animation_graph_resources_struct>(this));
			Add(Content = new TI.Struct<animation_graph_contents_struct>(this));
			Add(RuntimeData = new TI.Struct<model_animation_runtime_data_struct>(this));
			Add(LastImportResults = new TI.Data(this));
			Add(AdditionalNodeData = new TI.Block<additional_node_data_block>(this, 255));
		}

		public model_animation_graph_group() : base(7)
		{
			version1_construct();
			AddXbox(CacheBlocks = new TI.Block<animation_graph_cache_block>(this, 0));
			AddXbox(CacheUnknown = new TI.Block<animation_graph_cache_unknown_block>(this, 0));
		}

		[TI.VersionCtorHalo2(1, 260)]
		public model_animation_graph_group(int major, int minor)
		{
			if (major == 1)
			{
				switch (minor)
				{
					case 260:
						version1_construct();
						Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
						Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
						break;
				}
			}
		}

		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			if (attr.Major == 1)
			{
				switch (attr.Minor)
				{
					case 260: // remove the two tag blocks we added in the versioning ctor
						this.RemoveAt(this.Count - 1);
						this.RemoveAt(this.Count - 1);
						AddXbox(CacheBlocks = new TI.Block<animation_graph_cache_block>(this, 0));
						AddXbox(CacheUnknown = new TI.Block<animation_graph_cache_unknown_block>(this, 0));
						break;
				}
			}

			return true;
		}
		#endregion
	};
	#endregion
	#endregion


	#region particle_model
	[TI.TagGroup((int)TagGroups.Enumerated.PRTM, 1, 292)]
	public partial class particle_model_group : TI.Definition
	{
		#region particle_models_block
		[TI.Definition(1, 8)]
		public class particle_models_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public particle_models_block() : base(3)
			{
				Add(/*model name = */ new TI.StringId());
				Add(/*index start = */ new TI.ShortInteger());
				Add(/*index count = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region particle_model_vertices_block
		[TI.Definition(1, 56)]
		public class particle_model_vertices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public particle_model_vertices_block() : base(5)
			{
				Add(/*position = */ new TI.RealPoint3D());
				Add(/*normal = */ new TI.RealVector3D());
				Add(/*tangent = */ new TI.RealVector3D());
				Add(/*binormal = */ new TI.RealVector3D());
				Add(/*texcoord = */ new TI.RealPoint2D());
			}
			#endregion
		}
		#endregion

		#region particle_model_indices_block
		[TI.Definition(1, 2)]
		public class particle_model_indices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public particle_model_indices_block() : base(1)
			{
				Add(/*index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<particle_model_vertices_block> RawVertices;
		public TI.Block<particle_model_indices_block> Indices;
		public TI.Block<cached_data_block> CachedData;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
		#endregion

		#region Ctor
		public particle_model_group() : base(16)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*orientation = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(new TI.Pad(16));
			Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*scale x = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*scale y = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*scale z = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*rotation = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*collision effect = */ new TI.TagReference(this)); // effe,snd!,foot,
			Add(/*death effect = */ new TI.TagReference(this)); // effe,snd!,foot,
			Add(/*locations = */ new TI.Block<effect_locations_block>(this, 32));
			Add(/*attached particle systems = */ new TI.Block<particle_system_definition_block_new>(this, 32));
			Add(/*models = */ new TI.Block<particle_models_block>(this, 256));
			Add(RawVertices = new TI.Block<particle_model_vertices_block>(this, 32768));
			Add(Indices = new TI.Block<particle_model_indices_block>(this, 32768));
			Add(CachedData = new TI.Block<cached_data_block>(this, 1));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
			Add(new TI.Pad(16 + 8 + 4));
		}
		#endregion
	};
	#endregion


	#region physics model
	public enum physics_shape_type : ushort
	{
		Sphere,
		Pill,
		Box,
		Triangle,
		Polyhedron,
		MultiSphere,
		Unused0,	// phantom
		Unused1,
		Unused2,
		Unused3,
		Unused4,
		Unused5,
		Unused6,
		Unused7,
		List,
		Mopp,
	};

	#region constraint_bodies_struct
	//constraint bodies
	[TI.Struct((int)StructGroups.Enumerated.csbs, 1, 116)]
	public class constraint_bodies_struct : TI.Definition
	{
		public class node_data
		{
			TI.Real Scale;
			TI.RealVector3D Forward, Left, Up;
			TI.RealPoint3D Position;

			public node_data(TI.Definition parent)
			{
				parent.Add(Scale = new TI.Real());
				parent.Add(Forward = new TI.RealVector3D());
				parent.Add(Left = new TI.RealVector3D());
				parent.Add(Up = new TI.RealVector3D());
				parent.Add(Position = new TI.RealPoint3D());
			}
		};

		#region Fields
		public TI.StringId Name;
		public TI.BlockIndex NodeIndexA, NodeIndexB;
		public node_data NodeA, NodeB;
		#endregion

		#region Ctor
		public constraint_bodies_struct() : base(15)
		{
			Add(Name = new TI.StringId());
			Add(NodeIndexA = new TI.BlockIndex()); // 1 nodes_block
			Add(NodeIndexB = new TI.BlockIndex()); // 1 nodes_block
			NodeA = new node_data(this);
			NodeB = new node_data(this);
			Add(/*edge index = */ new TI.BlockIndex()); // 1 physics_model_node_constraint_edge_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region physics_model
	[TI.TagGroup((int)TagGroups.Enumerated.phmo, 2, 3, 396)]
	public partial class physics_model_group : TI.Definition, ITagImportInfo
	{
		#region Havok Shapes
		public /*abstract*/ class hkShape
		{
			TI.ShortInteger Size, ReferenceCount;
			public TI.Skip UserData;

			public hkShape(TI.Definition parent)
			{
				parent.Add(new TI.Skip(4));							// vtable
				parent.Add(Size = new TI.ShortInteger());
				parent.Add(ReferenceCount = new TI.ShortInteger());
				parent.Add(UserData = new TI.Skip(4)); // pointer to the shape block this shape is in
			}
		};

		public class hkConvexShape : hkShape
		{
			public TI.Real Radius;

			public hkConvexShape(TI.Definition parent) : base(parent)
			{
				parent.Add(Radius = new TI.Real());
			}
		};

		public class hkSphereShape : hkConvexShape
		{
			public hkSphereShape(TI.Definition parent) : base(parent)	{}
		};
		#endregion

		// I made this up. Up this made I. Not a real definition.
		#region havok_shape_base_block
		public abstract class havok_shape_base_block : TI.Definition
		{
			public TI.StringId Name;
			public TI.BlockIndex Material;
			public TI.Flags Flags;
			public TI.Real RelativeMassScale, Friction, Restitution,
				Volume, Mass;
			public TI.Skip MassDistributionsIndex;
			TI.BlockIndex Phantom;

			public hkShape HavokShapeObject;

			protected havok_shape_base_block(int field_count) : base(10 + field_count)
			{
				Add(Name = new TI.StringId());
				Add(Material =  new TI.BlockIndex()); // 1 materials_block
				Add(Flags =  new TI.Flags(TI.FieldType.WordFlags));
				Add(RelativeMassScale = new TI.Real());
				Add(Friction = new TI.Real(TI.FieldType.RealFraction));
				Add(Restitution = new TI.Real(TI.FieldType.RealFraction));
				Add(Volume = new TI.Real());
				Add(Mass = new TI.Real());
				Add(MassDistributionsIndex = new TI.Skip(2));
				Add(Phantom = new TI.BlockIndex()); // 1 phantoms_block
					// TODO: investigate the upper 8 bits of phantom
			}
		};
		#endregion

		// I made this up. Up this made I. Not a real definition.
		#region havok_constraints_base_block
		public abstract class havok_constraints_base_block : TI.Definition
		{
			public TI.Struct<constraint_bodies_struct> ConstraintBodies;

			protected havok_constraints_base_block(int field_count) : base(2 + field_count)
			{
				Add(ConstraintBodies = new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
			}
		};
		#endregion

		#region shape_info
		public partial class shape_info
		{
			public TI.Enum ShapeType;
			public TI.BlockIndex Shape;

			public shape_info(TI.Definition parent)
			{
				parent.Add(ShapeType = new TI.Enum());
				parent.Add(Shape = new TI.BlockIndex()); // 2
			}
		};
		#endregion

		#region mass_distribution_data
		public class mass_distribution_data
		{
			public TI.RealVector3D CenterOfMass;
			public TI.RealVector3D InertiaTensorI, InertiaTensorJ, InertiaTensorK;

			public mass_distribution_data(TI.Definition parent)
			{
				parent.Add(CenterOfMass = new TI.RealVector3D());
				parent.Add(new TI.Skip(4));
				parent.Add(InertiaTensorI = new TI.RealVector3D());
				parent.Add(new TI.Skip(4));
				parent.Add(InertiaTensorJ = new TI.RealVector3D());
				parent.Add(new TI.Skip(4));
				parent.Add(InertiaTensorK = new TI.RealVector3D());
				parent.Add(new TI.Skip(4));
			}
		};
		#endregion


		#region phantom_types_block
		[TI.Definition(2, 104)]
		public class phantom_types_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public phantom_types_block() : base(20)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*minimum size = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*maximum size = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(2));
				Add(/*marker name = */ new TI.StringId());
				Add(/*alignment marker name = */ new TI.StringId());
				Add(new TI.Pad(8));
				Add(/*hookes law e = */ new TI.Real());
				Add(/*linear dead radius = */ new TI.Real());
				Add(/*center acc = */ new TI.Real());
				Add(/*center max vel = */ new TI.Real());
				Add(/*axis acc = */ new TI.Real());
				Add(/*axis max vel = */ new TI.Real());
				Add(/*direction acc = */ new TI.Real());
				Add(/*direction max vel = */ new TI.Real());
				Add(new TI.Pad(28));
				Add(/*alignment hookes law e = */ new TI.Real());
				Add(/*alignment acc = */ new TI.Real());
				Add(/*alignment max vel = */ new TI.Real());
				Add(new TI.Pad(8));
			}
			#endregion
		}
		#endregion

		#region physics_model_node_constraint_edge_block
		[TI.Definition(1, 28)]
		public class physics_model_node_constraint_edge_block : TI.Definition
		{
			#region physics_model_constraint_edge_constraint_block
			[TI.Definition(1, 12)]
			public class physics_model_constraint_edge_constraint_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public physics_model_constraint_edge_constraint_block() : base(4)
				{
					Add(/*type = */ new TI.Enum());
					Add(/*index = */ new TI.BlockIndex()); // 2
					Add(/*flags = */ new TI.Flags());
					Add(/*friction = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public physics_model_node_constraint_edge_block() : base(6)
			{
				Add(new TI.Pad(4));
				Add(/*node a = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*node b = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*constraints = */ new TI.Block<physics_model_constraint_edge_constraint_block>(this, 64));
				Add(/*node a material = */ new TI.StringId());
				Add(/*node b material = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region rigid_bodies_block
		[TI.Definition(2, 144)]
		public partial class rigid_bodies_block : TI.Definition
		{
			#region Fields
			public shape_info ShapeInfo;
			public TI.Real Mass;
			public mass_distribution_data MassDistribution;
			#endregion

			#region Ctor
			public rigid_bodies_block() : base(27)
			{
				Add(/*node = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*region = */ new TI.BlockIndex()); // 1 regions_block
				Add(/*permutation = */ new TI.BlockIndex()); // 2
				Add(new TI.Pad(2));
				Add(/*bounding sphere offset = */ new TI.RealPoint3D());
				Add(/*bounding sphere radius = */ new TI.Real());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*motion type = */ new TI.Enum());
				Add(/*no phantom power alt = */ new TI.BlockIndex()); // 1 rigid_bodies_block
				Add(/*size = */ new TI.Enum());
				Add(/*inertia tensor scale = */ new TI.Real());
				Add(/*linear damping = */ new TI.Real());
				Add(/*angular damping = */ new TI.Real());
				Add(/*center off mass offset = */ new TI.RealVector3D());
				ShapeInfo = new shape_info(this);
				Add(Mass = new TI.Real());
				MassDistribution = new mass_distribution_data(this); // inertia tensor x,y,z
				Add(/*bounding sphere pad = */ new TI.Real());
				Add(new TI.Pad(12));
			}
			#endregion
		}
		#endregion

		#region materials_block
		[TI.Definition(1, 12)]
		public class materials_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public materials_block() : base(4)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*global material name = */ new TI.StringId());
				Add(/*phantom type = */ new TI.BlockIndex()); // 1 phantom_types_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			}
			#endregion
		}
		#endregion

		#region spheres_block
		[TI.Definition(1, 128)]
		public class spheres_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public spheres_block() : base(18)
			{
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*radius = */ new TI.Real());

				// sphere object:
				HavokShapeObject = new hkShape(this);
				Add(new TI.Skip(4));
				Add(/*rotation i = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation j = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation k = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*translation = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region multi_spheres_block
		[TI.Definition(1, 176)]
		public class multi_spheres_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public multi_spheres_block() : base(21)
			{
				HavokShapeObject = new hkShape(this);
				Add(/*num spheres = */ new TI.LongInteger());

				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region pills_block
		[TI.Definition(1, 80)]
		public class pills_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public pills_block() : base(9)
			{
				HavokShapeObject = new hkShape(this);
				Add(/*radius = */ new TI.Real());
				Add(/*bottom = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*top = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region boxes_block
		[TI.Definition(1, 144)]
		public class boxes_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public boxes_block() : base(20)
			{
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*radius = */ new TI.Real());
				Add(/*half extents = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				// box object:
				HavokShapeObject = new hkShape(this);
				Add(new TI.Skip(4));
				Add(/*rotation i = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation j = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation k = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*translation = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region triangles_block
		[TI.Definition(1, 96)]
		public class triangles_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public triangles_block() : base(11)
			{
				HavokShapeObject = new hkShape(this);
				Add(/*radius = */ new TI.Real());
				Add(/*point a = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*point b = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*point c = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region polyhedra_block
		[TI.Definition(1, 256)]
		public class polyhedra_block : havok_shape_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public polyhedra_block() : base(35)
			{
				HavokShapeObject = new hkShape(this);
				Add(/*radius = */ new TI.Real());
				Add(/*aabb half extents = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*aabb center = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(new TI.Skip(4));
				Add(/*four vectors size = */ new TI.LongInteger());
				Add(/*four vectors capacity = */ new TI.LongInteger());
				Add(/*num vertices = */ new TI.LongInteger());

				Add(/*four vectors x = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors y = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors z = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(/*four vectors x = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors y = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors z = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(/*four vectors x = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors y = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors z = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(new TI.Skip(4));
				Add(/*plane equations size = */ new TI.LongInteger());
				Add(/*plane equations capacity = */ new TI.LongInteger());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region polyhedron_four_vectors_block
		[TI.Definition(1, 48)]
		public class polyhedron_four_vectors_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public polyhedron_four_vectors_block() : base(6)
			{
				Add(/*four vectors x = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors y = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*four vectors z = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region polyhedron_plane_equations_block
		[TI.Definition(1, 16)]
		public class polyhedron_plane_equations_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public polyhedron_plane_equations_block() : base(1)
			{
				Add(new TI.Skip(16));
			}
			#endregion
		}
		#endregion

		#region mass_distributions_block
		[TI.Definition(1, 64)]
		public class mass_distributions_block : TI.Definition
		{
			public mass_distribution_data MassDistribution;

			public mass_distributions_block() : base(8)
			{
				MassDistribution = new mass_distribution_data(this);
			}
		}
		#endregion

		#region lists_block
		[TI.Definition(1, 56)]
		public class lists_block : TI.Definition
		{
			#region Fields
			public hkShape HavokShapeObject;
			#endregion

			#region Ctor
			public lists_block() : base(18)
			{
				HavokShapeObject = new hkShape(this);
				Add(new TI.Skip(4));
				Add(/*child shapes size = */ new TI.LongInteger());
				Add(/*child shapes capacity = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region list_shapes_block
		[TI.Definition(1, 8)]
		public class list_shapes_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public list_shapes_block() : base(3)
			{
				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region mopps_block
		[TI.Definition(1, 20)]
		public class mopps_block : TI.Definition
		{
			#region Fields
			public hkShape HavokShapeObject;
			#endregion

			#region Ctor
			public mopps_block() : base(7)
			{
				HavokShapeObject = new hkShape(this);
				Add(new TI.Pad(2));
				Add(/*list = */ new TI.BlockIndex()); // 1 lists_block
				Add(/*code offset = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region hinge_constraints_block
		[TI.Definition(1, 120)]
		public class hinge_constraints_block : havok_constraints_base_block
		{
			public hinge_constraints_block() : base(0)
			{
			}
		}
		#endregion

		#region ragdoll_constraints_block
		[TI.Definition(1, 148)]
		public class ragdoll_constraints_block : havok_constraints_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public ragdoll_constraints_block() : base(7)
			{
				Add(/*min twist = */ new TI.Real());
				Add(/*max twist = */ new TI.Real());
				Add(/*min cone = */ new TI.Real());
				Add(/*max cone = */ new TI.Real());
				Add(/*min plane = */ new TI.Real());
				Add(/*max plane = */ new TI.Real());
				Add(/*max friciton torque = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region regions_block
		[TI.Definition(1, 16)]
		public class regions_block : TI.Definition
		{
			#region permutations_block
			[TI.Definition(1, 16)]
			public class permutations_block : TI.Definition
			{
				#region rigid_body_indices_block
				[TI.Definition(1, 2)]
				public class rigid_body_indices_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public rigid_body_indices_block() : base(1)
					{
						Add(/*rigid body = */ new TI.BlockIndex()); // 1 rigid_bodies_block
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public permutations_block() : base(2)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*rigid bodies = */ new TI.Block<rigid_body_indices_block>(this, 64));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public regions_block() : base(2)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*permutations = */ new TI.Block<permutations_block>(this, 32));
			}
			#endregion
		}
		#endregion

		#region nodes_block
		[TI.Definition(1, 12)]
		public class nodes_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public nodes_block() : base(5)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*parent = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*sibling = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*child = */ new TI.BlockIndex()); // 1 nodes_block
			}
			#endregion
		}
		#endregion

		#region point_to_path_curve_block
		[TI.Definition(1, 20)]
		public class point_to_path_curve_block : TI.Definition
		{
			#region point_to_path_curve_point_block
			[TI.Definition(1, 16)]
			public class point_to_path_curve_point_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public point_to_path_curve_point_block() : base(2)
				{
					Add(/*position = */ new TI.RealPoint3D());
					Add(/*t value = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public point_to_path_curve_block() : base(4)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*node index = */ new TI.BlockIndex()); // 1 nodes_block
				Add(new TI.Pad(2));
				Add(/*points = */ new TI.Block<point_to_path_curve_point_block>(this, 1024));
			}
			#endregion
		}
		#endregion

		#region limited_hinge_constraints_block
		[TI.Definition(1, 132)]
		public class limited_hinge_constraints_block : havok_constraints_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public limited_hinge_constraints_block() : base(3)
			{
				Add(/*limit friction = */ new TI.Real());
				Add(/*limit min angle = */ new TI.Real());
				Add(/*limit max angle = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region ball_and_socket_constraints_block
		[TI.Definition(1, 120)]
		public class ball_and_socket_constraints_block : havok_constraints_base_block
		{
			public ball_and_socket_constraints_block() : base(0)
			{
			}
		}
		#endregion

		#region stiff_spring_constraints_block
		[TI.Definition(1, 124)]
		public class stiff_spring_constraints_block : havok_constraints_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public stiff_spring_constraints_block() : base(1)
			{
				Add(/*spring_length = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region prismatic_constraints_block
		[TI.Definition(1, 132)]
		public class prismatic_constraints_block : havok_constraints_base_block
		{
			#region Fields
			#endregion

			#region Ctor
			public prismatic_constraints_block() : base(3)
			{
				Add(/*min_limit = */ new TI.Real());
				Add(/*max_limit = */ new TI.Real());
				Add(/*max_friction_force = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region phantoms_block
		[TI.Definition(1, 32)]
		public class phantoms_block : TI.Definition
		{
			#region Fields
			public hkShape HavokShapeObject;
			#endregion

			#region Ctor
			public phantoms_block() : base(9)
			{
				HavokShapeObject = new hkShape(this);
				Add(new TI.Pad(4 + 4));
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<rigid_bodies_block> RigidBodies;
		public TI.Block<materials_block> Materials;
		public TI.Block<spheres_block> Spheres;
		public TI.Block<multi_spheres_block> MultiSpheres;
		public TI.Block<pills_block> Pills;
		public TI.Block<boxes_block> Boxes;
		public TI.Block<triangles_block> Triangles;
		public TI.Block<polyhedra_block> Polyhedra;
		public TI.Block<polyhedron_four_vectors_block> PolyhedraFourVectors;
		public TI.Block<polyhedron_plane_equations_block> PolyhedraPlaneEquations;
		public TI.Block<mass_distributions_block> MassDistributions;
		public TI.Block<lists_block> Lists;
		public TI.Block<list_shapes_block> ListShapes;
		public TI.Block<mopps_block> Mopps;
		public TI.Data MoppCodes;

		public TI.Block<global_tag_import_info_block> ImportInfo;
		public TI.Block<global_error_report_categories_block> Errors;

		public TI.Block<phantoms_block> Phantoms;
		#endregion

		#region Ctor
		public physics_model_group() : base(34)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*mass = */ new TI.Real());
			Add(/*low freq. deactivation scale = */ new TI.Real());
			Add(/*high freq. deactivation scale = */ new TI.Real());
			Add(new TI.Pad(24));
			Add(/*phantom types = */ new TI.Block<phantom_types_block>(this, 16));
			Add(/*node edges = */ new TI.Block<physics_model_node_constraint_edge_block>(this, 4096));
			Add(RigidBodies = new TI.Block<rigid_bodies_block>(this, 64));
			Add(Materials = new TI.Block<materials_block>(this, 64));
			Add(Spheres = new TI.Block<spheres_block>(this, 1024));
			Add(MultiSpheres = new TI.Block<multi_spheres_block>(this, 1024));
			Add(Pills = new TI.Block<pills_block>(this, 1024));
			Add(Boxes = new TI.Block<boxes_block>(this, 1024));
			Add(Triangles = new TI.Block<triangles_block>(this, 1024));
			Add(Polyhedra = new TI.Block<polyhedra_block>(this, 1024));
			Add(PolyhedraFourVectors = new TI.Block<polyhedron_four_vectors_block>(this, 4096));
			Add(PolyhedraPlaneEquations = new TI.Block<polyhedron_plane_equations_block>(this, 1024));
			Add(MassDistributions = new TI.Block<mass_distributions_block>(this, 256));
			Add(Lists = new TI.Block<lists_block>(this, 64));
			Add(ListShapes = new TI.Block<list_shapes_block>(this, 256));
			Add(Mopps = new TI.Block<mopps_block>(this, 64));
			Add(MoppCodes = new TI.Data(this));
			Add(/*hinge constraints = */ new TI.Block<hinge_constraints_block>(this, 64));
			Add(/*ragdoll constraints = */ new TI.Block<ragdoll_constraints_block>(this, 64));
			Add(/*regions = */ new TI.Block<regions_block>(this, 16));
			Add(/*nodes = */ new TI.Block<nodes_block>(this, 255));
			Add(ImportInfo = new TI.Block<global_tag_import_info_block>(this, 1));
			Add(Errors = new TI.Block<global_error_report_categories_block>(this, 64));
			Add(/*point to path curves = */ new TI.Block<point_to_path_curve_block>(this, 64));
			Add(/*limited hinge constraints = */ new TI.Block<limited_hinge_constraints_block>(this, 64));
			Add(/*ball and socket constraints = */ new TI.Block<ball_and_socket_constraints_block>(this, 64));
			Add(/*stiff spring constraints = */ new TI.Block<stiff_spring_constraints_block>(this, 64));
			Add(/*prismatic constraints = */ new TI.Block<prismatic_constraints_block>(this, 64));
			Add(Phantoms = new TI.Block<phantoms_block>(this, 1024));
		}
		#endregion
	};
	#endregion
	#endregion


	#region render model
	public class global_model_skinned_uncompressed_vertex
	{
		public const int kFieldCount = 9; // with 4 weights

		public TI.RealPoint3D Position;
		public TI.ByteInteger[] NodeIndices = new TI.ByteInteger[4];
		public TI.Real[] NodeWeights;

		public global_model_skinned_uncompressed_vertex(TI.Definition parent, int node_weight_count)
		{
			NodeWeights = new TI.Real[node_weight_count];

			parent.Add(Position = new TI.RealPoint3D());
			parent.Add(NodeIndices[0] = new TI.ByteInteger());
			parent.Add(NodeIndices[1] = new TI.ByteInteger());
			parent.Add(NodeIndices[2] = new TI.ByteInteger());
			parent.Add(NodeIndices[3] = new TI.ByteInteger());
			for (int x = 0; x < NodeWeights.Length; x++ )
				parent.Add(NodeWeights[x] = new TI.Real());
		}
		public global_model_skinned_uncompressed_vertex(TI.Definition parent) : this(parent, 4)
		{
		}

		internal void SetFrom(global_model_skinned_uncompressed_vertex obj)
		{
			Position.X = obj.Position.X;
			Position.Y = obj.Position.Y;
			Position.Z = obj.Position.Z;
			NodeIndices[0].Value = obj.NodeIndices[0].Value;
			NodeIndices[1].Value = obj.NodeIndices[1].Value;
			NodeIndices[2].Value = obj.NodeIndices[2].Value;
			NodeIndices[3].Value = obj.NodeIndices[3].Value;
			for (int x = 0; x < NodeWeights.Length; x++ )
				NodeWeights[x].Value = obj.NodeWeights[x].Value;
		}
	};

	public partial class global_geometry_raw_point
	{
		public const int kFieldCount = 15;

		public TI.RealPoint3D Position;
		public TI.Real[] NodeWeight = new TI.Real[4];
		public TI.LongInteger[] NodeIndex = new TI.LongInteger[4];
		public TI.LongInteger UseNewNodeIndices;
		public TI.LongInteger AdjustedCompoundNodeIndex;

		public global_geometry_raw_point(TI.Definition parent)
		{
			parent.Add(Position = new TI.RealPoint3D());					// 0x0
			parent.Add(/*Node Index (OLD) = */ new TI.LongInteger());		// 0xC
			parent.Add(/*Node Index (OLD) = */ new TI.LongInteger());		// 0x10
			parent.Add(/*Node Index (OLD) = */ new TI.LongInteger());		// 0x14
			parent.Add(/*Node Index (OLD) = */ new TI.LongInteger());		// 0x18
			for (int x = 0; x < NodeWeight.Length; x++ )
				parent.Add(NodeWeight[x] = new TI.Real());
			for (int x = 0; x < NodeIndex.Length; x++)
				parent.Add(NodeIndex[x] = new TI.LongInteger());

			parent.Add(UseNewNodeIndices = new TI.LongInteger());			// 0x3C
			parent.Add(AdjustedCompoundNodeIndex = new TI.LongInteger());	// 0x40
		}
	};

	#region global_geometry_compression_info_block
	[TI.Definition(1, 56)]
	public class global_geometry_compression_info_block : TI.Definition
	{
		public TI.RealBounds PositionX, PositionY, PositionZ,
			TexCoordX, TexCoordY,
			SecondaryTexCoordX, SecondaryTexCoordY;

		public global_geometry_compression_info_block() : base(7)
		{
			Add(PositionX = new TI.RealBounds());
			Add(PositionY = new TI.RealBounds());
			Add(PositionZ = new TI.RealBounds());
			Add(TexCoordX = new TI.RealBounds());
			Add(TexCoordY = new TI.RealBounds());
			Add(SecondaryTexCoordX = new TI.RealBounds());
			Add(SecondaryTexCoordY = new TI.RealBounds());
		}
	}
	#endregion


	#region global_geometry_section_info_struct
	[TI.Struct((int)StructGroups.Enumerated.SINF, 1, 44)]
	public partial class global_geometry_section_info_struct : TI.Definition
	{
		#region Fields
		public TI.ShortInteger TotalVertexCount;
		public TI.ShortInteger TotalTriangleCount;
		public TI.ShortInteger TotalPartCount;
		public TI.Enum GeometryClassification;
		public TI.Flags GeometryCompressionFlags;
		public TI.Block<global_geometry_compression_info_block> GeometryCompressionInfo;
		public TI.ByteInteger HardwareNodeCount;
		public TI.ByteInteger NodeMapSize;
		public TI.ShortInteger SoftwarePlaneCount;
		public TI.ShortInteger TotalSubpartCount;
		public TI.Flags SectionLightingFlags;
		#endregion

		#region Ctor
		public global_geometry_section_info_struct() : base(19)
		{
			Add(TotalVertexCount = new TI.ShortInteger());
			Add(TotalTriangleCount = new TI.ShortInteger());
			Add(TotalPartCount = new TI.ShortInteger());
			Add(/*Shadow-Casting Triangle Count = */ new TI.ShortInteger());
			Add(/*Shadow-Casting Part Count = */ new TI.ShortInteger());
			Add(/*Opaque Point Count = */ new TI.ShortInteger());
			Add(/*Opaque Vertex Count = */ new TI.ShortInteger());
			Add(/*Opaque Part Count = */ new TI.ShortInteger());
			Add(/*Opaque Max Nodes/Vertex = */ new TI.ByteInteger());
			Add(/*Transparent Max Nodes/Vertex = */ new TI.ByteInteger());
			Add(/*Shadow-Casting Rigid Triangle Count = */ new TI.ShortInteger());
			Add(GeometryClassification = new TI.Enum());
			Add(GeometryCompressionFlags = new TI.Flags(TI.FieldType.WordFlags));
			Add(GeometryCompressionInfo = new TI.Block<global_geometry_compression_info_block>(this, 1));
			Add(HardwareNodeCount = new TI.ByteInteger());
			Add(NodeMapSize = new TI.ByteInteger());
			Add(SoftwarePlaneCount = new TI.ShortInteger());
			Add(TotalSubpartCount = new TI.ShortInteger());
			Add(SectionLightingFlags = new TI.Flags(TI.FieldType.WordFlags));
		}
		#endregion
	}
	#endregion

	// TODO: Consider moving this to a field_block<>
	#region global_geometry_section_strip_index_block
	[TI.Definition(1, 2)]
	public class global_geometry_section_strip_index_block : TI.Definition
	{
		public TI.ShortInteger Index;

		public global_geometry_section_strip_index_block() : base(1)
		{
			Add(Index = new TI.ShortInteger());
		}
	}
	#endregion

	// TODO: Consider moving this to a field_block<>
	#region global_geometry_section_vertex_buffer_block
	[TI.Definition(1, 32)]
	public class global_geometry_section_vertex_buffer_block : TI.Definition
	{
		public TI.VertexBuffer VertexBuffer;

		public global_geometry_section_vertex_buffer_block() : base(1)
		{
			Add(VertexBuffer = new TI.VertexBuffer());
		}
	}
	#endregion

	#region global_geometry_part
	public abstract class global_geometry_part_base : TI.Definition
	{
		public TI.Enum Type;
		public TI.Flags Flags;
		public TI.BlockIndex Material;
		public TI.ShortInteger StripStartIndex;
		public TI.ShortInteger StripLength;

		public TI.ByteInteger MaxNodesVertex;
		public TI.ByteInteger ContributingCompoundNodeCount;

		public global_model_skinned_uncompressed_vertex Centroid;
		public TI.Real LodMipmapMagicNumber;
		public TI.Skip _Skip;

		protected global_geometry_part_base(int field_count) : base(field_count) { }
	};

	#region global_geometry_part_block
	[TI.Definition(1, 100)]
	public partial class global_geometry_part_block : global_geometry_part_base
	{
		public TI.ShortInteger FirstVertexIndex, VertexCount,
			FirstTriangleIndex, TriangleCount;
		public TI.Flags VertexUsageFlags;

		public global_geometry_part_block() : base(19)
		{
			Add(Type = new TI.Enum());
			Add(Flags = TI.Flags.Word);
			Add(Material = new TI.BlockIndex()); // 1 global_geometry_material_block
			Add(/*Geometry Subclassification = */ new TI.Enum());
			Add(StripStartIndex = new TI.ShortInteger());
			Add(StripLength = new TI.ShortInteger());
			Add(/*First Strip Segment Index =*/ new TI.ShortInteger());
			Add(/*Strip Segment Count =*/ new TI.ShortInteger());
			Add(FirstVertexIndex = new TI.ShortInteger());
			Add(VertexCount = new TI.ShortInteger());
			Add(FirstTriangleIndex = new TI.ShortInteger());
			Add(TriangleCount = new TI.ShortInteger());
			Add(MaxNodesVertex = new TI.ByteInteger());
			Add(ContributingCompoundNodeCount = new TI.ByteInteger());
			Add(VertexUsageFlags = TI.Flags.Word);
			Add(new TI.Pad(12 + 1 + 3));
			Centroid = new global_model_skinned_uncompressed_vertex(this, 3);
			Add(LodMipmapMagicNumber = new TI.Real());
			Add(_Skip = new TI.Skip(24));
		}
	};
	#endregion

	#region global_geometry_part_block_new
	[TI.Definition(1, 72)]
	public partial class global_geometry_part_block_new : global_geometry_part_base
	{
		public TI.ShortInteger FirstSubpartIndex;
		public TI.ShortInteger SubpartCount;

		public global_geometry_part_block_new() : base(19)
		{
			Add(Type = new TI.Enum());
			Add(Flags = TI.Flags.Word);
			Add(Material = new TI.BlockIndex()); // 1 global_geometry_material_block
			Add(StripStartIndex = new TI.ShortInteger());
			Add(StripLength = new TI.ShortInteger());
			Add(FirstSubpartIndex = new TI.ShortInteger());
			Add(SubpartCount = new TI.ShortInteger());
			Add(MaxNodesVertex = new TI.ByteInteger());
			Add(ContributingCompoundNodeCount = new TI.ByteInteger());
			Centroid = new global_model_skinned_uncompressed_vertex(this, 3);
			Add(LodMipmapMagicNumber = new TI.Real());
			Add(_Skip = new TI.Skip(24));
		}

		internal void SetFromOld(global_geometry_part_block old)
		{
			Type.Value = old.Type.Value;
			Flags.Value = old.Flags.Value;
			Material.Value = old.Material.Value;
			StripStartIndex.Value = old.StripStartIndex.Value;
			StripLength.Value = old.StripLength.Value;
			FirstSubpartIndex.Value = 0;
			SubpartCount.Value = 1;
			MaxNodesVertex.Value = old.MaxNodesVertex.Value;
			ContributingCompoundNodeCount.Value = old.ContributingCompoundNodeCount.Value;
			Centroid.SetFrom(old.Centroid);
			LodMipmapMagicNumber.Value = old.LodMipmapMagicNumber.Value;
			_Skip.Data = old._Skip.Data; // NOTE: we can do this because noting ever modifies this skip buffer
		}
	}
	#endregion
	#endregion

	#region global_subparts_block
	[TI.Definition(1, 8)]
	public class global_subparts_block : TI.Definition
	{
		public TI.ShortInteger IndicesStartIndex, IndicesLength;
		public TI.ShortInteger VisibilityBoundsIndex,
			PartIndex;

		public global_subparts_block() : base(4)
		{
			Add(IndicesStartIndex = new TI.ShortInteger());
			Add(IndicesLength = new TI.ShortInteger());
			Add(VisibilityBoundsIndex = new TI.ShortInteger());
			Add(PartIndex = new TI.ShortInteger());
		}

		internal void SetFrom(global_geometry_part_block_new part)
		{
			IndicesStartIndex.Value = part.StripStartIndex.Value;
			IndicesLength.Value = part.StripLength.Value;
			VisibilityBoundsIndex.Value = part.StripStartIndex.Value;

			part.FirstSubpartIndex.Value = part.StripStartIndex.Value;
		}
	}
	#endregion

	#region global_visibility_bounds_block
	[TI.Definition(1, 20)]
	public class global_visibility_bounds_block : TI.Definition
	{
		public TI.RealPoint3D Position; // The tag definition actually declared this as three separate real fields
		public TI.Real Radius;
		public TI.ByteInteger Node0;

		public global_visibility_bounds_block() : base(4)
		{
			Add(Position = new TI.RealPoint3D());
			Add(Radius = new TI.Real());
			Add(Node0 = new TI.ByteInteger());
			Add(TI.Pad._24);
		}
	}
	#endregion

	#region global_geometry_section_raw_vertex_block
	[TI.Definition(1, 196)]
	public partial class global_geometry_section_raw_vertex_block : TI.Definition
	{
		#region Fields
		public global_geometry_raw_point Point;

		public TI.RealPoint2D Texcoord;
		public TI.RealVector3D Normal;
		public TI.RealVector3D Binormal;
		public TI.RealVector3D Tangent;
		public TI.RealVector3D AnisotropicBinormal;
		public TI.RealPoint2D SecondaryTexcoord;
		public TI.RealColor PrimaryLightmapColor;
		public TI.RealPoint2D PrimaryLightmapTexcoord;
		public TI.RealVector3D PrimaryLightmapIncidentDirection;
		#endregion

		public global_geometry_section_raw_vertex_block() : base(global_geometry_raw_point.kFieldCount + 10)
		{
			Point = new global_geometry_raw_point(this);					// 0x0

			Add(Texcoord = new TI.RealPoint2D());							// 0x44
			Add(Normal = new TI.RealVector3D());							// 0x4C
			Add(Binormal = new TI.RealVector3D());							// 0x58
			Add(Tangent = new TI.RealVector3D());							// 0x6C
			Add(AnisotropicBinormal = new TI.RealVector3D());				// 0x78
			Add(SecondaryTexcoord = new TI.RealPoint2D());					// 0x84
			Add(PrimaryLightmapColor = new TI.RealColor());					// 0x8C
			Add(PrimaryLightmapTexcoord = new TI.RealPoint2D());			// 0x98
			Add(PrimaryLightmapIncidentDirection = new TI.RealVector3D());	// 0xA0

			// Future Secondary LM Color, Texcoord and Incident Direction?
			Add(new TI.Pad(12 + 8 + 12));
		}
	}
	#endregion

	#region global_geometry_section_struct
	[TI.Struct((int)StructGroups.Enumerated.SECT, 2, 108)]
	public partial class global_geometry_section_struct : TI.Definition, IDisposable
	{
		const int kOldPartsIndex = 7; // field index of Parts in the old definition

		public TI.Block<global_geometry_part_block_new> Parts;
		public TI.Block<global_subparts_block> Subparts;
		public TI.Block<global_visibility_bounds_block> VisibilityBounds;
		public TI.Block<global_geometry_section_raw_vertex_block> RawVertices;
		public TI.Block<global_geometry_section_strip_index_block> StripIndices;
		public TI.Data VisibilityMoppCode;
		public TI.Block<global_geometry_section_strip_index_block> MoppReorderTable;
		public TI.Block<global_geometry_section_vertex_buffer_block> VertexBuffers;

		#region Upgrade
		void upgrade_build_version2_layout()
		{
			Add(Parts);
			Add(Subparts);
			Add(VisibilityBounds = new TI.Block<global_visibility_bounds_block>(this, 32768));
			Add(RawVertices);
			Add(StripIndices);
			Add(VisibilityMoppCode = new TI.Data(this));
			Add(MoppReorderTable = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
			Add(VertexBuffers);
			Add(TI.Pad.DWord);
		}
		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			if (attr.Major == 1)
			{
				// Clear the layout of the old definition
				var old_parts = this[kOldPartsIndex] as TI.Block<global_geometry_part_block>;
				this.Clear();

				// Initialize the new parts
				Parts = new TI.Block<global_geometry_part_block_new>(this, 255);
				Parts.Resize(old_parts.Count);
				for (int x = 0; x < Parts.Count; x++)
					Parts[x].SetFromOld(old_parts[x]);
				old_parts = null;

				// Initialize the subparts
				Subparts = new TI.Block<global_subparts_block>(this, 32768);
				Subparts.Resize(Parts.Count);
				for (int x = 0; x < Parts.Count; x++)
					Subparts[x].SetFrom(Parts[x]);

				// Initialize the layout to the newest definition
				upgrade_build_version2_layout();
			}

			return true;
		}
		#endregion

		#region Construct
		public global_geometry_section_struct() : base(9) { version2_construct(); }

		void version1_construct()
		{
			Add(RawVertices = new TI.Block<global_geometry_section_raw_vertex_block>(this, 32767));
			Add(new TI.VertexBuffer()); Add(new TI.VertexBuffer()); Add(new TI.VertexBuffer());
			Add(new TI.VertexBuffer()); // transparent position
			Add(VertexBuffers = new TI.Block<global_geometry_section_vertex_buffer_block>(this, 512));
			Add(StripIndices = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
			Add(/*Parts =*/ new TI.Block<global_geometry_part_block>(this, 255));
			Add(new TI.Pad(96));
		}
		void version2_construct()
		{
			Add(Parts = new TI.Block<global_geometry_part_block_new>(this, 255));
			Add(Subparts = new TI.Block<global_subparts_block>(this, 32768));
			Add(VisibilityBounds = new TI.Block<global_visibility_bounds_block>(this, 32768));
			Add(RawVertices = new TI.Block<global_geometry_section_raw_vertex_block>(this, 32767));
			Add(StripIndices = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
			Add(VisibilityMoppCode = new TI.Data(this));
			Add(MoppReorderTable = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
			Add(VertexBuffers = new TI.Block<global_geometry_section_vertex_buffer_block>(this, 512));
			Add(TI.Pad.DWord); // pointer to stuff when used in tools
		}
		[TI.VersionCtorHalo2(1, 272)]
		[TI.VersionCtorHalo2(2, 108)]
		public global_geometry_section_struct(int major, int minor) : base(9)
		{
			if (major == 1) version1_construct();
			else if (major == 2) version2_construct();
		}
		#endregion
	}
	#endregion

	#region global_geometry_point_data_struct
	[TI.Struct((int)StructGroups.Enumerated.PDAT, 1, 56)]
	public partial class global_geometry_point_data_struct : TI.Definition
	{
		#region global_geometry_raw_point_block
		[TI.Definition(1, 68)]
		public class global_geometry_raw_point_block : TI.Definition
		{
			public global_geometry_raw_point Point;

			public global_geometry_raw_point_block() : base(global_geometry_raw_point.kFieldCount)
			{
				Point = new global_geometry_raw_point(this);
			}
		}
		#endregion

		#region global_geometry_rigid_point_group_block
		[TI.Definition(1, 4)]
		public class global_geometry_rigid_point_group_block : TI.Definition
		{
			public TI.ByteInteger RigidNodeIndex, PointIndex;
			public TI.ShortInteger PointCount;

			public global_geometry_rigid_point_group_block() : base(3)
			{
				Add(RigidNodeIndex = new TI.ByteInteger());
				Add(PointIndex /*Nodes/Point*/ = new TI.ByteInteger());
				Add(PointCount = new TI.ShortInteger());
			}
		}
		#endregion

		// TODO: Consider moving this to a field_block<>
		#region global_geometry_point_data_index_block
		[TI.Definition(1, 2)]
		public class global_geometry_point_data_index_block : TI.Definition
		{
			public TI.ShortInteger Index;

			public global_geometry_point_data_index_block() : base(1)
			{
				Add(Index = new TI.ShortInteger());
			}
		}
		#endregion

		public TI.Block<global_geometry_raw_point_block> RawPoints;
		public TI.Data RuntimePointData;
		public TI.Block<global_geometry_rigid_point_group_block> RigidPointGroups;
		public TI.Block<global_geometry_point_data_index_block> VertexPointIndices;

		public global_geometry_point_data_struct() : base(4)
		{
			Add(RawPoints = new TI.Block<global_geometry_raw_point_block>(this, 32767));
			Add(RuntimePointData = new TI.Data(this));
			Add(RigidPointGroups = new TI.Block<global_geometry_rigid_point_group_block>(this, 32767));
			Add(VertexPointIndices = new TI.Block<global_geometry_point_data_index_block>(this, 32767));
		}
	}
	#endregion

	#region global_geometry_isq_info_struct
	[TI.Struct((int)StructGroups.Enumerated.ISQI, 1, 92)]
	public class global_geometry_isq_info_struct : TI.Definition
	{
		// TODO: Consider moving this to a field_block<>
		#region global_geometry_plane_block
		[TI.Definition(1, 16)]
		public class global_geometry_plane_block : TI.Definition
		{
			public TI.RealPlane3D Plane;

			public global_geometry_plane_block() : base(1)
			{
				Add(Plane = new TI.RealPlane3D());
			}
		}
		#endregion

		#region global_geometry_rigid_plane_group_block
		[TI.Definition(1, 4)]
		public class global_geometry_rigid_plane_group_block : TI.Definition
		{
			public TI.ByteInteger RigidNodeIndex, PartIndex;
			public TI.ShortInteger TriangleCount;

			public global_geometry_rigid_plane_group_block() : base(3)
			{
				Add(RigidNodeIndex = new TI.ByteInteger());
				Add(PartIndex = new TI.ByteInteger());
				Add(TriangleCount = new TI.ShortInteger());
			}
		}
		#endregion

		#region global_geometry_explicit_edge_block
		[TI.Definition(1, 8)]
		public class global_geometry_explicit_edge_block : TI.Definition
		{
			public TI.ShortInteger VertexIndex0, VertexIndex1,
				TriangleIndex0, TriangleIndex1;

			public global_geometry_explicit_edge_block() : base(1)
			{
				Add(VertexIndex0 = new TI.ShortInteger());
				Add(VertexIndex1 = new TI.ShortInteger());
				Add(TriangleIndex0 = new TI.ShortInteger());
				Add(TriangleIndex1 = new TI.ShortInteger());
			}
		}
		#endregion

		public TI.Block<global_geometry_plane_block> RawPlanes;
		public TI.Block<global_geometry_rigid_plane_group_block> RigidPlaneGroups;
		public TI.Block<global_geometry_explicit_edge_block> ExplicitEdges;

		public global_geometry_isq_info_struct() : base(7)
		{
			Add(TI.Flags.Word);
			Add(TI.Pad.Word);
			Add(RawPlanes = new TI.Block<global_geometry_plane_block>(this, 65535));
			Add(/*Runtime Plane Data = */ new TI.Data(this));
			Add(RigidPlaneGroups = new TI.Block<global_geometry_rigid_plane_group_block>(this, 65280));
			Add(new TI.Pad(32)); // this was probably padding for a vertex buffer
			Add(ExplicitEdges = new TI.Block<global_geometry_explicit_edge_block>(this, 65535));
		}
	};
	#endregion


	#region render_model_region_block
	[TI.Definition(1, 20)]
	public partial class render_model_region_block : TI.Definition
	{
		#region render_model_permutation_block
		[TI.Definition(1, 16)]
		public partial class render_model_permutation_block : TI.Definition
		{
			public TI.StringId Name;
			public TI.ShortInteger L1, L2, L3, L4, L5, L6;

			public render_model_permutation_block() : base(7)
			{
				Add(Name = new TI.StringId(true));
				Add(L1 = new TI.ShortInteger());
				Add(L2 = new TI.ShortInteger());
				Add(L3 = new TI.ShortInteger());
				Add(L4 = new TI.ShortInteger());
				Add(L5 = new TI.ShortInteger());
				Add(L6 = new TI.ShortInteger());
			}
		}
		#endregion

		public TI.StringId Name;
		public TI.ShortInteger NodeMapOffset, NodeMapSize;
		public TI.Block<render_model_permutation_block> Permutations;

		public render_model_region_block() : base(4)
		{
			Add(Name = new TI.StringId(true));
			Add(NodeMapOffset = new TI.ShortInteger());
			Add(NodeMapSize = new TI.ShortInteger());
			Add(Permutations = new TI.Block<render_model_permutation_block>(this, 32));
		}
	}
	#endregion

	#region render_model_section_block
	[TI.Definition(1, 104)]
	public partial class render_model_section_block : TI.Definition
	{
		#region render_model_section_data_block
		[TI.Definition(2, 180)]
		public partial class render_model_section_data_block : TI.Definition
		{
			#region render_model_forward_shared_edge_block & render_model_backward_shared_edge_block
			[TI.Definition(1, 2)]
			public class render_model_shared_edge_block : TI.Definition
			{
				public render_model_shared_edge_block() : base(1)
				{
					Add(/*triangle index = */ new TI.ShortInteger());
				}
			}
			#endregion

			#region render_model_shared_edge_group_block
			[TI.Definition(1, 8)]
			public class render_model_shared_edge_group_block : TI.Definition
			{
				public render_model_shared_edge_group_block() : base(6)
				{
					Add(/*first shared edge index = */ new TI.ShortInteger());
					Add(/*shared edge count = */ new TI.ShortInteger());
					Add(TI.Pad.Byte);
					Add(/*adjacent region index = */ new TI.ByteInteger());
					Add(TI.Pad.Byte);
					Add(/*section set index = */ new TI.ByteInteger());
				}
			}
			#endregion

			#region render_model_dsq_raw_vertex_block
			[TI.Definition(1, 32)]
			public class render_model_dsq_raw_vertex_block : TI.Definition
			{
				public render_model_dsq_raw_vertex_block() : base(3)
				{
					Add(/*position = */ new TI.RealPoint3D());
					Add(/*plane = */ new TI.RealPlane3D());
					Add(/*node index = */ new TI.LongInteger());
				}
			}
			#endregion

			#region render_model_dsq_silhouette_quad_block
			[TI.Definition(1, 8)]
			public class render_model_dsq_silhouette_quad_block : TI.Definition
			{
				public render_model_dsq_silhouette_quad_block() : base(1)
				{
					Add(/*vertex index = */ new TI.ShortInteger());
					Add(/*vertex index = */ new TI.ShortInteger());
					Add(/*vertex index = */ new TI.ShortInteger());
					Add(/*vertex index = */ new TI.ShortInteger());
				}
			}
			#endregion


			#region render_model_node_map_block
			[TI.Definition(1, 1)]
			public class render_model_node_map_block : TI.Definition
			{
				#region Fields
				public TI.ByteInteger NodeIndex;
				#endregion

				public render_model_node_map_block() : base(1)
				{
					Add(NodeIndex = new TI.ByteInteger());
				}
			}
			#endregion

			public TI.Struct<global_geometry_section_struct> Section;
			public TI.Struct<global_geometry_point_data_struct> PointData;
			public TI.Block<render_model_node_map_block> NodeMap;

			#region Upgrade
			internal override bool Upgrade()
			{
				TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
				if (attr.Major == 1)
				{
					this.Clear();
					Add(Section);
					Add(PointData);
					Add(NodeMap);
					Add(TI.Pad.DWord);
				}

				return true;
			}
			#endregion

			#region Construct
			public render_model_section_data_block() : base(4) { version2_construct(); }

			void version1_construct()
			{
				Add(Section = new TI.Struct<global_geometry_section_struct>(this));
				Add(PointData = new TI.Struct<global_geometry_point_data_struct>(this));
				Add(NodeMap = new TI.Block<render_model_node_map_block>(this, 40));

				Add(/*isq info =*/ new TI.Struct<global_geometry_isq_info_struct>(this));
				Add(/*forward shared edges =*/ new TI.Block<render_model_shared_edge_block>(this, 16384));
				Add(/*forward shared edge groups =*/ new TI.Block<render_model_shared_edge_group_block>(this, 512));
				Add(/*backward shared edges =*/ new TI.Block<render_model_shared_edge_block>(this, 16384));
				Add(/*backward shared edge groups =*/ new TI.Block<render_model_shared_edge_group_block>(this, 512));

				Add(/*raw vertices =*/ new TI.Block<render_model_dsq_raw_vertex_block>(this, 65536));
				Add(/*strip indices =*/ new TI.Block<global_geometry_section_strip_index_block/*render_model_dsq_strip_index_block*/>(this, 262144));
				Add(/*silhouette quads =*/ new TI.Block<render_model_dsq_silhouette_quad_block>(this, 65536));
				Add(/*Carmack-silhouette quad count = */ new TI.ShortInteger());
				Add(new TI.Pad(2 + 4));
			}
			void version2_construct()
			{
				Add(Section = new TI.Struct<global_geometry_section_struct>(this));
				Add(PointData = new TI.Struct<global_geometry_point_data_struct>(this));
				Add(NodeMap = new TI.Block<render_model_node_map_block>(this, 40));
				Add(TI.Pad.DWord); // pointer (to a 12 byte allocation) set\used by tools for post-processing, ignore
			}
			[TI.VersionCtorHalo2(1, 360)]
			[TI.VersionCtorHalo2(2, 180)]
			public render_model_section_data_block(int major, int minor) : base(13)
			{
				if (major == 1) version1_construct();
				else if (major == 2) version2_construct();
			}
			#endregion
		}
		#endregion

		public TI.Enum GeometryClassification;
		public TI.Struct<global_geometry_section_info_struct> SectionInfo;
		public TI.BlockIndex RigidNode;
		public TI.Flags Flags;
		public TI.Block<render_model_section_data_block> SectionData;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;

		public render_model_section_block() : base(7)
		{
			Add(GeometryClassification = new TI.Enum());
			Add(new TI.Pad(2));
			Add(SectionInfo = new TI.Struct<global_geometry_section_info_struct>(this));
			Add(RigidNode = new TI.BlockIndex()); // 1 render_model_node_block
			Add(Flags = TI.Flags.Word);
			Add(SectionData = new TI.Block<render_model_section_data_block>(this, 1));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
		}
	}
	#endregion

	#region render_model_invalid_section_pairs_block
	[TI.Definition(1, 4)]
	public class render_model_invalid_section_pairs_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public render_model_invalid_section_pairs_block() : base(1)
		{
			Add(/*bits = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region render_model_section_group_block
	[TI.Definition(1, 16)]
	public class render_model_section_group_block : TI.Definition
	{
		#region render_model_compound_node_block
		[TI.Definition(1, 16)]
		public class render_model_compound_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public render_model_compound_node_block() : base(7)
			{
				Add(/*node index = */ new TI.ByteInteger());
				Add(/*node index = */ new TI.ByteInteger());
				Add(/*node index = */ new TI.ByteInteger());
				Add(/*node index = */ new TI.ByteInteger());

				Add(/*node weight = */ new TI.Real());
				Add(/*node weight = */ new TI.Real());
				Add(/*node weight = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Flags DetailLevels;
		public TI.Block<render_model_compound_node_block> CompoundNodes;
		#endregion

		#region Ctor
		public render_model_section_group_block() : base(3)
		{
			Add(DetailLevels = new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(CompoundNodes = new TI.Block<render_model_compound_node_block>(this, 255));
		}
		#endregion
	}
	#endregion

	#region render_model_node_block
	[TI.Definition(1, 96)]
	public partial class render_model_node_block : TI.Definition
	{
		#region Fields
		public TI.StringId Name;
		public TI.BlockIndex ParentNode;
		public TI.BlockIndex FirstChildNode;
		public TI.BlockIndex NextSiblingNode;
		public TI.ShortInteger ImportNodeIndex;
		public TI.RealPoint3D DefaultTranslation;
		public TI.RealQuaternion DefaultRotation;
		public TI.RealVector3D InverseForward;
		public TI.RealVector3D InverseLeft;
		public TI.RealVector3D InverseUp;
		public TI.RealPoint3D InversePosition;
		public TI.Real InverseScale;
		public TI.Real DistFromParent;
		#endregion

		#region Ctor
		public render_model_node_block() : base(13)
		{
			Add(Name = new TI.StringId(true));
			Add(ParentNode = new TI.BlockIndex()); // 1 render_model_node_block
			Add(FirstChildNode = new TI.BlockIndex()); // 1 render_model_node_block
			Add(NextSiblingNode = new TI.BlockIndex()); // 1 render_model_node_block
			Add(ImportNodeIndex = new TI.ShortInteger());
			Add(DefaultTranslation = new TI.RealPoint3D());
			Add(DefaultRotation = new TI.RealQuaternion());
			Add(InverseForward = new TI.RealVector3D());
			Add(InverseLeft = new TI.RealVector3D());
			Add(InverseUp = new TI.RealVector3D());
			Add(InversePosition = new TI.RealPoint3D());
			Add(InverseScale = new TI.Real());
			Add(DistFromParent = new TI.Real());
		}
		#endregion
	}
	#endregion

	#region render_model_node_map_block_OLD
	[TI.Definition(1, 1)]
	public class render_model_node_map_block_OLD : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public render_model_node_map_block_OLD() : base(1)
		{
			Add(/*node index = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region render_model_marker_group_block
	[TI.Definition(1, 16)]
	public partial class render_model_marker_group_block : TI.Definition
	{
		#region render_model_marker_block
		[TI.Definition(1, 36)]
		public partial class render_model_marker_block : TI.Definition
		{
			#region Fields
			public TI.ByteInteger RegionIndex;
			public TI.ByteInteger PermutationIndex;
			public TI.ByteInteger NodeIndex;
			public TI.RealPoint3D Translation;
			public TI.RealQuaternion Rotation;
			public TI.Real Scale;
			#endregion

			#region Ctor
			public render_model_marker_block() : base(7)
			{
				Add(RegionIndex = new TI.ByteInteger());
				Add(PermutationIndex = new TI.ByteInteger());
				Add(NodeIndex = new TI.ByteInteger());
				Add(new TI.Pad(1));
				Add(Translation = new TI.RealPoint3D());
				Add(Rotation = new TI.RealQuaternion());
				Add(Scale = new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.StringId Name;
		public TI.Block<render_model_marker_block> Markers;
		#endregion

		#region Ctor
		public render_model_marker_group_block() : base(2)
		{
			Add(Name = new TI.StringId(true));
			Add(Markers = new TI.Block<render_model_marker_block>(this, 256));
		}
		#endregion
	}
	#endregion

	// PRT - Precomputed Radiance Transfer
	// PCA - Principal Component Analysis
	#region prt_info_block
	[TI.Definition(1, 108)]
	public partial class prt_info_block : TI.Definition
	{
		#region prt_lod_info_block
		[TI.Definition(1, 16)]
		public class prt_lod_info_block : TI.Definition
		{
			#region prt_section_info_block
			[TI.Definition(1, 8)]
			public class prt_section_info_block : TI.Definition
			{
				public TI.LongInteger SectionIndex, PcaDataOffset;

				public prt_section_info_block() : base(2)
				{
					Add(SectionIndex = new TI.LongInteger());
					Add(PcaDataOffset = new TI.LongInteger());
				}
			}
			#endregion

			public TI.LongInteger ClusterOffset;
			public TI.Block<prt_section_info_block> SectionInfo;

			public prt_lod_info_block() : base(2)
			{
				Add(ClusterOffset = new TI.LongInteger());
				Add(SectionInfo = new TI.Block<prt_section_info_block>(this, 255));
			}
		}
		#endregion

		// TODO: Consider moving this to a field_block<>
		#region prt_cluster_basis_block
		[TI.Definition(1, 4)]
		public class prt_cluster_basis_block : TI.Definition
		{
			public TI.Real BasisData;

			public prt_cluster_basis_block() : base(1)
			{
				Add(BasisData = new TI.Real());
			}
		}
		#endregion

		//Render.VertexBufferInterface.VertexBuffersHalo2.kTypePcaClusterId
		//Render.VertexBufferInterface.VertexBuffersHalo2.PcaVertexWeights
		// TODO: Consider moving this to a field_block<>
		#region prt_raw_pca_data_block
		[TI.Definition(1, 4)]
		public class prt_raw_pca_data_block : TI.Definition
		{
			public TI.Real RawPcaData;

			public prt_raw_pca_data_block() : base(1)
			{
				Add(RawPcaData = new TI.Real());
			}
		}
		#endregion

		// TODO: Consider moving this to a field_block<>
		#region prt_vertex_buffers_block
		[TI.Definition(1, 32)]
		public class prt_vertex_buffers_block : TI.Definition
		{
			public TI.VertexBuffer VertexBuffer;

			public prt_vertex_buffers_block() : base(1)
			{
				Add(VertexBuffer = new TI.VertexBuffer());
			}
		}
		#endregion

		#region Fields
		public TI.Block<prt_lod_info_block> LodInfo;
		public TI.Block<prt_cluster_basis_block> ClusterBasis;
		public TI.Block<prt_raw_pca_data_block> RawPcaData;
		public TI.Block<prt_vertex_buffers_block> VertexBuffers;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
		#endregion

		#region Ctor
		public prt_info_block() : base(14)
		{
			Add(/*SH Order = */ new TI.ShortInteger());
			Add(/*num of clusters = */ new TI.ShortInteger());
			Add(/*pca vectors per cluster = */ new TI.ShortInteger());
			Add(/*number of rays = */ new TI.ShortInteger());
			Add(/*number of bounces = */ new TI.ShortInteger());
			Add(/*mat index for sbsfc scattering = */ new TI.ShortInteger());
			Add(/*length scale = */ new TI.Real());
			Add(/*number of lods in model = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(LodInfo = new TI.Block<prt_lod_info_block>(this, 6));
			Add(ClusterBasis = new TI.Block<prt_cluster_basis_block>(this, 34560));
			Add(RawPcaData = new TI.Block<prt_raw_pca_data_block>(this, 150405120));
			Add(VertexBuffers = new TI.Block<prt_vertex_buffers_block>(this, 255));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
		}
		#endregion
	}
	#endregion

	#region section_render_leaves_block
	[TI.Definition(1, 12)]
	public class section_render_leaves_block : TI.Definition
	{
		#region node_render_leaves_block
		[TI.Definition(1, 24)]
		public class node_render_leaves_block : TI.Definition
		{
			TI.Block<bsp_leaf_block> CollisionLeaves;
			TI.Block<bsp_surface_reference_block> SurfaceReferences;

			public node_render_leaves_block() : base(2)
			{
				Add(CollisionLeaves = new TI.Block<bsp_leaf_block>(this, 65536));
				Add(SurfaceReferences = new TI.Block<bsp_surface_reference_block>(this, 262144));
			}
		}
		#endregion

		public TI.Block<node_render_leaves_block> NodeRenderLeaves;

		public section_render_leaves_block() : base(1)
		{
			Add(NodeRenderLeaves = new TI.Block<node_render_leaves_block>(this, 64));
		}
	}
	#endregion

	#region render_model
	[TI.TagGroup((int)TagGroups.Enumerated.mode, 5, 184)]
	public partial class render_model_group : TI.Definition, ITagImportInfo
	{
		#region Fields
		public TI.StringId Name;
		public TI.Flags Flags;
		public TI.Block<global_tag_import_info_block> ImportInfo;
		public TI.Block<global_geometry_compression_info_block> CompressionInfo;
		public TI.Block<render_model_region_block> Regions;
		public TI.Block<render_model_section_block> Sections;
		public TI.Block<render_model_invalid_section_pairs_block> InvalidSectionPairBits;
		public TI.Block<render_model_section_group_block> SectionGroups;
		public TI.ByteInteger SectionGroupIndex1;
		public TI.ByteInteger SectionGroupIndex2;
		public TI.ByteInteger SectionGroupIndex3;
		public TI.ByteInteger SectionGroupIndex4;
		public TI.ByteInteger SectionGroupIndex5;
		public TI.ByteInteger SectionGroupIndex6;
		public TI.LongInteger NodeListChecksum;
		public TI.Block<render_model_node_block> Nodes;
		public TI.Block<render_model_node_map_block_OLD> NodeMap;
		public TI.Block<render_model_marker_group_block> MarkerGroups;
		public TI.Block<global_geometry_material_block> Materials;
		public TI.Block<global_error_report_categories_block> Errors;
		public TI.Block<prt_info_block> PrtInfo;
		public TI.Block<section_render_leaves_block> SectionRenderLeaves;
		#endregion

		public render_model_group() : base(25)
		{
			Add(Name = new TI.StringId(true));
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 4));
			Add(ImportInfo = new TI.Block<global_tag_import_info_block>(this, 1));
			Add(CompressionInfo = new TI.Block<global_geometry_compression_info_block>(this, 1));
			Add(Regions = new TI.Block<render_model_region_block>(this, 16));
			Add(Sections = new TI.Block<render_model_section_block>(this, 255));
			Add(InvalidSectionPairBits = new TI.Block<render_model_invalid_section_pairs_block>(this, 1013));
			Add(SectionGroups = new TI.Block<render_model_section_group_block>(this, 6));
			Add(SectionGroupIndex1 = new TI.ByteInteger());
			Add(SectionGroupIndex2 = new TI.ByteInteger());
			Add(SectionGroupIndex3 = new TI.ByteInteger());
			Add(SectionGroupIndex4 = new TI.ByteInteger());
			Add(SectionGroupIndex5 = new TI.ByteInteger());
			Add(SectionGroupIndex6 = new TI.ByteInteger());
			Add(new TI.Pad(2));
			Add(NodeListChecksum = new TI.LongInteger());
			Add(Nodes = new TI.Block<render_model_node_block>(this, 255));
			Add(NodeMap = new TI.Block<render_model_node_map_block_OLD>(this, 640));
			Add(MarkerGroups = new TI.Block<render_model_marker_group_block>(this, 4096));
			Add(Materials = new TI.Block<global_geometry_material_block>(this, 1024));
			Add(Errors = new TI.Block<global_error_report_categories_block>(this, 64));
			Add(/*don't draw over camera cosine angle = */ new TI.Real());
			Add(PrtInfo = new TI.Block<prt_info_block>(this, 1));
			Add(SectionRenderLeaves = new TI.Block<section_render_leaves_block>(this, 255));
		}
	};
	#endregion
	#endregion
}