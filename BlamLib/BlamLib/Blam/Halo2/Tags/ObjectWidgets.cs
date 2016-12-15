/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region antenna
	[TI.TagGroup((int)TagGroups.Enumerated.ant_, 1, 180)]
	public class antenna_group : TI.Definition
	{
		#region antenna_vertex_block
		[TI.Definition(1, 128)]
		public class antenna_vertex_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public antenna_vertex_block() : base(9)
			{
				Add(/*spring strength coefficient = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Pad(24));
				Add(/*angles = */ new TI.RealEulerAngles2D());
				Add(/*length = */ new TI.Real());
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*LOD color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(new TI.Pad(40 + 12));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public antenna_group() : base(9)
		{
			Add(/*attachment marker name = */ new TI.StringId(true));
			Add(/*bitmaps = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*physics = */ new TI.TagReference(this, TagGroups.pphy));
			Add(new TI.Pad(80));
			Add(/*spring strength coefficient = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*falloff pixels = */ new TI.Real());
			Add(/*cutoff pixels = */ new TI.Real());
			Add(new TI.Pad(40));
			Add(/*vertices = */ new TI.Block<antenna_vertex_block>(this, 20));
		}
		#endregion
	};
	#endregion

	#region cloth_properties
	//cloth properties
	[TI.Struct((int)StructGroups.Enumerated.clpr, 1, 48)]
	public class cloth_properties : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public cloth_properties() : base(8)
		{
			Add(/*Integration type = */ new TI.Enum());
			Add(/*Number iterations = */ new TI.ShortInteger());
			Add(/*weight = */ new TI.Real());
			Add(/*drag = */ new TI.Real());
			Add(/*wind_scale = */ new TI.Real());
			Add(/*wind_flappiness_scale = */ new TI.Real());
			Add(/*longest_rod = */ new TI.Real());
			Add(new TI.Pad(24));
		}
		#endregion
	}
	#endregion

	#region cloth
	[TI.TagGroup((int)TagGroups.Enumerated.clwd, 0, 132)]
	public class cloth_group : TI.Definition
	{
		#region cloth_vertices_block
		[TI.Definition(1, 20)]
		public class cloth_vertices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cloth_vertices_block() : base(2)
			{
				Add(/*initial position = */ new TI.RealPoint3D());
				Add(/*uv = */ new TI.RealVector2D());
			}
			#endregion
		}
		#endregion

		#region cloth_indices_block
		[TI.Definition(1, 2)]
		public class cloth_indices_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cloth_indices_block() : base(1)
			{
				Add(/*index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region cloth_links_block
		[TI.Definition(1, 16)]
		public class cloth_links_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cloth_links_block() : base(5)
			{
				Add(/*attachment bits = */ new TI.LongInteger());
				Add(/*index1 = */ new TI.ShortInteger());
				Add(/*index2 = */ new TI.ShortInteger());
				Add(/*default_distance = */ new TI.Real());
				Add(/*damping_multiplier = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public cloth_group() : base(12)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*marker attachment name = */ new TI.StringId());
			Add(/*Shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*grid x dimension = */ new TI.ShortInteger());
			Add(/*grid y dimension = */ new TI.ShortInteger());
			Add(/*grid spacing x = */ new TI.Real());
			Add(/*grid spacing y = */ new TI.Real());
			Add(/*properties = */ new TI.Struct<cloth_properties>(this));
			Add(/*vertices = */ new TI.Block<cloth_vertices_block>(this, 128));
			Add(/*indices = */ new TI.Block<cloth_indices_block>(this, 768));
			Add(/*strip indices = */ new TI.Block<cloth_indices_block>(this, 768));
			Add(/*links = */ new TI.Block<cloth_links_block>(this, 640));
		}
		#endregion
	};
	#endregion

	#region contrail
	[TI.TagGroup((int)TagGroups.Enumerated.cont, 3, 260)]
	public class contrail_group : TI.Definition
	{
		#region contrail_point_states_block
		[TI.Definition(1, 72)]
		public class contrail_point_states_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public contrail_point_states_block() : base(8)
			{
				Add(/*duration = */ new TI.RealBounds());
				Add(/*transition duration = */ new TI.RealBounds());
				Add(/*physics = */ new TI.TagReference(this, TagGroups.pphy));
				Add(new TI.UselessPad(32));
				Add(/*width = */ new TI.Real());
				Add(/*color lower bound = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*color upper bound = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*scale flags = */ new TI.Flags());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public contrail_group() : base(46)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*scale flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*point generation rate = */ new TI.Real());
			Add(/*point velocity = */ new TI.RealBounds());
			Add(/*point velocity cone angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*inherited velocity fraction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*render type = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*texture repeats u = */ new TI.Real());
			Add(/*texture repeats v = */ new TI.Real());
			Add(/*texture animation u = */ new TI.Real());
			Add(/*texture animation v = */ new TI.Real());
			Add(/*animation rate = */ new TI.Real());
			Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*first sequence index = */ new TI.ShortInteger());
			Add(/*sequence count = */ new TI.ShortInteger());
			Add(new TI.UselessPad(64));
			Add(new TI.Pad(40));
			Add(/*shader flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*framebuffer blend function = */ new TI.Enum());
			Add(/*framebuffer fade mode = */ new TI.Enum());
			Add(/*map flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(28));
			Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*anchor = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*u-animation function = */ new TI.Enum());
			Add(/*u-animation period = */ new TI.Real());
			Add(/*u-animation phase = */ new TI.Real());
			Add(/*u-animation scale = */ new TI.Real());
			Add(new TI.Pad(2));
			Add(/*v-animation function = */ new TI.Enum());
			Add(/*v-animation period = */ new TI.Real());
			Add(/*v-animation phase = */ new TI.Real());
			Add(/*v-animation scale = */ new TI.Real());
			Add(new TI.Pad(2));
			Add(/*rotation-animation function = */ new TI.Enum());
			Add(/*rotation-animation period = */ new TI.Real());
			Add(/*rotation-animation phase = */ new TI.Real());
			Add(/*rotation-animation scale = */ new TI.Real());
			Add(/*rotation-animation center = */ new TI.RealPoint2D());
			Add(new TI.Pad(4));
			Add(/*zsprite radius scale = */ new TI.Real());
			Add(new TI.Pad(20));
			Add(/*point states = */ new TI.Block<contrail_point_states_block>(this, 16));
		}
		#endregion
	};
	#endregion

	#region liquid
	[TI.TagGroup((int)TagGroups.Enumerated.tdtl, 1, 116)]
	public class liquid_group : TI.Definition
	{
		#region liquid_arc_block
		[TI.Definition(1, 276)]
		public class liquid_arc_block : TI.Definition
		{
			#region liquid_core_block
			[TI.Definition(1, 76)]
			public class liquid_core_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public liquid_core_block() : base(8)
				{
					Add(new TI.Pad(12));
					Add(/*bitmap index = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*thickness = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*color = */ new TI.Struct<color_function_struct>(this));
					Add(/*brightness-time = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*brightness-facing = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*along-axis scale = */ new TI.Struct<scalar_function_struct>(this));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public liquid_arc_block() : base(31)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*sprite count = */ new TI.Enum());
				Add(/*natural length = */ new TI.Real());
				Add(/*instances = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*instance spread angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*instance rotation period = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*material effects = */ new TI.TagReference(this, TagGroups.foot));
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(8));
				Add(/*horizontal range = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*vertical range = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*vertical negative scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*roughness = */ new TI.Struct<scalar_function_struct>(this));
				Add(new TI.Pad(64));
				Add(/*octave 1 frequency = */ new TI.Real());
				Add(/*octave 2 frequency = */ new TI.Real());
				Add(/*octave 3 frequency = */ new TI.Real());
				Add(/*octave 4 frequency = */ new TI.Real());
				Add(/*octave 5 frequency = */ new TI.Real());
				Add(/*octave 6 frequency = */ new TI.Real());
				Add(/*octave 7 frequency = */ new TI.Real());
				Add(/*octave 8 frequency = */ new TI.Real());
				Add(/*octave 9 frequency = */ new TI.Real());
				Add(new TI.Pad(28));
				Add(/*octave flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*cores = */ new TI.Block<liquid_core_block>(this, 4));
				Add(/*range-scale = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*brightness-scale = */ new TI.Struct<scalar_function_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public liquid_group() : base(8)
		{
			Add(new TI.Pad(2));
			Add(/*type = */ new TI.Enum());
			Add(/*attachment marker name = */ new TI.StringId());
			Add(new TI.Pad(56));
			Add(/*falloff distance from camera = */ new TI.Real());
			Add(/*cutoff distance from camera = */ new TI.Real());
			Add(new TI.Pad(32));
			Add(/*arcs = */ new TI.Block<liquid_arc_block>(this, 3));
		}
		#endregion
	};
	#endregion
}