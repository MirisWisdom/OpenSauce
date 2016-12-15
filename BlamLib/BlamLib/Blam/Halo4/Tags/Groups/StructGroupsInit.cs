/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo4
{
	partial class StructGroups
	{
		static StructGroups()
		{
			GroupsInitialize();
			//play.Definition = new Tags.cache_file_resource_layout_table().State;

			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Halo4, x, true);
		}
	};
};