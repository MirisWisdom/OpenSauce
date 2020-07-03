/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region chud_animation_definition
	[TI.TagGroup((int)TagGroups.Enumerated.chad, 1, 92)]
	public class chud_animation_definition_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public chud_animation_definition_group()
		{
			// word flags
			// PAD16?
			// tag block [0x20]
			// unknown [4]
			// tag block [0x20] [rotation]
			// unknown [4]
			// tag block [0x20] [scaling]
			// unknown [0x14]
			// tag block [0x20] [fading]
			// unknown [4]
			// tag block [0x20]
			// unknown [4]
			// tag block [0x20] [movement]
			// unknown [4]
			// long
		}
		#endregion
	};
	#endregion

	#region chud_definition
	[TI.TagGroup((int)TagGroups.Enumerated.chdt, 1, 40)]
	public class chud_definition_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public chud_definition_group()
		{
		}
		#endregion
	};
	#endregion

	#region chud_globals_definition
	[TI.TagGroup((int)TagGroups.Enumerated.chgd, 1, 240)]
	public class chud_globals_definition_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public chud_globals_definition_group()
		{
		}
		#endregion
	};
	#endregion

	#region gui_datasource_definition
	[TI.TagGroup((int)TagGroups.Enumerated.dsrc, 1, 28)]
	public class gui_datasource_definition_group : TI.Definition
	{
		#region Fields
		public TI.StringId Name;
		#endregion

		#region Ctor
		public gui_datasource_definition_group()
		{
			Add(Name = new TI.StringId());
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x28]
				// unknown [0x18]
				// tag block [0x8]
					// string id
					// string id [value]
				// unknown [0x4]
		}
		#endregion
	};
	#endregion
}