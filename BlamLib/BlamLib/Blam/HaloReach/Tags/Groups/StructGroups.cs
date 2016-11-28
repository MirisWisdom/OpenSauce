/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.HaloReach
{
	partial class StructGroups
	{
		public static TagGroupCollection Groups;
		static void GroupsInitialize()
		{
			Groups = new TagGroupCollection(false,
				play
			);
		}

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>cache_file_resource_layout_table_struct</summary>
			play,
		};
	};
};