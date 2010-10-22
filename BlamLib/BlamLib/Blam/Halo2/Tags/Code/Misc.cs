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

namespace BlamLib.Blam.Halo2.Tags
{
	#region decorator_cache_block_data_block
	partial class decorator_cache_block_data_block
	{
		#region Reconstruct
		internal bool Reconstruct(geometry_block_info_struct gbi)
		{
			int index = 0;
			byte[][] data = gbi.GeometryBlock;

			if (data == null) return false;

			foreach (geometry_block_resource_block gb in gbi.Resources)
			{
				using (IO.EndianReader er = new BlamLib.IO.EndianReader(data[index]))
				{
					switch (gb.Type.Value)
					{
						#region TagBlock
						case (int)geometry_block_resource_type.TagBlock:
							int count = gb.GetCount();
							switch (gb.PrimaryLocater.Value)
							{
								case OffsetPlacements:
									Placements.Resize(count);
									Placements.Read(er);
									break;

								case OffsetDecalVertices:
									DecalVertices.Resize(count);
									DecalVertices.Read(er);
									break;

								case OffsetDecalIndices:
									DecalIndices.Resize(count);
									DecalIndices.Read(er);
									break;

								case OffsetSpriteVertices:
									SpriteVertices.Resize(count);
									SpriteVertices.Read(er);
									break;

								case OffsetSpriteIndices:
									SpriteIndices.Resize(count);
									SpriteIndices.Read(er);
									break;
							}
							break;
						#endregion
					}
				}

				index++;
			}

			return true;
		}
		#endregion
	};
	#endregion

	#region decorator_cache_block_block
	partial class decorator_cache_block_block
	{
		#region Reconstruct
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			// recreate the section data
			if (CacheBlockData.Count != 1)
			{
				decorator_cache_block_data_block cdata;
				CacheBlockData.Add(out cdata);

				result = cdata.Reconstruct(GeometryBlockInfo.Value);
			}

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
		#endregion
	};
	#endregion
}