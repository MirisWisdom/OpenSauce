/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib.Blam;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for halo 1 bsp data provider. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface IHalo1BSPDataProvider : IColladaDataProvider
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the render mesh should be included.
		/// </summary>
		/// <value>	true if include render mesh, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		bool IncludeRenderMesh { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the portals should be included.
		/// </summary>
		/// <value>	true if include portals, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		bool IncludePortals { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the fog planes should be included.
		/// </summary>
		/// <value>	true if include fog planes, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		bool IncludeFogPlanes { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the markers. </summary>
		/// <returns>	The markers. </returns>
		///-------------------------------------------------------------------------------------------------
		StructureBSPData.StructureBSPMarkerList GetMarkers();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Collada Halo1 Structure BSP exporter. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ColladaBSPExporter : ColladaExporterHalo1
	{
		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="arguments"> 	Export arguments for the collada interface. </param>
		/// <param name="tagIndex">  	The tag index containing the tag being exported. </param>
		/// <param name="tagManager">	The tag manager of the tag being exported. </param>
		///-------------------------------------------------------------------------------------------------
		public ColladaBSPExporter(IColladaSettings settings, Managers.TagIndexBase tagIndex, Managers.TagManager tagManager)
			: base(settings, tagIndex)
		{
			mTagManager = tagManager;
		}
		#endregion
		
		#region Fields
		private IHalo1BSPDataProvider mBSPDataProvider = null;
		private Managers.TagManager mTagManager;
		private string TagName
		{
			get { return Path.GetFileNameWithoutExtension(mTagManager.Name); }
		}
		#endregion Fields

		#region Element Creation
		#region Create Effects
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a semitransparent, green effect for portals. </summary>
		/// <returns>	The new portals effect. </returns>
		///-------------------------------------------------------------------------------------------------
		private Fx.ColladaEffect CreatePortalsEffect()
		{
			var effect = CreateDefaultEffect("portals");
			effect.ProfileCOMMON[0].Technique.Phong.Emission.Color.SetColor(0, 1, 0, 1);
			effect.ProfileCOMMON[0].Technique.Phong.Transparency.Float.Value = 0.25f;
			effect.ProfileCOMMON[0].Technique.Phong.Diffuse.Color.SetColor(0, 1, 0, 1);
			return effect;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a semitransparent, yellow effect for fogplanes. </summary>
		/// <returns>	The new fog planes effect. </returns>
		///-------------------------------------------------------------------------------------------------
		private Fx.ColladaEffect CreateFogPlanesEffect()
		{
			var effect = CreateDefaultEffect("fogplanes");
			effect.ProfileCOMMON[0].Technique.Phong.Emission.Color.SetColor(1, 1, 0, 1);
			effect.ProfileCOMMON[0].Technique.Phong.Transparency.Float.Value = 0.25f;
			effect.ProfileCOMMON[0].Technique.Phong.Diffuse.Color.SetColor(1, 1, 0, 1);
			return effect;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the portals effect to the effect list. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateEffectListPortals()
		{
			listEffect.Add(CreatePortalsEffect());
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the fog planes effect to the effect list. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateEffectListFogPlanes()
		{
			listEffect.Add(CreateFogPlanesEffect());
		}
		#endregion
		#region Create Materials
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populate the material list for portals geometry. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateMaterialListPortals()
		{
			string shaderName = "portals";
			listMaterial.Add(
				CreateMaterial(shaderName,
					shaderName,
					shaderName));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populate the material list for fogplane geometry. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateMaterialListFogPlanes()
		{
			string shader_name = "fogplanes";
			listMaterial.Add(
				CreateMaterial(shader_name,
					shader_name,
					shader_name));
		}
		#endregion
		#region Create Geometry
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a vertex index list from a set of bsp surfaces. </summary>
		/// <param name="definition">	 	The bsp tag definition. </param>
		/// <param name="surface_offset">	The surface index to start collecting indices from. </param>
		/// <param name="surface_count"> 	The number of surfaces to collect indices from. </param>
		/// <param name="index_offset">  	The amount to offset the collected indices by. </param>
		/// <returns>	The new bsp indices. </returns>
		///-------------------------------------------------------------------------------------------------
		private List<int> CreateIndicesBSP(H1.Tags.structure_bsp_group definition
			, int surface_offset
			, int surface_count
			, int index_offset)
		{
			var indices = new List<int>();

			for (int i = 0; i < surface_count; i++)
			{
				indices.Add(definition.Surfaces[surface_offset + i].A3 + index_offset);
				indices.Add(definition.Surfaces[surface_offset + i].A2 + index_offset);
				indices.Add(definition.Surfaces[surface_offset + i].A1 + index_offset);
			}

			return indices;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a geometry element for a BSP lightmap. </summary>
		/// <param name="index">	The lightmap index to create a geometry from. </param>
		///-------------------------------------------------------------------------------------------------
		private void CreateRenderGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = mTagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Geometry geometryData = new Geometry(ColladaUtilities.FormatName(TagName, " ", "_") + "_" + definition.Lightmaps[index].Bitmap.ToString()
				, 2
				, Geometry.VertexComponent.POSITION
				| Geometry.VertexComponent.NORMAL 
				| Geometry.VertexComponent.BINORMAL 
				| Geometry.VertexComponent.TANGENT 
				| Geometry.VertexComponent.TEXCOORD);

			// add all of the vertices used in the render geometry
			foreach (var material in definition.Lightmaps[index].Materials)
			{
				// read vertex information from the uncompressed vertex data
				System.IO.BinaryReader uncompressed_reader = new System.IO.BinaryReader(
					new System.IO.MemoryStream(material.UncompressedVertices.Value));

				int vertex_count = material.VerticesCount;

				for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++)
				{
					Geometry.Vertex common_vertex = new Geometry.Vertex(
						//RealPoint3D position
						new LowLevel.Math.real_point3d(
							uncompressed_reader.ReadSingle() * 100,
							uncompressed_reader.ReadSingle() * 100,
							uncompressed_reader.ReadSingle() * 100),
						//RealVector3D  normal
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()),
						//RealVector3D  binormal
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()),
						//RealVector3D  tangent
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()));

					//RealPoint2D   texcoord0
					common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
						uncompressed_reader.ReadSingle(),
						(uncompressed_reader.ReadSingle() * -1) + 1));

					//RealPoint2D   texcoord1
					if (material.LightmapVerticesCount != 0)
					{
						int position = (int)uncompressed_reader.BaseStream.Position;
						uncompressed_reader.BaseStream.Position = (material.VerticesCount * 56) + (vertex_index * 20) + 12;

						common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
							uncompressed_reader.ReadSingle(),
							(uncompressed_reader.ReadSingle() * -1) + 1));

						uncompressed_reader.BaseStream.Position = position;
					}
					else
					{
						common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(0, 1));
					}

					geometryData.AddVertex(common_vertex);
				};
			}

			// add part definitions for the lightmap materials
			// an index offset is necessary since the vertex list is global for this geometry, rather than local to each material
			int index_offset = 0;
			foreach (var material in definition.Lightmaps[index].Materials)
			{
				Geometry.Part common_part = new Geometry.Part(Path.GetFileNameWithoutExtension(material.Shader.ToString()));
				common_part.AddIndices(CreateIndicesBSP(definition, material.Surfaces, material.SurfaceCount, index_offset));

				index_offset += material.VerticesCount;

				geometryData.AddPart(common_part);
			}

			// create the geometry element
			CreateGeometry(geometryData);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a geometry element for a single cluster portal. </summary>
		/// <param name="index">	The lightmap index to create a geometry from. </param>
		///-------------------------------------------------------------------------------------------------
		private void CreatePortalsGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = mTagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Geometry geometryData = new Geometry("portal-" + index.ToString()
				, 0
				, Geometry.VertexComponent.POSITION
				| Geometry.VertexComponent.NORMAL);

			foreach (var vertex in definition.ClusterPortals[index].Vertices)
			{
				geometryData.AddVertex(new Geometry.Vertex(vertex.Value.ToPoint3D(100),
					new LowLevel.Math.real_vector3d(0, 0, 1)));
			}

			// we only have one part since it only has one material
			Geometry.Part common_part = new Geometry.Part("portals");
			common_part.AddIndices(BuildFaceIndices(definition.ClusterPortals[index].Vertices.Count));
			geometryData.AddPart(common_part);

			// create the geometry element
			CreateGeometry(geometryData);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a geometry element for a single fog plane. </summary>
		/// <param name="index">	Index of the fog plane to create a geometry element for. </param>
		///-------------------------------------------------------------------------------------------------
		private void CreateFogPlaneGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = mTagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Geometry geometryData = new Geometry("fogplane-" + index.ToString()
				, 0
				, Geometry.VertexComponent.POSITION);

			foreach (var vertex in definition.FogPlanes[index].Vertices)
			{
				geometryData.AddVertex(new Geometry.Vertex(vertex.Value.ToPoint3D(100)));
			}

			// we only have one part since it only has one material
			Geometry.Part common_part = new Geometry.Part("fogplanes");
			common_part.AddIndices(BuildFaceIndices(definition.FogPlanes[index].Vertices.Count));
			geometryData.AddPart(common_part);

			// create the geometry element
			CreateGeometry(geometryData);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates geometries for the relevant BSP meshes that are to be included in the collada
		/// 	file.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateGeometryList()
		{
			IHalo1BSPDataProvider provider = GetDataProvider<IHalo1BSPDataProvider>();

			H1.Tags.structure_bsp_group definition = mTagManager.TagDefinition as H1.Tags.structure_bsp_group;

			if (provider.IncludeRenderMesh)
			{
				for (int i = 0; i < definition.Lightmaps.Count; i++)
				{
					CreateRenderGeometry(i);
				}
			}

			if (provider.IncludePortals)
			{
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
				{
					CreatePortalsGeometry(i);
				}
			}

			if (provider.IncludeFogPlanes)
			{
				for (int i = 0; i < definition.FogPlanes.Count; i++)
				{
					CreateFogPlaneGeometry(i);
				}
			}
		}
		#endregion
		#region Create Markers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates nodes for the BSP markers. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateMarkerList()
		{
			var markers = mBSPDataProvider.GetMarkers();

			var markerList = new List<Marker>();

			// create common marker definitions for the bsp markers
			foreach (var marker in markers)
			{
				var commonMarker = new Marker(marker.Name,
					marker.Position.ToPoint3D(100), 
					TagInterface.RealQuaternion.Invert(marker.Rotation),
					-1);

				markerList.Add(commonMarker);
			}

			// create the marker node elements
			CreateMarkers(markerList
				, new LowLevel.Math.real_vector3d(1, 0, 0)
				, new LowLevel.Math.real_vector3d(0, -1, 0)
				, new LowLevel.Math.real_vector3d(0, 0, 1)
				, ColladaUtilities.ColladaRotationOrder.XYZ
			);
		}
		#endregion
		#region Create Nodes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates nodes for all the geometry elements in the collada file. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateNodeList()
		{
			// Create the list of materials for the geometry to use
			var materialReferences = new MaterialReferenceList();

			Action<string> addMaterialRef = 
				effectName =>
				{
					string name = ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(effectName), " ", "_");

					materialReferences.Add(new MaterialReference(
						effectName,
						ColladaUtilities.BuildUri(ColladaElement.FormatID<Fx.ColladaMaterial>(name)),
						name));
				};

			if (mShaderDataProvider != null)
			{
				// create a list of every shader used 
				if (mBSPDataProvider.IncludeRenderMesh)
				{
					foreach (var effect in mShaderDataProvider.GetEffects())
					{
						addMaterialRef(effect.Name);
					}
				}

				// if portals are included add the portals shader to the names
				if (mBSPDataProvider.IncludePortals)
				{
					addMaterialRef("portals");
				}

				// if fogplanes are included add the fogplanes shader to the names
				if (mBSPDataProvider.IncludeFogPlanes)
				{
					addMaterialRef("fogplanes");
				}
			}

			// Create a node with a geometry instance for all included geometry
			var definition = mTagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Func<int, Core.ColladaNode> addGeometryInstance =
				geometryIndex =>
				{
					string name = listGeometry[geometryIndex].Name;
					var node = CreateNode(name, "", name, Enums.ColladaNodeType.NODE);

					string url = ColladaUtilities.BuildUri(listGeometry[geometryIndex].ID);
					node.Add(CreateInstanceGeometry(url, name, materialReferences));

					return node;
				};

			int geometry_offset = 0;
			if (mBSPDataProvider.IncludeRenderMesh)
			{
				// create geometry instance for all of the lightmaps
				for (int i = 0; i < definition.Lightmaps.Count; i++)
				{
					listNode.Add(addGeometryInstance(geometry_offset + i));
				}
				geometry_offset += definition.Lightmaps.Count;
			}

			if (mBSPDataProvider.IncludePortals)
			{
				// create geometry instance for all of the portal meshes
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
				{
					listNode.Add(addGeometryInstance(geometry_offset + i));
				}
				geometry_offset += definition.ClusterPortals.Count;
			}

			if (mBSPDataProvider.IncludeFogPlanes)
			{
				// create geometry instance for all of the fogplane meshes
				for (int i = 0; i < definition.FogPlanes.Count; i++)
				{
					listNode.Add(addGeometryInstance(geometry_offset + i));
				}
				geometry_offset += definition.FogPlanes.Count;
			}
		}
		#endregion
		#endregion

		#region Library Creation
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates the library_visual_scenes element in the collada file. The node list is added
		/// 	under a node named "frame" since that is required when creating new BSPs.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		void AddLibraryVisualScenes()
		{
			// add the main scene node
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = new List<Core.ColladaNode>();

			var frame = new BlamLib.Render.COLLADA.Core.ColladaNode();
			frame.Name = "frame";
			frame.AddRange(listNode);

			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.Add(frame);
		}
		#endregion

		protected override bool BuildColladaInstanceImpl()
		{
			COLLADAFile = new ColladaFile();

			COLLADAFile.Version = "1.4.1";
			AddAsset(
				System.Environment.UserName,
				"OpenSauceIDE:ColladaBuilder",
				"meter", 0.0254, Enums.ColladaUpAxisEnum.Z_UP);

			mBSPDataProvider = GetDataProvider<IHalo1BSPDataProvider>();
			
			if (mBSPDataProvider.IncludeRenderMesh)
			{
				CreateImageList();
				CreateEffectList();
				CreateMaterialList();
			}

			if (mBSPDataProvider.IncludePortals)
			{
				CreateEffectListPortals();
				CreateMaterialListPortals();
			}

			if (mBSPDataProvider.IncludeFogPlanes)
			{
				CreateEffectListFogPlanes();
				CreateMaterialListFogPlanes();
			}

			CreateGeometryList();
			CreateMarkerList();
			CreateNodeList();

			AddLibraryImages();
			AddLibraryEffects();
			AddLibraryMaterials();
			AddLibraryGeometries();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	};
}