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
	#region weather_system
	partial class weather_system_group
	{
		#region global_particle_system_lite_block
		partial class global_particle_system_lite_block
		{
			#region particle_system_lite_data_block
			partial class particle_system_lite_data_block
			{
				const int OffsetParticlesRenderData = 0;
				const int OffsetParticlesOtherData = 8;

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
										case OffsetParticlesRenderData:
											ParticlesRenderData.Resize(count);
											ParticlesRenderData.Read(er);
											break;

										case OffsetParticlesOtherData:
											ParticlesOtherData.Resize(count);
											ParticlesOtherData.Read(er);
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
			};
			#endregion

			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				bool result = true;

				if (ParticleSystemData.Count != 1)
				{
					particle_system_lite_data_block data;
					ParticleSystemData.Add(out data);

					result = data.Reconstruct(GeometryBlockInfo.Value);
				}

				GeometryBlockInfo.Value.ClearPostReconstruction();

				return result;
			}
		};
		#endregion
	};
	#endregion
}