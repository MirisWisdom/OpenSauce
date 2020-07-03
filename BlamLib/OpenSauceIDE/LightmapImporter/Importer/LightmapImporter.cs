/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Blam;
using BlamLib.Blam.Halo1.Tags;
using BlamLib.Managers;
using BlamLib.Messaging;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Enums;
using BlamLib.TagInterface;
using String = System.String;

namespace OpenSauceIDE.LightmapImporter.Importer
{
	/// <summary>	A lightmap importer. </summary>
	public class LightmapImporter
		: IMessageSource
	{
		#region Fields
		private TagIndexHandler<BlamLib.Managers.TagIndex> mTagHandler;
		private DatumIndex mBSPDatumIndex;
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="rootPath">	The users HEK root directory. </param>
		public LightmapImporter()
		{
			mTagHandler = null;
			mBSPDatumIndex = DatumIndex.Null;
		}

		#region Messaging
		/// <summary>	Event queue for all listeners interested in MessageSent events. </summary>
		public event EventHandler<MessageArgs> MessageSent;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sends a message. </summary>
		///
		/// <param name="message">	The message. </param>
		private void SendMessage(string message)
		{
			var handler = MessageSent;

			if(handler != null)
			{
				handler(this, new MessageArgs(message));
			}
		}
		#endregion

		public class Lightmap
		{
			public class Face
			{
				public RealPoint2D Vertex0 = new RealPoint2D();
				public RealPoint2D Vertex1 = new RealPoint2D();
				public RealPoint2D Vertex2 = new RealPoint2D();
			}

			public int LightmapIndex;
			public List<Face> Faces;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Import texcoords from a COLLADA file into a matching BSP. </summary>
		///
		/// <param name="tagsPath"> 	The tags directory. </param>
		/// <param name="targetBSP">	The target BSP. </param>
		/// <param name="source">   	The source file. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public bool ImportTexcoords(string tagsPath, string targetBSP, string source)
		{
			// Load the COLLADA file
			var colladaFile = LoadCOLLADAFile(source);
			if (colladaFile == null)
			{
				return false;
			}
			SendMessage(String.Format("Loaded {0}", Path.GetFileName(source)));

			// Process the COLLADA geometry
			var lightmapMeshes = CollateLightmapGeometry(colladaFile);
			if (lightmapMeshes == null)
			{
				return false;
			}
			SendMessage(String.Format("COLLADA lightmap count: {0}", lightmapMeshes.Count));

			int faceCount = 0;
			lightmapMeshes.ForEach(entry => faceCount += entry.Faces.Count);

			// Open the structure BSP tag
			var structureBSP = LoadStructureBSP(tagsPath, targetBSP);
			if (structureBSP == null)
			{
				return false;
			}
			SendMessage(String.Format("Loaded {0}.scenario_structure_bsp", Path.GetFileName(targetBSP)));

			// Verify the data is compatible
			if (!VerifyMeshData(lightmapMeshes, structureBSP))
			{
				return false;
			}
			SendMessage(String.Format("COLLADA to BSP mesh compatibility verified", lightmapMeshes.Count));

			// Apply the new coordinates
			if (!OverwriteLightmapUVs(lightmapMeshes, structureBSP))
			{
				return false;
			}
			SendMessage(String.Format("Coordinates written", lightmapMeshes.Count));

			// Save and unload the BSP
			if (!SaveStructureBSP())
			{
				return false;
			}
			UnloadStructureBSP();

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads the specified COLLADA file. </summary>
		///
		/// <param name="path">	Data relative pathname of the file. </param>
		///
		/// <returns>	The COLLADA file. </returns>
		private ColladaFile LoadCOLLADAFile(string path)
		{
			var xmlSerializer = new XmlSerializer(typeof(ColladaFile));

			// Build the collada file path
			if (!File.Exists(path))
			{
				SendMessage("The provided source file does not exist");
				return null;
			}

			// Deserialize the COLLADA file
			ColladaFile colladaFile = null;
			using (var reader = XmlReader.Create(path))
			{
				if (!xmlSerializer.CanDeserialize(reader))
				{
					SendMessage("Unable to deserialize the source file");
					return null;
				}

				colladaFile = xmlSerializer.Deserialize(reader) as ColladaFile;

				if (colladaFile == null)
				{
					SendMessage("Failed to deserialize the source file");
					return null;
				}
			}

			return colladaFile;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Collate's the lightmap geometry from a COLLADA file into a usable format. </summary>
		///
		/// <param name="colladaFile">	The collada file. </param>
		///
		/// <returns>	A list of lightmap mesh data. </returns>
		List<Lightmap> CollateLightmapGeometry(ColladaFile colladaFile)
		{
			var lightmapMeshes = new List<Lightmap>();

			// Collect the geometry info
			foreach (var geometry in colladaFile.LibraryGeometries.Geometry)
			{
				if (geometry.Mesh == null)
				{
					SendMessage(String.Format("The geometry {0} does not have a mesh", geometry.Name));
					return null;
				}

				if (geometry.Mesh.Triangles.Count == 0)
				{
					SendMessage(String.Format("The geometry {0} has no triangles", geometry.Name));
					return null;
				}

				// Get the lightmap index from the end of the geometry name
				if(geometry.Name.EndsWith("-1"))
				{
					continue;
				}

				var indexString = Regex.Match(geometry.Name, @"\d+$").Value;
				if (System.String.IsNullOrEmpty(indexString))
				{
					SendMessage(String.Format("The geometry {0} does not have the lightmap index at the end of it's name", geometry.Name));
					return null;
				}
				int lightmapIndex = Int32.Parse(indexString);

				// Create the lightmap entry
				var lightmap = new Lightmap() { LightmapIndex = lightmapIndex, Faces = new List<Lightmap.Face>() };

				foreach (var triangles in geometry.Mesh.Triangles)
				{
					// Get the input object containing the texture coordinates
					var texcoord1Input = triangles.Input.Find(entry => (entry.Set == 1) && (entry.Semantic == ColladaInputSharedSemantic.TEXCOORD));
					if (texcoord1Input == null)
					{
						SendMessage(String.Format("Unable to find a second texture coordinate for geometry {0}", geometry.Name));
						return null;
					}
					var sourceID = texcoord1Input.Source.TrimStart('#');

					// Get the source array for the coord data
					var sourceArray = geometry.Mesh.Source.Find(
						entry =>
						{
							return entry.ID.EndsWith(sourceID);
						}
					);

					if (sourceArray == null)
					{
						SendMessage(String.Format("Unable to find a texture coordinate source array for geometry {0}", geometry.Name));
						return null;
					}

					if (sourceArray.FloatArray == null)
					{
						SendMessage(String.Format("Unable to find a texture coordinate source array for geometry {0}", geometry.Name));
						return null;
					}

					// Collect the vertex data
					int sourceArrayStride = (int)sourceArray.TechniqueCommon.Accessor.Stride;
					var indexStride = (triangles.P.Values.Count / 3) / (int)triangles.Count;

					foreach (var faceIndex in Enumerable.Range(0, (int)triangles.Count))
					{
						var face = new Lightmap.Face();

						// Get the index of the texture coordinates for the face vertices
						int faceOffset = (indexStride * 3) * faceIndex; // Offset to start of current face

						int vertex0Offset = faceOffset + (0 * indexStride) + (int)texcoord1Input.Offset;
						int vertex1Offset = faceOffset + (1 * indexStride) + (int)texcoord1Input.Offset;
						int vertex2Offset = faceOffset + (2 * indexStride) + (int)texcoord1Input.Offset;

						int vertex0Index = triangles.P.Values[vertex0Offset];
						int vertex1Index = triangles.P.Values[vertex1Offset];
						int vertex2Index = triangles.P.Values[vertex2Offset];

						// Get the data from the float array
						int uIndex = (vertex0Index * sourceArrayStride);
						int vIndex = (vertex0Index * sourceArrayStride) + 1;
						face.Vertex0.X = sourceArray.FloatArray.Values[uIndex];
						face.Vertex0.Y = (sourceArray.FloatArray.Values[vIndex] - 1) * -1;

						uIndex = (vertex1Index * sourceArrayStride);
						vIndex = (vertex1Index * sourceArrayStride) + 1;
						face.Vertex1.X = sourceArray.FloatArray.Values[uIndex];
						face.Vertex1.Y = (sourceArray.FloatArray.Values[vIndex] - 1) * -1;

						uIndex = (vertex2Index * sourceArrayStride);
						vIndex = (vertex2Index * sourceArrayStride) + 1;
						face.Vertex2.X = sourceArray.FloatArray.Values[uIndex];
						face.Vertex2.Y = (sourceArray.FloatArray.Values[vIndex] - 1) * -1;

						lightmap.Faces.Add(face);
					}
				}

				lightmapMeshes.Add(lightmap);
			}

			return lightmapMeshes;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads a structure bsp. </summary>
		///
		/// <param name="path">	Tags relative pathname of the bsp tag. </param>
		///
		/// <returns>	The loaded structure bsp. </returns>
		structure_bsp_group LoadStructureBSP(string tagsDirectory, string path)
		{
			// Load the structure bsp
			var tagsPath = new BlamPath(tagsDirectory);
			mTagHandler = new TagIndexHandler<BlamLib.Managers.TagIndex>(BlamLib.BlamVersion.Halo1_CE, tagsPath.Root);

			mBSPDatumIndex = mTagHandler.IndexInterface.Open(path, BlamLib.Blam.Halo1.TagGroups.sbsp);
			if (!BlamLib.Managers.TagIndex.IsValid(mBSPDatumIndex))
			{
				SendMessage("Failed to load the target BSP");
				return null;
			}

			var tagManager = mTagHandler.IndexInterface[mBSPDatumIndex];
			var structureBSP = tagManager.TagDefinition as structure_bsp_group;
			if (structureBSP == null)
			{
				SendMessage("Failed to load the target BSP");
				return null;
			}

			return structureBSP;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Saves the current structure bsp. </summary>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool SaveStructureBSP()
		{
			if(!mTagHandler.IndexInterface.Save(mBSPDatumIndex))
			{
				SendMessage("Failed to save the target BSP");
				return false;
			}

			return true;
		}

		/// <summary>	Unload the current structure bsp. </summary>
		void UnloadStructureBSP()
		{
			mTagHandler.IndexInterface.UnloadAll();
			mTagHandler = null;

			mBSPDatumIndex = DatumIndex.Null;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Verifies that the lightmap mesh data is compatible with the loaded bsp. </summary>
		///
		/// <param name="lightmapMeshes">	The lightmap meshes. </param>
		/// <param name="BSP">			 	The bsp. </param>
		///
		/// <returns>	true if it passes, false if it fails. </returns>
		bool VerifyMeshData(List<Lightmap> lightmapMeshes, structure_bsp_group BSP)
		{
			// Verify the BSP matches the loaded geometry
			foreach (var lightmap in lightmapMeshes)
			{
				// Get the lightmap objects from the bsp and collada file
				if(lightmap.LightmapIndex < 0 || lightmap.LightmapIndex >= BSP.Lightmaps.Count)
				{
					SendMessage(String.Format("A mesh in the COLLADA file has an out of bounds lightmap index ({0})", lightmap.LightmapIndex));
					return false;
				}

				var bspLightmap = BSP.Lightmaps[lightmap.LightmapIndex];

				// Check the face count matches
				int colladaFaceCount = lightmap.Faces.Count;
				int bspFaceCount = 0;
				bspLightmap.Materials.Elements.ForEach(entry => bspFaceCount += entry.SurfaceCount);

				SendMessage(String.Format("COLLADA lightmap mesh {0}'s face count: {1}", lightmap.LightmapIndex, colladaFaceCount));
				SendMessage(String.Format("BSP lightmap mesh {0}'s face count: {1}", lightmap.LightmapIndex, bspFaceCount));

				if (colladaFaceCount != bspFaceCount)
				{
					SendMessage(String.Format("COLLADA lightmap mesh {0}'s face count does not match the bsp", lightmap.LightmapIndex));
					return false;
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Overwrite the lightmap uv's in the target bsp. </summary>
		///
		/// <param name="lightmapMeshes">	The lightmap meshes. </param>
		/// <param name="BSP">			 	The bsp. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool OverwriteLightmapUVs(List<Lightmap> lightmapMeshes, structure_bsp_group BSP)
		{
			int bspLightmapCount = BSP.Lightmaps.Count - 1;

			// Replace the vertex uv's
			foreach (var lightmap in lightmapMeshes)
			{
				var bspLightmap = BSP.Lightmaps[lightmap.LightmapIndex];

				int startFaceOffset = 0;
				foreach (var material in bspLightmap.Materials)
				{
					// Write the lightmap UV's to the BSP's vertex data
					try
					{
						var binaryWriter = new BinaryWriter(new MemoryStream(material.UncompressedVertices.Value));

						// Skip past the normal texture coordinates
						int vertexOffset = material.VerticesCount * 56;
						int faceIndex = startFaceOffset;
						foreach (int surfaceIndex in Enumerable.Range(0, material.SurfaceCount))
						{
							var colladaFace = lightmap.Faces[faceIndex];
							var surface = BSP.Surfaces[material.Surfaces + surfaceIndex];

							// Write the coordinate values
							binaryWriter.Seek(vertexOffset + ((int)surface.A1.Value * 20) + 12, SeekOrigin.Begin);
							binaryWriter.Write(colladaFace.Vertex2.X);
							binaryWriter.Write(colladaFace.Vertex2.Y);

							binaryWriter.Seek(vertexOffset + ((int)surface.A2.Value * 20) + 12, SeekOrigin.Begin);
							binaryWriter.Write(colladaFace.Vertex1.X);
							binaryWriter.Write(colladaFace.Vertex1.Y);

							binaryWriter.Seek(vertexOffset + ((int)surface.A3.Value * 20) + 12, SeekOrigin.Begin);
							binaryWriter.Write(colladaFace.Vertex0.X);
							binaryWriter.Write(colladaFace.Vertex0.Y);

							faceIndex++;
						}

						binaryWriter.Flush();
					}
					catch (Exception exception)
					{
						SendMessage(String.Format("Failed to write to the target BSP due to an exception: {0}", exception.Message));
						return false;
					}

					startFaceOffset += material.SurfaceCount;
				}
			}

			return true;
		}
	}
}
