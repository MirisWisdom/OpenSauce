/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region bsp3d_nodes_block
	[TI.Definition(1, 8)]
	public class bsp3d_nodes_block : TI.Definition
	{
		public TI.ShortInteger Plane;
		public TI.ShortInteger BackChild;
		public TI.ShortInteger FrontChild;

		public bsp3d_nodes_block()
		{
			Add(Plane = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(BackChild = new TI.ShortInteger());
			Add(FrontChild = new TI.ShortInteger());
		}
	}
	#endregion

	#region planes_block
	[TI.Definition(1, 16)]
	public class planes_block : TI.Definition
	{
		public TI.RealPlane3D Plane;

		public planes_block()
		{
			Add(Plane = new TI.RealPlane3D());
		}
	}
	#endregion

	#region leaves_block (halo1 style)
	[TI.Definition(2, 8)]
	public class leaves_block : TI.Definition
	{
		public TI.Flags Flags;
		public TI.ShortInteger Bsp2DReferenceCount;
		public TI.LongInteger FirstBsp2DReference;

		public leaves_block()
		{
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(Bsp2DReferenceCount = new TI.ShortInteger());
			Add(FirstBsp2DReference = new TI.LongInteger());
		}
	}
	#endregion

	#region bsp2d_references_block
	[TI.Definition(1, 4)]
	public class bsp2d_references_block : TI.Definition
	{
		public TI.ShortInteger Plane, Bsp2DNode;

		public bsp2d_references_block()
		{
			Add(Plane = new TI.ShortInteger());
			Add(Bsp2DNode = new TI.ShortInteger());
		}
	}
	#endregion

	#region bsp2d_nodes_block
	[TI.Definition(1, 16)]
	public class bsp2d_nodes_block : TI.Definition
	{
		public TI.RealPlane2D Plane;
		public TI.ShortInteger LeftChild, RightChild;
		
		public bsp2d_nodes_block()
		{
			Add(Plane = new TI.RealPlane2D());
			Add(LeftChild = new TI.ShortInteger());
			Add(RightChild = new TI.ShortInteger());
		}
	}
	#endregion

	#region surfaces_block (halo1 style)
	[TI.Definition(2, 12)]
	public class surfaces_block : TI.Definition
	{
		public TI.LongInteger Plane, FirstEdge;
		public TI.Flags Flags;
		public TI.ByteInteger BreakableSurface;
		public TI.ShortInteger Material;

		public surfaces_block()
		{
			Add(Plane = new TI.LongInteger());
			Add(FirstEdge = new TI.LongInteger());
			Add(Flags = new TI.Flags(TI.FieldType.ByteFlags));
			Add(BreakableSurface = new TI.ByteInteger());
			Add(Material = new TI.ShortInteger());
		}
	}
	#endregion

	#region edges_block
	[TI.Definition(2, 12)]
	public class edges_block : TI.Definition
	{
		public TI.ShortInteger StartVertex, EndVertex,
			ForwardEdge, ReverseEdge,
			LeftSurface, RightSurface;

		public edges_block()
		{
			Add(StartVertex = new TI.ShortInteger());
			Add(EndVertex = new TI.ShortInteger());
			Add(ForwardEdge = new TI.ShortInteger());
			Add(ReverseEdge = new TI.ShortInteger());
			Add(LeftSurface = new TI.ShortInteger());
			Add(RightSurface = new TI.ShortInteger());
		}
	}
	#endregion

	#region vertices_block
	[TI.Definition(1, 16)]
	public class vertices_block : TI.Definition
	{
		public TI.RealPoint3D Point;
		public TI.ShortInteger FirstEdge;

		public vertices_block()
		{
			Add(Point = new TI.RealPoint3D());
			Add(FirstEdge = new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
	}
	#endregion

	#region global_collision_bsp_struct
	[TI.Struct((int)StructGroups.Enumerated.cbsp, 3, 96)]
	public class global_collision_bsp_struct : TI.Definition
	{
		public global_collision_bsp_struct()
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
	}
	#endregion

	#region global_collision_bsp_block
	[TI.Definition(3, 96)]
	public class global_collision_bsp_block : TI.Definition
	{
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
	}
	#endregion

	#region mopp_code_block
	[TI.Definition(1, 64)]
	public class mopp_code_block : TI.Definition
	{
		public mopp_code_block()
		{
			#region hkpMoppCode
			Add(new TI.Skip(4)); // vtable
			Add(new TI.Skip(2 + 2)); // hkReferencedObject data
			Add(new TI.Pad(8)); // alignment

			// CodeInfo
			Add(new TI.RealQuaternion()); // m_offset (4th float is scale)

			// hkArray<hkUint8> m_data;
			Add(new TI.Skip(4)); // m_data
			Add(new TI.LongInteger()); // m_size
			Add(new TI.LongInteger()); // m_capacityAndFlags

			// hkEnum<BuildType, hkInt8> m_buildType;
			Add(TI.Enum.Byte);
			Add(TI.Pad._24); // alignment
			#endregion

			// code
			Add(new TI.Block<byte_block>(this, 0));
			Add(TI.Pad.DWord); // alignment
		}
	};
	#endregion

	#region collision_bsp_physics_struct
	[TI.Definition(1, 36)]
	public class collision_bsp_physics_struct : TI.Definition
	{
		public TI.Block<mopp_code_block> MoppCode;

		public collision_bsp_physics_struct() : base(3)
		{
			Add(/*bsps = */ new TI.Block<global_collision_bsp_block>(this, 0));
			Add(MoppCode = new TI.Block<mopp_code_block>(this, 0));
			Add(TI.UnknownPad.BlockHalo3); // [0x?] (physics related)
		}
	};
	#endregion

	#region collision_model
	[TI.TagGroup((int)TagGroups.Enumerated.coll, 11, 68)]
	public class collision_model_group : TI.Definition
	{
		#region collision_model_material_block
		[TI.Definition(1, 4)]
		public class collision_model_material_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public collision_model_material_block()
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
			[TI.Definition(3, 40)]
			public class collision_model_permutation_block : TI.Definition
			{
				#region collision_model_bsp_block
				[TI.Definition(1, 100)]
				public class collision_model_bsp_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public collision_model_bsp_block()
					{
						Add(/*node index = */ new TI.ShortInteger());
						Add(new TI.Pad(2));
						Add(/*bsp = */ new TI.Struct<global_collision_bsp_struct>(this));
					}
					#endregion
				}
				#endregion

				#region Ctor
				public collision_model_permutation_block()
				{
					Add(/*name = */ new TI.StringId());
					Add(/*bsps = */ new TI.Block<collision_model_bsp_block>(this, 64));
					// TODO: figure
					Add(TI.UnknownPad.BlockHalo3);//Add(/*bsp_physics = */ new TI.Block<collision_bsp_physics_block>(this, 1024));
					Add(TI.UnknownPad.BlockHalo3);
				}
				#endregion
			}
			#endregion

			#region Ctor
			public collision_model_region_block()
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
			#region Ctor
			public collision_model_pathfinding_sphere_block()
			{
				Add(/*node = */ new TI.BlockIndex()); // 1 collision_model_node_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
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
			#region Ctor
			public collision_model_node_block()
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
		#endregion

		#region Ctor
		public collision_model_group()
		{
			Add(new TI.UnknownPad(20)); // somewhere in here is a flags field, but in h2 there was only one flag and I'm pretty sure it was never used...
				// dword checksum?
			Add(/*materials = */ new TI.Block<collision_model_material_block>(this, 32));
			Add(/*regions = */ new TI.Block<collision_model_region_block>(this, 16));
			Add(/*pathfinding spheres = */ new TI.Block<collision_model_pathfinding_sphere_block>(this, 32));
			Add(/*nodes = */ new TI.Block<collision_model_node_block>(this, 255));
		}
		#endregion
	};
	#endregion


	#region animation_aiming_screen_struct
	[TI.Struct((int)StructGroups.Enumerated.aaim, 1, 24)]
	public class animation_aiming_screen_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_aiming_screen_struct()
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

	#region animation_graph_resources_struct
	[TI.Struct((int)StructGroups.Enumerated.MAgr, 4, 92)]
	public class animation_graph_resources_struct : TI.Definition
	{
		#region animation_graph_node_block
		[TI.Definition(2, 32)]
		public class animation_graph_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_graph_node_block()
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
			public animation_graph_sound_reference_block()
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
			public animation_graph_effect_reference_block()
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
			public animation_blend_screen_block()
			{
				Add(/*label = */ new TI.StringId());
				Add(/*aiming screen = */ new TI.Struct<animation_aiming_screen_struct>(this));
			}
			#endregion
		}
		#endregion

		#region animation_pool_block
		[TI.Definition(7, 136)]
		public class animation_pool_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public animation_pool_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*node list checksum = */ new TI.LongInteger());
				Add(/*production or import checksum = */ new TI.LongInteger());
				Add(/*blend screen = */ new TI.BlockIndex(TI.FieldType.ByteBlockIndex)); // 1 animation_blend_screen_block
				Add(/*type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*frame info type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*node count = */ new TI.ByteInteger());
				Add(/*frame count = */ new TI.ShortInteger());

				// TODO GODDAMNIT!
				Add(new TI.UnknownPad(118));
			}
			#endregion
		};
		#endregion

		#region Fields
		public TI.Block<animation_pool_block> Animations;
		#endregion

		#region Ctor
		public animation_graph_resources_struct()
		{
			Add(/*parent animation graph = */ new TI.TagReference(this, TagGroups.jmad));
			Add(/*inheritance flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*private flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*animation codec pack = */ new TI.ShortInteger());
			Add(/*skeleton nodes = */ new TI.Block<animation_graph_node_block>(this, 255));
			Add(/*sound references = */ new TI.Block<animation_graph_sound_reference_block>(this, 512));
			Add(/*effect references = */ new TI.Block<animation_graph_effect_reference_block>(this, 512));
			Add(/*blend screens = */ new TI.Block<animation_blend_screen_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // [0x] ?
			Add(Animations = new TI.Block<animation_pool_block>(this, 2048));
		}
		#endregion
	};
	#endregion

	#region animation_graph_contents_struct
	[TI.Struct((int)StructGroups.Enumerated.MAgc, 1, 36)]
	public class animation_graph_contents_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public animation_graph_contents_struct()
		{
		}
		#endregion
	};
	#endregion

	#region model_animation_graph
	[TI.TagGroup((int)TagGroups.Enumerated.jmad, 2, 236)]
	public class model_animation_graph_group : TI.Definition
	{
		#region Fields
		public TI.Struct<animation_graph_resources_struct> Resources;
		#endregion

		#region Ctor
		public model_animation_graph_group()
		{
			Add(Resources = new TI.Struct<animation_graph_resources_struct>(this));
		}
		#endregion
	};
	#endregion


	#region constraint_bodies_struct
	//constraint bodies
	[TI.Struct((int)StructGroups.Enumerated.csbs, 1, 116)]
	public class constraint_bodies_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public constraint_bodies_struct()
		{
			Add(/*name = */ new TI.StringId());
			Add(/*node a = */ new TI.BlockIndex()); // 1 nodes_block
			Add(/*node b = */ new TI.BlockIndex()); // 1 nodes_block
			Add(/*a scale = */ new TI.Real());
			Add(/*a forward = */ new TI.RealVector3D());
			Add(/*a left = */ new TI.RealVector3D());
			Add(/*a up = */ new TI.RealVector3D());
			Add(/*a position = */ new TI.RealPoint3D());
			Add(/*b scale = */ new TI.Real());
			Add(/*b forward = */ new TI.RealVector3D());
			Add(/*b left = */ new TI.RealVector3D());
			Add(/*b up = */ new TI.RealVector3D());
			Add(/*b position = */ new TI.RealPoint3D());
			Add(/*edge index = */ new TI.BlockIndex()); // 1 physics_model_node_constraint_edge_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region physics_model
	[TI.TagGroup((int)TagGroups.Enumerated.phmo, 3, 408)] // size is currently a GUESS
	public class physics_model_group : TI.Definition
	{
		#region physics_model_node_constraint_edge_block
		[TI.Definition(1, 28)]
		public class physics_model_node_constraint_edge_block : TI.Definition
		{
			#region physics_model_constraint_edge_constraint_block
			[TI.Definition(2, 36)]
			public class physics_model_constraint_edge_constraint_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public physics_model_constraint_edge_constraint_block()
				{
					Add(/*type = */ new TI.Enum());
					Add(/*index = */ new TI.BlockIndex()); // 2
					Add(/*flags = */ new TI.Flags());
					Add(/*friction = */ new TI.Real());
					// TODO: unknown
					Add(TI.UnknownPad.BlockHalo3);
					Add(TI.UnknownPad.BlockHalo3); // [0x?]
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public physics_model_node_constraint_edge_block()
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
		[TI.Definition(3, 176)]
		public class rigid_bodies_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public rigid_bodies_block()
			{
				Add(/*node = */ new TI.BlockIndex()); // 1 nodes_block
				Add(/*region = */ new TI.BlockIndex()); // 1 regions_block
				Add(/*permutattion = */ new TI.BlockIndex()); // 2
				Add(new TI.Pad(2));
				Add(/*bouding sphere offset = */ new TI.RealPoint3D());
				Add(/*bounding sphere radius = */ new TI.Real());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*motion type = */ new TI.Enum());
				Add(/*no phantom power alt = */ new TI.BlockIndex()); // 1 rigid_bodies_block
				Add(/*size = */ new TI.Enum());
				Add(/*inertia tensor scale = */ new TI.Real());
				//Add(/*linear damping = */ new TI.Real());
				//Add(/*angular damping = */ new TI.Real());
				//Add(/*center off mass offset = */ new TI.RealVector3D());
				// TODO: figure out where these values are
				Add(new TI.UnknownPad(52));
				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*mass = */ new TI.Real());
				Add(/*center of mass = */ new TI.RealVector3D());	Add(new TI.Skip(4));
				Add(/*intertia tensor x = */ new TI.RealVector3D());Add(new TI.Skip(4));
				Add(/*intertia tensor y = */ new TI.RealVector3D());Add(new TI.Skip(4));
				Add(/*intertia tensor z = */ new TI.RealVector3D());Add(new TI.Skip(4));
				Add(/*bounding sphere pad = */ new TI.Real());		Add(new TI.Pad(12));
			}
			#endregion
		}
		#endregion

		// I THINK this is still only 12 bytes...
		// If its changed, they added 4 extra bytes of data to it
		#region materials_block
		[TI.Definition(1, 12)]
		public class materials_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public materials_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*global material name = */ new TI.StringId());
				Add(/*phantom type = */ new TI.BlockIndex()); // 1 phantom_types_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				// TODO: unknown
				Add(/*? = */ new TI.BlockIndex(TI.FieldType.ByteBlockIndex));
			}
			#endregion
		}
		#endregion


		#region hinge_constraints_block
		[TI.Definition(1, 120)]
		public class hinge_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hinge_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region ragdoll_constraints_block
		[TI.Definition(1, 148)]
		public class ragdoll_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ragdoll_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
				Add(/*min twist = */ new TI.Real());
				Add(/*max twist = */ new TI.Real());
				Add(/*min cone = */ new TI.Real());
				Add(/*max cone = */ new TI.Real());
				Add(/*min plane = */ new TI.Real());
				Add(/*max plane = */ new TI.Real());
				Add(/*max friction torque = */ new TI.Real());
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
					public rigid_body_indices_block()
					{
						Add(/*rigid body = */ new TI.BlockIndex()); // 1 rigid_bodies_block
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public permutations_block()
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
			public regions_block()
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
			public nodes_block()
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

		#region limited_hinge_constraints_block
		[TI.Definition(1, 132)]
		public class limited_hinge_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public limited_hinge_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
				Add(/*limit friction = */ new TI.Real());
				Add(/*limit min angle = */ new TI.Real());
				Add(/*limit max angle = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region ball_and_socket_constraints_block
		[TI.Definition(1, 120)]
		public class ball_and_socket_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ball_and_socket_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region stiff_spring_constraints_block
		[TI.Definition(1, 124)]
		public class stiff_spring_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public stiff_spring_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
				Add(/*spring_length = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region prismatic_constraints_block
		[TI.Definition(1, 132)]
		public class prismatic_constraints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public prismatic_constraints_block()
			{
				Add(/*constraint bodies = */ new TI.Struct<constraint_bodies_struct>(this));
				Add(new TI.Pad(4));
				Add(/*min_limit = */ new TI.Real());
				Add(/*max_limit = */ new TI.Real());
				Add(/*max_friction_force = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public physics_model_group()
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*mass = */ new TI.Real());
			Add(/*low freq. deactivation scale = */ new TI.Real());
			Add(/*high freq. deactivation scale = */ new TI.Real());
			Add(new TI.Pad(24));
			Add(TI.UnknownPad.BlockHalo3); // [0x20]
				// string id
				// real
				// real
				// real
				// real
				// real
				// unknown [0x8]
			Add(TI.UnknownPad.BlockHalo3);
			Add(TI.UnknownPad.BlockHalo3); // [0x18] constraint
			Add(/*node edges = */ new TI.Block<physics_model_node_constraint_edge_block>(this, 4096));
			Add(/*rigid bodies = */ new TI.Block<rigid_bodies_block>(this, 64));
			Add(/*materials = */ new TI.Block<materials_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // [0x70] unknown shape
			Add(TI.UnknownPad.BlockHalo3); // [0x80] unknown shape
			Add(TI.UnknownPad.BlockHalo3); // [0x60] pills
			Add(TI.UnknownPad.BlockHalo3); // [0xB0] ?
			Add(TI.UnknownPad.BlockHalo3); // [0x80] ?
			Add(TI.UnknownPad.BlockHalo3); // [0x80] boxes
			Add(TI.UnknownPad.BlockHalo3); // [0x??] not a shape type
			Add(TI.UnknownPad.BlockHalo3); // [0x??] not a shape type
			Add(TI.UnknownPad.BlockHalo3); // [0x??] not a shape type
			Add(TI.UnknownPad.BlockHalo3); // [0x40] list
			Add(TI.UnknownPad.BlockHalo3); // [0x??] not a shape type
			Add(TI.UnknownPad.BlockHalo3); // [0x20] mopp
			Add(/*mopp codes = */ new TI.Data(this));
			Add(/*hinge constraints = */ new TI.Block<hinge_constraints_block>(this, 64));
			Add(/*ragdoll constraints = */ new TI.Block<ragdoll_constraints_block>(this, 64));
			Add(/*regions = */ new TI.Block<regions_block>(this, 16));
			Add(/*nodes = */ new TI.Block<nodes_block>(this, 255));
			Add(TI.UnknownPad.BlockHalo3); // this is where the import info\errors blocks would be..
			Add(TI.UnknownPad.BlockHalo3); // [0x?] point to path curves
			Add(/*limited hinge constraints = */ new TI.Block<limited_hinge_constraints_block>(this, 64));
			Add(/*ball and socket constraints = */ new TI.Block<ball_and_socket_constraints_block>(this, 64));
			Add(/*stiff spring constraints = */ new TI.Block<stiff_spring_constraints_block>(this, 64));
			Add(/*prismatic constraints = */ new TI.Block<prismatic_constraints_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // [0x2C] phantoms
		}
		#endregion
	};
	#endregion


	#region render_model_region_block
	[TI.Definition(2, 16)]
	public class render_model_region_block : TI.Definition
	{
		#region render_model_permutation_block
		[TI.Definition(1, 16)]
		public class render_model_permutation_block : TI.Definition
		{
			#region Fields
			public TI.StringId Name;
			public TI.ShortInteger L1, L2, L3, L4, L5, L6;
			#endregion

			#region Ctor
			public render_model_permutation_block()
			{
				Add(Name = new TI.StringId());
				Add(L1 = new TI.ShortInteger());
				Add(L2 = new TI.ShortInteger());
				Add(L3 = new TI.ShortInteger());
				Add(L4 = new TI.ShortInteger());
				Add(L5 = new TI.ShortInteger());
				Add(L6 = new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.StringId Name;
		public TI.Block<render_model_permutation_block> Permutations;
		#endregion

		#region Ctor
		public render_model_region_block()
		{
			Add(Name = new TI.StringId());
			Add(Permutations = new TI.Block<render_model_permutation_block>(this, 32));
		}
		#endregion
	}
	#endregion

	#region render_model_node_block
	[TI.Definition(1, 96)]
	public class render_model_node_block : TI.Definition
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
		public render_model_node_block()
		{
			Add(Name = new TI.StringId());
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

	#region render_model_marker_group_block
	[TI.Definition(1, 16)]
	public class render_model_marker_group_block : TI.Definition
	{
		#region render_model_marker_block
		[TI.Definition(1, 36)]
		public class render_model_marker_block : TI.Definition
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
			public render_model_marker_block()
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
		public render_model_marker_group_block()
		{
			Add(Name = new TI.StringId());
			Add(Markers = new TI.Block<render_model_marker_block>(this, 256));
		}
		#endregion
	}
	#endregion

	#region render_model_section_block
	[TI.Definition(2, 76)]
	public class render_model_section_block : TI.Definition
	{
		#region global_geometry_part_block_new
		[TI.Definition(2, 16)]
		public class global_geometry_part_block_new : TI.Definition
		{
			public global_geometry_part_block_new() : base()
			{
				Add(new TI.ShortInteger()); // suppose to be material index
				Add(new TI.ShortInteger()); // seems like an index to me
				Add(new TI.ShortInteger()); // start index
				Add(new TI.ShortInteger()); // count
				Add(new TI.ShortInteger());
				Add(new TI.ShortInteger());
				Add(new TI.ByteInteger());
				Add(new TI.ByteInteger());
				Add(new TI.ShortInteger());
			}
		};
		#endregion

		#region global_subparts_block
		[TI.Definition(1, 8)]
		public class global_subparts_block : TI.Definition
		{
			public global_subparts_block() : base(4)
			{
				Add(/*indices_start_index = */ new TI.ShortInteger());
				Add(/*indices_length = */ new TI.ShortInteger());
				Add(/*visibility_bounds_index = */ new TI.ShortInteger());
				Add(/*Part Index = */ new TI.ShortInteger());
			}
		}
		#endregion

		public render_model_section_block() : base()
		{
			Add(/*Parts =*/ new TI.Block<global_geometry_part_block_new>(this, 255));
			Add(/*Subparts =*/ new TI.Block<global_subparts_block>(this, 32768));
			Add(new TI.ShortInteger()); //-
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger()); //-
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger());
			Add(new TI.ShortInteger()); // (seems to equal the index of this block)
			Add(new TI.ShortInteger());
			Add(new TI.ByteInteger()); // TransparentMaxNodesPerVertex
			Add(new TI.ByteInteger()); // NodeIndex
			Add(new TI.ByteInteger()); // Type
			Add(new TI.ByteInteger()); // OpaqueMaxNodesPerVertex
			Add(new TI.ByteInteger());
			Add(TI.UnknownPad.Byte); // pad?
			Add(TI.UnknownPad.Word); // pad?
			Add(new TI.UnknownPad(24));
		}
	};
	#endregion

	#region render_model
	[TI.TagGroup((int)TagGroups.Enumerated.mode, 6, 244)] // TODO: pretty sure thats the right size
	public class render_model_group : TI.Definition
	{
		#region Fields
		public TI.StringId Name;
		public TI.Flags Flags;
		public TI.Block<render_model_region_block> Regions;
		public TI.Block<render_model_node_block> Nodes;
		public TI.Block<render_model_marker_group_block> MarkerGroups;
		public TI.Block<global_geometry_material_block> Materials;
		public TI.ByteInteger SectionGroupIndex1;
		public TI.ByteInteger SectionGroupIndex2;
		public TI.ByteInteger SectionGroupIndex3;
		public TI.ByteInteger SectionGroupIndex4;
		public TI.ByteInteger SectionGroupIndex5;
		public TI.ByteInteger SectionGroupIndex6;
		public TI.Block<global_geometry_compression_info_block> CompressionInfo;
		public TI.LongInteger ResourceId;
		#endregion

		#region Ctor
		public render_model_group()
		{
			Add(Name = new TI.StringId());
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 4)); // the dword is a postprocessed field
			Add(Regions = new TI.Block<render_model_region_block>(this, 16));
			// TODO
			Add(new TI.UnknownPad(24));
			Add(Nodes = new TI.Block<render_model_node_block>(this, 255));
			Add(MarkerGroups = new TI.Block<render_model_marker_group_block>(this, 4096));
			Add(Materials = new TI.Block<global_geometry_material_block>(this, 1024));
			Add(TI.UnknownPad.BlockHalo3); // errors?
			Add(SectionGroupIndex1 = new TI.ByteInteger());
			Add(SectionGroupIndex2 = new TI.ByteInteger());
			Add(SectionGroupIndex3 = new TI.ByteInteger());
			Add(SectionGroupIndex4 = new TI.ByteInteger());
			Add(SectionGroupIndex5 = new TI.ByteInteger());
			Add(SectionGroupIndex6 = new TI.ByteInteger());
			Add(new TI.Pad(2));

			Add(TI.UnknownPad.BlockHalo3); // [0x4C] render_model_section_block
			Add(CompressionInfo = new TI.Block<global_geometry_compression_info_block>(this, 1));
			Add(TI.UnknownPad.BlockHalo3); // [0x30]
				// unknown [0x10]
				// real
				// real
				// real
				// real
				// long (might byte 4 byte indexers)
				// real
				// real
				// real
			Add(TI.UnknownPad.BlockHalo3); // [0x18] functions
				// string id (input)
				// tag data // mapping_function
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x20] (count matches render_model_section_block)
				// tag data
				// tag block [0x2]
			Add(TI.UnknownPad.BlockHalo3); // [0xC] looks like render_model_section_group_block
				// tag block [0x1]
			Add(TI.UnknownPad.BlockHalo3); // [0xC]
			Add(TI.UnknownPad.BlockHalo3); // ??
			Add(TI.UnknownPad.BlockHalo3); // [0x10]
				// tag block ??
				// short (block index?)
				// word
			Add(ResourceId = new TI.LongInteger()); // datum to cache_file_resource_gestalt_58_block
			Add(TI.UnknownPad.DWord);
			Add(TI.UnknownPad.BlockHalo3); // [0x??] maybe this points to the render_geometry_api_resource_definition block data in the zone tag
		}
		#endregion
	};
	#endregion
}