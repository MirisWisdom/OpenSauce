/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
//using DX = Microsoft.DirectX;
//using D3D = Microsoft.DirectX.Direct3D;

namespace BlamLib.Blam.Halo2.Tags
{
	#region model collision
	#region collision_model
	partial class collision_model_group
	{
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
	};
	#endregion
	#endregion


	#region model animations
	// animations - http://forums.remnantmods.com/viewtopic.php?f=13&t=1574&st=0&sk=t&sd=a

	enum animation_codec_ship
	{
		None,									// 1 0, 0
		UncompressedStaticData,					// 0 0, 1
		UncompressedAnimatedData,				// 1 0, 2
		_8byteQuantizedRotationOnly,			// 1 0, 3

		ByteKeyframeLightlyQuantized,			// 2 1, 4
		WordKeyframeLightlyQuantized,			// 2 2, 5

		ReverseByteKeyframeLightlyQuantized,	// 2 1, 6
		ReverseWordKeyframeLightlyQuantized,	// 2 2, 7

		BlendScreen,							// 1 0, 8
	};
	enum animation_codec_alpha
	{
		None,
		UncompressedStaticData,
		UncompressedAnimatedData,
		_8byteQuantizedRotationOnly,

		LegacyQuantizedEverything,				// 1 0
		LegacyByteKeyframeRotations,			// 3 1
		LegacyShortKeyframeRotations,			// 3 2
		LegacyByteKeyframeEverything,			// 3 1
		LegacyShortKeyframeEverything,			// 3 2
		_6byteQuantizedRotationOnly,			// 1 0
		_6byteQuantizedEverything,				// 1 0

		ByteKeyframeHeavilyQuantized,			// 3 1
		WordKeyframeHeavilyQuantized,			// 3 2
		ByteKeyframeLightlyQuantized,
		WordKeyframeLightlyQuantized,

		ReverseByteKeyframeHeavilyQuantized,	// 3 1
		ReverseWordKeyframeHeavilyQuantized,	// 3 2
		ReverseByteKeyframeLightlyQuantized,
		ReverseWordKeyframeLightlyQuantized,
	};
	public enum animation_codec
	{
		None,
		UncompressedStaticData,
		UncompressedAnimatedData,
		_8byteQuantizedRotationOnly,

		ByteKeyframeLightlyQuantized,
		WordKeyframeLightlyQuantized,

		ReverseByteKeyframeLightlyQuantized,
		ReverseWordKeyframeLightlyQuantized,

		BlendScreen,

		// Alpha follows
		LegacyQuantizedEverything,
		LegacyByteKeyframeRotations,
		LegacyShortKeyframeRotations,
		LegacyByteKeyframeEverything,
		LegacyShortKeyframeEverything,
		_6byteQuantizedRotationOnly,
		_6byteQuantizedEverything,

		ByteKeyframeHeavilyQuantized,
		WordKeyframeHeavilyQuantized,
		ReverseByteKeyframeHeavilyQuantized,
		ReverseWordKeyframeHeavilyQuantized,
	};

	static class animation_codec_extensions
	{
		public static animation_codec ToCodec(this animation_codec_ship codec)
		{
			return (animation_codec)codec;
		}

		public static animation_codec ToCodec(this animation_codec_alpha codec)
		{
			switch(codec)
			{
				case animation_codec_alpha.None:
				case animation_codec_alpha.UncompressedStaticData:
				case animation_codec_alpha.UncompressedAnimatedData:
				case animation_codec_alpha._8byteQuantizedRotationOnly:
					return (animation_codec)codec;

				case animation_codec_alpha.LegacyQuantizedEverything:
					return animation_codec.LegacyQuantizedEverything;
				case animation_codec_alpha.LegacyByteKeyframeRotations:
					return animation_codec.LegacyByteKeyframeRotations;
				case animation_codec_alpha.LegacyShortKeyframeRotations:
					return animation_codec.LegacyShortKeyframeRotations;
				case animation_codec_alpha.LegacyByteKeyframeEverything:
					return animation_codec.LegacyByteKeyframeEverything;
				case animation_codec_alpha.LegacyShortKeyframeEverything:
					return animation_codec.LegacyShortKeyframeEverything;
				case animation_codec_alpha._6byteQuantizedRotationOnly:
					return animation_codec._6byteQuantizedRotationOnly;
				case animation_codec_alpha._6byteQuantizedEverything:
					return animation_codec._6byteQuantizedEverything;

				case animation_codec_alpha.ByteKeyframeHeavilyQuantized:
					return animation_codec.ByteKeyframeHeavilyQuantized;
				case animation_codec_alpha.WordKeyframeHeavilyQuantized:
					return animation_codec.WordKeyframeHeavilyQuantized;
				case animation_codec_alpha.ByteKeyframeLightlyQuantized:
					return animation_codec.ByteKeyframeLightlyQuantized;
				case animation_codec_alpha.WordKeyframeLightlyQuantized:
					return animation_codec.WordKeyframeLightlyQuantized;

				case animation_codec_alpha.ReverseByteKeyframeHeavilyQuantized:
					return animation_codec.ReverseByteKeyframeHeavilyQuantized;
				case animation_codec_alpha.ReverseWordKeyframeHeavilyQuantized:
					return animation_codec.ReverseWordKeyframeHeavilyQuantized;
				case animation_codec_alpha.ReverseByteKeyframeLightlyQuantized:
					return animation_codec.ReverseByteKeyframeLightlyQuantized;
				case animation_codec_alpha.ReverseWordKeyframeLightlyQuantized:
					return animation_codec.ReverseWordKeyframeLightlyQuantized;

				default: throw new Debug.Exceptions.UnreachableException(codec);
			}
		}
	};

	#region animation_graph_resources_struct
	partial class animation_graph_resources_struct
	{
		[System.Diagnostics.Conditional("DEBUG")]
		public void OutputData(string data)
		{
			using (System.IO.StreamWriter sw = new System.IO.StreamWriter(data + "animations.txt"))
			{
				foreach (animation_pool_block anim in Animations)
				{
					using (System.IO.FileStream fs = new System.IO.FileStream(data + anim.Name.ToString().Replace(':', '_') + ".bin", System.IO.FileMode.OpenOrCreate))
					{
						fs.Write(anim.AnimationData.Value, 0, anim.AnimationData.Value.Length);
						fs.Close();
					}

					sw.WriteLine("Name:				{0}", anim.Name);
					sw.WriteLine("Type:				{0}", anim.Type.Value);
					sw.WriteLine("FrameInfoType:		{0}", anim.FrameInfoType.Value);
					sw.WriteLine("FrameCount:			{0}", anim.FrameCount.Value);
					sw.WriteLine("Weight:				{0}", anim.Weight.Value);
					sw.WriteLine("LoopFrameIndex:		{0}", anim.LoopFrameIndex.Value);
					sw.WriteLine("Size:				{0}", anim.AnimationData.Value.Length);
					sw.WriteLine("	StaticFlags:	{0}", anim.DataSizes.Value.StaticNodeFlags.Value);
					sw.WriteLine("	AnimatedFlags:	{0}", anim.DataSizes.Value.AnimatedNodeFlags.Value);
					sw.WriteLine("	Movement:		{0}", anim.DataSizes.Value.MovementData.Value);
					sw.WriteLine("	PillOffset:		{0}", anim.DataSizes.Value.PillOffsetData.Value);
					sw.WriteLine("	Default:		{0}", anim.DataSizes.Value.DefaultData.Value);
					sw.WriteLine("	Uncompressed:	{0}", anim.DataSizes.Value.UncompressedData.Value);
					sw.WriteLine("	Compressed:		{0}", anim.DataSizes.Value.CompressedData.Value);
					sw.WriteLine();
				}

				sw.Close();
			}
		}
	};
	#endregion

	#region model_animation_graph
	partial class model_animation_graph_group
	{
		#region animation_graph_cache_block
		partial class animation_graph_cache_block
		{
			public DatumIndex GetOriginal() { return OwnerTagIndex.Value; }
			public int GetSizeOf() { return BlockSize.Value; }
			public ResourcePtr GetOffset() { return BlockOffset.Value; }

			byte[] Data = null;
			public System.IO.MemoryStream Stream = null;
			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				if (c.EngineVersion.IsXbox1())
				{
					var rsrc_ptr = GetOffset();
					var rsrc_cache = Program.Halo2.FromLocation(c as Halo2.CacheFile, rsrc_ptr);

					// the shared cache isn't loaded, break
					if (rsrc_cache == null)
						return false;

					c.InputStream.Seek(rsrc_ptr.Offset);
					Data = c.InputStream.ReadBytes(GetSizeOf());

					Stream = new System.IO.MemoryStream(Data, false);
				}
				return true;
			}

			public void Close()
			{
				if(Stream != null)
				{
					Stream.Close();
					Stream = null;
				}
				Data = null;
			}
		};
		#endregion

		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			if (c.EngineVersion.IsXbox1())
			{
				animation_graph_cache_block cache;
				byte[] temp;
				foreach (animation_graph_resources_struct.animation_pool_block anim
					in Resources.Value.Animations)
				{
					cache = CacheBlocks[anim.ParentGraphBlockIndex];
					temp = new byte[anim.GetTotalSize()];

					if (cache.Stream == null)
						result = false;

					cache.Stream.Position = anim.ParentGraphBlockOffset.Value;
					cache.Stream.Read(temp, 0, temp.Length);
					anim.AnimationData.Reset(temp);

					temp = null;
					cache = null;
				}

				foreach (animation_graph_cache_block cb in CacheBlocks) cb.Close(); // clean up resources
				//CacheBlocks.DeleteAll();
				//CacheUnknown.DeleteAll();
			}

			return result;
		}
	};
	#endregion
	#endregion


	#region particle_model
	partial class particle_model_group
	{
		/// <summary>
		/// Both Xbox and PC don't need any actual reconstruction
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			GeometryBlockInfo.Value.ClearPostReconstruction();

			return true;
		}
	};
	#endregion


	#region physics model
	partial class physics_model_group
	{
		#region shape_info
		partial class shape_info
		{
			public physics_shape_type GetShapeType() { return (physics_shape_type)ShapeType.Value; }

			public TI.Definition GetShape(physics_model_group def)
			{
				var shape_type = GetShapeType();
				int shape_index = Shape.Value;

				if (shape_index == -1)
					throw new Debug.ExceptionLog("Tried to access an invalid {0} shape index in {1}",
						Util.EnumToString(shape_type), def.owner.TagIndex.ToString());

				switch (shape_type)
				{
					case physics_shape_type.Sphere:		return def.Spheres[shape_index];

					case physics_shape_type.Pill:		return def.Pills[shape_index];
					case physics_shape_type.Box:		return def.Boxes[shape_index];
					case physics_shape_type.Triangle:	return def.Triangles[shape_index];
					case physics_shape_type.Polyhedron:	return def.Polyhedra[shape_index];
					case physics_shape_type.MultiSphere:return def.MultiSpheres[shape_index];

					case physics_shape_type.List:		return def.ListShapes[shape_index];
					case physics_shape_type.Mopp:		return def.Mopps[shape_index];
				}

				return null;
			}
		};
		#endregion

		#region rigid_bodies_block
		partial class rigid_bodies_block
		{
			internal void ReconstructShapeData(physics_model_group owner)
			{
				var havok_shape = ShapeInfo.GetShape(owner);
			}
		};
		#endregion

		void ReconstructRigidBodyShapeData()
		{
			foreach (var rb in RigidBodies)
				rb.ReconstructShapeData(this);
		}

		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			//ReconstructRigidBodyShapeData();

			return true;
		}

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
	};
	#endregion


	#region render model
	public enum global_geometry_level_of_detail
	{
		SuperLow,
		Low,
		Medium,
		High,
		SuperHigh,
		Hollywood,
	};

	public enum global_geometry_classification
	{
		Worldspace,
		Rigid,
		RigidBoned,
		Skinned,
		Unsupported,
	};

	[Flags]
	public enum global_geometry_compression
	{
		Position = 1 << 0,
		Texcoord = 1 << 1,
		SecondaryTexCoord = 1 << 2,
	};

	[Flags]
	public enum global_section_lighting_flags
	{
		HasLmTexCoords = 1 << 0,
		HasLmIncRad = 1 << 1,
		HasLmColor = 1 << 2,
	};

	partial class global_geometry_raw_point
	{
		internal void Default()
		{
			NodeWeight[0].Value = 1.0f;
			NodeWeight[0].Value = 0.0f;
			NodeWeight[0].Value = 0.0f;
			NodeWeight[0].Value = 0.0f;
			NodeIndex[0].Value = -1;
			NodeIndex[1].Value = -1;
			NodeIndex[2].Value = -1;
			NodeIndex[3].Value = -1;
			UseNewNodeIndices.Value = 1;
			AdjustedCompoundNodeIndex.Value = -1;
		}
	}

	#region global_geometry_section_info_struct
	partial class global_geometry_section_info_struct
	{
		public bool Convert(Halo1.Tags.gbxmodel_group.model_geometry_block h1)
		{
			return true;
		}
	};
	#endregion

	#region global_geometry_part_block_new
	partial class global_geometry_part_block_new
	{
		public bool Convert(Halo1.Tags.gbxmodel_group.model_geometry_block.model_geometry_part_block h1)
		{
			return true;
		}
	};
	#endregion

	#region global_geometry_section_raw_vertex_block
	partial class global_geometry_section_raw_vertex_block
	{
		internal void Default()
		{
			Point.Default();
			SecondaryTexcoord.Y = 1.0f;
		}

		const uint k_lighting_flags_HasLmTexCoords = (uint)global_section_lighting_flags.HasLmTexCoords;
		const uint k_lighting_flags_HasLmIncRad = (uint)global_section_lighting_flags.HasLmIncRad;
		const uint k_lighting_flags_HasLmColor = (uint)global_section_lighting_flags.HasLmColor;

		void ReconstructRawVertex(global_geometry_section_info_struct section_info,
			int stream_source, BlamLib.Render.VertexBufferInterface.StreamReader stream_reader)
		{
			LowLevel.Math.real_quaternion quat = new LowLevel.Math.real_quaternion();

			if (stream_source == 0)
			{
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypePosition,
					ref quat))
				{
					Point.Position.X = quat.Vector.I;
					Point.Position.Y = quat.Vector.J;
					Point.Position.Z = quat.Vector.K;
				}
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeNodeIndices,
					ref quat))
				{
					Point.NodeIndex[0].Value = ((int)quat.Vector.I) - 1;
					Point.NodeIndex[1].Value = ((int)quat.Vector.J) - 1;
					Point.NodeIndex[2].Value = ((int)quat.Vector.K) - 1;
					Point.NodeIndex[3].Value = ((int)quat.W) - 1;
				}
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeNodeWeights,
					ref quat))
				{
					Point.NodeWeight[0].Value = quat.Vector.I;
					Point.NodeWeight[1].Value = quat.Vector.J;
					Point.NodeWeight[2].Value = quat.Vector.K;
					Point.NodeWeight[3].Value = quat.W;
				}
			}
			else if (stream_source == 1)
			{
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeTexCoord,
					ref quat))
				{
					Texcoord.X = quat.Vector.I;
					Texcoord.Y = quat.Vector.J;
				}
			}
			else if (stream_source == 2)
			{
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeNormal,
					ref quat))
				{
					Normal.I = quat.Vector.I;
					Normal.J = quat.Vector.J;
					Normal.K = quat.Vector.K;
				}
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeBinormal,
					ref quat))
				{
					Binormal.I = quat.Vector.I;
					Binormal.J = quat.Vector.J;
					Binormal.K = quat.Vector.K;
				}
				if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.kTypeTangent,
					ref quat))
				{
					Tangent.I = quat.Vector.I;
					Tangent.J = quat.Vector.J;
					Tangent.K = quat.Vector.K;
				}
			}

			if (section_info != null && stream_source > 2)
			{
				int x = 2;

				// The following LM vertex sources only have one element, so we can call this here
				stream_reader.GetStreamedElement(0, ref quat);

				if (section_info.SectionLightingFlags.Test(k_lighting_flags_HasLmTexCoords) 
					&& stream_source == ++x)
				{
					PrimaryLightmapTexcoord.X = quat.Vector.I;
					PrimaryLightmapTexcoord.Y = quat.Vector.J;
				}
				else if (section_info.SectionLightingFlags.Test(k_lighting_flags_HasLmIncRad)
					&& stream_source == ++x)
				{
					PrimaryLightmapIncidentDirection.I = quat.Vector.I;
					PrimaryLightmapIncidentDirection.J = quat.Vector.J;
					PrimaryLightmapIncidentDirection.K = quat.Vector.K;
				}
				else if (section_info.SectionLightingFlags.Test(k_lighting_flags_HasLmColor)
					&& stream_source == ++x)
				{
					// alpha is quad.W, which LM color doesn't use
					PrimaryLightmapColor.R = quat.Vector.I;
					PrimaryLightmapColor.G = quat.Vector.J;
					PrimaryLightmapColor.B = quat.Vector.K;
				}
			}

			#region Unused vertex elements
// 			if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.VertexBuffersHalo2.kTypeAnisoBinormal,
// 				ref quat))
// 			{
// 				AnisotropicBinormal.I = quat.Vector.I;
// 				AnisotropicBinormal.J = quat.Vector.J;
// 				AnisotropicBinormal.K = quat.Vector.K;
// 			}
// 
// 			if (stream_reader.FindStreamedElement(BlamLib.Render.VertexBufferInterface.VertexBuffersHalo2.kTypeSecondaryTexCoord,
// 				ref quat))
// 			{
// 				SecondaryTexcoord.X = quat.Vector.I;
// 				SecondaryTexcoord.Y = quat.Vector.J;
// 			}
			#endregion
		}
		internal void Reconstruct(global_geometry_section_info_struct section_info,
			geometry_block_resource_block gb,
			IO.EndianReader er,
			BlamLib.Render.VertexBufferInterface.StreamReader[] stream_readers)
		{
			short stream_source = gb.SecondaryLocater.Value;

			if(stream_source == 0)
				Default();

			var stream_r = stream_readers[stream_source];
			if (!stream_r.UsesNullDefinition)
			{
				stream_r.Read(er);
				stream_r.Denormalize();
				ReconstructRawVertex(section_info, stream_source, stream_r);
			}
		}

		public bool Convert(Halo1.Tags.model_group.model_geometry_block.model_geometry_part_block.model_vertex_uncompressed_block h1)
		{
			return true;
		}
	};
	#endregion

	#region global_geometry_section_struct
	partial class global_geometry_section_struct
	{
		#region Reconstruct
		const int OffsetParts = 0;
		const int OffsetSubparts = 8;
		const int OffsetVisibilityBounds = 16;
		//const int OffsetRawVertices = 24;
		const int OffsetStripIndices = 32;
		const int OffsetVisibilityMoppCode = 40;
		const int OffsetMoppReorderTable = 48;
		const int OffsetVertexBuffers = 56;

		/// <summary></summary>
		/// <param name="c"></param>
		/// <param name="section_info">Can be null if tag data doesn't have it</param>
		/// <param name="gbi"></param>
		/// <returns></returns>
		internal bool Reconstruct(Blam.CacheFile c,
			global_geometry_section_info_struct section_info,
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
								case OffsetParts:
									Parts.Resize(count);
									Parts.Read(er);
									break;

								case OffsetSubparts:
									Subparts.Resize(count);
									Subparts.Read(er);
									break;

								case OffsetVisibilityBounds:
									VisibilityBounds.Resize(count);
									VisibilityBounds.Read(er);
									break;

								case OffsetStripIndices:
									StripIndices.Resize(count);
									StripIndices.Read(er);
									break;

								case OffsetMoppReorderTable:
									MoppReorderTable.Resize(count);
									MoppReorderTable.Read(er);
									break;

								case OffsetVertexBuffers:
									VertexBuffers.Resize(count);
									VertexBuffers.Read(er);
									break;
							}
							break;
						#endregion
						#region TagData
						case (int)geometry_block_resource_type.TagData:
							switch (gb.PrimaryLocater.Value)
							{
								case OffsetVisibilityMoppCode:
									VisibilityMoppCode.Reset(er.ReadBytes(gb.Size));
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

							if (RawVertices.Count == 0)
							{
								int vertex_count = section_info != null ?
									section_info.TotalVertexCount :
									gb.Size.Value / VertexBuffers[0].VertexBuffer.StrideSize;

								RawVertices.Resize(vertex_count);
							}

							for (x = 0; x < RawVertices.Count; x++)
								RawVertices[x].Reconstruct(section_info, gb, 
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
		#endregion

		public bool Convert(Halo1.Tags.gbxmodel_group.model_geometry_block h1)
		{
			return true;
		}

		#region Rendering
		#region Device shit
		//D3D.VertexDeclaration Declaration = null;
//		D3D.IndexBuffer IndexBuffer = null;
//		short[] Strips = null;
		internal void OnCreateIndexBuffer(object sender, EventArgs e)
		{
//			if (!Util.ReferenceEquals(IndexBuffer, sender))
//				IndexBuffer = (D3D.IndexBuffer)sender;
//			IndexBuffer.SetData(Strips, 0, D3D.LockFlags.None);
//			IndexBuffer.Unlock();
		}

		public void Dispose()
		{
//			if (IndexBuffer != null)
//			{
//				IndexBuffer.Dispose();
//				IndexBuffer = null;
//			}
			//Declaration = null;
//			Strips = null;
		}
		#endregion

		public void CreateForRender()
		{
			// TODO: update this shit fool
// 			D3D.Device d = Editor.Renderer.GetDevice();
// 			IndexBuffer = new D3D.IndexBuffer(d, 0, D3D.Usage.WriteOnly, D3D.Pool.Managed, true);
// 			IndexBuffer.Created += new EventHandler(OnCreateIndexBuffer);
// 			this.OnCreateIndexBuffer(IndexBuffer, null);
// 
// 			Halo2.Render.VertexBufferDefinition[] defs = new Halo2.Render.VertexBufferDefinition[VertexBuffers.Count];
// 			for (int x = 0; x < defs.Length; x++)
// 				defs[x] = VertexBuffers[x].VertexBuffer.VertexDeclaration;
// 			Declaration = new D3D.VertexDeclaration(d, Halo2.Render.VertexBufferDefinition.Merge(defs).VertexElements);
		}

		public void Render(
			global_geometry_section_info_struct section_info,
			TI.Block<global_geometry_material_block> materials
			)
		{
			// TODO: update this shit fool
// 			D3D.Device d = Editor.Renderer.GetDevice();
// 			foreach(global_geometry_section_vertex_buffer_block svb in VertexBuffers)
// 			{
// 				TI.VertexBuffer vb = svb.VertexBuffer;
// 				d.SetStreamSource(vb.StreamIndex, vb.Buffer, 0);
// 			}
// 			d.VertexDeclaration = Declaration;
// 			d.Indices = IndexBuffer;
// 			global_geometry_material_block mat;
// 			foreach(global_geometry_part_block_new part in Parts)
// 			{
// 				d.DrawIndexedPrimitives(
// 					D3D.PrimitiveType.TriangleStrip,
// 					0, 0,
// 					section_info.TotalVertexCount,
// 					part.StripStartIndex,
// 					part.StripLength);
// 				mat = materials[part.Material];
// 			}
		}
		#endregion
	};
	#endregion

	#region global_geometry_point_data_struct
	partial class global_geometry_point_data_struct
	{
		#region Reconstruct
		const int OffsetRawPoints = 68;
		const int OffsetRuntimePointData = 76;
		const int OffsetRigidPointGroups = 84;
		const int OffsetVertexPointIndices = 92;

		internal bool Reconstruct(
			BlamLib.Blam.CacheFile c,
			global_geometry_section_info_struct section,
			geometry_block_info_struct gbi
			)
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
								case OffsetRawPoints:
									RawPoints.Resize(count);
									RawPoints.Read(er);
									break;

								case OffsetRigidPointGroups:
									RigidPointGroups.Resize(count);
									RigidPointGroups.Read(er);
									break;

								case OffsetVertexPointIndices:
									VertexPointIndices.Resize(count);
									VertexPointIndices.Read(er);
									break;
							}
							break;
						#endregion
						#region TagData
						case (int)geometry_block_resource_type.TagData:
							switch (gb.PrimaryLocater.Value)
							{
								case OffsetRuntimePointData:
									RuntimePointData.Reset(er.ReadBytes(gb.Size));
									break;
							}
							break;
						#endregion
						#region VertexBuffer
						//case (int)geometry_block_resource_type.VertexBuffer:
						#endregion
					}

					index++;
				}
			}

			return true;
		}
		#endregion
	};
	#endregion


	#region render_model_region_block
	partial class render_model_region_block
	{
		#region render_model_permutation_block
		partial class render_model_permutation_block
		{
			public bool Convert(Halo1.Tags.model_group.model_region_block.model_region_permutation_block h1)
			{
				string name = h1.Name.Value;
				if (name == "__base") name = "base";
				this.Name.ResetFromString(name);

				this.L1.Value = (short)h1.SuperLow.Value;
				this.L2.Value = (short)h1.Low.Value;
				this.L3.Value = (short)h1.Medium.Value;
				this.L4.Value = (short)h1.High.Value;
				this.L5.Value = (short)h1.SuperHigh.Value;
				this.L6.Value = this.L5.Value;

				return true;
			}

			public int SectionIndexFromLod(global_geometry_level_of_detail lod)
			{
				switch (lod)
				{
					case global_geometry_level_of_detail.SuperLow: return L1;
					case global_geometry_level_of_detail.Low: return L2;
					case global_geometry_level_of_detail.Medium: return L3;
					case global_geometry_level_of_detail.High: return L4;
					case global_geometry_level_of_detail.SuperHigh: return L5;
					case global_geometry_level_of_detail.Hollywood: return L6;
				}
				return -1;
			}
		};
		#endregion

		public bool Convert(Halo1.Tags.model_group.model_region_block h1)
		{
			this.Name.ResetFromString(h1.Name.Value);
			this.NodeMapOffset.Value = -1;
			this.NodeMapSize.Value = 0;

			this.Permutations.Resize(h1.Permutations.Count);
			for (int x = 0; x < h1.Permutations.Count; x++)
				this.Permutations[x].Convert(h1.Permutations[x]);
			return true;
		}
	};
	#endregion

	#region render_model_section_block
	partial class render_model_section_block
	{
		#region render_model_section_data_block
		partial class render_model_section_data_block
		{
			#region Reconstruct
			const int OffsetNodeMap = 100;

			internal bool Reconstruct(Blam.CacheFile c,
				global_geometry_section_info_struct section,
				geometry_block_info_struct gbi)
			{
				Section.Value.Reconstruct(c, section, gbi);
				PointData.Value.Reconstruct(c, section, gbi);

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
									case OffsetNodeMap:
										NodeMap.Resize(count);
										NodeMap.Read(er);
										break;
								}
								break;
							#endregion
						}

						index++;
					}
				}

				return true;
			}
			#endregion

			public bool Convert(Halo1.Tags.gbxmodel_group.model_geometry_block h1)
			{
				this.Section.Value.Convert(h1);

				return true;
			}

			public void Render(
				global_geometry_section_info_struct section_info,
				TI.Block<global_geometry_material_block> materials
				)
			{
				Section.Value.Render(section_info, materials);
			}
		};
		#endregion

		#region Reconstruct
		internal override bool Reconstruct(Blam.CacheFile c)
		{
			bool result = true;

			if (c.EngineVersion == BlamVersion.Halo2_Alpha) return result;

			// remove 'geometry postprocessed'
			Flags.Value = 0;

			// recreate the section data
			if (SectionData.Count != 1)
			{
				render_model_section_data_block section_data;
				SectionData.Add(out section_data);

				result = section_data.Reconstruct(c, SectionInfo.Value, GeometryBlockInfo.Value);
			}

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
		#endregion

		public bool Convert(Halo1.Tags.gbxmodel_group.model_geometry_block h1)
		{
			this.SectionInfo.Value.Convert(h1);
			this.SectionData.Add();
			this.SectionData[0].Convert(h1);

			return true;
		}

		public void Render(TI.Block<global_geometry_material_block> materials)
		{
			if (SectionData.Count != 1) return;

			SectionData[0].Render(SectionInfo.Value, materials);
		}
	};
	#endregion

	#region render_model_node_block
	partial class render_model_node_block
	{
		public bool Convert(Halo1.Tags.model_group.model_node_block h1)
		{
			this.Name.ResetFromString(h1.Name.Value);
			this.ParentNode.Value = h1.ParentNode.Value;
			this.FirstChildNode.Value = h1.FirstChildNode.Value;
			this.NextSiblingNode.Value = h1.NextSiblingNode.Value;
			this.ImportNodeIndex.Value = -1;
			this.DefaultTranslation.Value = h1.DefaultTranslation.Value;
			this.DefaultRotation.Value = h1.DefaultRotation.Value;

			this.DistFromParent.Value = h1.NodeDistFromParent;
			return true;
		}
	};
	#endregion

	#region render_model_marker_group_block
	partial class render_model_marker_group_block
	{
		#region render_model_marker_block
		partial class render_model_marker_block
		{
			public bool Convert(Halo1.Tags.model_group.model_markers_block.model_marker_instance_block h1)
			{
				this.RegionIndex.Value = (byte)h1.RegionIndex.Value;
				this.PermutationIndex.Value = (byte)h1.PermutationIndex.Value;
				this.NodeIndex.Value = (byte)h1.NodeIndex.Value;
				this.Translation.Value = h1.Translation.Value;
				this.Rotation.Value = h1.Rotation.Value;

				return true;
			}
		};
		#endregion

		public bool Convert(Halo1.Tags.model_group.model_markers_block h1)
		{
			this.Name.ResetFromString(h1.Name.Value);

			this.Markers.Resize(h1.Instances.Count);
			for (int x = 0; x < h1.Instances.Count; x++)
				this.Markers[x].Convert(h1.Instances[x]);

			return true;
		}
	};
	#endregion

	#region prt_info_block
	partial class prt_info_block
	{
		const int OffsetVertexBuffers = 0;

		void ReconstructRawPca(int pca_data_offset,
			int stream_source, Render.VertexBufferInterface.StreamReader stream_reader)
		{
			LowLevel.Math.real_quaternion quat = new LowLevel.Math.real_quaternion();

			if(stream_reader.FindStreamedElement(Render.VertexBufferInterface.VertexBuffersGen2.kTypePcaClusterId, 
				ref quat))
			{
				RawPcaData[(pca_data_offset*5)+0].RawPcaData.Value = quat.Vector.I;
			}
			if (stream_reader.FindStreamedElement(Render.VertexBufferInterface.VertexBuffersGen2.kTypePcaVertexWeights,
				ref quat))
			{
				RawPcaData[(pca_data_offset*5)+1].RawPcaData.Value = quat.Vector.I;
				RawPcaData[(pca_data_offset*5)+2].RawPcaData.Value = quat.Vector.J;
				RawPcaData[(pca_data_offset*5)+3].RawPcaData.Value = quat.Vector.K;
				RawPcaData[(pca_data_offset*5)+4].RawPcaData.Value = quat.W;
			}
		}
		void ReconstructRawPcaData(int pca_data_offset, 
			geometry_block_resource_block gb,
			IO.EndianReader er,
			Render.VertexBufferInterface.StreamReader[] stream_readers)
		{
			short stream_source = gb.SecondaryLocater.Value;
			// HACK: fucking PRT is gay.
			if (stream_source == -1)
				stream_source = 0;

			var stream_r = stream_readers[stream_source];

			if (!stream_r.UsesNullDefinition)
			{
				stream_r.Read(er);
				stream_r.Denormalize();

				ReconstructRawPca(pca_data_offset, stream_source, stream_r);
			}
		}

		bool ReadVertexBufferHack(Blam.CacheFile c,
			geometry_block_info_struct gbi)
		{
			var rsrc_cache = Program.Halo2.FromLocation(c as Halo2.CacheFile, gbi.GetBlockOffset());

			// the shared cache isn't loaded, break
			if (rsrc_cache == null)
				return false;

			// seek to the actual data
			var er = rsrc_cache.InputStream;
			er.Seek(gbi.GetBlockOffset().Offset + 8);

			VertexBuffers.Add();
			VertexBuffers.Read(er);

			return true;
		}

		bool ReconstructRawPcaData(Blam.CacheFile c,
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

							// HACK: Figure out why we PRT stores the vertex buffer in the geometry block info's
							// "section data" and not in a tag block resource
							if (VertexBuffers.Count == 0 && !ReadVertexBufferHack(c, gbi))
								break;

							var stream_readers = new Render.VertexBufferInterface.StreamReader[VertexBuffers.Count];
							for (x = 0; x < VertexBuffers.Count; x++)
								VertexBuffers[x].VertexBuffer.InitializeStreamReader(vb_defs, out stream_readers[x]);

							int vertex_count = gb.Size.Value / VertexBuffers[0].VertexBuffer.StrideSize;
							if (RawPcaData.Count == 0)
								RawPcaData.Resize(vertex_count * 5); // 5x as there are 5 fields in the Pca data

							for (x = 0; x < vertex_count; x++)
								ReconstructRawPcaData(x, gb, er, stream_readers);
							break;
						#endregion
					}
				}
				index++;
			}

			VertexBuffers.DeleteAll();

			return true;
		}

		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			bool result = true;

			if (RawPcaData.Count == 0)
				result = ReconstructRawPcaData(c, GeometryBlockInfo.Value);

			GeometryBlockInfo.Value.ClearPostReconstruction();

			return result;
		}
	};
	#endregion


	#region render_model
	partial class render_model_group
	{
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			Flags.Value &= unchecked((uint)~8); // remove geometry postprocessed
			
			return true;
		}

		void RenderLod(global_geometry_level_of_detail lod)
		{
			foreach(render_model_region_block region in Regions)
			{
				if (region.Permutations.Count < 1) return;
				render_model_region_block.render_model_permutation_block perm;
				perm = region.Permutations[0];
				int section_index = perm.SectionIndexFromLod(lod);
				
				Debug.Assert.If(section_index != -1);
				Sections[section_index].Render(Materials);
			}
		}

		public void Render()
		{
			RenderLod(global_geometry_level_of_detail.Medium);
		}

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

		public bool Convert(Halo1.Tags.gbxmodel_group h1)
		{
			int x;

			this.Regions.Resize(h1.Regions.Count);
			for (x = 0; x < h1.Regions.Count; x++)
				this.Regions[x].Convert(h1.Regions[x]);

			this.Sections.Resize(h1.Geometries.Count);
			for (x = 0; x < h1.Geometries.Count; x++)
				this.Sections[x].Convert(h1.Geometries[x]);

			this.InvalidSectionPairBits.Resize((this.Sections.Count / 4) + 1);

			this.SectionGroups.Add();
			this.SectionGroups[0].DetailLevels.Value = 0x3F;

			this.Nodes.Resize(h1.Nodes.Count);
			for (x = 0; x < h1.Nodes.Count; x++)
				this.Nodes[x].Convert(h1.Nodes[x]);

			this.MarkerGroups.Resize(h1.Markers.Count);
			for (x = 0; x < h1.Markers.Count; x++)
				this.MarkerGroups[x].Convert(h1.Markers[x]);

			// TODO: this kind of logic with tag references b/w two engines doesn't work anymore, fix this
			//this.Materials.Resize(h1.Shaders.Count);
			//for (x = 0; x < h1.Shaders.Count; x++)
			//	this.Materials[x].Shader.Value = h1.Shaders[x].Shader.Value;
			
			return true;
		}
	};
	#endregion
	#endregion
};