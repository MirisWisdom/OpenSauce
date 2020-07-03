/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region breakable_surface
	[TI.TagGroup((int)TagGroups.Enumerated.bsdt, 0, 52)]
	public class breakable_surface_group : TI.Definition
	{
		#region Fields
		public TI.Real MaximumVitality;
		public TI.TagReference Effect;
		public TI.TagReference Sound;
		public TI.Block<particle_system_definition_block_new> ParticleEffects;
		public TI.Real ParticleDensity;
		#endregion

		#region Ctor
		public breakable_surface_group() : base(5)
		{
			Add(MaximumVitality = new TI.Real());
			Add(Effect = new TI.TagReference(this, TagGroups.effe));
			Add(Sound = new TI.TagReference(this, TagGroups.snd_));
			Add(ParticleEffects = new TI.Block<particle_system_definition_block_new>(this, 32));
			Add(ParticleDensity = new TI.Real());
		}
		#endregion
	};
	#endregion

	#region detail_object_collection
	[TI.TagGroup((int)TagGroups.Enumerated.dobc, 1, 128)]
	public class detail_object_collection_group : TI.Definition
	{
		#region detail_object_type_block
		[TI.Definition(1, 96)]
		public class detail_object_type_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public detail_object_type_block() : base(14)
			{
				Add(/*Name = */ new TI.String());
				Add(/*Sequence Index = */ new TI.ByteInteger());
				Add(/*type flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(new TI.Pad(2));
				Add(/*Color Override Factor = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Pad(8));
				Add(/*Near Fade Distance = */ new TI.Real());
				Add(/*Far Fade Distance = */ new TI.Real());
				Add(/*Size = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*Minimum Color = */ new TI.RealColor());
				Add(/*Maximum Color = */ new TI.RealColor());
				Add(/*ambient color = */ new TI.Color());
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public detail_object_collection_group() : base(7)
		{
			Add(/*Collection Type = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*Global z Offset = */ new TI.Real());
			Add(new TI.Pad(44));
			Add(/*Sprite Plate = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*Types = */ new TI.Block<detail_object_type_block>(this, 16));
			Add(new TI.Pad(48));
		}
		#endregion
	};
	#endregion

	#region patchy_fog
	[TI.TagGroup((int)TagGroups.Enumerated.fpch, 1, 88)]
	public class patchy_fog_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public patchy_fog_group() : base(23)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(60));
			Add(/*rotation multiplier = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*strafing multiplier = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*zoom multiplier = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(32));
			Add(/*noise map scale = */ new TI.Real());
			Add(/*noise map = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*noise vertical scale forward = */ new TI.Real());
			Add(/*noise vertical scale up = */ new TI.Real());
			Add(/*noise opacity scale up = */ new TI.Real());
			Add(new TI.UselessPad(20));
			Add(/*animation period = */ new TI.Real());
			Add(new TI.UselessPad(4));
			Add(/*wind velocity = */ new TI.RealBounds());
			Add(/*wind period = */ new TI.RealBounds());
			Add(/*wind acceleration weight = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*wind perpendicular weight = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*wind constant velocity x = */ new TI.Real());
			Add(/*wind constant velocity y = */ new TI.Real());
			Add(/*wind constant velocity z = */ new TI.Real());
			Add(new TI.UselessPad(20));
		}
		#endregion
	};
	#endregion

	#region planar_fog
	[TI.TagGroup((int)TagGroups.Enumerated.fog, 1, 132)]
	public class planar_fog_group : TI.Definition
	{
		#region planar_fog_patchy_fog_block
		[TI.Definition(1, 60)]
		public class planar_fog_patchy_fog_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public planar_fog_patchy_fog_block() : base(8)
			{
				Add(/*color = */ new TI.RealColor());
				Add(new TI.Pad(12));
				Add(/*density = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
				Add(/*distance = */ new TI.RealBounds());
				Add(new TI.UselessPad(16));
				Add(/*min depth fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.UselessPad(12));
				Add(/*patchy fog = */ new TI.TagReference(this, TagGroups.fpch));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public planar_fog_group() : base(23)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*priority = */ new TI.ShortInteger());
			Add(/*global material name = */ new TI.StringId());
			Add(new TI.Pad(2 + 2));
			Add(new TI.UselessPad(72 + 4));
			Add(/*maximum density = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(4));
			Add(/*opaque distance = */ new TI.Real());
			Add(new TI.UselessPad(4));
			Add(/*opaque depth = */ new TI.Real());
			Add(/*atmospheric-planar depth = */ new TI.RealBounds());
			Add(/*eye offset scale = */ new TI.Real());
			Add(new TI.UselessPad(32));
			Add(/*color = */ new TI.RealColor());
			Add(new TI.UselessPad(100));
			Add(/*patchy fog = */ new TI.Block<planar_fog_patchy_fog_block>(this, 1));
			Add(/*background sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*sound environment = */ new TI.TagReference(this, TagGroups.snde));
			Add(/*environment damping factor = */ new TI.Real());
			Add(/*background sound gain = */ new TI.Real());
			Add(/*enter sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*exit sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.UselessPad(80));
		}
		#endregion
	};
	#endregion

	#region bsp_leaf_block
	[TI.Definition(1, 8)]
	public class bsp_leaf_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger Cluster;
		public TI.ShortInteger SurfaceReferenceCount;
		public TI.LongInteger FirstSurfaceReferenceIndex;
		#endregion

		#region Ctor
		public bsp_leaf_block() : base(3)
		{
			Add(Cluster = new TI.ShortInteger());
			Add(SurfaceReferenceCount = new TI.ShortInteger());
			Add(FirstSurfaceReferenceIndex = new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region bsp_surface_reference_block
	[TI.Definition(1, 8)]
	public class bsp_surface_reference_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger StripIndex;
		public TI.ShortInteger LightmapTriangleIndex;
		public TI.LongInteger BspNodeIndex;
		#endregion

		#region Ctor
		public bsp_surface_reference_block() : base(3)
		{
			Add(StripIndex = new TI.ShortInteger());
			Add(LightmapTriangleIndex = new TI.ShortInteger());
			Add(BspNodeIndex = new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region pathfinding_data_block
	[TI.Definition(1, 156)]
	public class pathfinding_data_block : TI.Definition
	{
		#region sector_block
		[TI.Definition(3, 8)]
		public class sector_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sector_block() : base(3)
			{
				Add(/*Path-finding sector flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*hint index = */ new TI.ShortInteger());
				Add(/*first link (do not set manually) = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region sector_link_block
		[TI.Definition(4, 16)]
		public class sector_link_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sector_link_block() : base(8)
			{
				Add(/*vertex 1 = */ new TI.ShortInteger());
				Add(/*vertex 2 = */ new TI.ShortInteger());
				Add(/*link flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*hint index = */ new TI.ShortInteger());
				Add(/*forward link = */ new TI.ShortInteger());
				Add(/*reverse link = */ new TI.ShortInteger());
				Add(/*left sector = */ new TI.ShortInteger());
				Add(/*right sector = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region ref_block
		[TI.Definition(1, 4)]
		public class ref_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ref_block() : base(1)
			{
				Add(/*node ref or sector ref = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region sector_bsp2d_nodes_block
		[TI.Definition(1, 20)]
		public class sector_bsp2d_nodes_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sector_bsp2d_nodes_block() : base(3)
			{
				Add(/*plane = */ new TI.RealPlane2D());
				Add(/*left child = */ new TI.LongInteger());
				Add(/*right child = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region surface_flags_block
		[TI.Definition(1, 4)]
		public class surface_flags_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public surface_flags_block() : base(1)
			{
				Add(/*flags = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region sector_vertex_block
		[TI.Definition(1, 12)]
		public class sector_vertex_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sector_vertex_block() : base(1)
			{
				Add(/*point = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region environment_object_refs
		[TI.Definition(2, 36)]
		public class environment_object_refs : TI.Definition
		{
			#region environment_object_bsp_refs
			[TI.Definition(2, 16)]
			public class environment_object_bsp_refs : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public environment_object_bsp_refs() : base(5)
				{
					Add(/*bsp reference = */ new TI.LongInteger());
					Add(/*first sector = */ new TI.LongInteger());
					Add(/*last sector = */ new TI.LongInteger());
					Add(/*node_index = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region environment_object_nodes
			[TI.Definition(2, 4)]
			public class environment_object_nodes : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public environment_object_nodes() : base(3)
				{
					Add(/*reference frame index = */ new TI.ShortInteger());
					Add(/*projection axis = */ new TI.ByteInteger());
					Add(/*projection sign = */ new TI.Flags(TI.FieldType.ByteFlags));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public environment_object_refs() : base(6)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*first sector = */ new TI.LongInteger());
				Add(/*last sector = */ new TI.LongInteger());
				Add(/*bsps = */ new TI.Block<environment_object_bsp_refs>(this, 1024));
				Add(/*nodes = */ new TI.Block<environment_object_nodes>(this, 255));
			}
			#endregion
		}
		#endregion

		#region pathfinding_hints_block
		[TI.Definition(4, 20)]
		public class pathfinding_hints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public pathfinding_hints_block() : base(10)
			{
				Add(/*hint type = */ new TI.Enum());
				Add(/*Next hint index = */ new TI.ShortInteger());
				Add(/*hint data 0 = */ new TI.ShortInteger());
				Add(/*hint data 1 = */ new TI.ShortInteger());
				Add(/*hint data 2 = */ new TI.ShortInteger());
				Add(/*hint data 3 = */ new TI.ShortInteger());
				Add(/*hint data 4 = */ new TI.ShortInteger());
				Add(/*hint data 5 = */ new TI.ShortInteger());
				Add(/*hint data 6 = */ new TI.ShortInteger());
				Add(/*hint data 7 = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region instanced_geometry_reference_block
		[TI.Definition(1, 4)]
		public class instanced_geometry_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public instanced_geometry_reference_block() : base(2)
			{
				Add(/*pathfinding object_index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region user_hint_block
		[TI.Definition(1, 108)]
		public class user_hint_block : TI.Definition
		{
			#region user_hint_point_block
			[TI.Definition(2, 16)]
			public class user_hint_point_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_point_block() : base(3)
				{
					Add(/*Point = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region user_hint_ray_block
			[TI.Definition(2, 28)]
			public class user_hint_ray_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_ray_block() : base(4)
				{
					Add(/*Point = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*Vector = */ new TI.RealVector3D());
				}
				#endregion
			}
			#endregion

			#region user_hint_line_segment_block
			[TI.Definition(2, 36)]
			public class user_hint_line_segment_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_line_segment_block() : base(7)
				{
					Add(/*Flags = */ new TI.Flags());
					Add(/*Point 0 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*Point 1 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region user_hint_parallelogram_block
			[TI.Definition(2, 68)]
			public class user_hint_parallelogram_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_parallelogram_block() : base(13)
				{
					Add(/*Flags = */ new TI.Flags());
					Add(/*Point 0 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*Point 1 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*Point 2 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*Point 3 = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region user_hint_polygon_block
			[TI.Definition(1, 16)]
			public class user_hint_polygon_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_polygon_block() : base(2)
				{
					Add(/*Flags = */ new TI.Flags());
					Add(/*Points = */ new TI.Block<user_hint_point_block>(this, 200));
				}
				#endregion
			}
			#endregion

			#region user_hint_jump_block
			[TI.Definition(1, 8)]
			public class user_hint_jump_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_jump_block() : base(4)
				{
					Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*geometry index = */ new TI.BlockIndex()); // 1 user_hint_parallelogram_block
					Add(/*force jump height = */ new TI.Enum());
					Add(/*control flags = */ new TI.Flags(TI.FieldType.WordFlags));
				}
				#endregion
			}
			#endregion

			#region user_hint_climb_block
			[TI.Definition(1, 4)]
			public class user_hint_climb_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public user_hint_climb_block() : base(2)
				{
					Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*geometry index = */ new TI.BlockIndex()); // 1 user_hint_line_segment_block
				}
				#endregion
			}
			#endregion

			#region user_hint_well_block
			[TI.Definition(1, 16)]
			public class user_hint_well_block : TI.Definition
			{
				#region user_hint_well_point_block
				[TI.Definition(1, 32)]
				public class user_hint_well_point_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public user_hint_well_point_block() : base(7)
					{
						Add(/*type = */ new TI.Enum());
						Add(new TI.Pad(2));
						Add(/*point = */ new TI.RealVector3D());
						Add(/*reference frame = */ new TI.ShortInteger());
						Add(new TI.Pad(2));
						Add(/*sector index = */ new TI.LongInteger());
						Add(/*normal = */ new TI.RealEulerAngles2D());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public user_hint_well_block() : base(2)
				{
					Add(/*flags = */ new TI.Flags());
					Add(/*points = */ new TI.Block<user_hint_well_point_block>(this, 200));
				}
				#endregion
			}
			#endregion

			#region user_hint_flight_block
			[TI.Definition(1, 12)]
			public class user_hint_flight_block : TI.Definition
			{
				#region user_hint_flight_point_block
				[TI.Definition(1, 12)]
				public class user_hint_flight_point_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public user_hint_flight_point_block() : base(1)
					{
						Add(/*point = */ new TI.RealVector3D());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public user_hint_flight_block() : base(2)
				{
					Add(new TI.UselessPad(4));
					Add(/*points = */ new TI.Block<user_hint_flight_point_block>(this, 10));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public user_hint_block() : base(11)
			{
				Add(new TI.UselessPad(48));
				Add(/*point geometry = */ new TI.Block<user_hint_point_block>(this, 200));
				Add(/*ray geometry = */ new TI.Block<user_hint_ray_block>(this, 200));
				Add(/*line segment geometry = */ new TI.Block<user_hint_line_segment_block>(this, 200));
				Add(/*parallelogram geometry = */ new TI.Block<user_hint_parallelogram_block>(this, 200));
				Add(/*polygon geometry = */ new TI.Block<user_hint_polygon_block>(this, 200));
				Add(new TI.UselessPad(48));
				Add(/*jump hints = */ new TI.Block<user_hint_jump_block>(this, 200));
				Add(/*climb hints = */ new TI.Block<user_hint_climb_block>(this, 200));
				Add(/*well hints = */ new TI.Block<user_hint_well_block>(this, 200));
				Add(/*flight hints = */ new TI.Block<user_hint_flight_block>(this, 50));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<sector_block> Sectors;
		public TI.Block<sector_link_block> Links;
		public TI.Block<ref_block> Refs;
		public TI.Block<sector_bsp2d_nodes_block> Bsp2dNodes;
		public TI.Block<surface_flags_block> SurfaceFlags;
		public TI.Block<sector_vertex_block> Vertices;
		public TI.Block<environment_object_refs> ObjectRefs;
		public TI.Block<pathfinding_hints_block> PathfindingHints;
		public TI.Block<instanced_geometry_reference_block> InstancedGeometryRefs;
		public TI.LongInteger StructureChecksum;
		public TI.Block<user_hint_block> UserplacedHints;
		#endregion

		#region Ctor
		public pathfinding_data_block() : base(12)
		{
			Add(Sectors = new TI.Block<sector_block>(this, 65534));
			Add(Links = new TI.Block<sector_link_block>(this, 262144));
			Add(Refs = new TI.Block<ref_block>(this, 131072));
			Add(Bsp2dNodes = new TI.Block<sector_bsp2d_nodes_block>(this, 131072));
			Add(SurfaceFlags = new TI.Block<surface_flags_block>(this, 4096));
			Add(Vertices = new TI.Block<sector_vertex_block>(this, 65535));
			Add(ObjectRefs = new TI.Block<environment_object_refs>(this, 2000));
			Add(PathfindingHints = new TI.Block<pathfinding_hints_block>(this, 32767));
			Add(InstancedGeometryRefs = new TI.Block<instanced_geometry_reference_block>(this, 1024));
			Add(StructureChecksum = new TI.LongInteger());
			Add(new TI.Pad(32));
			Add(UserplacedHints = new TI.Block<user_hint_block>(this, 1));
		}
		#endregion
	}
	#endregion

	#region structure_bsp_detail_object_data_block
	[TI.Definition(1, 52)]
	public class structure_bsp_detail_object_data_block : TI.Definition
	{
		#region global_detail_object_cells_block
		[TI.Definition(1, 32)]
		public class global_detail_object_cells_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_detail_object_cells_block() : base(8)
			{
				Add(/* = */ new TI.ShortInteger());
				Add(/* = */ new TI.ShortInteger());
				Add(/* = */ new TI.ShortInteger());
				Add(/* = */ new TI.ShortInteger());
				Add(/* = */ new TI.LongInteger());
				Add(/* = */ new TI.LongInteger());
				Add(/* = */ new TI.LongInteger());
				Add(new TI.Pad(12));
			}
			#endregion
		}
		#endregion

		#region global_detail_object_block
		[TI.Definition(1, 6)]
		public class global_detail_object_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_detail_object_block() : base(5)
			{
				Add(/* = */ new TI.ByteInteger());
				Add(/* = */ new TI.ByteInteger());
				Add(/* = */ new TI.ByteInteger());
				Add(/* = */ new TI.ByteInteger());
				Add(/* = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region global_detail_object_counts_block
		[TI.Definition(1, 2)]
		public class global_detail_object_counts_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_detail_object_counts_block() : base(1)
			{
				Add(/* = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region global_z_reference_vector_block
		[TI.Definition(1, 16)]
		public class global_z_reference_vector_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_z_reference_vector_block() : base(4)
			{
				Add(/* = */ new TI.Real());
				Add(/* = */ new TI.Real());
				Add(/* = */ new TI.Real());
				Add(/* = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_detail_object_data_block() : base(5)
		{
			Add(/*Cells = */ new TI.Block<global_detail_object_cells_block>(this, 262144));
			Add(/*Instances = */ new TI.Block<global_detail_object_block>(this, 2097152));
			Add(/*Counts = */ new TI.Block<global_detail_object_counts_block>(this, 8388608));
			Add(/*z Reference Vectors = */ new TI.Block<global_z_reference_vector_block>(this, 262144));
			Add(new TI.Pad(1 + 3));
		}
		#endregion
	}
	#endregion

	#region structure_bsp_cluster_data_block_new
	[TI.Definition(1, 108)]
	public partial class structure_bsp_cluster_data_block_new : TI.Definition
	{
		#region Fields
		public TI.Struct<global_geometry_section_struct> Section;
		#endregion

		#region Ctor
		public structure_bsp_cluster_data_block_new() : base(1)
		{
			Add(Section = new TI.Struct<global_geometry_section_struct>(this));
		}
		#endregion
	};
	#endregion

	#region structure_bsp_cluster_block
	[TI.Definition(3, 216)]
	public partial class structure_bsp_cluster_block : TI.Definition
	{
		#region structure_bsp_cluster_portal_index_block
		[TI.Definition(1, 2)]
		public class structure_bsp_cluster_portal_index_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_cluster_portal_index_block() : base(1)
			{
				Add(/*Portal Index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_cluster_instanced_geometry_index_block
		[TI.Definition(1, 2)]
		public class structure_bsp_cluster_instanced_geometry_index_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_cluster_instanced_geometry_index_block() : base(1)
			{
				Add(/*Instanced Geometry Index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Struct<global_geometry_section_info_struct> SectionInfo;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
		public TI.Block<structure_bsp_cluster_data_block_new> ClusterData;
		public TI.RealBounds BoundsX, BoundsY, BoundsZ;
		public TI.ByteInteger ScenarioSkyIndex;
		public TI.ByteInteger MediaIndex;
		public TI.ByteInteger ScenarioVisibleSkyIndex;
		public TI.ByteInteger ScenarioAtmosphericFogIndex;
		public TI.ByteInteger PlanarFogDesignator;
		public TI.ByteInteger VisibleFogPlaneIndex;
		public TI.BlockIndex BackgroundSound;
		public TI.BlockIndex SoundEnvironment;
		public TI.BlockIndex Weather;
		public TI.ShortInteger TransitionStructureBsp;
		public TI.Flags Flags;
		public TI.Block<predicted_resource_block> PredictedResources;
		public TI.Block<structure_bsp_cluster_portal_index_block> Portals;
		public TI.LongInteger ChecksumFromStructure;
		public TI.Block<structure_bsp_cluster_instanced_geometry_index_block> InstancedGeometryIndices;
		public TI.Block<global_geometry_section_strip_index_block> IndexReorderTable;
		public TI.Data CollisionMoppCode;
		#endregion

		#region Ctor
		public structure_bsp_cluster_block() : base(25)
		{
			Add(SectionInfo = new TI.Struct<global_geometry_section_info_struct>(this));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
			Add(ClusterData = new TI.Block<structure_bsp_cluster_data_block_new>(this, 1));
			Add(BoundsX = new TI.RealBounds());
			Add(BoundsY = new TI.RealBounds());
			Add(BoundsZ = new TI.RealBounds());
			Add(ScenarioSkyIndex = new TI.ByteInteger());
			Add(MediaIndex = new TI.ByteInteger());
			Add(ScenarioVisibleSkyIndex = new TI.ByteInteger());
			Add(ScenarioAtmosphericFogIndex = new TI.ByteInteger());
			Add(PlanarFogDesignator = new TI.ByteInteger());
			Add(VisibleFogPlaneIndex = new TI.ByteInteger());
			Add(BackgroundSound = new TI.BlockIndex()); // 1 structure_bsp_background_sound_palette_block
			Add(SoundEnvironment = new TI.BlockIndex()); // 1 structure_bsp_sound_environment_palette_block
			Add(Weather = new TI.BlockIndex()); // 1 structure_bsp_weather_palette_block
			Add(TransitionStructureBsp = new TI.ShortInteger());
			Add(new TI.Pad(2 + 4));
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));
			Add(Portals = new TI.Block<structure_bsp_cluster_portal_index_block>(this, 512));
			Add(ChecksumFromStructure = new TI.LongInteger());
			Add(InstancedGeometryIndices = new TI.Block<structure_bsp_cluster_instanced_geometry_index_block>(this, 1024));
			Add(IndexReorderTable = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
			Add(CollisionMoppCode = new TI.Data(this));
		}
		#endregion
	};
	#endregion

	#region visibility_struct
	[TI.Struct((int)StructGroups.Enumerated.svis, 1, 88)]
	public class visibility_struct : TI.Definition
	{
		#region Fields
		public TI.ShortInteger ProjectionCount;
		public TI.ShortInteger ClusterCount;
		public TI.ShortInteger VolumeCount;
		public TI.Data Projections;
		public TI.Data Visibility;
		public TI.Data ClusterRemapTable;
		public TI.Data VisibilityVolumes;
		#endregion

		#region Ctor
		public visibility_struct() : base(8)
		{
			Add(ProjectionCount = new TI.ShortInteger());
			Add(ClusterCount = new TI.ShortInteger());
			Add(VolumeCount = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(Projections = new TI.Data(this));
			Add(Visibility = new TI.Data(this));
			Add(ClusterRemapTable = new TI.Data(this));
			Add(VisibilityVolumes = new TI.Data(this));
		}
		#endregion
	}
	#endregion

	#region structure_instanced_geometry_render_info_struct
	[TI.Struct((int)StructGroups.Enumerated.igri, 2, 108)]
	public partial class structure_instanced_geometry_render_info_struct : TI.Definition
	{
		#region Fields
		public TI.Struct<global_geometry_section_info_struct> SectionInfo;
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
		public TI.Block<structure_bsp_cluster_data_block_new> RenderData;
		public TI.Block<global_geometry_section_strip_index_block> IndexReorderTable;
		#endregion

		#region Ctor
		public structure_instanced_geometry_render_info_struct() : base(4)
		{
			Add(SectionInfo = new TI.Struct<global_geometry_section_info_struct>(this));
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
			Add(RenderData = new TI.Block<structure_bsp_cluster_data_block_new>(this, 1));
			Add(IndexReorderTable = new TI.Block<global_geometry_section_strip_index_block>(this, 65535));
		}
		#endregion
	};
	#endregion

	#region structure_bsp_debug_info_block
	[TI.Definition(1, 100)]
	public class structure_bsp_debug_info_block : TI.Definition
	{
		#region structure_bsp_debug_info_render_line_block
		[TI.Definition(1, 32)]
		public class structure_bsp_debug_info_render_line_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_debug_info_render_line_block() : base(6)
			{
				Add(/*Type = */ new TI.Enum());
				Add(/*Code = */ new TI.ShortInteger());
				Add(/*Pad Thai = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*Point 0 = */ new TI.RealPoint3D());
				Add(/*Point 1 = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_debug_info_indices_block
		[TI.Definition(1, 4)]
		public class structure_bsp_debug_info_indices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_debug_info_indices_block() : base(1)
			{
				Add(/*Index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_cluster_debug_info_block
		[TI.Definition(1, 92)]
		public class structure_bsp_cluster_debug_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_cluster_debug_info_block() : base(8)
			{
				Add(/*Errors = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*Warnings = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(28));
				Add(/*Lines = */ new TI.Block<structure_bsp_debug_info_render_line_block>(this, 32767));
				Add(/*Fog Plane Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Visible Fog Plane Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Vis. Fog Omission Cluster Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Containing Fog Zone Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_fog_plane_debug_info_block
		[TI.Definition(1, 68)]
		public class structure_bsp_fog_plane_debug_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_fog_plane_debug_info_block() : base(6)
			{
				Add(/*Fog Zone Index = */ new TI.LongInteger());
				Add(new TI.Pad(24));
				Add(/*Connected Plane Designator = */ new TI.LongInteger());
				Add(/*Lines = */ new TI.Block<structure_bsp_debug_info_render_line_block>(this, 32767));
				Add(/*Intersected Cluster Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Inf. Extent Cluster Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_fog_zone_debug_info_block
		[TI.Definition(1, 80)]
		public class structure_bsp_fog_zone_debug_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_fog_zone_debug_info_block() : base(7)
			{
				Add(/*Media Index = */ new TI.LongInteger());
				Add(/*Base Fog Plane Index = */ new TI.LongInteger());
				Add(new TI.Pad(24));
				Add(/*Lines = */ new TI.Block<structure_bsp_debug_info_render_line_block>(this, 32767));
				Add(/*Immersed Cluster Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Bounding Fog Plane Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
				Add(/*Collision Fog Plane Indices = */ new TI.Block<structure_bsp_debug_info_indices_block>(this, 32767));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_debug_info_block() : base(4)
		{
			Add(new TI.Pad(64));
			Add(/*Clusters = */ new TI.Block<structure_bsp_cluster_debug_info_block>(this, 512));
			Add(/*Fog Planes = */ new TI.Block<structure_bsp_fog_plane_debug_info_block>(this, 127));
			Add(/*Fog Zones = */ new TI.Block<structure_bsp_fog_zone_debug_info_block>(this, 127));
		}
		#endregion
	}
	#endregion

	#region global_structure_physics_struct
	//structure physics
	[TI.Struct((int)StructGroups.Enumerated.spdf, 1, 80)]
	public class global_structure_physics_struct : TI.Definition
	{
		#region breakable_surface_key_table_block
		[TI.Definition(1, 32)]
		public class breakable_surface_key_table_block : TI.Definition
		{
			#region Fields
			public TI.ShortInteger InstancedGeometryIndex;
			public TI.ShortInteger BreakableSurfaceIndex;
			public TI.LongInteger SeedSurfaceIndex;
			public TI.Real X0, X1, Y0, Y1, Z0, Z1;
			#endregion

			#region Ctor
			public breakable_surface_key_table_block() : base(9)
			{
				Add(InstancedGeometryIndex = new TI.ShortInteger());
				Add(BreakableSurfaceIndex = new TI.ShortInteger());
				Add(SeedSurfaceIndex = new TI.LongInteger());
				Add(X0 = new TI.Real());
				Add(X1 = new TI.Real());
				Add(Y0 = new TI.Real());
				Add(Y1 = new TI.Real());
				Add(Z0 = new TI.Real());
				Add(Z1 = new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Data MoppCode;
		public TI.RealPoint3D MoppBoundsMin, MoppBoundsMax;
		public TI.Data BreakableSurfacesMoppCode;
		public TI.Block<breakable_surface_key_table_block> BreakableSurfaceKeyTable;
		#endregion

		#region Ctor
		public global_structure_physics_struct() : base(6)
		{
			Add(MoppCode = new TI.Data(this));
			Add(new TI.Pad(4));
			Add(MoppBoundsMin = new TI.RealPoint3D());
			Add(MoppBoundsMax = new TI.RealPoint3D());
			Add(BreakableSurfacesMoppCode = new TI.Data(this));
			Add(BreakableSurfaceKeyTable = new TI.Block<breakable_surface_key_table_block>(this, 8192));
		}
		#endregion
	}
	#endregion

	#region render_lighting_struct
	[TI.Struct((int)StructGroups.Enumerated.rnli, 1, 84)]
	public class render_lighting_struct : TI.Definition
	{
		#region Fields
		public TI.RealColor Ambient;
		public TI.RealVector3D ShadowDirection;
		public TI.Real LightingAccuracy;
		public TI.Real ShadowOpacity;
		public TI.RealColor PrimaryDirectionColor;
		public TI.RealVector3D PrimaryDirection;
		public TI.RealColor SecondaryDirectionColor;
		public TI.RealVector3D SecondaryDirection;
		public TI.ShortInteger ShIndex;
		#endregion

		#region Ctor
		public render_lighting_struct() : base(10)
		{
			Add(Ambient = new TI.RealColor());
			Add(ShadowDirection = new TI.RealVector3D());
			Add(LightingAccuracy = new TI.Real());
			Add(ShadowOpacity = new TI.Real());
			Add(PrimaryDirectionColor = new TI.RealColor());
			Add(PrimaryDirection = new TI.RealVector3D());
			Add(SecondaryDirectionColor = new TI.RealColor());
			Add(SecondaryDirection = new TI.RealVector3D());
			Add(ShIndex = new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region scenario_structure_bsp
	[TI.TagGroup((int)TagGroups.Enumerated.sbsp, 5, 792)]
	public partial class scenario_structure_bsp_group : TI.Definition, ITagImportInfo
	{
		#region structure_collision_materials_block
		[TI.Definition(1, 36)]
		public class structure_collision_materials_block : TI.Definition
		{
			#region Fields
			public TI.TagReference OldShader;
			public TI.BlockIndex ConveyorSurfaceIndex;
			public TI.TagReference NewShader;
			#endregion

			#region Ctor
			public structure_collision_materials_block() : base(4)
			{
				Add(OldShader = new TI.TagReference(this, TagGroups.shad));
				Add(new TI.Pad(2));
				Add(ConveyorSurfaceIndex = new TI.BlockIndex()); // 1 structure_bsp_conveyor_surface_block
				Add(NewShader = new TI.TagReference(this, TagGroups.shad));
			}
			#endregion
		}
		#endregion

		#region UNUSED_structure_bsp_node_block
		[TI.Definition(1, 6)]
		public class UNUSED_structure_bsp_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public UNUSED_structure_bsp_node_block() : base(1)
			{
				Add(new TI.Skip(6));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_leaf_block
		[TI.Definition(2, 8)]
		public class structure_bsp_leaf_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_leaf_block() : base(3)
			{
				Add(/*Cluster = */ new TI.ShortInteger());
				Add(/*Surface Reference Count = */ new TI.ShortInteger());
				Add(/*First Surface Reference Index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_surface_reference_block
		[TI.Definition(2, 8)]
		public class structure_bsp_surface_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_surface_reference_block() : base(3)
			{
				Add(/*Strip Index = */ new TI.ShortInteger());
				Add(/*Lightmap Triangle Index = */ new TI.ShortInteger());
				Add(/*BSP Node Index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_cluster_portal_block
		[TI.Definition(1, 40)]
		public class structure_bsp_cluster_portal_block : TI.Definition
		{
			#region structure_bsp_cluster_portal_vertex_block
			[TI.Definition(1, 12)]
			public class structure_bsp_cluster_portal_vertex_block : TI.Definition
			{
				#region Fields
				public TI.RealPoint3D Point;
				#endregion

				#region Ctor
				public structure_bsp_cluster_portal_vertex_block() : base(1)
				{
					Add(Point = new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			public TI.Block<structure_bsp_cluster_portal_vertex_block> Vertices;
			#endregion

			#region Ctor
			public structure_bsp_cluster_portal_block() : base(7)
			{
				Add(/*Back Cluster = */ new TI.ShortInteger());
				Add(/*Front Cluster = */ new TI.ShortInteger());
				Add(/*Plane Index = */ new TI.LongInteger());
				Add(/*Centroid = */ new TI.RealPoint3D());
				Add(/*Bounding Radius = */ new TI.Real());
				Add(/*Flags = */ new TI.Flags());
				Add(Vertices = new TI.Block<structure_bsp_cluster_portal_vertex_block>(this, 128));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_fog_plane_block
		[TI.Definition(1, 24)]
		public class structure_bsp_fog_plane_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_fog_plane_block() : base(5)
			{
				Add(/*Scenario Planar Fog Index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*plane = */ new TI.RealPlane3D());
				Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*Priority = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_weather_polyhedron_block
		[TI.Definition(1, 28)]
		public class structure_bsp_weather_polyhedron_block : TI.Definition
		{
			#region structure_bsp_weather_polyhedron_plane_block
			[TI.Definition(1, 16)]
			public class structure_bsp_weather_polyhedron_plane_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public structure_bsp_weather_polyhedron_plane_block() : base(1)
				{
					Add(/*plane = */ new TI.RealPlane3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_weather_polyhedron_block() : base(3)
			{
				Add(/*Bounding Sphere Center = */ new TI.RealPoint3D());
				Add(/*Bounding Sphere Radius = */ new TI.Real());
				Add(/*Planes = */ new TI.Block<structure_bsp_weather_polyhedron_plane_block>(this, 16));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_sky_owner_cluster_block
		[TI.Definition(1, 2)]
		public class structure_bsp_sky_owner_cluster_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_sky_owner_cluster_block() : base(1)
			{
				Add(/*Cluster Owner = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_conveyor_surface_block
		[TI.Definition(1, 24)]
		public class structure_bsp_conveyor_surface_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_conveyor_surface_block() : base(2)
			{
				Add(/* = */ new TI.RealVector3D());
				Add(/* = */ new TI.RealVector3D());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_breakable_surface_block
		[TI.Definition(1, 24)]
		public class structure_bsp_breakable_surface_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_breakable_surface_block() : base(5)
			{
				Add(/*Instanced Geometry Instance = */ new TI.BlockIndex()); // 1 structure_bsp_instanced_geometry_instances_block
				Add(/*Breakable Surface Index = */ new TI.ShortInteger());
				Add(/*Centroid = */ new TI.RealPoint3D());
				Add(/*Radius = */ new TI.Real());
				Add(/*Collision Surface Index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_pathfinding_edges_block
		[TI.Definition(1, 1)]
		public class structure_bsp_pathfinding_edges_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_pathfinding_edges_block() : base(1)
			{
				Add(/*Midpoint = */ new TI.ByteInteger());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_marker_block
		[TI.Definition(1, 60)]
		public class structure_bsp_marker_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.RealQuaternion Rotation;
			public TI.RealPoint3D Position;
			#endregion

			#region Ctor
			public structure_bsp_marker_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(Rotation = new TI.RealQuaternion());
				Add(Position = new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_runtime_decal_block
		[TI.Definition(1, 16)]
		public class structure_bsp_runtime_decal_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_runtime_decal_block() : base(1)
			{
				Add(new TI.Skip(16));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_environment_object_palette_block
		[TI.Definition(1, 36)]
		public class structure_bsp_environment_object_palette_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_environment_object_palette_block() : base(3)
			{
				Add(/*Definition = */ new TI.TagReference(this, TagGroups.scen));
				Add(/*Model = */ new TI.TagReference(this, TagGroups.mode));
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_environment_object_block
		[TI.Definition(1, 104)]
		public class structure_bsp_environment_object_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_environment_object_block() : base(8)
			{
				Add(/*Name = */ new TI.String());
				Add(/*Rotation = */ new TI.RealQuaternion());
				Add(/*Translation = */ new TI.RealPoint3D());
				Add(/*palette_index = */ new TI.BlockIndex()); // 1 structure_bsp_environment_object_palette_block
				Add(new TI.Pad(2));
				Add(/*Unique ID = */ new TI.LongInteger());
				Add(/*Exported Object Type = */ new TI.Tag());
				Add(/*Scenario Object Name = */ new TI.String());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_lightmap_data_block
		[TI.Definition(1, 16)]
		public class structure_bsp_lightmap_data_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_lightmap_data_block() : base(1)
			{
				Add(/*Bitmap Group = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region global_map_leaf_block
		[TI.Definition(1, 24)]
		public class global_map_leaf_block : TI.Definition
		{
			#region map_leaf_face_block
			[TI.Definition(1, 16)]
			public class map_leaf_face_block : TI.Definition
			{
				#region map_leaf_face_vertex_block
				[TI.Definition(1, 12)]
				public class map_leaf_face_vertex_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public map_leaf_face_vertex_block() : base(1)
					{
						Add(/*vertex = */ new TI.RealPoint3D());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public map_leaf_face_block() : base(2)
				{
					Add(/*node index = */ new TI.LongInteger());
					Add(/*vertices = */ new TI.Block<map_leaf_face_vertex_block>(this, 64));
				}
				#endregion
			}
			#endregion

			#region map_leaf_connection_index_block
			[TI.Definition(1, 4)]
			public class map_leaf_connection_index_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public map_leaf_connection_index_block() : base(1)
				{
					Add(/*connection index = */ new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public global_map_leaf_block() : base(2)
			{
				Add(/*faces = */ new TI.Block<map_leaf_face_block>(this, 512));
				Add(/*connection indices = */ new TI.Block<map_leaf_connection_index_block>(this, 512));
			}
			#endregion
		}
		#endregion

		#region global_leaf_connection_block
		[TI.Definition(1, 28)]
		public class global_leaf_connection_block : TI.Definition
		{
			#region leaf_connection_vertex_block
			[TI.Definition(1, 12)]
			public class leaf_connection_vertex_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public leaf_connection_vertex_block() : base(1)
				{
					Add(/*vertex = */ new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public global_leaf_connection_block() : base(5)
			{
				Add(/*plane index = */ new TI.LongInteger());
				Add(/*back leaf index = */ new TI.LongInteger());
				Add(/*front leaf index = */ new TI.LongInteger());
				Add(/*vertices = */ new TI.Block<leaf_connection_vertex_block>(this, 64));
				Add(/*area = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region structure_bsp_precomputed_lighting_block
		[TI.Definition(1, 96)]
		public class structure_bsp_precomputed_lighting_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_precomputed_lighting_block() : base(5)
			{
				Add(/*Index = */ new TI.LongInteger());
				Add(/*Light Type = */ new TI.Enum());
				Add(/*Attachment Index = */ new TI.ByteInteger());
				Add(/*Object Type = */ new TI.ByteInteger());
				Add(/*Visibility = */ new TI.Struct<visibility_struct>(this));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_instanced_geometry_definition_block
		[TI.Definition(1, 260)]
		public partial class structure_bsp_instanced_geometry_definition_block : TI.Definition
		{
			#region Fields
			public TI.Struct<structure_instanced_geometry_render_info_struct> RenderInfo;
			#endregion

			#region Ctor
			public structure_bsp_instanced_geometry_definition_block()
				: base(8)
			{
				Add(RenderInfo = new TI.Struct<structure_instanced_geometry_render_info_struct>(this));
				Add(/*Checksum = */ new TI.LongInteger());
				Add(/*Bounding Sphere Center = */ new TI.RealPoint3D());
				Add(/*Bounding Sphere Radius = */ new TI.Real());
				Add(/*Collision Info = */ new TI.Struct<global_collision_bsp_struct>(this));
				Add(/*bsp_physics = */ new TI.Block<collision_bsp_physics_block>(this, 1024));
				Add(/*Render Leaves = */ new TI.Block<structure_bsp_leaf_block>(this, 65536));
				Add(/*Surface References = */ new TI.Block<structure_bsp_surface_reference_block>(this, 262144));
			}
			#endregion
		};
		#endregion

		#region structure_bsp_instanced_geometry_instances_block
		[TI.Definition(3, 88)]
		public partial class structure_bsp_instanced_geometry_instances_block : TI.Definition
		{
			#region Fields
			public TI.Real Scale;
			public TI.RealVector3D Forward;
			public TI.RealVector3D Left;
			public TI.RealVector3D Up;
			public TI.RealPoint3D Position;
			public TI.BlockIndex InstanceDefinition;
			#endregion

			#region Ctor
			public structure_bsp_instanced_geometry_instances_block()
				: base(13)
			{
				Add(Scale = new TI.Real());
				Add(Forward = new TI.RealVector3D());
				Add(Left = new TI.RealVector3D());
				Add(Up = new TI.RealVector3D());
				Add(Position = new TI.RealPoint3D());
				Add(InstanceDefinition = new TI.BlockIndex()); // 1 structure_bsp_instanced_geometry_definition_block
				Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(4));
				Add(new TI.Skip(12 + 4));
				Add(/*Checksum = */ new TI.LongInteger());
				Add(/*Name = */ new TI.StringId());
				Add(/*Pathfinding Policy = */ new TI.Enum());
				Add(/*Lightmapping Policy = */ new TI.Enum());
			}
			#endregion
		};
		#endregion

		#region structure_bsp_sound_cluster_block
		[TI.Definition(1, 28)]
		public class structure_bsp_sound_cluster_block : TI.Definition
		{
			#region structure_sound_cluster_portal_designators
			[TI.Definition(1, 2)]
			public class structure_sound_cluster_portal_designators : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public structure_sound_cluster_portal_designators() : base(1)
				{
					Add(/*portal designator = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region structure_sound_cluster_interior_cluster_indices
			[TI.Definition(1, 2)]
			public class structure_sound_cluster_interior_cluster_indices : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public structure_sound_cluster_interior_cluster_indices() : base(1)
				{
					Add(/*interior cluster index = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_sound_cluster_block() : base(3)
			{
				Add(new TI.Pad(2 + 2));
				Add(/*enclosing portal designators = */ new TI.Block<structure_sound_cluster_portal_designators>(this, 512));
				Add(/*interior cluster indices = */ new TI.Block<structure_sound_cluster_interior_cluster_indices>(this, 512));
			}
			#endregion
		}
		#endregion

		#region transparent_planes_block
		[TI.Definition(1, 20)]
		public class transparent_planes_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public transparent_planes_block() : base(3)
			{
				Add(/*Section Index = */ new TI.ShortInteger());
				Add(/*Part Index = */ new TI.ShortInteger());
				Add(/*plane = */ new TI.RealPlane3D());
			}
			#endregion
		}
		#endregion

		#region global_water_definitions_block
		[TI.Definition(1, 188)]
		public partial class global_water_definitions_block : TI.Definition
		{
			#region water_geometry_section_block
			[TI.Definition(1, 108)]
			public class water_geometry_section_block : TI.Definition
			{
				public TI.Struct<global_geometry_section_struct> Section;

				public water_geometry_section_block() : base(1)
				{
					Add(Section = new TI.Struct<global_geometry_section_struct>(this));
				}
			}
			#endregion

			#region Fields
			public TI.Block<water_geometry_section_block> Section;
			public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
			#endregion

			#region Ctor
			public global_water_definitions_block() : base(23)
			{
				Add(/*Shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(Section = new TI.Block<water_geometry_section_block>(this, 1));
				Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
				Add(/*Sun Spot Color = */ new TI.RealColor());
				Add(/*Reflection Tint = */ new TI.RealColor());
				Add(/*Refraction Tint = */ new TI.RealColor());
				Add(/*Horizon Color = */ new TI.RealColor());
				Add(/*Sun Specular Power = */ new TI.Real());
				Add(/*Reflection Bump Scale = */ new TI.Real());
				Add(/*Refraction Bump Scale = */ new TI.Real());
				Add(/*Fresnel Scale = */ new TI.Real());
				Add(/*Sun Dir Heading = */ new TI.Real());
				Add(/*Sun Dir Pitch = */ new TI.Real());
				Add(/*FOV = */ new TI.Real());
				Add(/*Aspect = */ new TI.Real());
				Add(/*Height = */ new TI.Real());
				Add(/*Farz = */ new TI.Real());
				Add(/*rotate_offset = */ new TI.Real());
				Add(/*Center = */ new TI.RealVector2D());
				Add(/*Extents = */ new TI.RealVector2D());
				Add(/*Fog Near = */ new TI.Real());
				Add(/*Fog Far = */ new TI.Real());
				Add(/*dynamic_height_bias = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region structure_portal_device_mapping_block
		[TI.Definition(1, 24)]
		public class structure_portal_device_mapping_block : TI.Definition
		{
			#region structure_device_portal_association_block
			[TI.Definition(1, 12)]
			public class structure_device_portal_association_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public structure_device_portal_association_block() : base(3)
				{
					Add(/*device id = */ new TI.Struct<scenario_object_id_struct>(this));
					Add(/*first game portal index = */ new TI.ShortInteger());
					Add(/*game portal count = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region game_portal_to_portal_mapping_block
			[TI.Definition(1, 2)]
			public class game_portal_to_portal_mapping_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_portal_to_portal_mapping_block() : base(1)
				{
					Add(/*portal index = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public structure_portal_device_mapping_block() : base(2)
			{
				Add(/*device portal associations = */ new TI.Block<structure_device_portal_association_block>(this, 128));
				Add(/*game portal to portal map = */ new TI.Block<game_portal_to_portal_mapping_block>(this, 128));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_audibility_block
		[TI.Definition(1, 72)]
		public class structure_bsp_audibility_block : TI.Definition
		{
			#region door_encoded_pas_block
			[TI.Definition(1, 4)]
			public class door_encoded_pas_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public door_encoded_pas_block() : base(1)
				{
					Add(/* = */ new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region cluster_door_portal_encoded_pas_block
			[TI.Definition(1, 4)]
			public class cluster_door_portal_encoded_pas_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public cluster_door_portal_encoded_pas_block() : base(1)
				{
					Add(/* = */ new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region ai_deafening_encoded_pas_block
			[TI.Definition(1, 4)]
			public class ai_deafening_encoded_pas_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ai_deafening_encoded_pas_block() : base(1)
				{
					Add(/* = */ new TI.LongInteger());
				}
				#endregion
			}
			#endregion

			#region encoded_cluster_distances_block
			[TI.Definition(1, 1)]
			public class encoded_cluster_distances_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public encoded_cluster_distances_block() : base(1)
				{
					Add(/* = */ new TI.ByteInteger());
				}
				#endregion
			}
			#endregion

			#region occluder_to_machine_door_mapping
			[TI.Definition(1, 1)]
			public class occluder_to_machine_door_mapping : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public occluder_to_machine_door_mapping() : base(1)
				{
					Add(/*machine door index = */ new TI.ByteInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_audibility_block() : base(7)
			{
				Add(/*door portal count = */ new TI.LongInteger());
				Add(/*cluster distance bounds = */ new TI.RealBounds());
				Add(/*encoded door pas = */ new TI.Block<door_encoded_pas_block>(this, 4096));
				Add(/*cluster door portal encoded pas = */ new TI.Block<cluster_door_portal_encoded_pas_block>(this, 2048));
				Add(/*ai deafening pas = */ new TI.Block<ai_deafening_encoded_pas_block>(this, 4088));
				Add(/*cluster distances = */ new TI.Block<encoded_cluster_distances_block>(this, 130816));
				Add(/*machine door mapping = */ new TI.Block<occluder_to_machine_door_mapping>(this, 128));
			}
			#endregion
		}
		#endregion

		#region structure_bsp_fake_lightprobes_block
		[TI.Definition(1, 92)]
		public class structure_bsp_fake_lightprobes_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_bsp_fake_lightprobes_block() : base(2)
			{
				Add(/*Object Identifier = */ new TI.Struct<scenario_object_id_struct>(this));
				Add(/*Render Lighting = */ new TI.Struct<render_lighting_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<global_tag_import_info_block> ImportInfo;
		public TI.Block<structure_collision_materials_block> CollisionMaterials;
		public TI.Block<global_collision_bsp_block> CollisionBSP;
		public TI.Data ClusterData;
		public TI.Block<structure_bsp_cluster_portal_block> ClusterPortals;
		public TI.Block<structure_bsp_fog_plane_block> FogPlanes;
		public TI.Block<structure_bsp_cluster_block> Clusters;
		public TI.Block<global_geometry_material_block> Materials;
		public TI.Block<structure_bsp_marker_block> Markers;
		public TI.Block<global_error_report_categories_block> Errors;
		public TI.Block<structure_bsp_instanced_geometry_definition_block> InstancedGeometriesDefinitions;
		public TI.Block<structure_bsp_instanced_geometry_instances_block> InstancedGeometryInstances;
		public TI.Block<global_water_definitions_block> WaterDefinitions;
		public TI.Block<decorator_placement_definition_block> Decorators;
		#endregion

		#region Ctor
		public scenario_structure_bsp_group() : base(55)
		{
			Add(ImportInfo = new TI.Block<global_tag_import_info_block>(this, 1));
			Add(new TI.Pad(4));
			Add(CollisionMaterials = new TI.Block<structure_collision_materials_block>(this, 512));
			Add(CollisionBSP = new TI.Block<global_collision_bsp_block>(this, 1));
			Add(/*Vehicle Floor = */ new TI.Real());
			Add(/*Vehicle Ceiling = */ new TI.Real());
			Add(/*UNUSED nodes = */ new TI.Block<UNUSED_structure_bsp_node_block>(this, 131072));
			Add(/*Leaves = */ new TI.Block<structure_bsp_leaf_block>(this, 65536));
			Add(/*World Bounds x = */ new TI.RealBounds());
			Add(/*World Bounds y = */ new TI.RealBounds());
			Add(/*World Bounds z = */ new TI.RealBounds());
			Add(/*Surface References = */ new TI.Block<structure_bsp_surface_reference_block>(this, 262144));
			Add(ClusterData = new TI.Data(this));
			Add(ClusterPortals = new TI.Block<structure_bsp_cluster_portal_block>(this, 512));
			Add(FogPlanes = new TI.Block<structure_bsp_fog_plane_block>(this, 127));
			Add(new TI.Pad(24));
			Add(/*Weather Palette = */ new TI.Block<structure_bsp_weather_palette_block>(this, 32));
			Add(/*Weather Polyhedra = */ new TI.Block<structure_bsp_weather_polyhedron_block>(this, 32));
			Add(/*Detail Objects = */ new TI.Block<structure_bsp_detail_object_data_block>(this, 1));
			Add(Clusters = new TI.Block<structure_bsp_cluster_block>(this, 512));
			Add(Materials = new TI.Block<global_geometry_material_block>(this, 1024));
			Add(/*Sky Owner Cluster = */ new TI.Block<structure_bsp_sky_owner_cluster_block>(this, 32));
			Add(/*Conveyor Surfaces = */ new TI.Block<structure_bsp_conveyor_surface_block>(this, 512));
			Add(/*Breakable Surfaces = */ new TI.Block<structure_bsp_breakable_surface_block>(this, 8448));
			Add(/*Pathfinding Data = */ new TI.Block<pathfinding_data_block>(this, 16));
			Add(/*Pathfinding Edges = */ new TI.Block<structure_bsp_pathfinding_edges_block>(this, 262144));
			Add(/*Background Sound Palette = */ new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
			Add(/*Sound Environment Palette = */ new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
			Add(/*Sound PAS Data = */ new TI.Data(this));
			Add(Markers = new TI.Block<structure_bsp_marker_block>(this, 1024));
			Add(/*Runtime Decals = */ new TI.Block<structure_bsp_runtime_decal_block>(this, 6144));
			Add(/*Environment Object Palette = */ new TI.Block<structure_bsp_environment_object_palette_block>(this, 100));
			Add(/*Environment Objects = */ new TI.Block<structure_bsp_environment_object_block>(this, 16384));
			Add(/*Lightmaps = */ new TI.Block<structure_bsp_lightmap_data_block>(this, 128));
			Add(new TI.Pad(4));
			Add(/*Leaf Map Leaves = */ new TI.Block<global_map_leaf_block>(this, 65536));
			Add(/*Leaf Map Connections = */ new TI.Block<global_leaf_connection_block>(this, 524288));
			Add(Errors = new TI.Block<global_error_report_categories_block>(this, 64));
			Add(/*Precomputed Lighting = */ new TI.Block<structure_bsp_precomputed_lighting_block>(this, 350));
			Add(InstancedGeometriesDefinitions = new TI.Block<structure_bsp_instanced_geometry_definition_block>(this, 512));
			Add(InstancedGeometryInstances = new TI.Block<structure_bsp_instanced_geometry_instances_block>(this, 1024));
			Add(/*)Ambience Sound Clusters = */ new TI.Block<structure_bsp_sound_cluster_block>(this, 512));
			Add(/*)Reverb Sound Clusters = */ new TI.Block<structure_bsp_sound_cluster_block>(this, 512));
			Add(/*Transparent Planes = */ new TI.Block<transparent_planes_block>(this, 32768));
			Add(new TI.Pad(96));
			Add(/*Vehicle Sperical Limit Radius = */ new TI.Real());
			Add(/*Vehicle Sperical Limit Center = */ new TI.RealPoint3D());
			Add(/*Debug Info = */ new TI.Block<structure_bsp_debug_info_block>(this, 1));
			Add(/*Decorators = */ new TI.TagReference(this, TagGroups.DECP));
			Add(/*structure_physics = */ new TI.Struct<global_structure_physics_struct>(this));
			Add(WaterDefinitions = new TI.Block<global_water_definitions_block>(this, 1));
			Add(/*)portal=>device mapping = */ new TI.Block<structure_portal_device_mapping_block>(this, 1));
			Add(/*)Audibility = */ new TI.Block<structure_bsp_audibility_block>(this, 1));
			Add(/*)Object Fake Lightprobes = */ new TI.Block<structure_bsp_fake_lightprobes_block>(this, 2048));
			Add(Decorators = new TI.Block<decorator_placement_definition_block>(this, 1));
		}
		#endregion
	};
	#endregion


	#region structure_lightmap_group_block
	[TI.Definition(1, 156)]
	public partial class structure_lightmap_group_block : TI.Definition
	{
		#region structure_lightmap_palette_color_block
		[TI.Definition(1, 1024)]
		public class structure_lightmap_palette_color_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_lightmap_palette_color_block() : base(2)
			{
				Add(/*FIRST palette color = */ new TI.LongInteger());
				Add(new TI.Skip(1020));
			}
			#endregion
		}
		#endregion

		#region lightmap_geometry_section_block
		[TI.Definition(1, 96)]
		public partial class lightmap_geometry_section_block : TI.Definition
		{
			#region lightmap_geometry_section_cache_data_block
			[TI.Definition(1, 108)]
			public partial class lightmap_geometry_section_cache_data_block : TI.Definition
			{
				public TI.Struct<global_geometry_section_struct> Geometry;

				public lightmap_geometry_section_cache_data_block() : base(1)
				{
					Add(Geometry = new TI.Struct<global_geometry_section_struct>(this));
				}
			}
			#endregion

			public TI.Struct<global_geometry_section_info_struct> GeometryInfo;
			public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
			public TI.Block<lightmap_geometry_section_cache_data_block> CacheData;

			public lightmap_geometry_section_block() : base(3)
			{
				Add(GeometryInfo = new TI.Struct<global_geometry_section_info_struct>(this));
				Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
				Add(CacheData = new TI.Block<lightmap_geometry_section_cache_data_block>(this, 1));
			}
		}
		#endregion

		#region lightmap_geometry_render_info_block
		[TI.Definition(1, 4)]
		public class lightmap_geometry_render_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lightmap_geometry_render_info_block() : base(3)
			{
				Add(/*bitmap index = */ new TI.ShortInteger());
				Add(/*palette index = */ new TI.ByteInteger());
				Add(new TI.Pad(1));
			}
			#endregion
		}
		#endregion

		#region structure_lightmap_lighting_environment_block
		[TI.Definition(1, 220)]
		public class structure_lightmap_lighting_environment_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public structure_lightmap_lighting_environment_block() : base(40)
			{
				Add(/*sample_point = */ new TI.RealPoint3D());

				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());
				Add(/*red coefficient = */ new TI.Real());

				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());
				Add(/*green coefficient = */ new TI.Real());

				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());
				Add(/*blue coefficient = */ new TI.Real());

				Add(/*mean incoming light direction = */ new TI.RealVector3D());
				Add(/*incoming light intensity = */ new TI.RealPoint3D());
				Add(/*specular bitmap index = */ new TI.LongInteger());
				Add(/*rotation axis = */ new TI.RealVector3D());
				Add(/*rotation speed = */ new TI.Real());
				Add(/*bump direction = */ new TI.RealVector3D());
				Add(/*color tint = */ new TI.RealColor());
				Add(/*procedural overide = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*procedural param0 = */ new TI.RealVector3D());
				Add(/*procedural param1.xyz = */ new TI.RealVector3D());
				Add(/*procedural param1.w = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region lightmap_vertex_buffer_bucket_block
		[TI.Definition(1, 68)]
		public partial class lightmap_vertex_buffer_bucket_block : TI.Definition
		{
			[Flags]
			public enum lightmap_vertex_buffer_bucket_flags
			{
				IncidentDirection = 1 << 0,
				Color = 1 << 1,
			};

			#region lightmap_bucket_raw_vertex_block
			[TI.Definition(1, 24)]
			public partial class lightmap_bucket_raw_vertex_block : TI.Definition
			{
				public TI.RealColor PrimaryLightmapColor;
				public TI.RealVector3D PrimaryLightmapIncidentDirection;

				public lightmap_bucket_raw_vertex_block() : base(2)
				{
					Add(PrimaryLightmapColor = new TI.RealColor());
					Add(PrimaryLightmapIncidentDirection = new TI.RealVector3D());
				}
			}
			#endregion

			#region lightmap_vertex_buffer_bucket_cache_data_block
			[TI.Definition(1, 12)]
			public partial class lightmap_vertex_buffer_bucket_cache_data_block : TI.Definition
			{
				public TI.Block<global_geometry_section_vertex_buffer_block> VertexBuffers;

				public lightmap_vertex_buffer_bucket_cache_data_block() : base(1)
				{
					Add(VertexBuffers = new TI.Block<global_geometry_section_vertex_buffer_block>(this, 512));
				}
			}
			#endregion

			public TI.Flags Flags;
			public TI.Block<lightmap_bucket_raw_vertex_block> RawVertices;
			public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
			public TI.Block<lightmap_vertex_buffer_bucket_cache_data_block> CacheData;

			public lightmap_vertex_buffer_bucket_block() : base(5)
			{
				Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(RawVertices = new TI.Block<lightmap_bucket_raw_vertex_block>(this, 32767));
				Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
				Add(CacheData = new TI.Block<lightmap_vertex_buffer_bucket_cache_data_block>(this, 1));
			}
		}
		#endregion

		#region lightmap_instance_bucket_reference_block
		[TI.Definition(1, 16)]
		public class lightmap_instance_bucket_reference_block : TI.Definition
		{
			#region lightmap_instance_bucket_section_offset_block
			[TI.Definition(1, 2)]
			public class lightmap_instance_bucket_section_offset_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public lightmap_instance_bucket_section_offset_block() : base(1)
				{
					Add(/*section offset = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public lightmap_instance_bucket_reference_block() : base(3)
			{
				Add(/*flags = */ new TI.ShortInteger());
				Add(/*bucket index = */ new TI.ShortInteger());
				Add(/*section offsets = */ new TI.Block<lightmap_instance_bucket_section_offset_block>(this, 255));
			}
			#endregion
		}
		#endregion

		#region lightmap_scenery_object_info_block
		[TI.Definition(1, 12)]
		public class lightmap_scenery_object_info_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lightmap_scenery_object_info_block() : base(5)
			{
				Add(/*unique ID = */ new TI.LongInteger());
				Add(/*origin BSP index = */ new TI.ShortInteger());
				Add(/*type = */ new TI.ByteInteger());
				Add(/*source = */ new TI.ByteInteger());
				Add(/*render model checksum = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<lightmap_geometry_section_block> Clusters;
		#endregion

		#region Ctor
		public structure_lightmap_group_block() : base(15)
		{
			Add(/*type = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*structure checksum = */ new TI.LongInteger());
			Add(/*section palette = */ new TI.Block<structure_lightmap_palette_color_block>(this, 128));
			Add(/*writable palettes = */ new TI.Block<structure_lightmap_palette_color_block>(this, 128));
			Add(/*bitmap group = */ new TI.TagReference(this, TagGroups.bitm));
			Add(Clusters = new TI.Block<lightmap_geometry_section_block>(this, 512));
			Add(/*cluster render info = */ new TI.Block<lightmap_geometry_render_info_block>(this, 1024));
			Add(/*poop definitions = */ new TI.Block<lightmap_geometry_section_block>(this, 512));
			Add(/*lighting environments = */ new TI.Block<structure_lightmap_lighting_environment_block>(this, 1024));
			Add(/*geometry buckets = */ new TI.Block<lightmap_vertex_buffer_bucket_block>(this, 1024));
			Add(/*instance render info = */ new TI.Block<lightmap_geometry_render_info_block>(this, 1024));
			Add(/*instance bucket refs = */ new TI.Block<lightmap_instance_bucket_reference_block>(this, 2000));
			Add(/*scenery object info = */ new TI.Block<lightmap_scenery_object_info_block>(this, 2000));
			Add(/*scenery object bucket refs = */ new TI.Block<lightmap_instance_bucket_reference_block>(this, 2000));

			// TODO: Xbox seems to have 60 bytes of padding here
		}
		#endregion
	}
	#endregion

	#region scenario_structure_lightmap
	[TI.TagGroup((int)TagGroups.Enumerated.ltmp, 1, 268)]
	public partial class scenario_structure_lightmap_group : TI.Definition, ITagImportInfo
	{
		#region Fields
		public TI.RealBounds SearchDistance;
		public TI.Real LuminelsPerWorldUnit;
		public TI.Real OutputWhiteReference;
		public TI.Real OutputBlackReference;
		public TI.Real OutputSchlickParameter;
		public TI.Real DiffuseMapScale;
		public TI.Real SunScale;
		public TI.Real SkyScale;
		public TI.Real IndirectScale;
		public TI.Real PrtScale;
		public TI.Real SurfaceLightScale;
		public TI.Real ScenarioLightScale;
		public TI.Real LightprobeInterpolationOverride;
		public TI.Block<structure_lightmap_group_block> LightmapGroups;
		public TI.Block<global_error_report_categories_block> Errors;
		#endregion

		#region Ctor
		public scenario_structure_lightmap_group() : base(18)
		{
			Add(SearchDistance = new TI.RealBounds());
			Add(LuminelsPerWorldUnit = new TI.Real());
			Add(OutputWhiteReference = new TI.Real());
			Add(OutputBlackReference = new TI.Real());
			Add(OutputSchlickParameter = new TI.Real());
			Add(DiffuseMapScale = new TI.Real());
			Add(SunScale = new TI.Real());
			Add(SkyScale = new TI.Real());
			Add(IndirectScale = new TI.Real());
			Add(PrtScale = new TI.Real());
			Add(SurfaceLightScale = new TI.Real());
			Add(ScenarioLightScale = new TI.Real());
			Add(LightprobeInterpolationOverride = new TI.Real());
			Add(new TI.Pad(72));
			Add(LightmapGroups = new TI.Block<structure_lightmap_group_block>(this, 256));
			Add(new TI.Pad(12));
			Add(Errors = new TI.Block<global_error_report_categories_block>(this, 64));
			Add(new TI.Pad(104));
		}
		#endregion
	};
	#endregion


	#region sky
	[TI.TagGroup((int)TagGroups.Enumerated.sky_, 1, 220)]
	public class sky_group : TI.Definition
	{
		#region sky_cubemap_block
		[TI.Definition(1, 20)]
		public class sky_cubemap_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_cubemap_block() : base(2)
			{
				Add(/*Cube Map Reference = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*Power Scale = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region sky_atmospheric_fog_block
		[TI.Definition(1, 24)]
		public class sky_atmospheric_fog_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_atmospheric_fog_block() : base(4)
			{
				Add(/*Color = */ new TI.RealColor());
				Add(/*Maximum Density = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Start Distance = */ new TI.Real());
				Add(/*Opaque Distance = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region sky_fog_block
		[TI.Definition(1, 16)]
		public class sky_fog_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_fog_block() : base(2)
			{
				Add(/*Color = */ new TI.RealColor());
				Add(/*Density = */ new TI.Real(TI.FieldType.RealFraction));
			}
			#endregion
		}
		#endregion

		#region sky_patchy_fog_block
		[TI.Definition(1, 88)]
		public class sky_patchy_fog_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_patchy_fog_block() : base(6)
			{
				Add(/*Color = */ new TI.RealColor());
				Add(new TI.Pad(12));
				Add(/*Density = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
				Add(/*Distance = */ new TI.RealBounds());
				Add(new TI.Pad(32));
				Add(/*Patchy Fog = */ new TI.TagReference(this, TagGroups.fpch));
			}
			#endregion
		}
		#endregion

		#region sky_light_fog_block
		[TI.Definition(1, 44)]
		public class sky_light_fog_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_light_fog_block() : base(8)
			{
				Add(/*Color = */ new TI.RealColor());
				Add(/*Maximum Density = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Start Distance = */ new TI.Real());
				Add(/*Opaque Distance = */ new TI.Real());
				Add(/*Cone = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*Atmospheric Fog Influence = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Secondary Fog Influence = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Sky Fog Influence = */ new TI.Real(TI.FieldType.RealFraction));
			}
			#endregion
		}
		#endregion

		#region sky_radiosity_light_block
		[TI.Definition(1, 40)]
		public class sky_radiosity_light_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_radiosity_light_block() : base(6)
			{
				Add(/*Flags = */ new TI.Flags());
				Add(/*Color = */ new TI.RealColor());
				Add(/*Power = */ new TI.Real());
				Add(/*Test Distance = */ new TI.Real());
				Add(new TI.Pad(12));
				Add(/*Diameter = */ new TI.Real(TI.FieldType.Angle));
			}
			#endregion
		}
		#endregion

		#region sky_light_block
		[TI.Definition(1, 72)]
		public class sky_light_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_light_block() : base(6)
			{
				Add(/*Direction Vector = */ new TI.RealVector3D());
				Add(/*Direction = */ new TI.RealEulerAngles2D());
				Add(/*Lens Flare = */ new TI.TagReference(this, TagGroups.lens));
				Add(/*Fog = */ new TI.Block<sky_light_fog_block>(this, 1));
				Add(/*Fog Opposite = */ new TI.Block<sky_light_fog_block>(this, 1));
				Add(/*Radiosity = */ new TI.Block<sky_radiosity_light_block>(this, 1));
			}
			#endregion
		}
		#endregion

		#region sky_shader_function_block
		[TI.Definition(1, 36)]
		public class sky_shader_function_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_shader_function_block() : base(2)
			{
				Add(new TI.Pad(4));
				Add(/*Global Function Name = */ new TI.String());
			}
			#endregion
		}
		#endregion

		#region sky_animation_block
		[TI.Definition(1, 36)]
		public class sky_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sky_animation_block() : base(4)
			{
				Add(/*Animation Index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*Period = */ new TI.Real());
				Add(new TI.Pad(28));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public sky_group() : base(25)
		{
			Add(/*Render Model = */ new TI.TagReference(this, TagGroups.mode));
			Add(/*Animation Graph = */ new TI.TagReference(this, TagGroups.jmad));
			Add(/*Flags = */ new TI.Flags());
			Add(/*Render Model Scale = */ new TI.Real());
			Add(/*Movement Scale = */ new TI.Real());
			Add(/*Cube Map = */ new TI.Block<sky_cubemap_block>(this, 1));
			Add(/*Indoor Ambient Color = */ new TI.RealColor());
			Add(new TI.Pad(4));
			Add(/*Outdoor Ambient Color = */ new TI.RealColor());
			Add(new TI.Pad(4));
			Add(/*Fog Spread Distance = */ new TI.Real());
			Add(/*Atmospheric Fog = */ new TI.Block<sky_atmospheric_fog_block>(this, 1));
			Add(/*Secondary Fog = */ new TI.Block<sky_atmospheric_fog_block>(this, 1));
			Add(/*Sky Fog = */ new TI.Block<sky_fog_block>(this, 1));
			Add(/*Patchy Fog = */ new TI.Block<sky_patchy_fog_block>(this, 1));
			Add(/*Amount = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Brightness = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Gamma Power = */ new TI.Real());
			Add(/*Lights = */ new TI.Block<sky_light_block>(this, 8));
			Add(/*Global Sky Rotation = */ new TI.Real(TI.FieldType.Angle));
			Add(/*Shader Functions = */ new TI.Block<sky_shader_function_block>(this, 8));
			Add(/*Animations = */ new TI.Block<sky_animation_block>(this, 8));
			Add(new TI.Pad(12));
			Add(/*Clear Color = */ new TI.RealColor());
		}
		#endregion
	};
	#endregion
}