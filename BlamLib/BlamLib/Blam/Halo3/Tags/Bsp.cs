/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region pathfinding_data_block
	[TI.Definition(2, 164)]
	public class pathfinding_data_block : TI.Definition
	{
		// only verified size
		#region sector_block
		[TI.Definition(3, 8)]
		public class sector_block : TI.Definition
		{
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

		// only verified size
		#region sector_link_block
		[TI.Definition(4, 16)]
		public class sector_link_block : TI.Definition
		{
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

		// only verified size
		#region ref_block
		[TI.Definition(1, 4)]
		public class ref_block : TI.Definition
		{
			#region Ctor
			public ref_block() : base(1)
			{
				Add(/*node ref or sector ref = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		// only verified size
		#region sector_bsp2d_nodes_block
		[TI.Definition(1, 20)]
		public class sector_bsp2d_nodes_block : TI.Definition
		{
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

		// only verified size
		#region sector_vertex_block
		[TI.Definition(1, 12)]
		public class sector_vertex_block : TI.Definition
		{
			#region Ctor
			public sector_vertex_block() : base(1)
			{
				Add(/*point = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		// different
		#region environment_object_refs
		[TI.Definition(3, 24)]
		public class environment_object_refs : TI.Definition
		{
			#region Ctor
			public environment_object_refs() : base()
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(TI.UnknownPad.BlockHalo3); // [0x18]
					// unknown [0xC] (tag block? if so, then it may be environment_object_bsp_refs)
					// tag block [0x4]
					// dword
				Add(/*first sector = */ new TI.LongInteger());
				Add(/*last sector = */ new TI.LongInteger());
			}
			#endregion
		};
		#endregion

		// only verified size
		#region pathfinding_hints_block
		[TI.Definition(4, 20)]
		public class pathfinding_hints_block : TI.Definition
		{
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

		// only verified size
		#region instanced_geometry_reference_block
		[TI.Definition(1, 4)]
		public class instanced_geometry_reference_block : TI.Definition
		{
			#region Ctor
			public instanced_geometry_reference_block() : base(2)
			{
				Add(/*pathfinding object_index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		public TI.Block<sector_block> Sectors;
		public TI.Block<sector_link_block> Links;
		public TI.Block<ref_block> Refs;
		public TI.Block<sector_bsp2d_nodes_block> Bsp2dNodes;
		public TI.Block<sector_vertex_block> Vertices;
		public TI.Block<environment_object_refs> ObjectRefs;
		public TI.Block<pathfinding_hints_block> PathfindingHints;
		public TI.Block<instanced_geometry_reference_block> InstancedGeometryRefs;
		public TI.LongInteger StructureChecksum;

		#region Ctor
		public pathfinding_data_block() : base(13)
		{
			Add(Sectors = new TI.Block<sector_block>(this, 65534));
			Add(Links = new TI.Block<sector_link_block>(this, 262144));
			Add(Refs = new TI.Block<ref_block>(this, 131072));
			Add(Bsp2dNodes = new TI.Block<sector_bsp2d_nodes_block>(this, 131072));
			// no more surface flags block
			Add(Vertices = new TI.Block<sector_vertex_block>(this, 65535));
			Add(ObjectRefs = new TI.Block<environment_object_refs>(this, 2000)); // i think max is just 1 now?
			Add(PathfindingHints = new TI.Block<pathfinding_hints_block>(this, 32767));
			Add(InstancedGeometryRefs = new TI.Block<instanced_geometry_reference_block>(this, 1024));
			Add(StructureChecksum = new TI.LongInteger());
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // [0xC]
				// tag block [0x4] long
			Add(new TI.UnknownPad(28)); // maybe 2 tag blocks plus 1 dword?
		}
		#endregion
	};
	#endregion

	#region global_structure_physics_struct
	//structure physics
	[TI.Struct((int)StructGroups.Enumerated.spdf, 2, 52)]
	public class global_structure_physics_struct : TI.Definition
	{
		public TI.Block<mopp_code_block> MoppCode, MoppCode2;

		public global_structure_physics_struct() : base(6)
		{
			Add(MoppCode = new TI.Block<mopp_code_block>(this, 0));
			Add(new TI.Pad(4));
			Add(/*MoppBoundsMin =*/ new TI.RealPoint3D());
			Add(/*MoppBoundsMax =*/ new TI.RealPoint3D());
			Add(MoppCode2 = new TI.Block<mopp_code_block>(this, 0));
		}
	};
	#endregion

	#region structure_bsp_cluster_block
	[TI.Definition(4, 220)]
	public class structure_bsp_cluster_block : TI.Definition
	{
		public TI.RealBounds BoundsX, BoundsY, BoundsZ;
		public TI.ByteInteger ScenarioSkyIndex;
		public TI.ByteInteger MediaIndex;
		public TI.ByteInteger ScenarioVisibleSkyIndex;
		public TI.ByteInteger ScenarioAtmosphericFogIndex;

		public TI.BlockIndex BackgroundSound;
		public TI.BlockIndex SoundEnvironment;
		public TI.BlockIndex Weather;

		public TI.Block<mopp_code_block> CollisionMoppCode;

		public structure_bsp_cluster_block() : base()
		{
			Add(BoundsX = new TI.RealBounds());
			Add(BoundsY = new TI.RealBounds());
			Add(BoundsZ = new TI.RealBounds());
			Add(ScenarioSkyIndex = new TI.ByteInteger());
			Add(MediaIndex = new TI.ByteInteger());
			Add(ScenarioVisibleSkyIndex = new TI.ByteInteger());
			Add(ScenarioAtmosphericFogIndex = new TI.ByteInteger());
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger());
			Add(BackgroundSound = new TI.BlockIndex()); // 1 structure_bsp_background_sound_palette_block
			Add(SoundEnvironment = new TI.BlockIndex()); // 1 structure_bsp_sound_environment_palette_block
			Add(Weather = new TI.BlockIndex()); // 1 structure_bsp_weather_palette_block
			Add(new TI.ShortInteger()); // TransitionStructureBsp?
			Add(new TI.ShortInteger()); // block index?
			Add(new TI.BlockIndex()); // doesnt seem to be to anything in the actual bsp data
			Add(new TI.ShortInteger()); // flags?
			Add(new TI.Flags(TI.FieldType.WordFlags));
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // [0x2]
			Add(new TI.UnknownPad(4 + 20)); // '20' starts a hkShapeContainer object
			Add(new TI.TagReference(this, TagGroups.sbsp));
			Add(new TI.UnknownPad(32));
			Add(CollisionMoppCode = new TI.Block<mopp_code_block>(this, 0));
			Add(TI.UnknownPad.DWord);
			Add(TI.UnknownPad.BlockHalo3); // [0x4]
			// 0xAC
			Add(TI.UnknownPad.BlockHalo3); // [0x30]
				// short (count, w/e the elements are they are sizeof(0x10))
				// short (or two bytes)
				// long offset
				// real_point3d
				// real[4]
				// real_point3d
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // [0x4]
				// short
				// short
			Add(TI.UnknownPad.BlockHalo3); // [0x10]
				// real_point3d
				// short
				// short
		}
	};
	#endregion

	#region scenario_structure_bsp
	[TI.TagGroup((int)TagGroups.Enumerated.sbsp, 6, 904)]
	public class scenario_structure_bsp_group : TI.Definition
	{
		#region structure_collision_materials_block
		[TI.Definition(2, 24)]
		public class structure_collision_materials_block : TI.Definition
		{
			#region Ctor
			public structure_collision_materials_block()
			{
				Add(/*Shader = */ new TI.TagReference(this, TagGroups.rm__));
				//Add(new TI.Pad(2));
				//Add(/*Conveyor Surface Index = */ new TI.BlockIndex()); // 1 structure_bsp_conveyor_surface_block
				// TODO: ugh
				Add(new TI.UnknownPad(8)); // short, short index, short index, short
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
				#region Ctor
				public structure_bsp_cluster_portal_vertex_block()
				{
					Add(/*Point = */ new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Ctor
			public structure_bsp_cluster_portal_block()
			{
				Add(/*Back Cluster = */ new TI.ShortInteger());
				Add(/*Front Cluster = */ new TI.ShortInteger());
				Add(/*Plane Index = */ new TI.LongInteger());
				Add(/*Centroid = */ new TI.RealPoint3D());
				Add(/*Bounding Radius = */ new TI.Real());
				Add(/*Flags = */ new TI.Flags());
				Add(/*Vertices = */ new TI.Block<structure_bsp_cluster_portal_vertex_block>(this, 128));
			}
			#endregion
		}
		#endregion


		#region structure_bsp_sky_owner_cluster_block
		[TI.Definition(1, 2)]
		public class structure_bsp_sky_owner_cluster_block : TI.Definition
		{
			#region Ctor
			public structure_bsp_sky_owner_cluster_block()
			{
				Add(/*Cluster Owner = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion


		#region structure_bsp_marker_block
		[TI.Definition(1, 60)]
		public class structure_bsp_marker_block : TI.Definition
		{
			public structure_bsp_marker_block() : base(3)
			{
				Add(/*Name = */ new TI.String());
				Add(/*Rotation = */ new TI.RealQuaternion());
				Add(/*Position = */ new TI.RealPoint3D());
			}
		}
		#endregion


		#region structure_bsp_sound_environment_palette_block
		[TI.Definition(2, 84)]
		public class structure_bsp_sound_environment_palette_block : TI.Definition
		{
			public structure_bsp_sound_environment_palette_block()
			{
				Add(/*Name = */ new TI.StringId());
				Add(/*Sound Environment = */ new TI.TagReference(this, TagGroups.snde));
				Add(/*Cutoff Distance = */ new TI.Real());
				Add(/*Interpolation Speed = */ new TI.Real());
				Add(/*Background Sound = */ new TI.TagReference(this, TagGroups.lsnd));
				Add(/*Inside Cluster Sound = */ new TI.TagReference(this, TagGroups.lsnd));
				Add(/*Cutoff Distance = */ new TI.Real());
				Add(/*Scale Flags = */ new TI.Flags());
				Add(/*Interior Scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Portal Scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Exterior Scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Interpolation Speed = */ new TI.Real());
			}
		}
		#endregion

		#region structure_bsp_environment_object_palette_block
		[TI.Definition(1, 36)]
		public class structure_bsp_environment_object_palette_block : TI.Definition
		{
			public structure_bsp_environment_object_palette_block()
			{
				Add(/*Definition = */ new TI.TagReference(this, TagGroups.scen));
				Add(/*Model = */ new TI.TagReference(this, TagGroups.mode));
				Add(new TI.Pad(4));
			}
		}
		#endregion

		#region structure_bsp_environment_object_block
		[TI.Definition(2, 108)]
		public class structure_bsp_environment_object_block : TI.Definition
		{
			public structure_bsp_environment_object_block()
			{
				Add(/*Name = */ new TI.String());
				Add(/*Rotation = */ new TI.RealQuaternion());
				Add(/*Translation = */ new TI.RealPoint3D());
				// TODO: unknown
				Add(/*? = */ new TI.Real());
				Add(/*palette_index = */ new TI.BlockIndex()); // 1 structure_bsp_environment_object_palette_block
				Add(new TI.Pad(2));
				Add(/*Unique ID = */ new TI.LongInteger());
				Add(/*Exported Object Type = */ new TI.Tag());
				Add(/*Scenario Object Name = */ new TI.String());
			}
		}
		#endregion

		#region structure_bsp_sound_cluster_block
		[TI.Definition(1, 28)]
		public class structure_bsp_sound_cluster_block : TI.Definition
		{
			#region structure_sound_cluster_portal_designators
			[TI.Definition(1, 2)]
			public class structure_sound_cluster_portal_designators : TI.Definition
			{
				public structure_sound_cluster_portal_designators() : base(1)
				{
					Add(/*portal designator = */ new TI.ShortInteger());
				}
			}
			#endregion

			#region structure_sound_cluster_interior_cluster_indices
			[TI.Definition(1, 2)]
			public class structure_sound_cluster_interior_cluster_indices : TI.Definition
			{
				public structure_sound_cluster_interior_cluster_indices() : base(1)
				{
					Add(/*interior cluster index = */ new TI.ShortInteger());
				}
			}
			#endregion

			public structure_bsp_sound_cluster_block() : base(3)
			{
				Add(new TI.Pad(2 + 2)); // first 'word' is used (index prob), second I don't think so...
				Add(/*enclosing portal designators = */ new TI.Block<structure_sound_cluster_portal_designators>(this, 512));
				Add(/*interior cluster indices = */ new TI.Block<structure_sound_cluster_interior_cluster_indices>(this, 512));
			}
		}
		#endregion

		#region transparent_planes_block
		[TI.Definition(1, 20)]
		public class transparent_planes_block : TI.Definition
		{
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

		#region structure_bsp_instanced_geometry_instances_block
		[TI.Definition(4, 120)]
		public class structure_bsp_instanced_geometry_instances_block : TI.Definition
		{
			public TI.Real Scale;
			public TI.RealVector3D Forward;
			public TI.RealVector3D Left;
			public TI.RealVector3D Up;
			public TI.RealPoint3D Position;
			public TI.BlockIndex InstanceDefinition;

			public structure_bsp_instanced_geometry_instances_block() : base(20)
			{
				Add(Scale = new TI.Real());
				Add(Forward = new TI.RealVector3D());
				Add(Left = new TI.RealVector3D());
				Add(Up = new TI.RealVector3D());
				Add(Position = new TI.RealPoint3D());
				Add(InstanceDefinition = new TI.BlockIndex()); // 1 structure_bsp_instanced_geometry_definition_block
				Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.BlockIndex()); // index to sbsp->0x350
				Add(new TI.Pad(2 + // only seen this as zero, assuming its pad
					4)); // only seen this as zero, assuming its the pad from h2
				
				//Add(new TI.Skip(12 + 4 + 4)); // +4 more than h2. this is all 'real' data, not sure if its actually the same as h2
				Add(new TI.Real());
				Add(new TI.Real());
				Add(new TI.Real());
				Add(new TI.Real());
				Add(new TI.Real());
				
				Add(/*Name = */ new TI.StringId());
				Add(/*Pathfinding Policy = */ new TI.Enum());
				Add(/*Lightmapping Policy = */ new TI.Enum());
				Add(new TI.Real());

				Add(TI.UnknownPad.BlockHalo3); // [0x70] (pretty sure this is havok related, see offset 0x6)
				Add(TI.UnknownPad.BlockHalo3); // ??
			}
		};
		#endregion

		#region structure_decorator_palette_block
		[TI.Definition(1, 16)]
		public class structure_decorator_palette_block : TI.Definition
		{
			#region Ctor
			public structure_decorator_palette_block()
			{
				Add(/*Definition = */ new TI.TagReference(this, TagGroups.dctr));
			}
			#endregion
		}
		#endregion


		#region structure_bsp_instanced_geometry_definition_block
		[TI.Definition(2, 184)]
		public class structure_bsp_instanced_geometry_definition_block : TI.Definition
		{
			// structure_bsp_leaf_block?
			#region block_98
			[TI.Definition(1, 4)]
			public class block_98 : TI.Definition
			{
				public TI.ShortInteger
					Start, // (first surface reference index?)
					_Count; // (surface reference count?)

				public block_98() : base(2)
				{
					Add(Start = new TI.ShortInteger());
					Add(_Count = new TI.ShortInteger());
				}
			};
			#endregion

			// structure_bsp_surface_reference_block?
			#region block_A4
			[TI.Definition(1, 4)]
			public class block_A4 : TI.Definition
			{
				public TI.ShortInteger
					Unknown0, // (either strip index or lightmap tri index)
					Unknown2; // (most likely bsp node index)

				public block_A4() : base(2)
				{
					Add(Unknown0 = new TI.ShortInteger());
					Add(Unknown2 = new TI.ShortInteger());
				}
			};
			#endregion

			public TI.LongInteger Unknown00;
			public TI.RealPoint3D BoundingSphereCenter;
			public TI.Real BoundingSphereRadis;
			public TI.Struct<global_collision_bsp_struct> CollisionInfo;

			public TI.Block<block_98> Unknown98;
			public TI.Block<block_A4> UnknownA4;
			public TI.ShortInteger UnknownB0, UnknownB2;
			public TI.Real UnknownB4;

			public structure_bsp_instanced_geometry_definition_block()
			{
				Add(Unknown00 = new TI.LongInteger());
				Add(BoundingSphereCenter = new TI.RealPoint3D());
				Add(BoundingSphereRadis = new TI.Real());
				Add(CollisionInfo = new TI.Struct<global_collision_bsp_struct>(this));

				// collision_bsp_physics_struct
				Add(/*bsps = */ new TI.Block<global_collision_bsp_block>(this, 0));
				Add(new TI.Block<mopp_code_block>(this, 0));
				Add(TI.UnknownPad.BlockHalo3); // [0x?] (physics related)

				Add(Unknown98 = new TI.Block<block_98>(this, 0));
				Add(UnknownA4 = new TI.Block<block_A4>(this, 0));
				Add(UnknownB0 = new TI.ShortInteger());
				Add(UnknownB2 = new TI.ShortInteger());
				Add(UnknownB4 = new TI.Real());
			}
		};
		#endregion

		#region structure_bsp_tag_resources
		[TI.Definition(1, 24)]
		public class structure_bsp_tag_resources : TI.Definition
		{
			public TI.Block<global_collision_bsp_block> BspCollision;
			public TI.Block<structure_bsp_instanced_geometry_definition_block> InstancedGeometryDefinitions;

			public structure_bsp_tag_resources() : base(2)
			{
				Add(BspCollision = new TI.Block<global_collision_bsp_block>(this, 1));
				Add(InstancedGeometryDefinitions = new TI.Block<structure_bsp_instanced_geometry_definition_block>(this, 0));
			}
		};
		#endregion

		#region structure_bsp_tag_resources_odst
		[TI.Definition(2, 48)]
		public class structure_bsp_tag_resources_odst : TI.Definition
		{
			public TI.Block<global_collision_bsp_block> BspCollision;
			public TI.Block<structure_bsp_instanced_geometry_definition_block> InstancedGeometryDefinitions;

			public structure_bsp_tag_resources_odst() : base(4)
			{
				Add(BspCollision = new TI.Block<global_collision_bsp_block>(this, 1));
				Add(TI.UnknownPad.BlockHalo3); // [0x?]
				Add(InstancedGeometryDefinitions = new TI.Block<structure_bsp_instanced_geometry_definition_block>(this, 0));
				Add(TI.UnknownPad.BlockHalo3); // [0x?]
			}
		};
		#endregion

		#region Fields

		public TI.Block<structure_collision_materials_block> CollisionMaterials;

		public TI.Block<structure_bsp_cluster_portal_block> ClusterPortals;

		public TI.Block<structure_bsp_cluster_block> Clusters;

		public TI.Block<global_geometry_material_block> Materials;

		public TI.Block<structure_bsp_instanced_geometry_instances_block> InstancedGeometryInstances;

		public TI.LongInteger BspRenderResourcesIndex;

		public TI.LongInteger BspTagResourcesIndex;
		#endregion

		#region Ctor
		public scenario_structure_bsp_group()
		{
			Add(new TI.UnknownPad(12));
				// dword (structure checksum?)
				// long?
				// dword
			Add(TI.UnknownPad.BlockHalo3); // [0x28]
				// unknown [0x10] (seems when this is zero, the first tag block is filled with NONE, and the second isnt even used)
				// tag block [0x4]
					// long
				// tag block [0x16]
					// long
					// real_point3d
			Add(TI.UnknownPad.BlockHalo3); // [0x4]
				// long (pretty sure block index)
			Add(CollisionMaterials = new TI.Block<structure_collision_materials_block>(this, 512));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x1], Cluster Data?
			Add(/*World Bounds x = */ new TI.RealBounds());
			Add(/*World Bounds y = */ new TI.RealBounds());
			Add(/*World Bounds z = */ new TI.RealBounds());
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x4]
				// short
				// short
			// 0x60
			Add(TI.UnknownPad.BlockHalo3); // [0x4] Surface References?
				// short
				// short // sound enviorn palette ref?
			Add(ClusterPortals = new TI.Block<structure_bsp_cluster_portal_block>(this, 512));
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // [0x8] // fog related
				// string id
				// short
				// short
			Add(TI.UnknownPad.BlockHalo3); // camera fx
				// string id (name)
				// reference (cfxs, settings)
				// byte (enum?)
				// unknown [3]
				// real
				// dword (real?)
				// real
				// real
				// real
				// dword (real?)
			Add(TI.UnknownPad.BlockHalo3); // ?? Weather Polyhedra?
			// 0xA8
			Add(TI.UnknownPad.BlockHalo3); // [0x34] Detail Objects (pretty sure)
				// unknown [0x34] (only seen it as zeros)
			Add(Clusters = new TI.Block<structure_bsp_cluster_block>(this, 512));
			// 0xC0
			Add(Materials = new TI.Block<global_geometry_material_block>(this, 1024));
			Add(/*Sky Owner Cluster = */ new TI.Block<structure_bsp_sky_owner_cluster_block>(this, 32));
			Add(TI.UnknownPad.BlockHalo3); // conveyor surfaces
			Add(TI.UnknownPad.BlockHalo3); // breakable surfaces
			// 0xF0
			Add(/*Pathfinding Data = */ new TI.Block<pathfinding_data_block>(this, 16));
			Add(TI.UnknownPad.BlockHalo3); // pathfinding edges
			Add(/*Sound Environment Palette = */ new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
			// 0x114
			Add(new TI.UnknownPad(44)); // 1 tag data, 2 tag blocks but idk the order
			Add(/*Markers = */ new TI.Block<structure_bsp_marker_block>(this, 1024));
			Add(TI.UnknownPad.BlockHalo3); // ?? Runtime Decals?
			Add(TI.UnknownPad.BlockHalo3); // [0x2]
				// short (block index?)
				// short (i think this is a index too)
			Add(/*Compression Infos = */ new TI.Block<global_geometry_compression_info_block>(this, 0));
			// 0x170
			Add(/*Environment Object Palette = */ new TI.Block<structure_bsp_environment_object_palette_block>(this, 100));
			Add(/*Environment Objects = */ new TI.Block<structure_bsp_environment_object_block>(this, 16384));
			Add(TI.UnknownPad.BlockHalo3); // Old Lightmaps
			Add(new TI.Pad(4));
			Add(TI.UnknownPad.BlockHalo3); // Old Leaf Map Leaves
			Add(TI.UnknownPad.BlockHalo3); // Old Leaf Map Connections
			// 0x1B0
			Add(InstancedGeometryInstances = new TI.Block<structure_bsp_instanced_geometry_instances_block>(this, 1024));
			Add(/*Decorator Palette = */ new TI.Block<structure_decorator_palette_block>(this, 0));
			Add(TI.UnknownPad.DWord);
			Add(new TI.UnknownPad(120));
			Add(BspRenderResourcesIndex = new TI.LongInteger()); // resource id (render_geometry_api_resource_definition)
			Add(TI.UnknownPad.DWord);
			// 0x24C
			Add(TI.UnknownPad.BlockHalo3); // structure_bsp_sound_cluster_block (Ambience Sound Clusters?)
			Add(TI.UnknownPad.BlockHalo3); // structure_bsp_sound_cluster_block (Reverb Sound Clusters?)
			Add(TI.UnknownPad.BlockHalo3); // structure_bsp_sound_cluster_block (? Sound Clusters)
			// 0x270
			Add(/*Transparent Planes = */ new TI.Block<transparent_planes_block>(this, 32768));
			Add(TI.UnknownPad.BlockHalo3); // breakable_surface_key_table_block?
			Add(/*structure_physics = */ new TI.Struct<global_structure_physics_struct>(this));
			Add(TI.UnknownPad.BlockHalo3); // [0x20] (i think this may be cluster related)
				// short (block index)
				// byte
				// byte
				// long
				// real bounds[3] (xyz bounds)
			Add(new TI.UnknownPad(24));
			Add(TI.UnknownPad.DWord);
			
			// 0x2E4
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x4C] render_model_section_block
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x2C] global_geometry_compression_info_block
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x30]
				// unknown [0x10]
				// real[4] (might be a hkVector4 or just a real_point3d + real)
				// long (might byte 4 byte indexers)
				// real
				// real
				// real
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x18] functions
				// string id (input)
				// mapping_function
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x20] (count matches render_model_section_block)
				// tag data
			Add(TI.UnknownPad.BlockHalo3); // ?
			Add(TI.UnknownPad.BlockHalo3); // ? (should be a 0xC block)
			Add(TI.UnknownPad.BlockHalo3); // ?
			// 0x350
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10]
			// 0x35C
			Add(new TI.UnknownPad(8 + 12));
				// resource id (will point to a null item, render_geometry_api_resource_definition)
				// dword
				// tag block

			Add(TI.UnknownPad.BlockHalo3); // tag block? maybe this points to the structure_bsp_tag_resources block data in the zone tag
			
			Add(BspTagResourcesIndex = new TI.LongInteger()); // resource id (structure_bsp_tag_resources)
			Add(TI.Pad.DWord);
			
			Add(TI.Pad.DWord);
		}
		#endregion
	};
	#endregion

	#region structure_design
	[TI.TagGroup((int)TagGroups.Enumerated.sbsp, 1, 64)]
	public class structure_design_group : TI.Definition
	{
		#region structure_design_soft_ceiling_block
		[TI.Definition(1, 20)]
		public class structure_design_soft_ceiling_block : TI.Definition
		{
			public structure_design_soft_ceiling_block() : base(3)
			{
				Add(/*Name = */ new TI.StringId());
				Add(TI.UnknownPad.DWord);
				Add(TI.UnknownPad.BlockHalo3); // [0x44]
			}
		}
		#endregion

		public TI.Block<mopp_code_block> MoppCode;
		public TI.Block<structure_design_soft_ceiling_block> SoftCeilings;

		public structure_design_group() : base(6)
		{
			Add(new TI.LongInteger()); // flags?
			Add(MoppCode = new TI.Block<mopp_code_block>(this, 0)); // may be for soft ceilings?
			Add(SoftCeilings = new TI.Block<structure_design_soft_ceiling_block>(this, 0));

			// water physics
			Add(new TI.Block<mopp_code_block>(this, 0)); // only seen instanced when water physics present
			Add(TI.UnknownPad.BlockHalo3); // [0x4] water physics names
				// string id name
			Add(TI.UnknownPad.BlockHalo3); // [0x2C]
				// short (either self indexing or index to water physics names. money is on the later)
				// word (maybe flags, only seen as zero)
				// real
				// real
				// real
				// dword
				// tag block [0x10]
					// real 4d (i think this may be a hkVector4D due to 'w's usage)
				// tag block ??
		}
	};
	#endregion
}