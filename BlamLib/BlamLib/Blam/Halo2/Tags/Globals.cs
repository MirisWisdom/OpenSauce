/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	[TagInterface.Definition(1, 8)]
	public class predicted_resource_block : Cache.predicted_resource_block
	{
		public predicted_resource_block() : base(3)
		{
			Add(Type = new TI.Enum());
			Add(ResourceIndex = new TI.ShortInteger());
			Add(TagIndex = new TI.LongInteger());
		}
	};

	#region tag_block_index_struct
	[TI.Struct((int)StructGroups.Enumerated.shtb, 1, 2)]
	public class tag_block_index_struct : TI.Definition
	{
		public tag_block_index_struct() : base(1)
		{
			Add(/*block index data = */ new TI.ShortInteger());
		}
	}
	#endregion

	#region tag_block_index_block
	[TI.Definition(1, 2)]
	public class tag_block_index_block : TI.Definition
	{
		public tag_block_index_block() : base(1)
		{
			Add(/*indices = */ new TI.Struct<tag_block_index_struct>(this));
		}
	}
	#endregion

	#region pixel32_block
	[TI.Definition(1, 4)]
	public class pixel32_block : TI.Definition
	{
		public pixel32_block() : base(1)
		{
			Add(/*color = */ new TI.Color());
		}
	}
	#endregion

	#region real_vector4d_block
	[TI.Definition(1, 16)]
	public class real_vector4d_block : TI.Definition
	{
		public real_vector4d_block() : base(2)
		{
			Add(/*vector3 = */ new TI.RealVector3D());
			Add(/* = */ new TI.Real());
		}
	}
	#endregion

	#region byte_block
	[TI.Definition(1, 1)]
	public class byte_block : TI.Definition
	{
		public byte_block() : base(1)
		{
			Add(/*Value = */ new TI.ByteInteger());
		}
	}
	#endregion

	#region global_tag_import_info_block
	[TI.Definition(1, 596)]
	public class global_tag_import_info_block : TI.Definition
	{
		#region tag_import_file_block
		[TI.Definition(1, 540)]
		public class tag_import_file_block : TI.Definition
		{
			public TI.String Path;
			public TI.String ModificationDate;
			public TI.LongInteger Checksum;
			public TI.LongInteger Size; // size of uncompressed input data
			public TI.Data ZippedData;

			public tag_import_file_block() : base(8)
			{
				Add(Path = TI.String.LongString);
				Add(ModificationDate = new TI.String());
				Add(new TI.Skip(8)); // FILETIME modification date
				Add(new TI.Pad(88));
				Add(Checksum = new TI.LongInteger());
				Add(Size = new TI.LongInteger());
				Add(ZippedData = new TI.Data(this));
				Add(new TI.Pad(128));
			}

			public byte[] Decompress()
			{
				return Util.ZLibBufferFromBytes(ZippedData.Value, 0, Size.Value);
			}
		}
		#endregion

		public TI.LongInteger ExporterBuild;
		public TI.String Version;
		public TI.String ImportDate;
		public TI.String Culprit;
		public TI.String ImportTime;
		public TI.Block<tag_import_file_block> Files;

		public global_tag_import_info_block() : base(9)
		{
			Add(ExporterBuild = new TI.LongInteger());
			Add(Version = TI.String.LongString);
			Add(ImportDate = new TI.String());
			Add(Culprit = new TI.String());
			Add(new TI.Pad(96));
			Add(ImportTime =  new TI.String());
			Add(new TI.Pad(4));
			Add(Files = new TI.Block<tag_import_file_block>(this, 1024));
			Add(new TI.Pad(128));
		}
	}
	#endregion

	#region global_error_report_categories_block
	[TI.Definition(1, 680)]
	public class global_error_report_categories_block : TI.Definition
	{
		#region error_reports_block
		public abstract class error_report_type_base : TI.Definition
		{
			public TI.RealColor Color;

			protected error_report_type_base(int field_count) : base(field_count) { }
		};

		[TI.Definition(1, 644)]
		public class error_reports_block : TI.Definition
		{
			#region error_report_vertices_block
			[TI.Definition(1, 52)]
			public class error_report_vertices_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex Position;

				public error_report_vertices_block() : base(11)
				{
					Position = new global_model_skinned_uncompressed_vertex(this);

					Add(Color = TI.RealColor.Argb);
					Add(/*Screen Size = */ new TI.Real());
				}
			}
			#endregion

			#region error_report_vectors_block
			[TI.Definition(1, 64)]
			public class error_report_vectors_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex Position;

				public error_report_vectors_block() : base(12)
				{
					Position = new global_model_skinned_uncompressed_vertex(this);
					Add(Color = TI.RealColor.Argb);
					Add(/*Normal = */ new TI.RealVector3D());
					Add(/*Screen Length = */ new TI.Real());
				}
			}
			#endregion

			#region error_report_lines_block
			[TI.Definition(1, 80)]
			public class error_report_lines_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex Start, End;

				public error_report_lines_block() : base(19)
				{
					Start = new global_model_skinned_uncompressed_vertex(this);
					End = new global_model_skinned_uncompressed_vertex(this);
					Add(Color = TI.RealColor.Argb);
				}
			}
			#endregion

			#region error_report_triangles_block
			[TI.Definition(1, 112)]
			public class error_report_triangles_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex PointA, PointB, PointC;

				public error_report_triangles_block() : base(28)
				{
					PointA = new global_model_skinned_uncompressed_vertex(this);
					PointB = new global_model_skinned_uncompressed_vertex(this);
					PointC = new global_model_skinned_uncompressed_vertex(this);
					Add(Color = TI.RealColor.Argb);
				}
			}
			#endregion

			#region error_report_quads_block
			[TI.Definition(1, 144)]
			public class error_report_quads_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex PointA, PointB, PointC, PointD;

				public error_report_quads_block() : base(37)
				{
					PointA = new global_model_skinned_uncompressed_vertex(this);
					PointB = new global_model_skinned_uncompressed_vertex(this);
					PointC = new global_model_skinned_uncompressed_vertex(this);
					PointD = new global_model_skinned_uncompressed_vertex(this);
					Add(Color = TI.RealColor.Argb);
				}
			}
			#endregion

			#region error_report_comments_block
			[TI.Definition(1, 68)]
			public class error_report_comments_block : error_report_type_base
			{
				public global_model_skinned_uncompressed_vertex Position;

				public error_report_comments_block() : base(11)
				{
					Add(/*Text = */ new TI.Data(this));
					Position = new global_model_skinned_uncompressed_vertex(this);
					Add(Color = TI.RealColor.Argb);
				}
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public error_reports_block() : base(19)
			{
				Add(/*Type = */ new TI.Enum());
				Add(/*Flags = */ TI.Flags.Word);
				Add(/*Text = */ new TI.Data(this));
				Add(/*Source Filename = */ new TI.String());
				Add(/*Source Line Number = */ new TI.LongInteger());
				Add(/*Vertices = */ new TI.Block<error_report_vertices_block>(this, 1024));
				Add(/*Vectors = */ new TI.Block<error_report_vectors_block>(this, 1024));
				Add(/*Lines = */ new TI.Block<error_report_lines_block>(this, 1024));
				Add(/*Triangles = */ new TI.Block<error_report_triangles_block>(this, 1024));
				Add(/*Quads = */ new TI.Block<error_report_quads_block>(this, 1024));
				Add(/*Comments = */ new TI.Block<error_report_comments_block>(this, 1024));
				Add(new TI.Pad(380));
				Add(/*Report Key = */ new TI.LongInteger());
				Add(/*Node Index = */ new TI.LongInteger());
				Add(/*Bounds x = */ new TI.RealBounds());
				Add(/*Bounds y = */ new TI.RealBounds());
				Add(/*Bounds z = */ new TI.RealBounds());
				Add(/*Color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(new TI.Pad(84));
			}
			#endregion
		}
		#endregion

		public TI.String Name;
		public TI.Enum ReportType;
		public TI.Flags Flags;
		public TI.Block<error_reports_block> Reports;

		public global_error_report_categories_block() : base(5)
		{
			Add(Name = TI.String.LongString);
			Add(ReportType = new TI.Enum());
			Add(Flags = TI.Flags.Word);
			Add(new TI.Pad(2 + 2 + 404));
			Add(Reports = new TI.Block<error_reports_block>(this, 1024));
		}
	}
	#endregion

	#region global_geometry_material_block
	[TI.Definition(1, 52)]
	public class global_geometry_material_block : TI.Definition
	{
		#region global_geometry_material_property_block
		[TI.Definition(1, 8)]
		public class global_geometry_material_property_block : TI.Definition
		{
			public TI.Enum Type;
			public TI.ShortInteger IntValue;
			public TI.Real RealValue;

			public global_geometry_material_property_block() : base(3)
			{
				Add(Type = new TI.Enum());
				Add(IntValue = new TI.ShortInteger());
				Add(RealValue = new TI.Real());
			}
		}
		#endregion

		public TI.TagReference OldShader;
		public TI.TagReference Shader;
		public TI.Block<global_geometry_material_property_block> Properties;
		public TI.ByteInteger BreakableSurfaceIndex;

		public global_geometry_material_block() : base(6)
		{
			Add(OldShader = new TI.TagReference(this, TagGroups.shad));
			Add(Shader = new TI.TagReference(this, TagGroups.shad));
			Add(Properties = new TI.Block<global_geometry_material_property_block>(this, 16));
			Add(new TI.Pad(4));
			Add(BreakableSurfaceIndex = new TI.ByteInteger());
			Add(new TI.Pad(3));
		}
	}
	#endregion

	#region cached_data_block
	[TI.Definition(1, 32)]
	public class cached_data_block : TI.Definition
	{
		public TI.VertexBuffer VertexBuffer;

		public cached_data_block() : base(1)
		{
			Add(VertexBuffer = new TI.VertexBuffer());
		}
	}
	#endregion


	#region primary_light_struct
	//Primary light
	[TI.Struct((int)StructGroups.Enumerated.prli, 1, 40)]
	public class primary_light_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public primary_light_struct() : base(4)
		{
			Add(/*Min lightmap color = */ new TI.RealColor());
			Add(/*Max lightmap color = */ new TI.RealColor());
			Add(/*exclusion angle from up = */ new TI.Real());
			Add(/*function = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region secondary_light_struct
	//Secondary light
	[TI.Struct((int)StructGroups.Enumerated.scli, 1, 64)]
	public class secondary_light_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public secondary_light_struct() : base(6)
		{
			Add(/*Min lightmap color = */ new TI.RealColor());
			Add(/*Max lightmap color = */ new TI.RealColor());
			Add(/*Min diffuse sample = */ new TI.RealColor());
			Add(/*Max diffuse sample = */ new TI.RealColor());
			Add(/*z axis rotation = */ new TI.Real());
			Add(/*function = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region ambient_light_struct
	//ambient light
	[TI.Struct((int)StructGroups.Enumerated.amli, 1, 36)]
	public class ambient_light_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public ambient_light_struct() : base(3)
		{
			Add(/*Min lightmap sample = */ new TI.RealColor());
			Add(/*Max lightmap sample = */ new TI.RealColor());
			Add(/*function = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region lightmap_shadows_struct
	//Lightmap shadows
	[TI.Struct((int)StructGroups.Enumerated.lmsh, 1, 12)]
	public class lightmap_shadows_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public lightmap_shadows_struct() : base(1)
		{
			Add(/*function 1 = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region chocolate_mountain
	[TI.TagGroup((int)TagGroups.Enumerated.gldf, 1, 12)]
	public class chocolate_mountain_group : TI.Definition
	{
		#region lighting_variables_block
		[TI.Definition(1, 160)]
		public class lighting_variables_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lighting_variables_block() : base(6)
			{
				Add(/*object affected = */ new TI.Flags());
				Add(/*Lightmap brightness offset = */ new TI.Real());
				Add(/*primary light = */ new TI.Struct<primary_light_struct>(this));
				Add(/*secondary light = */ new TI.Struct<secondary_light_struct>(this));
				Add(/*ambient light = */ new TI.Struct<ambient_light_struct>(this));
				Add(/*lightmap shadows = */ new TI.Struct<lightmap_shadows_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public chocolate_mountain_group() : base(1)
		{
			Add(/*lighting variables = */ new TI.Block<lighting_variables_block>(this, 13));
		}
		#endregion
	};
	#endregion


	#region multiplayer_color_block
	[TI.Definition(1, 12)]
	public class multiplayer_color_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public multiplayer_color_block() : base(1)
		{
			Add(/*color = */ new TI.RealColor());
		}
		#endregion
	}
	#endregion

	#region multiplayer_universal_block
	[TI.Definition(1, 60)]
	public class multiplayer_universal_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public multiplayer_universal_block() : base(4)
		{
			Add(/*random player names = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*team names = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*team colors = */ new TI.Block<multiplayer_color_block>(this, 32));
			Add(/*multiplayer text = */ new TI.TagReference(this, TagGroups.unic));
		}
		#endregion
	}
	#endregion

	#region vehicles_block
	[TI.Definition(1, 16)]
	public class vehicles_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public vehicles_block() : base(1)
		{
			Add(/*vehicle = */ new TI.TagReference(this, TagGroups.vehi));
		}
		#endregion
	}
	#endregion

	#region sounds_block
	[TI.Definition(1, 16)]
	public class sounds_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sounds_block() : base(1)
		{
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
		}
		#endregion
	}
	#endregion

	#region sound_response_extra_sounds_struct
	[TI.Struct((int)StructGroups.Enumerated.masd_sound, 2, 128)]
	public class sound_response_extra_sounds_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_response_extra_sounds_struct() : base(8)
		{
			Add(/*japanese sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*german sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*french sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*spanish sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*italian sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*korean sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*chinese sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*portuguese sound = */ new TI.TagReference(this, TagGroups.snd_));
		}
		#endregion
	}
	#endregion

	#region sound_response_definition_block
	[TI.Definition(1, 152)]
	public class sound_response_definition_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_response_definition_block() : base(5)
		{
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*english sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(/*probability = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region game_engine_general_event_block
	[TI.Definition(1, 244)]
	public class game_engine_general_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_general_event_block() : base(19)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2 + 2));
			Add(/*display string = */ new TI.StringId());
			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(28));
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4 + 16));
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region game_engine_slayer_event_block
	[TI.Definition(1, 244)]
	public class game_engine_slayer_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_slayer_event_block() : base(19)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2 + 2));
			Add(/*display string = */ new TI.StringId());
			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(28));
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4 + 16));
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region game_engine_ctf_event_block
	[TI.Definition(1, 244)]
	public class game_engine_ctf_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_ctf_event_block() : base(19)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2 + 2));
			Add(/*display string = */ new TI.StringId());
			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(28));
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4 + 16));
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region game_engine_oddball_event_block
	[TI.Definition(1, 244)]
	public class game_engine_oddball_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_oddball_event_block() : base(19)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2 + 2));
			Add(/*display string = */ new TI.StringId());
			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(28));
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4 + 16));
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region game_engine_king_event_block
	[TI.Definition(1, 244)]
	public class game_engine_king_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_king_event_block() : base(19)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2 + 2));
			Add(/*display string = */ new TI.StringId());
			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(28));
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4 + 16));
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region material_physics_properties_struct
	//physics properties
	[TI.Struct((int)StructGroups.Enumerated.mphp, 2, 16)]
	public class material_physics_properties_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public material_physics_properties_struct() : base(4)
		{
			Add(new TI.Pad(4));
			Add(/*friction = */ new TI.Real());
			Add(/*restitution = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*density = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region materials_sweeteners_struct
	//sweet, sweetening, sweeteners!
	[TI.Struct((int)StructGroups.Enumerated.msst, 1, 228)]
	public class materials_sweeteners_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public materials_sweeteners_struct() : base(15)
		{
			Add(/*sound sweetener (small) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener (medium) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener (large) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener rolling = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*sound sweetener grinding = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*sound sweetener (melee) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this));
			Add(/*effect sweetener (small) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (medium) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (large) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener rolling = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener grinding = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (melee) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/* = */ new TI.TagReference(this));
			Add(/*sweetener inheritance flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region global_ui_campaign_level_block
	[TI.Definition(1, 2904)]
	public class global_ui_campaign_level_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_ui_campaign_level_block() : base(4)
		{
			Add(/*Campaign ID = */ new TI.LongInteger());
			Add(/*Map ID = */ new TI.LongInteger());
			Add(/*Bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Skip(576 + 2304));
		}
		#endregion
	}
	#endregion

	#region global_ui_multiplayer_level_block
	[TI.Definition(1, 3180)]
	public class global_ui_multiplayer_level_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_ui_multiplayer_level_block() : base(23)
		{
			Add(/*Map ID = */ new TI.LongInteger());
			Add(/*Bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Skip(576 + 2304));
			Add(/*Path = */ new TI.String(TI.StringType.Ascii, 256));
			Add(/*Sort Order = */ new TI.LongInteger());
			Add(/*Flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(new TI.Pad(3));
			Add(/*Max Teams None = */ new TI.ByteInteger());
			Add(/*Max Teams CTF = */ new TI.ByteInteger());
			Add(/*Max Teams Slayer = */ new TI.ByteInteger());
			Add(/*Max Teams Oddball = */ new TI.ByteInteger());
			Add(/*Max Teams KOTH = */ new TI.ByteInteger());
			Add(/*Max Teams Race = */ new TI.ByteInteger());
			Add(/*Max Teams Headhunter = */ new TI.ByteInteger());
			Add(/*Max Teams Juggernaut = */ new TI.ByteInteger());
			Add(/*Max Teams Territories = */ new TI.ByteInteger());
			Add(/*Max Teams Assault = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 10 = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 11 = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 12 = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 13 = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 14 = */ new TI.ByteInteger());
			Add(/*Max Teams Stub 15 = */ new TI.ByteInteger());
		}
		#endregion
	}
	#endregion

	#region globals
	[TI.TagGroup((int)TagGroups.Enumerated.matg, 3, 760)]
	public partial class globals_group : TI.Definition
	{
		#region havok_cleanup_resources_block
		[TI.Definition(1, 16)]
		public class havok_cleanup_resources_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public havok_cleanup_resources_block() : base(1)
			{
				Add(/*object cleanup effect = */ new TI.TagReference(this, TagGroups.effe));
			}
			#endregion
		}
		#endregion

		#region collision_damage_block
		[TI.Definition(1, 80)]
		public class collision_damage_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public collision_damage_block() : base(10)
			{
				Add(/*collision damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*min game acc (default) = */ new TI.Real());
				Add(/*max game acc (default) = */ new TI.Real());
				Add(/*min game scale (default) = */ new TI.Real());
				Add(/*max game scale (default) = */ new TI.Real());
				Add(/*min abs acc (default) = */ new TI.Real());
				Add(/*max abs acc (default) = */ new TI.Real());
				Add(/*min abs scale (default) = */ new TI.Real());
				Add(/*max abs scale (default) = */ new TI.Real());
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region sound_globals_block
		[TI.Definition(2, 68)]
		public class sound_globals_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_globals_block() : base(5)
			{
				Add(/*sound classes = */ new TI.TagReference(this, TagGroups.sncl));
				Add(/*sound effects = */ new TI.TagReference(this, TagGroups.sfx_));
				Add(/*sound mix = */ new TI.TagReference(this, TagGroups.snmx));
				Add(/*sound combat dialogue constants = */ new TI.TagReference(this, TagGroups.spk_));
				Add(/* = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region ai_globals_block
		[TI.Definition(1, 372)]
		public class ai_globals_block : TI.Definition
		{
			#region ai_globals_gravemind_block
			[TI.Definition(1, 12)]
			public class ai_globals_gravemind_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ai_globals_gravemind_block() : base(3)
				{
					Add(/*min retreat time = */ new TI.Real());
					Add(/*ideal retreat time = */ new TI.Real());
					Add(/*max retreat time = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public ai_globals_block() : base(41)
			{
				Add(/*danger broadly facing = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shooting near = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shooting at = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger extremely close = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shield damage = */ new TI.Real());
				Add(/*danger exetended shield damage = */ new TI.Real());
				Add(/*danger body damage = */ new TI.Real());
				Add(/*danger extended body damage = */ new TI.Real());
				Add(new TI.Pad(48));
				Add(/*global dialogue tag = */ new TI.TagReference(this, TagGroups.adlg));
				Add(/*default mission dialogue sound effect = */ new TI.StringId());
				Add(new TI.Pad(20));
				Add(/*jump down = */ new TI.Real());
				Add(/*jump step = */ new TI.Real());
				Add(/*jump crouch = */ new TI.Real());
				Add(/*jump stand = */ new TI.Real());
				Add(/*jump storey = */ new TI.Real());
				Add(/*jump tower = */ new TI.Real());
				Add(/*max jump down height down = */ new TI.Real());
				Add(/*max jump down height step = */ new TI.Real());
				Add(/*max jump down height crouch = */ new TI.Real());
				Add(/*max jump down height stand = */ new TI.Real());
				Add(/*max jump down height storey = */ new TI.Real());
				Add(/*max jump down height tower = */ new TI.Real());
				Add(/*hoist step = */ new TI.RealBounds());
				Add(/*hoist crouch = */ new TI.RealBounds());
				Add(/*hoist stand = */ new TI.RealBounds());
				Add(new TI.Pad(24));
				Add(/*vault step = */ new TI.RealBounds());
				Add(/*vault crouch = */ new TI.RealBounds());
				Add(new TI.Pad(48));
				Add(/*gravemind properties = */ new TI.Block<ai_globals_gravemind_block>(this, 1));
				Add(new TI.Pad(48));
				Add(/*scary target threhold = */ new TI.Real());
				Add(/*scary weapon threhold = */ new TI.Real());
				Add(/*player scariness = */ new TI.Real());
				Add(/*berserking actor scariness = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region game_globals_damage_block
		[TI.Definition(1, 12)]
		public class game_globals_damage_block : TI.Definition
		{
			#region damage_group_block
			[TI.Definition(1, 16)]
			public class damage_group_block : TI.Definition
			{
				#region armor_modifier_block
				[TI.Definition(1, 8)]
				public class armor_modifier_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public armor_modifier_block() : base(2)
					{
						Add(/*name = */ new TI.StringId());
						Add(/*damage multiplier = */ new TI.Real());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public damage_group_block() : base(2)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*armor modifiers = */ new TI.Block<armor_modifier_block>(this, 2147483647));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public game_globals_damage_block() : base(1)
			{
				Add(/*damage groups = */ new TI.Block<damage_group_block>(this, 2147483647));
			}
			#endregion
		}
		#endregion

		#region sound_block
		[TI.Definition(1, 16)]
		public class sound_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_block() : base(1)
			{
				Add(/*sound (OBSOLETE) = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region camera_block
		[TI.Definition(1, 28)]
		public class camera_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public camera_block() : base(4)
			{
				Add(/*default unit camera track = */ new TI.TagReference(this, TagGroups.trak));
				Add(/*default change pause = */ new TI.Real());
				Add(/*first person change pause = */ new TI.Real());
				Add(/*following camera change pause = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region player_control_block
		[TI.Definition(1, 132)]
		public class player_control_block : TI.Definition
		{
			#region look_function_block
			[TI.Definition(1, 4)]
			public class look_function_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public look_function_block() : base(1)
				{
					Add(/*scale = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public player_control_block() : base(27)
			{
				Add(/*magnetism friction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*magnetism adhesion = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*inconsequential target scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Pad(12));
				Add(/*crosshair location = */ new TI.RealPoint2D());
				Add(/*seconds to start = */ new TI.Real());
				Add(/*seconds to full speed = */ new TI.Real());
				Add(/*decay rate = */ new TI.Real());
				Add(/*full speed multiplier = */ new TI.Real());
				Add(/*pegged magnitude = */ new TI.Real());
				Add(/*pegged angular threshold = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*look default pitch rate = */ new TI.Real());
				Add(/*look default yaw rate = */ new TI.Real());
				Add(/*look peg threshold [0,1] = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*look yaw acceleration time = */ new TI.Real());
				Add(/*look yaw acceleration scale = */ new TI.Real());
				Add(/*look pitch acceleration time = */ new TI.Real());
				Add(/*look pitch acceleration scale = */ new TI.Real());
				Add(/*look autolevelling scale = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*gravity_scale = */ new TI.Real());
				Add(new TI.Pad(2));
				Add(/*minimum autolevelling ticks = */ new TI.ShortInteger());
				Add(/*minimum angle for vehicle flipping = */ new TI.Real(TI.FieldType.Angle));
				Add(/*look function = */ new TI.Block<look_function_block>(this, 16));
				Add(/*minimum action hold time = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region difficulty_block
		[TI.Definition(1, 644)]
		public class difficulty_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public difficulty_block() : base(112)
			{
				Add(/*easy enemy damage = */ new TI.Real());
				Add(/*normal enemy damage = */ new TI.Real());
				Add(/*hard enemy damage = */ new TI.Real());
				Add(/*imposs. enemy damage = */ new TI.Real());
				Add(/*easy enemy vitality = */ new TI.Real());
				Add(/*normal enemy vitality = */ new TI.Real());
				Add(/*hard enemy vitality = */ new TI.Real());
				Add(/*imposs. enemy vitality = */ new TI.Real());
				Add(/*easy enemy shield = */ new TI.Real());
				Add(/*normal enemy shield = */ new TI.Real());
				Add(/*hard enemy shield = */ new TI.Real());
				Add(/*imposs. enemy shield = */ new TI.Real());
				Add(/*easy enemy recharge = */ new TI.Real());
				Add(/*normal enemy recharge = */ new TI.Real());
				Add(/*hard enemy recharge = */ new TI.Real());
				Add(/*imposs. enemy recharge = */ new TI.Real());
				Add(/*easy friend damage = */ new TI.Real());
				Add(/*normal friend damage = */ new TI.Real());
				Add(/*hard friend damage = */ new TI.Real());
				Add(/*imposs. friend damage = */ new TI.Real());
				Add(/*easy friend vitality = */ new TI.Real());
				Add(/*normal friend vitality = */ new TI.Real());
				Add(/*hard friend vitality = */ new TI.Real());
				Add(/*imposs. friend vitality = */ new TI.Real());
				Add(/*easy friend shield = */ new TI.Real());
				Add(/*normal friend shield = */ new TI.Real());
				Add(/*hard friend shield = */ new TI.Real());
				Add(/*imposs. friend shield = */ new TI.Real());
				Add(/*easy friend recharge = */ new TI.Real());
				Add(/*normal friend recharge = */ new TI.Real());
				Add(/*hard friend recharge = */ new TI.Real());
				Add(/*imposs. friend recharge = */ new TI.Real());
				Add(/*easy infection forms = */ new TI.Real());
				Add(/*normal infection forms = */ new TI.Real());
				Add(/*hard infection forms = */ new TI.Real());
				Add(/*imposs. infection forms = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*easy rate of fire = */ new TI.Real());
				Add(/*normal rate of fire = */ new TI.Real());
				Add(/*hard rate of fire = */ new TI.Real());
				Add(/*imposs. rate of fire = */ new TI.Real());
				Add(/*easy projectile error = */ new TI.Real());
				Add(/*normal projectile error = */ new TI.Real());
				Add(/*hard projectile error = */ new TI.Real());
				Add(/*imposs. projectile error = */ new TI.Real());
				Add(/*easy burst error = */ new TI.Real());
				Add(/*normal burst error = */ new TI.Real());
				Add(/*hard burst error = */ new TI.Real());
				Add(/*imposs. burst error = */ new TI.Real());
				Add(/*easy new target delay = */ new TI.Real());
				Add(/*normal new target delay = */ new TI.Real());
				Add(/*hard new target delay = */ new TI.Real());
				Add(/*imposs. new target delay = */ new TI.Real());
				Add(/*easy burst separation = */ new TI.Real());
				Add(/*normal burst separation = */ new TI.Real());
				Add(/*hard burst separation = */ new TI.Real());
				Add(/*imposs. burst separation = */ new TI.Real());
				Add(/*easy target tracking = */ new TI.Real());
				Add(/*normal target tracking = */ new TI.Real());
				Add(/*hard target tracking = */ new TI.Real());
				Add(/*imposs. target tracking = */ new TI.Real());
				Add(/*easy target leading = */ new TI.Real());
				Add(/*normal target leading = */ new TI.Real());
				Add(/*hard target leading = */ new TI.Real());
				Add(/*imposs. target leading = */ new TI.Real());
				Add(/*easy overcharge chance = */ new TI.Real());
				Add(/*normal overcharge chance = */ new TI.Real());
				Add(/*hard overcharge chance = */ new TI.Real());
				Add(/*imposs. overcharge chance = */ new TI.Real());
				Add(/*easy special fire delay = */ new TI.Real());
				Add(/*normal special fire delay = */ new TI.Real());
				Add(/*hard special fire delay = */ new TI.Real());
				Add(/*imposs. special fire delay = */ new TI.Real());
				Add(/*easy guidance vs player = */ new TI.Real());
				Add(/*normal guidance vs player = */ new TI.Real());
				Add(/*hard guidance vs player = */ new TI.Real());
				Add(/*imposs. guidance vs player = */ new TI.Real());
				Add(/*easy melee delay base = */ new TI.Real());
				Add(/*normal melee delay base = */ new TI.Real());
				Add(/*hard melee delay base = */ new TI.Real());
				Add(/*imposs. melee delay base = */ new TI.Real());
				Add(/*easy melee delay scale = */ new TI.Real());
				Add(/*normal melee delay scale = */ new TI.Real());
				Add(/*hard melee delay scale = */ new TI.Real());
				Add(/*imposs. melee delay scale = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*easy grenade chance scale = */ new TI.Real());
				Add(/*normal grenade chance scale = */ new TI.Real());
				Add(/*hard grenade chance scale = */ new TI.Real());
				Add(/*imposs. grenade chance scale = */ new TI.Real());
				Add(/*easy grenade timer scale = */ new TI.Real());
				Add(/*normal grenade timer scale = */ new TI.Real());
				Add(/*hard grenade timer scale = */ new TI.Real());
				Add(/*imposs. grenade timer scale = */ new TI.Real());
				Add(new TI.Pad(16 + 16 + 16));
				Add(/*easy major upgrade (normal) = */ new TI.Real());
				Add(/*normal major upgrade (normal) = */ new TI.Real());
				Add(/*hard major upgrade (normal) = */ new TI.Real());
				Add(/*imposs. major upgrade (normal) = */ new TI.Real());
				Add(/*easy major upgrade (few) = */ new TI.Real());
				Add(/*normal major upgrade (few) = */ new TI.Real());
				Add(/*hard major upgrade (few) = */ new TI.Real());
				Add(/*imposs. major upgrade (few) = */ new TI.Real());
				Add(/*easy major upgrade (many) = */ new TI.Real());
				Add(/*normal major upgrade (many) = */ new TI.Real());
				Add(/*hard major upgrade (many) = */ new TI.Real());
				Add(/*imposs. major upgrade (many) = */ new TI.Real());
				Add(/*easy player vehicle ram chance = */ new TI.Real());
				Add(/*normal player vehicle ram chance = */ new TI.Real());
				Add(/*hard player vehicle ram chance = */ new TI.Real());
				Add(/*imposs. player vehicle ram chance = */ new TI.Real());
				Add(new TI.Pad(16 + 16 + 16 + 84));
			}
			#endregion
		}
		#endregion

		#region grenades_block
		[TI.Definition(1, 68)]
		public class grenades_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public grenades_block() : base(6)
			{
				Add(/*maximum count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*throwing effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(16));
				Add(/*equipment = */ new TI.TagReference(this, TagGroups.eqip));
				Add(/*projectile = */ new TI.TagReference(this, TagGroups.proj));
			}
			#endregion
		}
		#endregion

		#region rasterizer_data_block
		[TI.Definition(1, 428)]
		public class rasterizer_data_block : TI.Definition
		{
			#region vertex_shader_reference_block
			[TI.Definition(1, 16)]
			public class vertex_shader_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public vertex_shader_reference_block() : base(1)
				{
					Add(/*vertex shader = */ new TI.TagReference(this, TagGroups.vrtx));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public rasterizer_data_block() : base(31)
			{
				// Explanation here
				Add(/*distance attenuation = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*vector normalization = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*gradients = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*glow = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(16));
				Add(/*global vertex shaders = */ new TI.Block<vertex_shader_reference_block>(this, 32));
				// Explanation here
				Add(/*default 2D = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*default 3D = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*default cube map = */ new TI.TagReference(this, TagGroups.bitm));
				// Explanation here
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				// Explanation here
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(36));
				// Explanation here
				Add(/*global shader = */ new TI.TagReference(this, TagGroups.shad));
				// Explanation here
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*refraction amount = */ new TI.Real());
				Add(/*distance falloff = */ new TI.Real());
				Add(/*tint color = */ new TI.RealColor());
				Add(/*hyper-stealth refraction = */ new TI.Real());
				Add(/*hyper-stealth distance falloff = */ new TI.Real());
				Add(/*hyper-stealth tint color = */ new TI.RealColor());
				// Explanation here
				Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region interface_tag_references
		[TI.Definition(1, 304)]
		public class interface_tag_references : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public interface_tag_references() : base(19)
			{
				Add(/*obsolete1 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*obsolete2 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*screen color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*hud color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*editor color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*dialog color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*hud globals = */ new TI.TagReference(this, TagGroups.hudg));
				Add(/*motion sensor sweep bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*motion sensor sweep bitmap mask = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*multiplayer hud bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/* = */ new TI.TagReference(this));
				Add(/*hud digits definition = */ new TI.TagReference(this, TagGroups.hud_));
				Add(/*motion sensor blip bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*interface goo map1 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*interface goo map2 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*interface goo map3 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*mainmenu ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
				Add(/*singleplayer ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
				Add(/*multiplayer ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
			}
			#endregion
		}
		#endregion

		#region cheat_weapons_block
		[TI.Definition(1, 16)]
		public class cheat_weapons_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cheat_weapons_block() : base(1)
			{
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.item));
			}
			#endregion
		}
		#endregion

		#region cheat_powerups_block
		[TI.Definition(1, 16)]
		public class cheat_powerups_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cheat_powerups_block() : base(1)
			{
				Add(/*powerup = */ new TI.TagReference(this, TagGroups.eqip));
			}
			#endregion
		}
		#endregion

		#region multiplayer_information_block
		[TI.Definition(1, 232)]
		public class multiplayer_information_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public multiplayer_information_block() : base(15)
			{
				Add(/*flag = */ new TI.TagReference(this, TagGroups.item));
				Add(/*unit = */ new TI.TagReference(this, TagGroups.unit));
				Add(/*vehicles = */ new TI.Block<vehicles_block>(this, 20));
				Add(/*hill shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*flag shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*ball = */ new TI.TagReference(this, TagGroups.item));
				Add(/*sounds = */ new TI.Block<sounds_block>(this, 60));
				Add(/*in game text = */ new TI.TagReference(this, TagGroups.unic));
				Add(new TI.Pad(40));
				Add(/*general events = */ new TI.Block<game_engine_general_event_block>(this, 128));
				Add(/*slayer events = */ new TI.Block<game_engine_slayer_event_block>(this, 128));
				Add(/*ctf events = */ new TI.Block<game_engine_ctf_event_block>(this, 128));
				Add(/*oddball events = */ new TI.Block<game_engine_oddball_event_block>(this, 128));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/*king events = */ new TI.Block<game_engine_king_event_block>(this, 128));
			}
			#endregion
		}
		#endregion

		#region player_information_block
		[TI.Definition(1, 372)]
		public class player_information_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_information_block() : base(38)
			{
				Add(/*unused = */ new TI.TagReference(this, TagGroups.unit));
				Add(new TI.Pad(28));
				Add(/*walking speed = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*run forward = */ new TI.Real());
				Add(/*run backward = */ new TI.Real());
				Add(/*run sideways = */ new TI.Real());
				Add(/*run acceleration = */ new TI.Real());
				Add(/*sneak forward = */ new TI.Real());
				Add(/*sneak backward = */ new TI.Real());
				Add(/*sneak sideways = */ new TI.Real());
				Add(/*sneak acceleration = */ new TI.Real());
				Add(/*airborne acceleration = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*grenade origin = */ new TI.RealPoint3D());
				Add(new TI.Pad(12));
				Add(/*stun movement penalty = */ new TI.Real());
				Add(/*stun turning penalty = */ new TI.Real());
				Add(/*stun jumping penalty = */ new TI.Real());
				Add(/*minimum stun time = */ new TI.Real());
				Add(/*maximum stun time = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*first person idle time = */ new TI.RealBounds());
				Add(/*first person skip fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Pad(16));
				Add(/*coop respawn effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*binoculars zoom count = */ new TI.LongInteger());
				Add(/*binoculars zoom range = */ new TI.RealBounds());
				Add(/*binoculars zoom in sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*binoculars zoom out sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(new TI.Pad(16));
				Add(/*active camouflage on = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*active camouflage off = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*active camouflage error = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*active camouflage ready = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*flashlight on = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*flashlight off = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*ice cream = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region player_representation_block
		[TI.Definition(1, 212)]
		public class player_representation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_representation_block() : base(5)
			{
				Add(/*first person hands = */ new TI.TagReference(this, TagGroups.mode));
				Add(/*first person body = */ new TI.TagReference(this, TagGroups.mode));
				Add(new TI.Pad(40 + 120));
				Add(/*third person unit = */ new TI.TagReference(this, TagGroups.unit));
				Add(/*third person variant = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region falling_damage_block
		[TI.Definition(1, 152)]
		public class falling_damage_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public falling_damage_block() : base(11)
			{
				Add(new TI.Pad(8));
				Add(/*harmful falling distance = */ new TI.RealBounds());
				Add(/*falling damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.Pad(8));
				Add(/*maximum falling distance = */ new TI.Real());
				Add(/*distance damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*vehicle environemtn collision damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*vehicle killed unit damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*vehicle collision damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*flaming death damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.Pad(28));
			}
			#endregion
		}
		#endregion

		#region old_materials_block
		[TI.Definition(1, 44)]
		public class old_materials_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public old_materials_block() : base(11)
			{
				Add(new TI.UselessPad(4));
				Add(/*new material name = */ new TI.StringId());
				Add(/*new general material name = */ new TI.StringId());
				Add(new TI.UselessPad(88 + 48));
				Add(/*ground friction scale = */ new TI.Real());
				Add(/*ground friction normal k1 scale = */ new TI.Real());
				Add(/*ground friction normal k0 scale = */ new TI.Real());
				Add(/*ground depth scale = */ new TI.Real());
				Add(/*ground damp fraction scale = */ new TI.Real());
				Add(new TI.UselessPad(76 + 624));
				Add(/*melee hit sound = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region materials_block
		[TI.Definition(1, 316)]
		public class materials_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public materials_block() : base(13)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*parent name = */ new TI.StringId());
				Add(new TI.Pad(2));
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*old material type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*general armor = */ new TI.StringId());
				Add(/*specific armor = */ new TI.StringId());
				Add(/*physics properties = */ new TI.Struct<material_physics_properties_struct>(this));
				Add(/*old material physics = */ new TI.TagReference(this, TagGroups.mpdt));
				Add(/*breakable surface = */ new TI.TagReference(this, TagGroups.bsdt));
				Add(/*sweeteners = */ new TI.Struct<materials_sweeteners_struct>(this));
				Add(/*material effects = */ new TI.TagReference(this, TagGroups.foot));
			}
			#endregion
		}
		#endregion

		#region multiplayer_ui_block
		[TI.Definition(1, 56)]
		public class multiplayer_ui_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public multiplayer_ui_block() : base(4)
			{
				Add(/*random player names = */ new TI.TagReference(this, TagGroups.unic));
				Add(/*obsolete profile colors = */ new TI.Block<multiplayer_color_block>(this, 32));
				Add(/*team colors = */ new TI.Block<multiplayer_color_block>(this, 32));
				Add(/*team names = */ new TI.TagReference(this, TagGroups.unic));
			}
			#endregion
		}
		#endregion

		#region runtime_levels_definition_block
		[TI.Definition(1, 12)]
		public class runtime_levels_definition_block : TI.Definition
		{
			#region runtime_campaign_level_block
			[TI.Definition(1, 264)]
			public class runtime_campaign_level_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public runtime_campaign_level_block() : base(3)
				{
					Add(/*Campaign ID = */ new TI.LongInteger());
					Add(/*Map ID = */ new TI.LongInteger());
					Add(/*Path = */ new TI.String(TI.StringType.Ascii, 256));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public runtime_levels_definition_block() : base(1)
			{
				Add(/*Campaign Levels = */ new TI.Block<runtime_campaign_level_block>(this, 20));
			}
			#endregion
		}
		#endregion

		#region ui_levels_definition_block
		[TI.Definition(1, 36)]
		public class ui_levels_definition_block : TI.Definition
		{
			#region ui_campaign_block
			[TI.Definition(1, 2884)]
			public class ui_campaign_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ui_campaign_block() : base(2)
				{
					Add(/*Campaign ID = */ new TI.LongInteger());
					Add(new TI.Skip(576 + 2304));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public ui_levels_definition_block() : base(3)
			{
				Add(/*Campaigns = */ new TI.Block<ui_campaign_block>(this, 4));
				Add(/*Campaign Levels = */ new TI.Block<global_ui_campaign_level_block>(this, 20));
				Add(/*Multiplayer Levels = */ new TI.Block<global_ui_multiplayer_level_block>(this, 50));
			}
			#endregion
		}
		#endregion

		#region Fields
		public s_cache_language_pack
				English,
				Japanese,
				German,
				French,
				Spanish,
				Italian,
				Korean,
				Chinese,
				Portuguese;
		#endregion

		#region Ctor
		public globals_group() : base(83)
		{
			Add(new TI.Pad(172));
			Add(/*language = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*havok cleanup resources = */ new TI.Block<havok_cleanup_resources_block>(this, 1));
			Add(/*collision damage = */ new TI.Block<collision_damage_block>(this, 1));
 			Add(/*sound globals = */ new TI.Block<sound_globals_block>(this, 1));
 			Add(/*ai globals = */ new TI.Block<ai_globals_block>(this, 1));
 			Add(/*damage table = */ new TI.Block<game_globals_damage_block>(this, 1));
 			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/*sounds = */ new TI.Block<sound_block>(this, 2));
			Add(/*camera = */ new TI.Block<camera_block>(this, 1));
			Add(/*player control = */ new TI.Block<player_control_block>(this, 1));
			Add(/*difficulty = */ new TI.Block<difficulty_block>(this, 1));
			Add(/*grenades = */ new TI.Block<grenades_block>(this, 2));
			Add(/*rasterizer data = */ new TI.Block<rasterizer_data_block>(this, 1));
			Add(/*interface tags = */ new TI.Block<interface_tag_references>(this, 1));
			Add(/*@weapon list (update _weapon_list enum in game_globals.h) = */ new TI.Block<cheat_weapons_block>(this, 20));
			Add(/*@cheat powerups = */ new TI.Block<cheat_powerups_block>(this, 20));
			Add(/*@multiplayer information = */ new TI.Block<multiplayer_information_block>(this, 1));
			Add(/*@player information = */ new TI.Block<player_information_block>(this, 1));
			Add(/*@player representation = */ new TI.Block<player_representation_block>(this, 4));
			Add(/*falling damage = */ new TI.Block<falling_damage_block>(this, 1));
			Add(/*old materials = */ new TI.Block<old_materials_block>(this, 33));
			Add(/*materials = */ new TI.Block<materials_block>(this, 256));
			Add(/*multiplayer UI = */ new TI.Block<multiplayer_ui_block>(this, 1));
			Add(/*profile colors = */ new TI.Block<multiplayer_color_block>(this, 32));
			Add(/*multiplayer globals = */ new TI.TagReference(this, TagGroups.mulg));
			Add(/*runtime level data = */ new TI.Block<runtime_levels_definition_block>(this, 1));
			Add(/*ui level data = */ new TI.Block<ui_levels_definition_block>(this, 1));
			Add(/*default global lighting = */ new TI.TagReference(this, TagGroups.gldf));
			
			// the following was added after the alpha
			//Add(new TI.Pad(252));
			English = new s_cache_language_pack(this);
			Japanese = new s_cache_language_pack(this);
			German = new s_cache_language_pack(this);
			French = new s_cache_language_pack(this);
			Spanish = new s_cache_language_pack(this);
			Italian = new s_cache_language_pack(this);
			Korean = new s_cache_language_pack(this);
			Chinese = new s_cache_language_pack(this);
			Portuguese = new s_cache_language_pack(this);
		}
		#endregion
	};
	#endregion

	#region material_effects
	[TI.TagGroup((int)TagGroups.Enumerated.foot, 1, 2, 12)]
	public class material_effects_group : TI.Definition
	{
		#region material_effect_block_v2
		[TI.Definition(1, 36)]
		public class material_effect_block_v2 : TI.Definition
		{
			#region old_material_effect_material_block
			[TI.Definition(1, 44)]
			public class old_material_effect_material_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public old_material_effect_material_block() : base(7)
				{
					Add(/*effect = */ new TI.TagReference(this, TagGroups.effe));
					Add(/*sound = */ new TI.TagReference(this)); // snd!,lsnd,
					Add(/*material name = */ new TI.StringId());
					Add(new TI.Skip(4));
					Add(/*sweetener mode = */ new TI.Enum(TI.FieldType.ByteEnum));
					Add(new TI.Pad(3));
					Add(new TI.UselessPad(4));
				}
				#endregion
			}
			#endregion

			#region material_effect_material_block
			[TI.Definition(2, 40)]
			public class material_effect_material_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public material_effect_material_block() : base(6)
				{
					Add(/*tag (effect or sound) = */ new TI.TagReference(this)); // snd!,lsnd,effe,
					Add(/*secondary tag (effect or sound) = */ new TI.TagReference(this)); // snd!,lsnd,effe,
					Add(/*material name = */ new TI.StringId());
					Add(new TI.Skip(2));
					Add(/*sweetener mode = */ new TI.Enum(TI.FieldType.ByteEnum));
					Add(new TI.Pad(1));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public material_effect_block_v2() : base(3)
			{
				Add(/*old materials (DO NOT USE) = */ new TI.Block<old_material_effect_material_block>(this, 33));
				Add(/*sounds = */ new TI.Block<material_effect_material_block>(this, 500));
				Add(/*effects = */ new TI.Block<material_effect_material_block>(this, 500));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public material_effects_group() : base(2)
		{
			Add(/*effects = */ new TI.Block<material_effect_block_v2>(this, 21));
			Add(new TI.UselessPad(128));
		}
		#endregion
	};
	#endregion

	#region material_physics
	[TI.TagGroup((int)TagGroups.Enumerated.mpdt, 0, 20)]
	public class material_physics_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public material_physics_group() : base(5)
		{
			// Explanation here
			Add(/*ground friction scale = */ new TI.Real());
			Add(/*ground friction normal k1 scale = */ new TI.Real());
			Add(/*ground friction normal k0 scale = */ new TI.Real());
			Add(/*ground depth scale = */ new TI.Real());
			Add(/*ground damp fraction scale = */ new TI.Real());
		}
		#endregion
	};
	#endregion

	#region weapons_block
	[TI.Definition(1, 16)]
	public class weapons_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapons_block() : base(1)
		{
			Add(/*weapon = */ new TI.TagReference(this, TagGroups.item));
		}
		#endregion
	}
	#endregion

	#region grenade_block
	[TI.Definition(1, 16)]
	public class grenade_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public grenade_block() : base(1)
		{
			Add(/*weapon = */ new TI.TagReference(this, TagGroups.eqip));
		}
		#endregion
	}
	#endregion

	#region powerup_block
	[TI.Definition(1, 16)]
	public class powerup_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public powerup_block() : base(1)
		{
			Add(/*weapon = */ new TI.TagReference(this, TagGroups.eqip));
		}
		#endregion
	}
	#endregion

	#region grenade_and_powerup_struct
	[TI.Struct((int)StructGroups.Enumerated.gapu, 2, 24)]
	public class grenade_and_powerup_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public grenade_and_powerup_struct() : base(2)
		{
			Add(/*grenades = */ new TI.Block<grenade_block>(this, 20));
			Add(/*powerups = */ new TI.Block<powerup_block>(this, 20));
		}
		#endregion
	}
	#endregion

	#region multiplayer_globals
	[TI.TagGroup((int)TagGroups.Enumerated.mulg, 1, 24)]
	public class multiplayer_globals_group : TI.Definition
	{
		#region multiplayer_runtime_block
		[TI.Definition(2, 1640)]
		public class multiplayer_runtime_block : TI.Definition
		{
			#region game_engine_flavor_event_block
			[TI.Definition(1, 244)]
			public class game_engine_flavor_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_engine_flavor_event_block() : base(19)
				{
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*event = */ new TI.Enum());
					Add(/*audience = */ new TI.Enum());
					Add(new TI.Pad(2 + 2));
					Add(/*display string = */ new TI.StringId());
					Add(/*required field = */ new TI.Enum());
					Add(/*excluded audience = */ new TI.Enum());
					Add(/*primary string = */ new TI.StringId());
					Add(/*primary string duration = */ new TI.LongInteger());
					Add(/*plural display string = */ new TI.StringId());
					Add(new TI.Pad(28));
					Add(/*sound delay (announcer only) = */ new TI.Real());
					Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
					Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
					Add(new TI.Pad(4 + 16));
					Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
				}
				#endregion
			}
			#endregion

			#region game_engine_juggernaut_event_block
			[TI.Definition(1, 244)]
			public class game_engine_juggernaut_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_engine_juggernaut_event_block() : base(19)
				{
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*event = */ new TI.Enum());
					Add(/*audience = */ new TI.Enum());
					Add(new TI.Pad(2 + 2));
					Add(/*display string = */ new TI.StringId());
					Add(/*required field = */ new TI.Enum());
					Add(/*excluded audience = */ new TI.Enum());
					Add(/*primary string = */ new TI.StringId());
					Add(/*primary string duration = */ new TI.LongInteger());
					Add(/*plural display string = */ new TI.StringId());
					Add(new TI.Pad(28));
					Add(/*sound delay (announcer only) = */ new TI.Real());
					Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
					Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
					Add(new TI.Pad(4 + 16));
					Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
				}
				#endregion
			}
			#endregion

			#region game_engine_territories_event_block
			[TI.Definition(1, 244)]
			public class game_engine_territories_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_engine_territories_event_block() : base(19)
				{
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*event = */ new TI.Enum());
					Add(/*audience = */ new TI.Enum());
					Add(new TI.Pad(2 + 2));
					Add(/*display string = */ new TI.StringId());
					Add(/*required field = */ new TI.Enum());
					Add(/*excluded audience = */ new TI.Enum());
					Add(/*primary string = */ new TI.StringId());
					Add(/*primary string duration = */ new TI.LongInteger());
					Add(/*plural display string = */ new TI.StringId());
					Add(new TI.Pad(28));
					Add(/*sound delay (announcer only) = */ new TI.Real());
					Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
					Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
					Add(new TI.Pad(4 + 16));
					Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
				}
				#endregion
			}
			#endregion

			#region game_engine_assault_event_block
			[TI.Definition(1, 244)]
			public class game_engine_assault_event_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_engine_assault_event_block() : base(19)
				{
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*event = */ new TI.Enum());
					Add(/*audience = */ new TI.Enum());
					Add(new TI.Pad(2 + 2));
					Add(/*display string = */ new TI.StringId());
					Add(/*required field = */ new TI.Enum());
					Add(/*excluded audience = */ new TI.Enum());
					Add(/*primary string = */ new TI.StringId());
					Add(/*primary string duration = */ new TI.LongInteger());
					Add(/*plural display string = */ new TI.StringId());
					Add(new TI.Pad(28));
					Add(/*sound delay (announcer only) = */ new TI.Real());
					Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
					Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
					Add(new TI.Pad(4 + 16));
					Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
				}
				#endregion
			}
			#endregion

			#region multiplayer_constants_block
			[TI.Definition(1, 384)]
			public class multiplayer_constants_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public multiplayer_constants_block() : base(21)
				{
					Add(/*maximum random spawn bias = */ new TI.Real());
					Add(/*teleporter recharge time = */ new TI.Real());
					Add(/*grenade danger weight = */ new TI.Real());
					Add(/*grenade danger inner radius = */ new TI.Real());
					Add(/*grenade danger outer radius = */ new TI.Real());
					Add(/*grenade danger lead time = */ new TI.Real());
					Add(/*vehicle danger min speed = */ new TI.Real());
					Add(/*vehicle danger weight = */ new TI.Real());
					Add(/*vehicle danger radius = */ new TI.Real());
					Add(/*vehicle danger lead time = */ new TI.Real());
					Add(/*vehicle nearby player dist = */ new TI.Real());
					Add(new TI.Pad(84 + 32 + 32));
					Add(/*hill shader = */ new TI.TagReference(this, TagGroups.shad));
					Add(new TI.Pad(16));
					Add(/*flag reset stop distance = */ new TI.Real());
					Add(/*bomb explode effect = */ new TI.TagReference(this, TagGroups.effe));
					Add(/*bomb explode dmg effect = */ new TI.TagReference(this, TagGroups.jpt_));
					Add(/*bomb defuse effect = */ new TI.TagReference(this, TagGroups.effe));
					Add(/*bomb defusal string = */ new TI.StringId());
					Add(/*blocked teleporter string = */ new TI.StringId());
					Add(new TI.Pad(4 + 32 + 32 + 32));
				}
				#endregion
			}
			#endregion

			#region game_engine_status_response_block
			[TI.Definition(1, 36)]
			public class game_engine_status_response_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public game_engine_status_response_block() : base(8)
				{
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2));
					Add(/*state = */ new TI.Enum());
					Add(new TI.Pad(2));
					Add(/*ffa message = */ new TI.StringId());
					Add(/*team message = */ new TI.StringId());
					Add(/* = */ new TI.TagReference(this));
					Add(new TI.Pad(4));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public multiplayer_runtime_block() : base(96)
			{
				Add(/*flag = */ new TI.TagReference(this, TagGroups.item));
				Add(/*ball = */ new TI.TagReference(this, TagGroups.item));
				Add(/*unit = */ new TI.TagReference(this, TagGroups.unit));
				Add(/*flag shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*hill shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*head = */ new TI.TagReference(this, TagGroups.item));
				Add(/*juggernaut powerup = */ new TI.TagReference(this, TagGroups.item));
				Add(/*da bomb = */ new TI.TagReference(this, TagGroups.item));
				Add(/* = */ new TI.TagReference(this));
				Add(/* = */ new TI.TagReference(this));
				Add(/* = */ new TI.TagReference(this));
				Add(/* = */ new TI.TagReference(this));
				Add(/* = */ new TI.TagReference(this));
				Add(/*weapons = */ new TI.Block<weapons_block>(this, 20));
				Add(/*vehicles = */ new TI.Block<vehicles_block>(this, 20));
				Add(/*arr! = */ new TI.Struct<grenade_and_powerup_struct>(this));
				Add(/*in game text = */ new TI.TagReference(this, TagGroups.unic));
				Add(/*sounds = */ new TI.Block<sounds_block>(this, 60));
				Add(/*general events = */ new TI.Block<game_engine_general_event_block>(this, 128));
				Add(/*flavor events = */ new TI.Block<game_engine_flavor_event_block>(this, 128));
				Add(/*slayer events = */ new TI.Block<game_engine_slayer_event_block>(this, 128));
				Add(/*ctf events = */ new TI.Block<game_engine_ctf_event_block>(this, 128));
				Add(/*oddball events = */ new TI.Block<game_engine_oddball_event_block>(this, 128));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/*king events = */ new TI.Block<game_engine_king_event_block>(this, 128));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/*juggernaut events = */ new TI.Block<game_engine_juggernaut_event_block>(this, 128));
				Add(/*territories events = */ new TI.Block<game_engine_territories_event_block>(this, 128));
				Add(/*invasion events = */ new TI.Block<game_engine_assault_event_block>(this, 128));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(/*default item collection 1 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*default item collection 2 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*default frag grenade count = */ new TI.LongInteger());
				Add(/*default plasma grenade count = */ new TI.LongInteger());
				Add(new TI.Pad(40));
				Add(/*dynamic zone upper height = */ new TI.Real());
				Add(/*dynamic zone lower height = */ new TI.Real());
				Add(new TI.Pad(40));
				Add(/*enemy inner radius = */ new TI.Real());
				Add(/*enemy outer radius = */ new TI.Real());
				Add(/*enemy weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*friend inner radius = */ new TI.Real());
				Add(/*friend outer radius = */ new TI.Real());
				Add(/*friend weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*enemy vehicle inner radius = */ new TI.Real());
				Add(/*enemy vehicle outer radius = */ new TI.Real());
				Add(/*enemy vehicle weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*friendly vehicle inner radius = */ new TI.Real());
				Add(/*friendly vehicle outer radius = */ new TI.Real());
				Add(/*friendly vehicle weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*empty vehicle inner radius = */ new TI.Real());
				Add(/*empty vehicle outer radius = */ new TI.Real());
				Add(/*empty vehicle weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*oddball inclusion inner radius = */ new TI.Real());
				Add(/*oddball inclusion outer radius = */ new TI.Real());
				Add(/*oddball inclusion weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*oddball exclusion inner radius = */ new TI.Real());
				Add(/*oddball exclusion outer radius = */ new TI.Real());
				Add(/*oddball exclusion weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*hill inclusion inner radius = */ new TI.Real());
				Add(/*hill inclusion outer radius = */ new TI.Real());
				Add(/*hill inclusion weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*hill exclusion inner radius = */ new TI.Real());
				Add(/*hill exclusion outer radius = */ new TI.Real());
				Add(/*hill exclusion weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*last race flag inner radius = */ new TI.Real());
				Add(/*last race flag outer radius = */ new TI.Real());
				Add(/*last race flag weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*dead ally inner radius = */ new TI.Real());
				Add(/*dead ally outer radius = */ new TI.Real());
				Add(/*dead ally weight = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*controlled territory inner radius = */ new TI.Real());
				Add(/*controlled territory outer radius = */ new TI.Real());
				Add(/*controlled territory weight = */ new TI.Real());
				Add(new TI.Pad(16 + 560 + 48));
				Add(/*multiplayer constants = */ new TI.Block<multiplayer_constants_block>(this, 1));
				Add(/*state responses = */ new TI.Block<game_engine_status_response_block>(this, 32));
				Add(/*scoreboard hud definition = */ new TI.TagReference(this, TagGroups.nhdt));
				Add(/*scoreboard emblem shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*scoreboard emblem bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*scoreboard dead emblem shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*scoreboard dead emblem bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public multiplayer_globals_group() : base(2)
		{
			Add(/*universal = */ new TI.Block<multiplayer_universal_block>(this, 1));
			Add(/*runtime = */ new TI.Block<multiplayer_runtime_block>(this, 1));
		}
		#endregion
	};
	#endregion

	#region g_default_variant_settings_block
	[TI.Definition(1, 8)]
	public class g_default_variant_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public g_default_variant_settings_block() : base(2)
		{
			Add(/*setting category = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*value = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region create_new_variant_struct
	[TI.Struct((int)StructGroups.Enumerated.cnvs, 1, 24)]
	public class create_new_variant_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public create_new_variant_struct() : base(5)
		{
			Add(/* = */ new TI.StringId());
			Add(/* = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*settings = */ new TI.Block<g_default_variant_settings_block>(this, 112));
			Add(/* = */ new TI.ByteInteger());
			Add(new TI.Pad(3));
		}
		#endregion
	}
	#endregion

	#region multiplayer_variant_settings_interface_definition
	[TI.TagGroup((int)TagGroups.Enumerated.goof, 1, 472)]
	public class multiplayer_variant_settings_interface_definition_group : TI.Definition
	{
		#region variant_setting_edit_reference_block
		[TI.Definition(1, 32)]
		public class variant_setting_edit_reference_block : TI.Definition
		{
			#region text_value_pair_block
			[TI.Definition(1, 16)]
			public class text_value_pair_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public text_value_pair_block() : base(1)
				{
					Add(/*value pairs = */ new TI.TagReference(this, TagGroups.sily));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public variant_setting_edit_reference_block() : base(4)
			{
				Add(/*setting category = */ new TI.Enum(TI.FieldType.LongEnum));
				Add(new TI.Pad(4));
				Add(/*options = */ new TI.Block<text_value_pair_block>(this, 32));
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			}
			#endregion
		}
		#endregion

		#region g_default_variants_block
		[TI.Definition(1, 24)]
		public class g_default_variants_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public g_default_variants_block() : base(5)
			{
				Add(/*variant name = */ new TI.StringId());
				Add(/*variant type = */ new TI.Enum(TI.FieldType.LongEnum));
				Add(/*settings = */ new TI.Block<g_default_variant_settings_block>(this, 112));
				Add(/*description index = */ new TI.ByteInteger());
				Add(new TI.Pad(3));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public multiplayer_variant_settings_interface_definition_group() : base(22)
		{
			Add(/* = */ new TI.TagReference(this, TagGroups.wgit));
			Add(/* = */ new TI.TagReference(this, TagGroups.wgit));
			Add(/* = */ new TI.TagReference(this, TagGroups.wgit));
			Add(/*game engine settings = */ new TI.Block<variant_setting_edit_reference_block>(this, 40));
			Add(/*default variant strings = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*default variants = */ new TI.Block<g_default_variants_block>(this, 100));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new slayer variant
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new king of the hill variant
			
			// Custom: hide
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			// Custom: edih

			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new oddball variant
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new juggernaut variant
			
			// Custom: hide
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			// Custom: edih

			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new capture the flag variant
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new assault variant
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this)); // create new territories variant
			
			// Custom: hide
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			Add(/* = */ new TI.Struct<create_new_variant_struct>(this));
			// Custom: edih
		}
		#endregion
	};
	#endregion
}