/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region global_hud_multitexture_overlay_definition
	[TI.Definition(1, 480)]
	public class global_hud_multitexture_overlay_definition : TI.Definition
	{
		#region global_hud_multitexture_overlay_effector_definition
		[TI.Definition(1, 220)]
		public class global_hud_multitexture_overlay_effector_definition : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_hud_multitexture_overlay_effector_definition() : base(15)
			{
				Add(new TI.Pad(64));
				Add(/*destination type = */ new TI.Enum());
				Add(/*destination = */ new TI.Enum());
				Add(/*source = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*in bounds = */ new TI.RealBounds());
				Add(/*out bounds = */ new TI.RealBounds());
				Add(new TI.Pad(64));
				Add(/*tint color lower bound = */ new TI.RealColor());
				Add(/*tint color upper bound = */ new TI.RealColor());
				Add(/*periodic function = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*function period = */ new TI.Real());
				Add(/*function phase = */ new TI.Real());
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public global_hud_multitexture_overlay_definition() : base(25)
		{
			Add(new TI.Pad(2));
			Add(/*type = */ new TI.ShortInteger());
			Add(/*framebuffer blend func = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*primary anchor = */ new TI.Enum());
			Add(/*secondary anchor = */ new TI.Enum());
			Add(/*tertiary anchor = */ new TI.Enum());
			Add(/*0 to 1 blend func = */ new TI.Enum());
			Add(/*1 to 2 blend func = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*primary scale = */ new TI.RealPoint2D());
			Add(/*secondary scale = */ new TI.RealPoint2D());
			Add(/*tertiary scale = */ new TI.RealPoint2D());
			Add(/*primary offset = */ new TI.RealPoint2D());
			Add(/*secondary offset = */ new TI.RealPoint2D());
			Add(/*tertiary offset = */ new TI.RealPoint2D());
			Add(/*primary = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*secondary = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*tertiary = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*primary wrap mode = */ new TI.Enum());
			Add(/*secondary wrap mode = */ new TI.Enum());
			Add(/*tertiary wrap mode = */ new TI.Enum());
			Add(new TI.Pad(2 + 184));
			Add(/*effectors = */ new TI.Block<global_hud_multitexture_overlay_effector_definition>(this, 30));
			Add(new TI.Pad(128));
		}
		#endregion
	}
	#endregion

	#region grenade_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.grhi, 1, 504)]
	public class grenade_hud_interface_group : TI.Definition
	{
		#region grenade_hud_overlay_block
		[TI.Definition(1, 136)]
		public class grenade_hud_overlay_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public grenade_hud_overlay_block() : base(19)
			{
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*default color = */ new TI.Color());
				Add(/*flashing color = */ new TI.Color());
				Add(/*flash period = */ new TI.Real());
				Add(/*flash delay = */ new TI.Real());
				Add(/*number of flashes = */ new TI.ShortInteger());
				Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flash length = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(new TI.Pad(4));
				Add(/*frame rate = */ new TI.Real());
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(/*type = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flags = */ new TI.Flags());
				Add(new TI.Pad(16 + 40));
			}
			#endregion
		}
		#endregion

		#region grenade_hud_sound_block
		[TI.Definition(1, 56)]
		public class grenade_hud_sound_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public grenade_hud_sound_block() : base(4)
			{
				Add(/*sound = */ new TI.TagReference(this)); // snd!,lsnd,
				Add(/*latched to = */ new TI.Flags());
				Add(/*scale = */ new TI.Real());
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public grenade_hud_interface_group() : base(72)
		{
			Add(/*anchor = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*maximum number of digits = */ new TI.ByteInteger());
			Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*number of fractional digits = */ new TI.ByteInteger());
			Add(new TI.Pad(1 + 12));
			Add(/*flash cutoff = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*Overlay bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*Overlays = */ new TI.Block<grenade_hud_overlay_block>(this, 16));
			Add(/*Warning sounds = */ new TI.Block<grenade_hud_sound_block>(this, 12));
			Add(new TI.Pad(68));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(/*width offset = */ new TI.ShortInteger());
			Add(/*offset from reference corner = */ new TI.Point2D());
			Add(/*override icon color = */ new TI.Color());
			Add(/*frame rate [0,30] = */ new TI.ByteInteger());
			Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*text index = */ new TI.ShortInteger());
			Add(new TI.Pad(48));
		}
		#endregion
	};
	#endregion

	#region global_new_hud_globals_constants_struct
	[TI.Struct((int)StructGroups.Enumerated.nhgs_const, 1, 184)]
	public class global_new_hud_globals_constants_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_new_hud_globals_constants_struct() : base(13)
		{
			Add(/*primary message sound = */ new TI.TagReference(this)); // snd!,lsnd,
			Add(/*secondary message sound = */ new TI.TagReference(this)); // snd!,lsnd,
			Add(/*boot griefer string = */ new TI.StringId());
			Add(/*cannot boot griefer string = */ new TI.StringId());
			Add(/*training shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*human training top right = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*human training top center = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*human training top left = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*human training middle = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*elite training top right = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*elite training top center = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*elite training top left = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*elite training middle = */ new TI.TagReference(this, TagGroups.bitm));
		}
		#endregion
	}
	#endregion

	#region global_new_hud_globals_struct
	[TI.Struct((int)StructGroups.Enumerated.nhgs, 1, 260)]
	public class global_new_hud_globals_struct : TI.Definition
	{
		#region hud_dashlights_block
		[TI.Definition(1, 52)]
		public class hud_dashlights_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_dashlights_block() : base(5)
			{
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region hud_waypoint_block
		[TI.Definition(1, 40)]
		public class hud_waypoint_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_waypoint_block() : base(6)
			{
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*onscreen sequence index = */ new TI.ShortInteger());
				Add(/*occluded sequence index = */ new TI.ShortInteger());
				Add(/*offscreen sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region new_hud_sound_block
		[TI.Definition(1, 40)]
		public class new_hud_sound_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public new_hud_sound_block() : base(4)
			{
				Add(/*chief sound = */ new TI.TagReference(this)); // snd!,lsnd,
				Add(/*latched to = */ new TI.Flags());
				Add(/*scale = */ new TI.Real());
				Add(/*dervish sound = */ new TI.TagReference(this)); // snd!,lsnd,
			}
			#endregion
		}
		#endregion

		#region player_training_entry_data_block
		[TI.Definition(1, 28)]
		public class player_training_entry_data_block : TI.Definition
		{
			#region Fields
			public TI.StringId DisplayString;
			#endregion

			#region Ctor
			public player_training_entry_data_block() : base(10)
			{
				Add(DisplayString = new TI.StringId());
				Add(/*display string2 = */ new TI.StringId());
				Add(/*display string3 = */ new TI.StringId());
				Add(/*max display time = */ new TI.ShortInteger());
				Add(/*display count = */ new TI.ShortInteger());
				Add(/*dissapear delay = */ new TI.ShortInteger());
				Add(/*redisplay delay = */ new TI.ShortInteger());
				Add(/*display delay (s) = */ new TI.Real());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<player_training_entry_data_block> PlayerTrainingData;
		#endregion

		#region Ctor
		public global_new_hud_globals_struct() : base(7)
		{
			Add(/*hud text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*dashlights = */ new TI.Block<hud_dashlights_block>(this, 9));
			Add(/*waypoint arrows = */ new TI.Block<hud_globals_group.hud_waypoint_arrow_block>(this, 4));
			Add(/*waypoints = */ new TI.Block<hud_waypoint_block>(this, 8));
			Add(/*hud sounds = */ new TI.Block<new_hud_sound_block>(this, 6));
			Add(PlayerTrainingData = new TI.Block<player_training_entry_data_block>(this, 32));
			Add(/*constants = */ new TI.Struct<global_new_hud_globals_constants_struct>(this));
		}
		#endregion
	}
	#endregion

	#region hud_globals
	[TI.TagGroup((int)TagGroups.Enumerated.hudg, 1, 1364)]
	public class hud_globals_group : TI.Definition
	{
		#region hud_button_icon_block
		[TI.Definition(1, 16)]
		public class hud_button_icon_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_button_icon_block() : base(7)
			{
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(/*width offset = */ new TI.ShortInteger());
				Add(/*offset from reference corner = */ new TI.Point2D());
				Add(/*override icon color = */ new TI.Color());
				Add(/*frame rate [0,30] = */ new TI.ByteInteger());
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*text index = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region hud_waypoint_arrow_block
		[TI.Definition(1, 104)]
		public class hud_waypoint_arrow_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_waypoint_arrow_block() : base(12)
			{
				Add(/*name = */ new TI.String());
				Add(new TI.Pad(8));
				Add(/*color = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*opacity = */ new TI.Real());
				Add(/*translucency = */ new TI.Real());
				Add(/*on screen sequence index = */ new TI.ShortInteger());
				Add(/*off screen sequence index = */ new TI.ShortInteger());
				Add(/*occluded sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(new TI.Pad(16));
				Add(/*flags = */ new TI.Flags());
				Add(new TI.Pad(24));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Struct<global_new_hud_globals_struct> NewGlobals;
		#endregion

		#region Ctor
		public hud_globals_group() : base(89)
		{
			Add(/*anchor = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*obsolete1 = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*obsolete2 = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*up time = */ new TI.Real());
			Add(/*fade time = */ new TI.Real());
			Add(/*icon color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*text spacing = */ new TI.Real());
			Add(/*item message text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*icon bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*alternate icon text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*button icons = */ new TI.Block<hud_button_icon_block>(this, 18));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*hud messages = */ new TI.TagReference(this, TagGroups.hmt_));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(/*uptime ticks = */ new TI.ShortInteger());
			Add(/*fade ticks = */ new TI.ShortInteger());
			Add(/*top offset = */ new TI.Real());
			Add(/*bottom offset = */ new TI.Real());
			Add(/*left offset = */ new TI.Real());
			Add(/*right offset = */ new TI.Real());
			Add(new TI.Pad(32));
			Add(/*arrow bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*waypoint arrows = */ new TI.Block<hud_waypoint_arrow_block>(this, 16));
			Add(new TI.Pad(80));
			Add(/*hud scale in multiplayer = */ new TI.Real());
			Add(new TI.Pad(256 + 16));
			Add(/*motion sensor range = */ new TI.Real());
			Add(/*motion sensor velocity sensitivity = */ new TI.Real());
			Add(/*motion sensor scale [DON'T TOUCH EVER] = */ new TI.Real());
			Add(/*default chapter title bounds = */ new TI.Rectangle2D());
			Add(new TI.Pad(44));
			Add(/*top offset = */ new TI.ShortInteger());
			Add(/*bottom offset = */ new TI.ShortInteger());
			Add(/*left offset = */ new TI.ShortInteger());
			Add(/*right offset = */ new TI.ShortInteger());
			Add(new TI.Pad(32));
			Add(/*indicator bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(/*multiplayer sequence index = */ new TI.ShortInteger());
			Add(/*color = */ new TI.Color());
			Add(new TI.Pad(16));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4 + 40));
			Add(/*carnage report bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*loading begin text = */ new TI.ShortInteger());
			Add(/*loading end text = */ new TI.ShortInteger());
			Add(/*checkpoint begin text = */ new TI.ShortInteger());
			Add(/*checkpoint end text = */ new TI.ShortInteger());
			Add(/*checkpoint sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(96));
			Add(NewGlobals = new TI.Struct<global_new_hud_globals_struct>(this));
		}
		#endregion
	};
	#endregion

	#region hud_message_text
	[TI.TagGroup((int)TagGroups.Enumerated.hmt_, 1, 128)]
	public class hud_message_text_group : TI.Definition
	{
		#region hud_message_elements_block
		[TI.Definition(1, 2)]
		public class hud_message_elements_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_message_elements_block() : base(2)
			{
				Add(/*type = */ new TI.ByteInteger());
				Add(/*data = */ new TI.ByteInteger());
			}
			#endregion
		}
		#endregion

		#region hud_messages_block
		[TI.Definition(1, 64)]
		public class hud_messages_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_messages_block() : base(5)
			{
				Add(/*name = */ new TI.String());
				Add(/*start index into text blob = */ new TI.ShortInteger());
				Add(/*start index of message block = */ new TI.ShortInteger());
				Add(/*panel count = */ new TI.ByteInteger());
				Add(new TI.Pad(3 + 24));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public hud_message_text_group() : base(4)
		{
			Add(/*text data = */ new TI.Data(this));
			Add(/*message elements = */ new TI.Block<hud_message_elements_block>(this, 8192));
			Add(/*messages = */ new TI.Block<hud_messages_block>(this, 1024));
			Add(new TI.Pad(84));
		}
		#endregion
	};
	#endregion

	#region hud_number
	[TI.TagGroup((int)TagGroups.Enumerated.hud_, 1, 100)]
	public class hud_number_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public hud_number_group() : base(8)
		{
			Add(/*digits bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*bitmap digit width = */ new TI.ByteInteger());
			Add(/*screen digit width = */ new TI.ByteInteger());
			Add(/*x offset = */ new TI.ByteInteger());
			Add(/*y offset = */ new TI.ByteInteger());
			Add(/*decimal point width = */ new TI.ByteInteger());
			Add(/*colon width = */ new TI.ByteInteger());
			Add(new TI.Pad(2 + 76));
		}
		#endregion
	};
	#endregion

	#region meter
	[TI.TagGroup((int)TagGroups.Enumerated.metr, 1, 172)]
	public class meter_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public meter_group() : base(16)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*stencil bitmaps = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*source bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*stencil sequence index = */ new TI.ShortInteger());
			Add(/*source sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(16 + 4));
			Add(/*interpolate colors... = */ new TI.Enum());
			Add(/*anchor colors... = */ new TI.Enum());
			Add(new TI.Pad(8));
			Add(/*empty color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*full color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(new TI.Pad(20));
			Add(/*unmask distance = */ new TI.Real());
			Add(/*mask distance = */ new TI.Real());
			Add(new TI.Pad(20));
			Add(/*encoded stencil = */ new TI.Data(this));
		}
		#endregion
	};
	#endregion

	#region mouse_cursor_definition
	[TI.TagGroup((int)TagGroups.Enumerated.mcsr, 1, 16)]
	public class mouse_cursor_definition_group : TI.Definition
	{
		#region mouse_cursor_bitmap_reference_block
		[TI.Definition(1, 16)]
		public class mouse_cursor_bitmap_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public mouse_cursor_bitmap_reference_block() : base(1)
			{
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public mouse_cursor_definition_group() : base(2)
		{
			Add(/*mouse cursor bitmaps = */ new TI.Block<mouse_cursor_bitmap_reference_block>(this, 4));
			Add(/*animation speed (fps) = */ new TI.Real());
		}
		#endregion
	};
	#endregion

	#region hud_widget_inputs_struct
	[TI.Struct((int)StructGroups.Enumerated.hwis, 2, 4)]
	public class hud_widget_inputs_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public hud_widget_inputs_struct() : base(4)
		{
			Add(/*input 1 = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*input 2 = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*input 3 = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*input 4 = */ new TI.Enum(TI.FieldType.ByteEnum));
		}
		#endregion
	}
	#endregion

	#region hud_widget_state_definition_struct
	[TI.Struct((int)StructGroups.Enumerated.hwsd, 1, 20)]
	public class hud_widget_state_definition_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public hud_widget_state_definition_struct() : base(14)
		{
			// Explanation here
			Add(/*[Y] unit flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[Y] extra flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[Y] weapon flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[Y] game engine state flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.UselessPad(8));
			Add(/*[N] unit flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[N] extra flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[N] weapon flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*[N] game engine state flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.UselessPad(8));
			Add(/*age cutoff = */ new TI.ByteInteger());
			Add(/*clip cutoff = */ new TI.ByteInteger());
			Add(/*total cutoff = */ new TI.ByteInteger());
			Add(new TI.Pad(1));
		}
		#endregion
	}
	#endregion

	#region hud_widget_effect_function_struct
	[TI.Struct((int)StructGroups.Enumerated.hwef, 1, 24)]
	public class hud_widget_effect_function_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public hud_widget_effect_function_struct() : base(4)
		{
			Add(/*input name = */ new TI.StringId());
			Add(/*range name = */ new TI.StringId());
			Add(/*time period in seconds = */ new TI.Real());
			Add(/*function = */ new TI.Struct<scalar_function_struct>(this));
		}
		#endregion
	}
	#endregion

	#region new_hud_dashlight_data_struct
	[TI.Struct((int)StructGroups.Enumerated.nhd2, 1, 8)]
	public class new_hud_dashlight_data_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public new_hud_dashlight_data_struct() : base(3)
		{
			Add(/*low clip cutoff = */ new TI.ShortInteger());
			Add(/*low ammo cutoff = */ new TI.ShortInteger());
			Add(/*age cutoff = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region screen_effect_bonus_struct
	[TI.Struct((int)StructGroups.Enumerated.sebs, 2, 32)]
	public class screen_effect_bonus_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public screen_effect_bonus_struct() : base(2)
		{
			Add(/*halfscreen screen effect = */ new TI.TagReference(this, TagGroups.egor));
			Add(/*quarterscreen screen effect = */ new TI.TagReference(this, TagGroups.egor));
		}
		#endregion
	}
	#endregion

	#region new_hud_definition
	[TI.TagGroup((int)TagGroups.Enumerated.nhdt, 1, 60)]
	public class new_hud_definition_group : TI.Definition
	{
		#region hud_widget_effect_block
		[TI.Definition(1, 124)]
		public class hud_widget_effect_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_widget_effect_block() : base(7)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*your mom = */ new TI.Struct<hud_widget_effect_function_struct>(this));
				Add(/*your mom = */ new TI.Struct<hud_widget_effect_function_struct>(this));
				Add(/*your mom = */ new TI.Struct<hud_widget_effect_function_struct>(this));
				Add(/*your mom = */ new TI.Struct<hud_widget_effect_function_struct>(this));
				Add(/*your mom = */ new TI.Struct<hud_widget_effect_function_struct>(this));
			}
			#endregion
		}
		#endregion

		#region hud_bitmap_widgets
		[TI.Definition(1, 120)]
		public class hud_bitmap_widgets : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_bitmap_widgets() : base(20)
			{
				Add(/*name = */ new TI.StringId());
				Add(/* = */ new TI.Struct<hud_widget_inputs_struct>(this));
				Add(/* = */ new TI.Struct<hud_widget_state_definition_struct>(this));
				Add(/*anchor = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*fullscreen sequence index = */ new TI.ByteInteger());
				Add(/*halfscreen sequence index = */ new TI.ByteInteger());
				Add(/*quarterscreen sequence index = */ new TI.ByteInteger());
				Add(new TI.Pad(1));
				Add(/*fullscreen offset = */ new TI.Point2D());
				Add(/*halfscreen offset = */ new TI.Point2D());
				Add(/*quarterscreen offset = */ new TI.Point2D());
				Add(/*fullscreen registration point = */ new TI.RealPoint2D());
				Add(/*halfscreen registration point = */ new TI.RealPoint2D());
				Add(/*quarterscreen registration point = */ new TI.RealPoint2D());
				Add(/*effect = */ new TI.Block<hud_widget_effect_block>(this, 1));
				Add(/*special hud type = */ new TI.Enum());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region hud_text_widgets
		[TI.Definition(1, 96)]
		public class hud_text_widgets : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_text_widgets() : base(21)
			{
				Add(/*name = */ new TI.StringId());
				Add(/* = */ new TI.Struct<hud_widget_inputs_struct>(this));
				Add(/* = */ new TI.Struct<hud_widget_state_definition_struct>(this));
				Add(/*anchor = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*string = */ new TI.StringId());
				Add(/*justification = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(12));
				Add(/*fullscreen font index = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*halfscreen font index = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*quarterscreen font index = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(1));
				Add(/*fullscreen scale = */ new TI.Real());
				Add(/*halfscreen scale = */ new TI.Real());
				Add(/*quarterscreen scale = */ new TI.Real());
				Add(/*fullscreen offset = */ new TI.Point2D());
				Add(/*halfscreen offset = */ new TI.Point2D());
				Add(/*quarterscreen offset = */ new TI.Point2D());
				Add(/*effect = */ new TI.Block<hud_widget_effect_block>(this, 1));
			}
			#endregion
		}
		#endregion

		#region hud_screen_effect_widgets
		[TI.Definition(1, 112)]
		public class hud_screen_effect_widgets : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_screen_effect_widgets() : base(15)
			{
				Add(/*name = */ new TI.StringId());
				Add(/* = */ new TI.Struct<hud_widget_inputs_struct>(this));
				Add(/* = */ new TI.Struct<hud_widget_state_definition_struct>(this));
				Add(/*anchor = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*fullscreen screen effect = */ new TI.TagReference(this, TagGroups.egor));
				Add(/*waa = */ new TI.Struct<screen_effect_bonus_struct>(this));
				Add(/*fullscreen sequence index = */ new TI.ByteInteger());
				Add(/*halfscreen sequence index = */ new TI.ByteInteger());
				Add(/*quarterscreen sequence index = */ new TI.ByteInteger());
				Add(new TI.Pad(1));
				Add(/*fullscreen offset = */ new TI.Point2D());
				Add(/*halfscreen offset = */ new TI.Point2D());
				Add(/*quarterscreen offset = */ new TI.Point2D());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public new_hud_definition_group() : base(5)
		{
			Add(/*DO NOT USE = */ new TI.TagReference(this, TagGroups.nhdt));
			Add(/*bitmap widgets = */ new TI.Block<hud_bitmap_widgets>(this, 256));
			Add(/*text widgets = */ new TI.Block<hud_text_widgets>(this, 256));
			Add(/*dashlight data = */ new TI.Struct<new_hud_dashlight_data_struct>(this));
			Add(/*screen effect widgets = */ new TI.Block<hud_screen_effect_widgets>(this, 4));
		}
		#endregion
	};
	#endregion

	#region text_value_pair_definition_group
	[TI.TagGroup((int)TagGroups.Enumerated.sily, 1, 48)]
	public class text_value_pair_definition_group : TI.Definition
	{
		#region text_value_pair_reference_block
		[TI.Definition(1, 12)]
		public class text_value_pair_reference_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.ShortInteger Value;
			public TI.StringId Name;
			#endregion

			public text_value_pair_reference_block() : base(4)
			{
				Add(Flags = new TI.Flags());
				Add(Value = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(Name = new TI.StringId());
			}
		};
		#endregion

		#region Fields
		public TI.LongInteger Index;
		public TI.TagReference Strings;
		public TI.StringId Title, Header, Description;
		public TI.Block<text_value_pair_reference_block> Values;
		#endregion

		public text_value_pair_definition_group() : base(7)
		{
			Add(Index = new TI.LongInteger());
			Add(new TI.Pad(4));
			Add(Strings = new TI.TagReference(this, TagGroups.unic));
			Add(Title = new TI.StringId());
			Add(Header = new TI.StringId());
			Add(Description = new TI.StringId());
			Add(Values = new TI.Block<text_value_pair_reference_block>(this, 32));
		}
	};
	#endregion


	#region unit_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.unhi, 1, 1404)]
	public class unit_hud_interface_group : TI.Definition
	{
		#region unit_hud_auxilary_overlay_block
		[TI.Definition(1, 132)]
		public class unit_hud_auxilary_overlay_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_hud_auxilary_overlay_block() : base(22)
			{
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*default color = */ new TI.Color());
				Add(/*flashing color = */ new TI.Color());
				Add(/*flash period = */ new TI.Real());
				Add(/*flash delay = */ new TI.Real());
				Add(/*number of flashes = */ new TI.ShortInteger());
				Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flash length = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(new TI.Pad(4));
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4));
				Add(/*type = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(24));
			}
			#endregion
		}
		#endregion

		#region unit_hud_sound_block
		[TI.Definition(1, 56)]
		public class unit_hud_sound_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_hud_sound_block() : base(4)
			{
				Add(/*sound = */ new TI.TagReference(this)); // snd!,lsnd,
				Add(/*latched to = */ new TI.Flags());
				Add(/*scale = */ new TI.Real());
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region unit_hud_auxilary_panel_block
		[TI.Definition(1, 324)]
		public class unit_hud_auxilary_panel_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_hud_auxilary_panel_block() : base(45)
			{
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2 + 16));
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*default color = */ new TI.Color());
				Add(/*flashing color = */ new TI.Color());
				Add(/*flash period = */ new TI.Real());
				Add(/*flash delay = */ new TI.Real());
				Add(/*number of flashes = */ new TI.ShortInteger());
				Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flash length = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(new TI.Pad(4));
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4));
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*meter bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*color at meter minimum = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*color at meter maximum = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*flash color = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*empty color = */ new TI.Color());
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*minumum meter value = */ new TI.ByteInteger());
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(/*alpha multiplier = */ new TI.ByteInteger());
				Add(/*alpha bias = */ new TI.ByteInteger());
				Add(/*value scale = */ new TI.ShortInteger());
				Add(/*opacity = */ new TI.Real());
				Add(/*translucency = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(new TI.Pad(4));
				Add(/*minimum fraction cutoff = */ new TI.Real());
				Add(/*flags = */ new TI.Flags());
				Add(new TI.Pad(24 + 64));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public unit_hud_interface_group() : base(161)
		{
			Add(/*anchor = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*meter bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*color at meter minimum = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*color at meter maximum = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*flash color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*empty color = */ new TI.Color());
			Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*minumum meter value = */ new TI.ByteInteger());
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(/*alpha multiplier = */ new TI.ByteInteger());
			Add(/*alpha bias = */ new TI.ByteInteger());
			Add(/*value scale = */ new TI.ShortInteger());
			Add(/*opacity = */ new TI.Real());
			Add(/*translucency = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(new TI.Pad(4));
			Add(/*overcharge minimum color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*overcharge maximum color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*overcharge flash color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*overcharge empty color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(new TI.Pad(16));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*meter bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*color at meter minimum = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*color at meter maximum = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*flash color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*empty color = */ new TI.Color());
			Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*minumum meter value = */ new TI.ByteInteger());
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(/*alpha multiplier = */ new TI.ByteInteger());
			Add(/*alpha bias = */ new TI.ByteInteger());
			Add(/*value scale = */ new TI.ShortInteger());
			Add(/*opacity = */ new TI.Real());
			Add(/*translucency = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(new TI.Pad(4));
			Add(/*medium health left color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*max color health fraction cutoff = */ new TI.Real());
			Add(/*min color health fraction cutoff = */ new TI.Real());
			Add(new TI.Pad(20));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*default color = */ new TI.Color());
			Add(/*flashing color = */ new TI.Color());
			Add(/*flash period = */ new TI.Real());
			Add(/*flash delay = */ new TI.Real());
			Add(/*number of flashes = */ new TI.ShortInteger());
			Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*flash length = */ new TI.Real());
			Add(/*disabled color = */ new TI.Color());
			Add(new TI.Pad(4));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
			Add(new TI.Pad(4 + 32));
			Add(/*anchor offset = */ new TI.Point2D());
			Add(/*width scale = */ new TI.Real());
			Add(/*height scale = */ new TI.Real());
			Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 20));
			Add(/*anchor = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*overlays = */ new TI.Block<unit_hud_auxilary_overlay_block>(this, 16));
			Add(new TI.Pad(16));
			Add(/*sounds = */ new TI.Block<unit_hud_sound_block>(this, 12));
			Add(/*meters = */ new TI.Block<unit_hud_auxilary_panel_block>(this, 16));
			Add(/*new hud = */ new TI.TagReference(this, TagGroups.nhdt));
			Add(new TI.Pad(356 + 48));
		}
		#endregion
	};
	#endregion


	#region user_interface_globals_definition
	[TI.TagGroup((int)TagGroups.Enumerated.wgtz, 3, 60)]
	public class user_interface_globals_definition_group : TI.Definition
	{
		#region user_interface_widget_reference_block
		[TI.Definition(1, 16)]
		public class user_interface_widget_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public user_interface_widget_reference_block() : base(1)
			{
				Add(/*widget tag = */ new TI.TagReference(this, TagGroups.wgit));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public user_interface_globals_definition_group() : base(4)
		{
			Add(/*shared globals = */ new TI.TagReference(this, TagGroups.wigl));
			Add(/*screen widgets = */ new TI.Block<user_interface_widget_reference_block>(this, 256));
			Add(/*mp variant settings ui = */ new TI.TagReference(this, TagGroups.goof));
			Add(/*game hopper descriptions = */ new TI.TagReference(this, TagGroups.unic));
		}
		#endregion
	};
	#endregion

	#region user_interface_list_skin_definition
	[TI.TagGroup((int)TagGroups.Enumerated.skin, 1, 88)]
	public class user_interface_list_skin_definition_group : TI.Definition
	{
		#region single_animation_reference_block
		[TI.Definition(1, 20)]
		public class single_animation_reference_block : TI.Definition
		{
			#region screen_animation_keyframe_reference_block
			[TI.Definition(1, 20)]
			public class screen_animation_keyframe_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public screen_animation_keyframe_reference_block() : base(3)
				{
					Add(new TI.Pad(4));
					Add(/*alpha = */ new TI.Real());
					Add(/*position = */ new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public single_animation_reference_block() : base(3)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation period = */ new TI.LongInteger());
				Add(/*keyframes = */ new TI.Block<screen_animation_keyframe_reference_block>(this, 64));
			}
			#endregion
		}
		#endregion

		#region text_block_reference_block
		[TI.Definition(1, 44)]
		public class text_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public text_block_reference_block() : base(10)
			{
				Add(/*text flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*custom font = */ new TI.Enum());
				Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*text bounds = */ new TI.Rectangle2D());
				Add(/*string id = */ new TI.StringId());
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region bitmap_block_reference_block
		[TI.Definition(1, 64)]
		public class bitmap_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public bitmap_block_reference_block() : base(17)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(/*bitmap blend method = */ new TI.Enum());
				Add(/*initial sprite frame = */ new TI.ShortInteger());
				Add(/*top-left = */ new TI.Point2D());
				Add(/*horiz texture wraps/second = */ new TI.Real());
				Add(/*vert texture wraps/second = */ new TI.Real());
				Add(/*bitmap tag = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*sprite animation speed fps = */ new TI.Real());
				Add(/*progress bottom-left = */ new TI.Point2D());
				Add(/*string identifier = */ new TI.StringId());
				Add(new TI.UselessPad(12 + 12));
				Add(/*progress scale = */ new TI.RealVector2D());
				Add(new TI.UselessPad(4 + 4));
			}
			#endregion
		}
		#endregion

		#region hud_block_reference_block
		[TI.Definition(1, 52)]
		public class hud_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_block_reference_block() : base(8)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(/*starting bitmap sequence index = */ new TI.ShortInteger());
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*bounds = */ new TI.Rectangle2D());
			}
			#endregion
		}
		#endregion

		#region player_block_reference_block
		[TI.Definition(1, 32)]
		public class player_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_block_reference_block() : base(9)
			{
				Add(new TI.Pad(4));
				Add(/*skin = */ new TI.TagReference(this, TagGroups.skin));
				Add(/*bottom-left = */ new TI.Point2D());
				Add(/*table order = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*maximum player count = */ new TI.ByteInteger());
				Add(/*row count = */ new TI.ByteInteger());
				Add(/*column count = */ new TI.ByteInteger());
				Add(/*row height = */ new TI.ShortInteger());
				Add(/*column width = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public user_interface_list_skin_definition_group() : base(11)
		{
			Add(/*list flags = */ new TI.Flags());
			Add(new TI.UselessPad(8));
			Add(/*arrows bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*up-arrows offset = */ new TI.Point2D());
			Add(/*down-arrows offset = */ new TI.Point2D());
			Add(new TI.UselessPad(32));
			Add(/*item animations = */ new TI.Block<single_animation_reference_block>(this, 7));
			Add(/*text blocks = */ new TI.Block<text_block_reference_block>(this, 64));
			Add(/*bitmap blocks = */ new TI.Block<bitmap_block_reference_block>(this, 64));
			Add(/*hud blocks = */ new TI.Block<hud_block_reference_block>(this, 64));
			Add(/*player blocks = */ new TI.Block<player_block_reference_block>(this, 64));
		}
		#endregion
	};
	#endregion

	#region bitmap_block_reference_block
	[TI.Definition(1, 64)]
	public class bitmap_block_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public bitmap_block_reference_block() : base(17)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*animation index = */ new TI.Enum());
			Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
			Add(/*bitmap blend method = */ new TI.Enum());
			Add(/*initial sprite frame = */ new TI.ShortInteger());
			Add(/*top-left = */ new TI.Point2D());
			Add(/*horiz texture wraps/second = */ new TI.Real());
			Add(/*vert texture wraps/second = */ new TI.Real());
			Add(/*bitmap tag = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*render depth bias = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*sprite animation speed fps = */ new TI.Real());
			Add(/*progress bottom-left = */ new TI.Point2D());
			Add(/*string identifier = */ new TI.StringId());
			Add(new TI.UselessPad(12 + 12));
			Add(/*progress scale = */ new TI.RealVector2D());
			Add(new TI.UselessPad(4 + 4));
		}
		#endregion
	}
	#endregion

	#region ui_model_scene_reference_block
	[TI.Definition(1, 84)]
	public class ui_model_scene_reference_block : TI.Definition
	{
		#region ui_object_reference_block
		[TI.Definition(1, 32)]
		public class ui_object_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ui_object_reference_block() : base(1)
			{
				Add(/*name = */ new TI.String());
			}
			#endregion
		}
		#endregion

		#region ui_light_reference_block
		[TI.Definition(1, 32)]
		public class ui_light_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ui_light_reference_block() : base(1)
			{
				Add(/*name = */ new TI.String());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public ui_model_scene_reference_block() : base(15)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*animation index = */ new TI.Enum());
			Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
			Add(/*render depth bias = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(/*objects = */ new TI.Block<ui_object_reference_block>(this, 32));
			Add(/*lights = */ new TI.Block<ui_light_reference_block>(this, 8));
			Add(/*animation scale factor = */ new TI.RealVector3D());
			Add(/*camera position = */ new TI.RealPoint3D());
			Add(new TI.UselessPad(24));
			Add(/*fov degress = */ new TI.Real());
			Add(/*ui viewport = */ new TI.Rectangle2D());
			Add(/*UNUSED intro anim = */ new TI.StringId());
			Add(/*UNUSED outro anim = */ new TI.StringId());
			Add(/*UNUSED ambient anim = */ new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region window_pane_reference_block
	[TI.Definition(1, 112)]
	public class window_pane_reference_block : TI.Definition
	{
		#region button_widget_reference_block
		[TI.Definition(1, 68)]
		public class button_widget_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public button_widget_reference_block() : base(13)
			{
				Add(/*text flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*custom font = */ new TI.Enum());
				Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*bounds = */ new TI.Rectangle2D());
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*bitmap offset = */ new TI.Point2D());
				Add(/*string id = */ new TI.StringId());
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(/*mouse region top offset = */ new TI.ShortInteger());
				Add(/*button flags = */ new TI.Flags());
			}
			#endregion
		}
		#endregion

		#region s_text_value_pair_reference_block_UNUSED
		[TI.Definition(1, 20)]
		public class s_text_value_pair_reference_block_UNUSED : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public s_text_value_pair_reference_block_UNUSED() : base(6)
			{
				Add(/*value type = */ new TI.Enum());
				Add(/*boolean value = */ new TI.Enum());
				Add(/*integer value = */ new TI.LongInteger());
				Add(/*fp value = */ new TI.Real());
				Add(/*text value string_id = */ new TI.StringId());
				Add(/*text label string_id = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region list_reference_block
		[TI.Definition(1, 28)]
		public class list_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public list_reference_block() : base(7)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*skin index = */ new TI.Enum());
				Add(/*num. visible items = */ new TI.ShortInteger());
				Add(/*bottom left = */ new TI.Point2D());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(/*UNUSED = */ new TI.Block<s_text_value_pair_reference_block_UNUSED>(this, 100));
			}
			#endregion
		}
		#endregion

		#region table_view_list_reference_block
		[TI.Definition(1, 44)]
		public class table_view_list_reference_block : TI.Definition
		{
			#region table_view_list_row_reference_block
			[TI.Definition(1, 20)]
			public class table_view_list_row_reference_block : TI.Definition
			{
				#region table_view_list_item_reference_block
				[TI.Definition(1, 36)]
				public class table_view_list_item_reference_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public table_view_list_item_reference_block() : base(8)
					{
						Add(/*text flags = */ new TI.Flags());
						Add(/*cell width = */ new TI.ShortInteger());
						Add(new TI.Pad(2));
						Add(/*bitmap top-left = */ new TI.Point2D());
						Add(/*bitmap tag = */ new TI.TagReference(this, TagGroups.bitm));
						Add(/*string id = */ new TI.StringId());
						Add(/*render depth bias = */ new TI.ShortInteger());
						Add(new TI.Pad(2));
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public table_view_list_row_reference_block() : base(4)
				{
					Add(/*flags = */ new TI.Flags());
					Add(/*row height = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*row cells = */ new TI.Block<table_view_list_item_reference_block>(this, 8));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public table_view_list_reference_block() : base(8)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(/*custom font = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*top-left = */ new TI.Point2D());
				Add(/*table rows = */ new TI.Block<table_view_list_row_reference_block>(this, 16));
			}
			#endregion
		}
		#endregion

		#region text_block_reference_block
		[TI.Definition(1, 44)]
		public class text_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public text_block_reference_block() : base(10)
			{
				Add(/*text flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*custom font = */ new TI.Enum());
				Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*text bounds = */ new TI.Rectangle2D());
				Add(/*string id = */ new TI.StringId());
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region s_text_value_pair_blocks_block_UNUSED
		[TI.Definition(1, 44)]
		public class s_text_value_pair_blocks_block_UNUSED : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public s_text_value_pair_blocks_block_UNUSED() : base(2)
			{
				Add(/*name = */ new TI.String());
				Add(/*text value pairs = */ new TI.Block<s_text_value_pair_reference_block_UNUSED>(this, 100));
			}
			#endregion
		}
		#endregion

		#region hud_block_reference_block
		[TI.Definition(1, 52)]
		public class hud_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public hud_block_reference_block() : base(8)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation index = */ new TI.Enum());
				Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
				Add(/*render depth bias = */ new TI.ShortInteger());
				Add(/*starting bitmap sequence index = */ new TI.ShortInteger());
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*bounds = */ new TI.Rectangle2D());
			}
			#endregion
		}
		#endregion

		#region player_block_reference_block
		[TI.Definition(1, 32)]
		public class player_block_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_block_reference_block() : base(9)
			{
				Add(new TI.Pad(4));
				Add(/*skin = */ new TI.TagReference(this, TagGroups.skin));
				Add(/*bottom-left = */ new TI.Point2D());
				Add(/*table order = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*maximum player count = */ new TI.ByteInteger());
				Add(/*row count = */ new TI.ByteInteger());
				Add(/*column count = */ new TI.ByteInteger());
				Add(/*row height = */ new TI.ShortInteger());
				Add(/*column width = */ new TI.ShortInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public window_pane_reference_block() : base(11)
		{
			Add(new TI.Pad(2));
			Add(/*animation index = */ new TI.Enum());
			Add(/*buttons = */ new TI.Block<button_widget_reference_block>(this, 64));
			Add(/*list block = */ new TI.Block<list_reference_block>(this, 1));
			Add(/*table view = */ new TI.Block<table_view_list_reference_block>(this, 1));
			Add(/*text blocks = */ new TI.Block<text_block_reference_block>(this, 64));
			Add(/*bitmap blocks = */ new TI.Block<bitmap_block_reference_block>(this, 64));
			Add(/*model scene blocks = */ new TI.Block<ui_model_scene_reference_block>(this, 32));
			Add(/*text-value blocks = */ new TI.Block<s_text_value_pair_blocks_block_UNUSED>(this, 100));
			Add(/*hud blocks = */ new TI.Block<hud_block_reference_block>(this, 64));
			Add(/*player blocks = */ new TI.Block<player_block_reference_block>(this, 64));
		}
		#endregion
	}
	#endregion

	#region user_interface_screen_widget_definition
	[TI.TagGroup((int)TagGroups.Enumerated.wgit, 898, 140)]
	public class user_interface_screen_widget_definition_group : TI.Definition
	{
		#region local_string_id_list_section_reference_block
		[TI.Definition(1, 16)]
		public class local_string_id_list_section_reference_block : TI.Definition
		{
			#region local_string_id_list_string_reference_block
			[TI.Definition(1, 4)]
			public class local_string_id_list_string_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public local_string_id_list_string_reference_block() : base(1)
				{
					Add(/*string id = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public local_string_id_list_section_reference_block() : base(2)
			{
				Add(/*section name = */ new TI.StringId());
				Add(/*local string section references = */ new TI.Block<local_string_id_list_string_reference_block>(this, 64));
			}
			#endregion
		}
		#endregion

		#region local_bitmap_reference_block
		[TI.Definition(1, 16)]
		public class local_bitmap_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public local_bitmap_reference_block() : base(1)
			{
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public user_interface_screen_widget_definition_group() : base(20)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*screen ID = */ new TI.Enum());
			Add(new TI.UselessPad(2 + 2));
			Add(/*button key type = */ new TI.Enum());
			Add(/*text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*string list tag = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*panes = */ new TI.Block<window_pane_reference_block>(this, 16));
			Add(/*shape group = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*header string id = */ new TI.StringId());
			Add(/*local strings = */ new TI.Block<local_string_id_list_section_reference_block>(this, 16));
			Add(/*local bitmaps = */ new TI.Block<local_bitmap_reference_block>(this, 16));
			Add(/*source color = */ new TI.RealColor());
			Add(/*destination color = */ new TI.RealColor());
			Add(/*accumulate zoom scale x = */ new TI.Real());
			Add(/*accumulate zoom scale y = */ new TI.Real());
			Add(/*refraction scale x = */ new TI.Real());
			Add(/*refraction scale y = */ new TI.Real());
			Add(/*mouse cursor definition = */ new TI.TagReference(this, TagGroups.mcsr));
			Add(new TI.UselessPad(40));
		}
		#endregion
	};
	#endregion

	#region user_interface_shared_globals_definition
	[TI.TagGroup((int)TagGroups.Enumerated.wigl, 1, 652)]
	public class user_interface_shared_globals_definition_group : TI.Definition
	{
		#region ui_error_category_block
		[TI.Definition(1, 52)]
		public class ui_error_category_block : TI.Definition
		{
			#region ui_error_block
			[TI.Definition(1, 24)]
			public class ui_error_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ui_error_block() : base(8)
				{
					Add(/*error = */ new TI.Enum(TI.FieldType.LongEnum));
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*default button = */ new TI.Enum(TI.FieldType.ByteEnum));
					Add(new TI.Pad(1));
					Add(/*title = */ new TI.StringId());
					Add(/*message = */ new TI.StringId());
					Add(/*ok = */ new TI.StringId());
					Add(/*cancel = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public ui_error_category_block() : base(10)
			{
				Add(/*category name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*default button = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(1));
				Add(/*string tag = */ new TI.TagReference(this, TagGroups.unic));
				Add(/*default title = */ new TI.StringId());
				Add(/*default message = */ new TI.StringId());
				Add(/*default ok = */ new TI.StringId());
				Add(/*default cancel = */ new TI.StringId());
				Add(/*error block = */ new TI.Block<ui_error_block>(this, 100));
			}
			#endregion
		}
		#endregion

		#region animation_reference_block
		[TI.Definition(1, 56)]
		public class animation_reference_block : TI.Definition
		{
			#region screen_animation_keyframe_reference_block
			[TI.Definition(1, 20)]
			public class screen_animation_keyframe_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public screen_animation_keyframe_reference_block() : base(3)
				{
					Add(new TI.Pad(4));
					Add(/*alpha = */ new TI.Real());
					Add(/*position = */ new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public animation_reference_block() : base(12)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*animation period = */ new TI.LongInteger());
				Add(/*keyframes = */ new TI.Block<screen_animation_keyframe_reference_block>(this, 64));
				Add(new TI.UselessPad(4 + 12));
				Add(/*animation period = */ new TI.LongInteger());
				Add(/*keyframes = */ new TI.Block<screen_animation_keyframe_reference_block>(this, 64));
				Add(new TI.UselessPad(4 + 12));
				Add(/*animation period = */ new TI.LongInteger());
				Add(/*ambient animation looping style = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*keyframes = */ new TI.Block<screen_animation_keyframe_reference_block>(this, 64));
				Add(new TI.UselessPad(16));
			}
			#endregion
		}
		#endregion

		#region shape_group_reference_block
		[TI.Definition(1, 36)]
		public class shape_group_reference_block : TI.Definition
		{
			#region shape_block_reference_block
			[TI.Definition(1, 52)]
			public class shape_block_reference_block : TI.Definition
			{
				#region point_block_reference_block
				[TI.Definition(1, 4)]
				public class point_block_reference_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public point_block_reference_block() : base(1)
					{
						Add(/*coordinates = */ new TI.Point2D());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public shape_block_reference_block() : base(7)
				{
					Add(/*flags = */ new TI.Flags());
					Add(/*animation index = */ new TI.Enum());
					Add(/*intro animation delay milliseconds = */ new TI.ShortInteger());
					Add(/*color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(/*points = */ new TI.Block<point_block_reference_block>(this, 16));
					Add(/*render depth bias = */ new TI.ShortInteger());
					Add(new TI.Pad(14));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public shape_group_reference_block() : base(3)
			{
				Add(/*shapes = */ new TI.Block<shape_block_reference_block>(this, 32));
				Add(/*model scene blocks = */ new TI.Block<ui_model_scene_reference_block>(this, 32));
				Add(/*bitmap blocks = */ new TI.Block<bitmap_block_reference_block>(this, 64));
			}
			#endregion
		}
		#endregion

		#region persistent_background_animation_block
		[TI.Definition(1, 20)]
		public class persistent_background_animation_block : TI.Definition
		{
			#region background_animation_keyframe_reference_block
			[TI.Definition(1, 20)]
			public class background_animation_keyframe_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public background_animation_keyframe_reference_block() : base(3)
				{
					Add(/*start transition index = */ new TI.LongInteger());
					Add(/*alpha = */ new TI.Real());
					Add(/*position = */ new TI.RealPoint3D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public persistent_background_animation_block() : base(3)
			{
				Add(new TI.Pad(4));
				Add(/*animation period = */ new TI.LongInteger());
				Add(/*interpolated keyframes = */ new TI.Block<background_animation_keyframe_reference_block>(this, 64));
			}
			#endregion
		}
		#endregion

		#region list_skin_reference_block
		[TI.Definition(1, 16)]
		public class list_skin_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public list_skin_reference_block() : base(1)
			{
				Add(/*list item skins = */ new TI.TagReference(this, TagGroups.skin));
			}
			#endregion
		}
		#endregion

		#region skill_to_rank_mapping_block
		[TI.Definition(1, 4)]
		public class skill_to_rank_mapping_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public skill_to_rank_mapping_block() : base(1)
			{
				Add(/*skill bounds = */ new TI.ShortIntegerBounds());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public user_interface_shared_globals_definition_group() : base(53)
		{
			Add(new TI.Pad(2 + 2 + 16 + 8 + 8 + 16 + 8 + 8));
			Add(/*overlayed screen alpha mod = */ new TI.Real());
			Add(/*inc. text update period = */ new TI.ShortInteger());
			Add(/*inc. text block character = */ new TI.ShortInteger());
			Add(/*callout text scale = */ new TI.Real());
			Add(/*progress bar color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*near clip plane distance = */ new TI.Real());
			Add(/*projection plane distance = */ new TI.Real());
			Add(/*far clip plane distance = */ new TI.Real());
			Add(/*overlayed interface color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(new TI.Pad(12));
			Add(/*errors = */ new TI.Block<ui_error_category_block>(this, 100));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound tag = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*global bitmaps tag = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*unicode string list tag = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*screen animations = */ new TI.Block<animation_reference_block>(this, 64));
			Add(/*shape groups = */ new TI.Block<shape_group_reference_block>(this, 64));
			Add(/*animations = */ new TI.Block<persistent_background_animation_block>(this, 100));
			Add(/*list item skins = */ new TI.Block<list_skin_reference_block>(this, 32));
			Add(/*button key type strings = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*game type strings = */ new TI.TagReference(this, TagGroups.unic));
			Add(/* = */ new TI.TagReference(this));
			Add(/*skill mappings = */ new TI.Block<skill_to_rank_mapping_block>(this, 65535));
			Add(/*full screen header text font = */ new TI.Enum());
			Add(/*large dialog header text font = */ new TI.Enum());
			Add(/*half dialog header text font = */ new TI.Enum());
			Add(/*qtr dialog header text font = */ new TI.Enum());
			Add(/*default text color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*full screen header text bounds = */ new TI.Rectangle2D());
			Add(/*full screen button key text bounds = */ new TI.Rectangle2D());
			Add(/*large dialog header text bounds = */ new TI.Rectangle2D());
			Add(/*large dialog button key text bounds = */ new TI.Rectangle2D());
			Add(/*half dialog header text bounds = */ new TI.Rectangle2D());
			Add(/*half dialog button key text bounds = */ new TI.Rectangle2D());
			Add(/*qtr dialog header text bounds = */ new TI.Rectangle2D());
			Add(/*qtr dialog button key text bounds = */ new TI.Rectangle2D());
			Add(/*main menu music = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*music fade time = */ new TI.LongInteger());
		}
		#endregion
	};
	#endregion

	#region weapon_hud_interface
	[TI.TagGroup((int)TagGroups.Enumerated.wphi, 2, 380)]
	public class weapon_hud_interface_group : TI.Definition
	{
		#region weapon_hud_static_block
		[TI.Definition(1, 180)]
		public class weapon_hud_static_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_hud_static_block() : base(23)
			{
				Add(/*state attached to = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*can use on map type = */ new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*interface bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*default color = */ new TI.Color());
				Add(/*flashing color = */ new TI.Color());
				Add(/*flash period = */ new TI.Real());
				Add(/*flash delay = */ new TI.Real());
				Add(/*number of flashes = */ new TI.ShortInteger());
				Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flash length = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(new TI.Pad(4));
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*multitex overlay = */ new TI.Block<global_hud_multitexture_overlay_definition>(this, 30));
				Add(new TI.Pad(4 + 40));
			}
			#endregion
		}
		#endregion

		#region weapon_hud_meter_block
		[TI.Definition(1, 180)]
		public class weapon_hud_meter_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_hud_meter_block() : base(25)
			{
				Add(/*state attached to = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*can use on map type = */ new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*meter bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*color at meter minimum = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*color at meter maximum = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*flash color = */ new TI.Color(TI.FieldType.RgbColor));
				Add(/*empty color = */ new TI.Color());
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*minumum meter value = */ new TI.ByteInteger());
				Add(/*sequence index = */ new TI.ShortInteger());
				Add(/*alpha multiplier = */ new TI.ByteInteger());
				Add(/*alpha bias = */ new TI.ByteInteger());
				Add(/*value scale = */ new TI.ShortInteger());
				Add(/*opacity = */ new TI.Real());
				Add(/*translucency = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
				Add(new TI.Pad(4 + 40));
			}
			#endregion
		}
		#endregion

		#region weapon_hud_number_block
		[TI.Definition(1, 160)]
		public class weapon_hud_number_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_hud_number_block() : base(24)
			{
				Add(/*state attached to = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*can use on map type = */ new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(/*anchor offset = */ new TI.Point2D());
				Add(/*width scale = */ new TI.Real());
				Add(/*height scale = */ new TI.Real());
				Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 20));
				Add(/*default color = */ new TI.Color());
				Add(/*flashing color = */ new TI.Color());
				Add(/*flash period = */ new TI.Real());
				Add(/*flash delay = */ new TI.Real());
				Add(/*number of flashes = */ new TI.ShortInteger());
				Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*flash length = */ new TI.Real());
				Add(/*disabled color = */ new TI.Color());
				Add(new TI.Pad(4));
				Add(/*maximum number of digits = */ new TI.ByteInteger());
				Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
				Add(/*number of fractional digits = */ new TI.ByteInteger());
				Add(new TI.Pad(1 + 12));
				Add(/*weapon specific flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 36));
			}
			#endregion
		}
		#endregion

		#region weapon_hud_crosshair_block
		[TI.Definition(1, 104)]
		public class weapon_hud_crosshair_block : TI.Definition
		{
			#region weapon_hud_crosshair_item_block
			[TI.Definition(1, 108)]
			public class weapon_hud_crosshair_item_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public weapon_hud_crosshair_item_block() : base(18)
				{
					Add(/*anchor offset = */ new TI.Point2D());
					Add(/*width scale = */ new TI.Real());
					Add(/*height scale = */ new TI.Real());
					Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2 + 20));
					Add(/*default color = */ new TI.Color());
					Add(/*flashing color = */ new TI.Color());
					Add(/*flash period = */ new TI.Real());
					Add(/*flash delay = */ new TI.Real());
					Add(/*number of flashes = */ new TI.ShortInteger());
					Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*flash length = */ new TI.Real());
					Add(/*disabled color = */ new TI.Color());
					Add(new TI.Pad(4));
					Add(/*frame rate = */ new TI.ShortInteger());
					Add(/*sequence index = */ new TI.ShortInteger());
					Add(/*flags = */ new TI.Flags());
					Add(new TI.Pad(32));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public weapon_hud_crosshair_block() : base(7)
			{
				Add(/*crosshair type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*can use on map type = */ new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(/*Crosshair bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*Crosshair overlays = */ new TI.Block<weapon_hud_crosshair_item_block>(this, 16));
				Add(new TI.Pad(40));
			}
			#endregion
		}
		#endregion

		#region weapon_hud_overlays_block
		[TI.Definition(1, 104)]
		public class weapon_hud_overlays_block : TI.Definition
		{
			#region weapon_hud_overlay_block
			[TI.Definition(1, 136)]
			public class weapon_hud_overlay_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public weapon_hud_overlay_block() : base(20)
				{
					Add(/*anchor offset = */ new TI.Point2D());
					Add(/*width scale = */ new TI.Real());
					Add(/*height scale = */ new TI.Real());
					Add(/*scaling flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2 + 20));
					Add(/*default color = */ new TI.Color());
					Add(/*flashing color = */ new TI.Color());
					Add(/*flash period = */ new TI.Real());
					Add(/*flash delay = */ new TI.Real());
					Add(/*number of flashes = */ new TI.ShortInteger());
					Add(/*flash flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*flash length = */ new TI.Real());
					Add(/*disabled color = */ new TI.Color());
					Add(new TI.Pad(4));
					Add(/*frame rate = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*sequence index = */ new TI.ShortInteger());
					Add(/*type = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*flags = */ new TI.Flags());
					Add(new TI.Pad(16 + 40));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public weapon_hud_overlays_block() : base(7)
			{
				Add(/*state attached to = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*can use on map type = */ new TI.Enum());
				Add(new TI.Pad(2 + 28));
				Add(/*Overlay bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*Overlays = */ new TI.Block<weapon_hud_overlay_block>(this, 16));
				Add(new TI.Pad(40));
			}
			#endregion
		}
		#endregion

		#region global_hud_screen_effect_definition
		[TI.Definition(1, 352)]
		public class global_hud_screen_effect_definition : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_hud_screen_effect_definition() : base(14)
			{
				Add(new TI.Pad(4));
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2 + 16));
				Add(/*mask (fullscreen) = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*mask (splitscreen) = */ new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(8 + 20 + 24 + 8 + 24 + 20 + 24));
				Add(/*screen effect flags = */ new TI.Flags());
				Add(new TI.Pad(32));
				Add(/*screen effect = */ new TI.TagReference(this, TagGroups.egor));
				Add(new TI.Pad(32));
				Add(/*screen effect flags = */ new TI.Flags());
				Add(new TI.Pad(32));
				Add(/*screen effect = */ new TI.TagReference(this, TagGroups.egor));
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public weapon_hud_interface_group() : base(27)
		{
			Add(/*child hud = */ new TI.TagReference(this, TagGroups.wphi));
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*inventory ammo cutoff = */ new TI.ShortInteger());
			Add(/*loaded ammo cutoff = */ new TI.ShortInteger());
			Add(/*heat cutoff = */ new TI.ShortInteger());
			Add(/*age cutoff = */ new TI.ShortInteger());
			Add(new TI.Pad(32));
			Add(/*anchor = */ new TI.Enum());
			Add(new TI.Pad(2 + 32));
			Add(/*static elements = */ new TI.Block<weapon_hud_static_block>(this, 16));
			Add(/*meter elements = */ new TI.Block<weapon_hud_meter_block>(this, 16));
			Add(/*number elements = */ new TI.Block<weapon_hud_number_block>(this, 16));
			Add(/*crosshairs = */ new TI.Block<weapon_hud_crosshair_block>(this, 19));
			Add(/*overlay elements = */ new TI.Block<weapon_hud_overlays_block>(this, 16));
			Add(new TI.Pad(4));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/*screen effect = */ new TI.Block<global_hud_screen_effect_definition>(this, 1));
			Add(new TI.Pad(132));
			Add(/*sequence index = */ new TI.ShortInteger());
			Add(/*width offset = */ new TI.ShortInteger());
			Add(/*offset from reference corner = */ new TI.Point2D());
			Add(/*override icon color = */ new TI.Color());
			Add(/*frame rate [0,30] = */ new TI.ByteInteger());
			Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
			Add(/*text index = */ new TI.ShortInteger());
			Add(new TI.Pad(48));
		}
		#endregion
	};
	#endregion
}