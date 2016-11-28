/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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