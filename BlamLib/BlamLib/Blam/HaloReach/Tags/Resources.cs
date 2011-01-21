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
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using CT = BlamLib.Blam.Cache.Tags;

namespace BlamLib.Blam.HaloReach.Tags
{
	#region cache_file_resource_layout_table
	[TI.Struct((int)StructGroups.Enumerated.play, 1, 60)]
	public class cache_file_resource_layout_table : Halo3.Tags.cache_file_resource_layout_table
	{
	};

	[TI.TagGroup((int)TagGroups.Enumerated.play, 1, 60)]
	public class cache_file_resource_layout_table_group : CT.cache_file_resource_layout_table_group
	{
		public TI.Struct<cache_file_resource_layout_table> ResourceLayoutTable;

		public cache_file_resource_layout_table_group() : base(1)
		{
			Add(ResourceLayoutTable = new TI.Struct<cache_file_resource_layout_table>(this));
		}

		public override CT.cache_file_resource_layout_table GetResourceLayoutTable() { return ResourceLayoutTable.Value; }
	};
	#endregion

	#region cache_file_resource_gestalt
	[TI.TagGroup((int)TagGroups.Enumerated.zone, 1, 532)]
	public partial class cache_file_resource_gestalt_group : Halo3.Tags.cache_file_resource_gestalt_group
	{
	};
	#endregion
}