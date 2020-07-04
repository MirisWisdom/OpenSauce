/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region structure_bsp_cluster_data_block_new
	partial class structure_bsp_cluster_data_block_new
	{
		public void Render(global_geometry_section_info_struct section_info,
					TI.Block<global_geometry_material_block> materials)
		{
			Section.Value.Render(section_info, materials);
		}
	};
	#endregion

	#region structure_bsp_cluster_block
	partial class structure_bsp_cluster_block
	{
		#region Reconstruct
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			// recreate the section data
			if (ClusterData.Count != 1)
			{
				structure_bsp_cluster_data_block_new cdata;
				ClusterData.Add(out cdata);

				result = cdata.Section.Value.Reconstruct(c, SectionInfo.Value, GeometryBlockInfo.Value);
			}

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
		#endregion

		public void Render(TI.Block<global_geometry_material_block> materials)
		{
			if (ClusterData.Count != 1) return;

			ClusterData[0].Render(SectionInfo.Value, materials);
		}
	};
	#endregion


	#region structure_instanced_geometry_render_info_struct
	partial class structure_instanced_geometry_render_info_struct
	{
		#region Reconstruct
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			// recreate the section data
			if (RenderData.Count != 1)
			{
				structure_bsp_cluster_data_block_new cdata;
				RenderData.Add(out cdata);

				result = cdata.Section.Value.Reconstruct(c, SectionInfo.Value, GeometryBlockInfo.Value);
			}

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
		#endregion

		public void Render(TI.Block<global_geometry_material_block> materials)
		{
			if (RenderData.Count != 1) return;

			RenderData[0].Render(SectionInfo.Value, materials);
		}
	};
	#endregion

	#region scenario_structure_bsp
	partial class scenario_structure_bsp_group
	{
		#region structure_bsp_instanced_geometry_definition_block
		partial class structure_bsp_instanced_geometry_definition_block
		{
			public void Render(TI.Block<global_geometry_material_block> materials)
			{
				RenderInfo.Value.Render(materials);
			}
		};
		#endregion

		#region structure_bsp_instanced_geometry_instances_block
		partial class structure_bsp_instanced_geometry_instances_block
		{
			public void Render(TI.Block<global_geometry_material_block> materials,
				TI.Block<structure_bsp_instanced_geometry_definition_block> instanced_geometries_definitions
				)
			{
				int index = InstanceDefinition.Value;
				Debug.Assert.If(index != -1);
				instanced_geometries_definitions[index].Render(materials);
			}
		};
		#endregion

		#region global_water_definitions_block
		partial class global_water_definitions_block
		{
			#region Reconstruct
			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				bool result = true;

				// recreate the section data
				if (Section.Count != 1)
				{
					water_geometry_section_block section;
					Section.Add(out section);

					result = section.Section.Value.Reconstruct(c, null, GeometryBlockInfo.Value);
				}

				GeometryBlockInfo.Value.ClearPostReconstruction();

				return result;
			}
			#endregion
		};
		#endregion

		public global_tag_import_info_block GetImportInfo()
		{
			if (ImportInfo.Count == 0) return null;

			return ImportInfo[0];
		}

		public global_error_report_categories_block GetErrors()
		{
			if (Errors.Count == 0) return null;

			return Errors[0];
		}

		void RenderClusters()
		{
			foreach (structure_bsp_cluster_block cluster in Clusters)
				cluster.Render(Materials);
		}

		void RenderInstanceGeometry()
		{
			foreach (structure_bsp_instanced_geometry_instances_block geometry in InstancedGeometryInstances)
				geometry.Render(Materials, InstancedGeometriesDefinitions);
		}

		public void Render()
		{
			RenderClusters();
			RenderInstanceGeometry();
		}
	};
	#endregion


	#region structure_lightmap_group_block
	partial class structure_lightmap_group_block
	{
		#region lightmap_geometry_section_block
		partial class lightmap_geometry_section_block
		{
			#region lightmap_geometry_section_cache_data_block
			partial class lightmap_geometry_section_cache_data_block
			{
				internal bool Reconstruct(Blam.CacheFile c,
					global_geometry_section_info_struct section_info,
					geometry_block_info_struct gbi)
				{
					return Geometry.Value.Reconstruct(c, section_info, gbi);
				}
			};
			#endregion

			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				bool result = true;

				if(CacheData.Count != 1)
				{
					lightmap_geometry_section_cache_data_block cache_data;
					CacheData.Add(out cache_data);

					result = cache_data.Reconstruct(c, GeometryInfo.Value, GeometryBlockInfo.Value);
				}

				GeometryBlockInfo.Value.ClearPostReconstruction();

				return result;
			}
		};
		#endregion

		#region lightmap_vertex_buffer_bucket_block
		partial class lightmap_vertex_buffer_bucket_block
		{
			const uint k_bucket_flags_IncDir = (uint)lightmap_vertex_buffer_bucket_flags.IncidentDirection;
			const uint k_bucket_flags_Color = (uint)lightmap_vertex_buffer_bucket_flags.Color;

			#region lightmap_bucket_raw_vertex_block
			partial class lightmap_bucket_raw_vertex_block
			{
				void ReconstructRawVertex(lightmap_vertex_buffer_bucket_block buffer_bucket,
					int stream_source, Render.VertexBufferInterface.StreamReader stream_reader)
				{
					LowLevel.Math.real_quaternion quat = new LowLevel.Math.real_quaternion();

					// The following LM vertex sources only have one element, so we can call this here
					stream_reader.GetStreamedElement(0, ref quat);

					if (buffer_bucket.Flags.Test(k_bucket_flags_IncDir) && 
						stream_source == 0)
					{
						PrimaryLightmapIncidentDirection.I = quat.Vector.I;
						PrimaryLightmapIncidentDirection.J = quat.Vector.J;
						PrimaryLightmapIncidentDirection.K = quat.Vector.K;
					}
					else if (buffer_bucket.Flags.Test(k_bucket_flags_Color) &&
						stream_source == 1)
					{
						// alpha is quad.W, which LM color doesn't use
						PrimaryLightmapColor.R = quat.Vector.I;
						PrimaryLightmapColor.G = quat.Vector.J;
						PrimaryLightmapColor.B = quat.Vector.K;
					}
				}

				internal void Reconstruct(lightmap_vertex_buffer_bucket_block buffer_bucket,
					geometry_block_resource_block gb,
					IO.EndianReader er,
					Render.VertexBufferInterface.StreamReader[] stream_readers)
				{
					short stream_source = gb.SecondaryLocater.Value;
					var stream_r = stream_readers[stream_source];

					if (!stream_r.UsesNullDefinition)
					{
						stream_r.Read(er);
						stream_r.Denormalize();
						ReconstructRawVertex(buffer_bucket, stream_source, stream_r);
					}
				}
			};
			#endregion

			#region lightmap_vertex_buffer_bucket_cache_data_block
			partial class lightmap_vertex_buffer_bucket_cache_data_block
			{
				const int OffsetVertexBuffers = 0;

				internal bool Reconstruct(Blam.CacheFile c,
					lightmap_vertex_buffer_bucket_block buffer_bucket,
					geometry_block_info_struct gbi)
				{
					int index = 0;
					int x;
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
										case OffsetVertexBuffers:
											VertexBuffers.Resize(count);
											VertexBuffers.Read(er);
											break;
									}
									break;
								#endregion
								#region VertexBuffer
								case (int)geometry_block_resource_type.VertexBuffer:
									var vb_defs = (c.TagIndexManager as InternalCacheTagIndex).kVertexBuffers;

									var stream_readers = new Render.VertexBufferInterface.StreamReader[VertexBuffers.Count];
									for (x = 0; x < VertexBuffers.Count; x++)
										VertexBuffers[x].VertexBuffer.InitializeStreamReader(vb_defs, out stream_readers[x]);

									int vertex_count = gb.Size.Value / VertexBuffers[0].VertexBuffer.StrideSize;
									if(buffer_bucket.RawVertices.Count == 0)
										buffer_bucket.RawVertices.Resize(vertex_count);
									for (x = 0; x < vertex_count; x++)
										buffer_bucket.RawVertices[x].Reconstruct(buffer_bucket, gb,
											er, stream_readers);
									break;
								#endregion
							}
						}

						index++;
					}

					VertexBuffers.DeleteAll();

					return true;
				}
			};
			#endregion
		

			internal override bool Reconstruct(Blam.CacheFile c)
			{
				bool result = true;

				if (CacheData.Count != 1)
				{
					lightmap_vertex_buffer_bucket_cache_data_block cache_data;
					CacheData.Add(out cache_data);

					result = cache_data.Reconstruct(c, this, GeometryBlockInfo);
				}

				GeometryBlockInfo.Value.ClearPostReconstruction();

				return result;
			}
		};
		#endregion
	};
	#endregion

	#region scenario_structure_lightmap
	partial class scenario_structure_lightmap_group
	{
		public global_tag_import_info_block GetImportInfo() { return null; }

		public global_error_report_categories_block GetErrors()
		{
			if (Errors.Count == 0) return null;

			return Errors[0];
		}
	};
	#endregion
}