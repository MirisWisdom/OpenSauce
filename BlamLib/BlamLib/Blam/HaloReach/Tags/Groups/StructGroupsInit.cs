/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.HaloReach
{
	partial class StructGroups
	{
		static StructGroups()
		{
			GroupsInitialize();
			play.Definition = new Tags.cache_file_resource_layout_table().State;

			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.HaloReach, x, true);
		}
	};
};