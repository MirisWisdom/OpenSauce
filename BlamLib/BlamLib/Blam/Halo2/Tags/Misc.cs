/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region camera_track_group
	[TI.TagGroup((int)TagGroups.Enumerated.trak, 2, 16)]
	public class camera_track_group : TI.Definition
	{
		#region camera_track_control_point_block
		[TI.Definition(1, 28)]
		public class camera_track_control_point_block : TI.Definition
		{
			#region Fields
			[TI.Field(TI.FieldType.RealVector3D, "position")]
			public TI.RealVector3D Position;
			[TI.Field(TI.FieldType.RealQuaternion, "orientation")]
			public TI.RealQuaternion Orientation;
			#endregion

			public camera_track_control_point_block() : base(3)
			{
				Add(Position = new TI.RealVector3D());
				Add(Orientation = new TI.RealQuaternion());
				Add(new TI.UselessPad(32));
			}
		};
		#endregion

		#region Fields
		[TI.FlagField(TI.FieldType.LongFlags, "flags")]
		private TI.Flags Flags;
		[TI.BlockField(typeof(camera_track_control_point_block), "control points", "camera_track_control_point_block", 16)]
		public TI.Block<camera_track_control_point_block> ControlPoints;
		#endregion

		public camera_track_group() : base(3)
		{
			Add(Flags = new TI.Flags());
			Add(ControlPoints = new TI.Block<camera_track_control_point_block>(this, 16));
			Add(new TI.UselessPad(32));
		}
	};
	#endregion

	#region color_table
	[TI.TagGroup((int)TagGroups.Enumerated.colo, 1, 12)]
	public class color_table_group : TI.Definition
	{
		#region color_block
		[TI.Definition(1, 48)]
		public class color_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public color_block() : base(2)
			{
				Add(/*name = */ new TI.String());
				Add(/*color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public color_table_group() : base(1)
		{
			Add(/*colors = */ new TI.Block<color_block>(this, 512));
		}
		#endregion
	};
	#endregion

	#region decorator_set
	[TI.TagGroup((int)TagGroups.Enumerated.DECR, 1, 2, 140)]
	public class decorator_set_group : TI.Definition
	{
		#region decorator_shader_reference_block
		[TI.Definition(1, 16)]
		public class decorator_shader_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public decorator_shader_reference_block() : base(1)
			{
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
			}
			#endregion
		}
		#endregion

		#region decorator_classes_block
		[TI.Definition(1, 24)]
		public class decorator_classes_block : TI.Definition
		{
			#region decorator_permutations_block
			[TI.Definition(1, 40)]
			public class decorator_permutations_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public decorator_permutations_block() : base(13)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*shader = */ new TI.BlockIndex(TI.FieldType.ByteBlockIndex)); // 1 decorator_shader_reference_block
					Add(new TI.Pad(3));
					Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
					Add(/*fade distance = */ new TI.Enum(TI.FieldType.ByteEnum));
					Add(/*index = */ new TI.ByteInteger());
					Add(/*distribution weight = */ new TI.ByteInteger());
					Add(/*scale = */ new TI.RealBounds());
					Add(/*tint 1 = */ new TI.Color(TI.FieldType.RgbColor));
					Add(/*tint 2 = */ new TI.Color(TI.FieldType.RgbColor));
					Add(/*base map tint percentage = */ new TI.Real());
					Add(/*lightmap tint percentage = */ new TI.Real());
					Add(/*wind scale = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public decorator_classes_block() : base(5)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(3));
				Add(/*scale = */ new TI.Real());
				Add(/*permutations = */ new TI.Block<decorator_permutations_block>(this, 64));
			}
			#endregion
		}
		#endregion

		#region decorator_models_block
		[TI.Definition(1, 8)]
		public class decorator_models_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public decorator_models_block() : base(3)
			{
				Add(/*model name = */ new TI.StringId());
				Add(/*index start = */ new TI.ShortInteger());
				Add(/*index count = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region decorator_model_vertices_block
		[TI.Definition(1, 56)]
		public class decorator_model_vertices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public decorator_model_vertices_block() : base(5)
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

		#region decorator_model_indices_block
		[TI.Definition(1, 2)]
		public class decorator_model_indices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public decorator_model_indices_block() : base(1)
			{
				Add(/*index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public decorator_set_group() : base(11)
		{
			Add(/*shaders = */ new TI.Block<decorator_shader_reference_block>(this, 8));
			Add(/*lighting min scale = */ new TI.Real());
			Add(/*lighting max scale = */ new TI.Real());
			Add(/*classes = */ new TI.Block<decorator_classes_block>(this, 8));
			Add(/*models = */ new TI.Block<decorator_models_block>(this, 256));
			Add(/*raw vertices = */ new TI.Block<decorator_model_vertices_block>(this, 32768));
			Add(/*indices = */ new TI.Block<decorator_model_indices_block>(this, 32768));
			Add(/*cached data = */ new TI.Block<cached_data_block>(this, 1));
			Add(/*geometry section info = */ new TI.Struct<geometry_block_info_struct>(this));
			Add(new TI.Pad(16));
			Add(new TI.Pad(4)); // TODO: PC ONLY?
		}
		#endregion
	};
	#endregion


	#region decorator_cache_block_data_block
	[TI.Definition(2, 156)]
	public partial class decorator_cache_block_data_block : TI.Definition
	{
		#region decorator_placement_block
		[TI.Definition(1, 24)]
		public class decorator_placement_block : TI.Definition
		{
			#region Fields
			public TI.LongInteger InternalData;
			public TI.LongInteger CompressedPosition;
			public TI.Color TintColor;
			public TI.Color LightmapColor;
			public TI.LongInteger CompressedLightDirection;
			public TI.LongInteger CompressedLight2Direction;
			#endregion

			#region Ctor
			public decorator_placement_block() : base(6)
			{
				Add(InternalData = new TI.LongInteger());
				Add(CompressedPosition = new TI.LongInteger());
				Add(TintColor = new TI.Color(TI.FieldType.RgbColor));
				Add(LightmapColor = new TI.Color(TI.FieldType.RgbColor));
				Add(CompressedLightDirection = new TI.LongInteger());
				Add(CompressedLight2Direction = new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region decal_vertices_block
		[TI.Definition(1, 32)]
		public class decal_vertices_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D Position;
			public TI.RealPoint2D Texcoord0;
			public TI.RealPoint2D Texcoord1;
			public TI.Color Color;
			#endregion

			#region Ctor
			public decal_vertices_block() : base(4)
			{
				Add(Position = new TI.RealPoint3D());
				Add(Texcoord0 = new TI.RealPoint2D());
				Add(Texcoord1 = new TI.RealPoint2D());
				Add(Color = new TI.Color(TI.FieldType.RgbColor));
			}
			#endregion
		}
		#endregion

		#region indices_block
		[TI.Definition(1, 2)]
		public class indices_block : TI.Definition
		{
			#region Fields
			public TI.ShortInteger Index;
			#endregion

			#region Ctor
			public indices_block() : base(1)
			{
				Add(Index = new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region sprite_vertices_block
		[TI.Definition(1, 48)]
		public class sprite_vertices_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D Position;
			public TI.RealVector3D Offset;
			public TI.RealVector3D Axis;
			public TI.RealPoint2D Texcoord;
			public TI.Color Color;
			#endregion

			#region Ctor
			public sprite_vertices_block() : base(5)
			{
				Add(Position = new TI.RealPoint3D());
				Add(Offset = new TI.RealVector3D());
				Add(Axis = new TI.RealVector3D());
				Add(Texcoord = new TI.RealPoint2D());
				Add(Color = new TI.Color(TI.FieldType.RgbColor));
			}
			#endregion
		}
		#endregion

		#region Fields
		const int OffsetPlacements = 0;
		const int OffsetDecalVertices = 8;
		const int OffsetDecalIndices = 16;
		const int OffsetSpriteVertices = 72;
		const int OffsetSpriteIndices = 88;

		public TI.Block<decorator_placement_block> Placements;
		public TI.Block<decal_vertices_block> DecalVertices;
		public TI.Block<indices_block> DecalIndices;
		public TI.VertexBuffer DecalVertexBuffer;
		public TI.Block<sprite_vertices_block> SpriteVertices;
		public TI.Block<indices_block> SpriteIndices;
		public TI.VertexBuffer SpriteVertexBuffer;
		#endregion

		#region Ctor
		public decorator_cache_block_data_block() : base(9)
		{
			Add(Placements = new TI.Block<decorator_placement_block>(this, 32768));
			Add(DecalVertices = new TI.Block<decal_vertices_block>(this, 65536));
			Add(DecalIndices = new TI.Block<indices_block>(this, 65536));
			Add(DecalVertexBuffer = new TI.VertexBuffer());
			Add(new TI.Pad(16));
			Add(SpriteVertices = new TI.Block<sprite_vertices_block>(this, 65536));
			Add(SpriteIndices = new TI.Block<indices_block>(this, 65536));
			Add(SpriteVertexBuffer = new TI.VertexBuffer());
			Add(new TI.Pad(16));
		}
		#endregion
	};
	#endregion

	#region decorator_cache_block_block
	[TI.Definition(1, 60)]
	public partial class decorator_cache_block_block : TI.Definition
	{
		#region Fields
		public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
		public TI.Block<decorator_cache_block_data_block> CacheBlockData;
		#endregion

		#region Ctor
		public decorator_cache_block_block() : base(3)
		{
			Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
			Add(CacheBlockData = new TI.Block<decorator_cache_block_data_block>(this, 1));
			Add(new TI.Pad(4 + 4));
		}
		#endregion
	};
	#endregion

	#region decorator_group_block
	[TI.Definition(1, 24)]
	public class decorator_group_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public decorator_group_block() : base(13)
		{
			Add(/*Decorator Set = */ new TI.BlockIndex(TI.FieldType.ByteBlockIndex)); // 1 scenario_decorator_set_palette_entry_block
			Add(/*Decorator Type = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*Shader Index = */ new TI.ByteInteger());
			Add(/*Compressed Radius = */ new TI.ByteInteger());
			Add(/*Cluster = */ new TI.ShortInteger());
			Add(/*Cache Block = */ new TI.BlockIndex()); // 1 decorator_cache_block_block
			Add(/*Decorator Start Index = */ new TI.ShortInteger());
			Add(/*Decorator Count = */ new TI.ShortInteger());
			Add(/*Vertex Start Offset = */ new TI.ShortInteger());
			Add(/*Vertex Count = */ new TI.ShortInteger());
			Add(/*Index Start Offset = */ new TI.ShortInteger());
			Add(/*Index Count = */ new TI.ShortInteger());
			Add(/*Compressed Bounding Center = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region decorator_cell_collection_block
	[TI.Definition(1, 24)]
	public class decorator_cell_collection_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public decorator_cell_collection_block() : base(11)
		{
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());
			Add(/*Child Index = */ new TI.ShortInteger());

			Add(/*Cache Block Index = */ new TI.BlockIndex()); // 1 decorator_cache_block_block
			Add(/*Group Count = */ new TI.ShortInteger());
			Add(/*Group Start Index = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region decorator_projected_decal_block
	[TI.Definition(1, 64)]
	public class decorator_projected_decal_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public decorator_projected_decal_block() : base(9)
		{
			Add(/*Decorator Set = */ new TI.BlockIndex(TI.FieldType.ByteBlockIndex)); // 1 scenario_decorator_set_palette_entry_block
			Add(/*Decorator Class = */ new TI.ByteInteger());
			Add(/*Decorator Permutation = */ new TI.ByteInteger());
			Add(/*Sprite Index = */ new TI.ByteInteger());
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Left = */ new TI.RealVector3D());
			Add(/*Up = */ new TI.RealVector3D());
			Add(/*Extents = */ new TI.RealVector3D());
			Add(/*Previous Position = */ new TI.RealPoint3D());
		}
		#endregion
	}
	#endregion

	#region decorator_placement_definition_block
	[TI.Definition(1, 64)]
	public class decorator_placement_definition_block : TI.Definition
	{
		#region Fields
		public TI.Block<decorator_cache_block_block> CacheBlocks;
		public TI.Block<decorator_group_block> Groups;
		public TI.Block<decorator_cell_collection_block> Cells;
		public TI.Block<decorator_projected_decal_block> Decals;
		#endregion

		#region Ctor
		public decorator_placement_definition_block() : base(6)
		{
			Add(/*Grid Origin = */ new TI.RealPoint3D());
			Add(/*Cell Count per Dimension = */ new TI.LongInteger());
			Add(CacheBlocks = new TI.Block<decorator_cache_block_block>(this, 4096));
			Add(Groups = new TI.Block<decorator_group_block>(this, 131072));
			Add(Cells = new TI.Block<decorator_cell_collection_block>(this, 65535));
			Add(Decals = new TI.Block<decorator_projected_decal_block>(this, 32768));
		}
		#endregion
	}
	#endregion

	#region decorators
	[TI.TagGroup((int)TagGroups.Enumerated.DECP, 1, 64)]
	public class decorators_group : TI.Definition
	{
		#region Fields
		public TI.Block<decorator_cache_block_block> CacheBlocks;
		public TI.Block<decorator_group_block> Groups;
		public TI.Block<decorator_cell_collection_block> Cells;
		public TI.Block<decorator_projected_decal_block> Decals;
		#endregion

		#region Ctor
		public decorators_group() : base(6)
		{
			Add(/*Grid Origin = */ new TI.RealPoint3D());
			Add(/*Cell Count per Dimension = */ new TI.LongInteger());
			Add(CacheBlocks = new TI.Block<decorator_cache_block_block>(this, 4096));
			Add(Groups = new TI.Block<decorator_group_block>(this, 131072));
			Add(Cells = new TI.Block<decorator_cell_collection_block>(this, 65535));
			Add(Decals = new TI.Block<decorator_projected_decal_block>(this, 32768));
		}
		#endregion
	};
	#endregion


	#region physics
	[TI.TagGroup((int)TagGroups.Enumerated.phys, 4, 128)]
	public class physics_group : TI.Definition
	{
		#region inertial_matrix_block
		[TI.Definition(1, 36)]
		public class inertial_matrix_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public inertial_matrix_block() : base(3)
			{
				Add(/*yy+zz    -xy     -zx = */ new TI.RealVector3D());
				Add(/* -xy    zz+xx    -yz = */ new TI.RealVector3D());
				Add(/* -zx     -yz    xx+yy = */ new TI.RealVector3D());
			}
			#endregion
		}
		#endregion

		#region powered_mass_point_block
		[TI.Definition(1, 128)]
		public class powered_mass_point_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public powered_mass_point_block() : base(10)
			{
				Add(/*name = */ new TI.String());
				Add(/*flags = */ new TI.Flags());
				Add(/*antigrav strength = */ new TI.Real());
				Add(/*antigrav offset = */ new TI.Real());
				Add(/*antigrav height = */ new TI.Real());
				Add(/*antigrav damp fraction = */ new TI.Real());
				Add(/*antigrav normal k1 = */ new TI.Real());
				Add(/*antigrav normal k0 = */ new TI.Real());
				Add(/*damage source region name = */ new TI.StringId());
				Add(new TI.Pad(64));
			}
			#endregion
		}
		#endregion

		#region mass_point_block
		[TI.Definition(1, 128)]
		public class mass_point_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public mass_point_block() : base(17)
			{
				Add(/*name = */ new TI.String());
				Add(/*powered mass point = */ new TI.BlockIndex()); // 1 powered_mass_point_block
				Add(/*model node = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags());
				Add(/*relative mass = */ new TI.Real());
				Add(/*mass = */ new TI.Real());
				Add(/*relative density = */ new TI.Real());
				Add(/*density = */ new TI.Real());
				Add(/*position = */ new TI.RealPoint3D());
				Add(/*forward = */ new TI.RealVector3D());
				Add(/*up = */ new TI.RealVector3D());
				Add(/*friction type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*friction parallel scale = */ new TI.Real());
				Add(/*friction perpendicular scale = */ new TI.Real());
				Add(/*radius = */ new TI.Real());
				Add(new TI.Pad(20));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public physics_group() : base(24)
		{
			Add(/*radius = */ new TI.Real());
			Add(/*moment scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*mass = */ new TI.Real());
			Add(/*center of mass = */ new TI.RealPoint3D());
			Add(/*density = */ new TI.Real());
			Add(/*gravity scale = */ new TI.Real());
			Add(/*ground friction = */ new TI.Real());
			Add(/*ground depth = */ new TI.Real());
			Add(/*ground damp fraction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*ground normal k1 = */ new TI.Real());
			Add(/*ground normal k0 = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*water friction = */ new TI.Real());
			Add(/*water depth = */ new TI.Real());
			Add(/*water density = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*air friction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(4));
			Add(/*xx moment = */ new TI.Real());
			Add(/*yy moment = */ new TI.Real());
			Add(/*zz moment = */ new TI.Real());
			Add(/*inertial matrix and inverse = */ new TI.Block<inertial_matrix_block>(this, 2));
			Add(/*powered mass points = */ new TI.Block<powered_mass_point_block>(this, 32));
			Add(/*mass points = */ new TI.Block<mass_point_block>(this, 32));
		}
		#endregion
	};
	#endregion

	#region point_physics
	[TI.TagGroup((int)TagGroups.Enumerated.pphy, 1, 64)]
	public class point_physics_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public point_physics_group() : base(8)
		{
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(28));
			Add(/*density = */ new TI.Real());
			Add(/*air friction = */ new TI.Real());
			Add(/*water friction = */ new TI.Real());
			Add(/*surface friction = */ new TI.Real());
			Add(/*elasticity = */ new TI.Real());
			Add(new TI.Pad(12));
		}
		#endregion
	};
	#endregion
}