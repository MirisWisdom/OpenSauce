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
using System;
using System.Collections.Generic;
using System.IO;
using BlamLib.Blam;
using BlamLib.Managers;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Halo1
{
	/// <summary>
	/// Provides readonly information about an object in a model tag for external use
	/// </summary>
	public class ColladaHalo1ModelInfo : ColladaHaloModelInfoBase
	{
		public int Permutation { get; private set; }
		public int LevelOfDetail { get; private set; }

		public ColladaHalo1ModelInfo(int internal_index, string name, 
			int vertex_count, int face_count, 
			int perm, int lod)
			: base(internal_index, name, vertex_count, face_count)
		{
			Permutation = perm;
			LevelOfDetail = lod;
		}
	};
	/// <summary>
	/// Provides readonly information about an object in a BSP tag for external use
	/// </summary>
	public class ColladaHalo1BSPInfo : ColladaHaloModelInfoBase
	{
		public BSPObjectType Type { get; private set; }

		//private ColladaHalo1BSPInfo() { }
		public ColladaHalo1BSPInfo(int internal_index, string name,
			int vertex_count, int face_count,
			BSPObjectType bsp_type)
			: base(internal_index, name, vertex_count, face_count)
		{
			Type = bsp_type;
		}
	};

	class ColladaHalo1 : ColladaInterface
	{
		#region Internal Classes
		/// <summary>
		/// Interface class to pass shader datum indices to the Halo1 exporter base class
		/// </summary>
		protected class ShaderInfoInternal : ColladaInfoInternal, IHaloShaderDatumList
		{
			struct ShaderReference
			{
				public DatumIndex Datum;
				public string Name;
			}
			List<ShaderReference> shaders = new List<ShaderReference>();

			bool ShaderExists(DatumIndex shader_datum)
			{
				foreach (var shader in shaders)
					if (shader.Datum == shader_datum)
						return true;
				return false;
			}

			public void AddShaderDatum(DatumIndex shader_datum, string name)
			{
				if (ShaderExists(shader_datum))
					return;

				ShaderReference shader = new ShaderReference();
				shader.Name = name;
				shader.Datum = shader_datum;

				shaders.Add(shader);
			}

			#region IHaloShaderDatumList Members
			public int GetShaderCount()
			{
				return shaders.Count;
			}

			public DatumIndex GetShaderDatum(int index)
			{
				return shaders[index].Datum;
			}

			public string GetShaderName(int index)
			{
				return shaders[index].Name;
			}
			#endregion
		}
		/// <summary>
		/// Interface class to pass geometry name and index information to the Halo 1 model exporter
		/// </summary>
		protected class ModelInfoInternal : ShaderInfoInternal, IHalo1ModelInterface
		{
			struct GeometryInfo
			{
				public string Name;
				public int Index;
			};

			public bool IsMultiplePermutations = false;
			public int Permutation = 0;

			List<GeometryInfo> geometries = new List<GeometryInfo>();

			/// <summary>
			/// Determines if a geometry with a matching index already exists in the list
			/// </summary>
			/// <param name="index">The geometry index to search for</param>
			/// <returns>True if a matching element is found</returns>
			bool GeometryExists(int index)
			{
				foreach (var geometry in geometries)
					if (geometry.Index == index)
						return true;
				return false;
			}
			/// <summary>
			/// Adds a geometry name and index pair to the list, if an element with a matching index does not already exist
			/// </summary>
			/// <param name="name">The geometry name</param>
			/// <param name="index">The geometry index</param>
			public void AddGeometry(string name, int index)
			{
				if (GeometryExists(index))
					return;

				GeometryInfo geometry = new GeometryInfo();
				geometry.Name = name;
				geometry.Index = index;

				geometries.Add(geometry);
			}

			#region IHalo1GeometryList Members
			public int GetGeometryCount()
			{
				return geometries.Count;
			}
			public string GetGeometryName(int index)
			{
				return geometries[index].Name;
			}
			public int GetGeometryIndex(int index)
			{
				return geometries[index].Index;
			}
			bool IHalo1ModelInterface.GetIsMultiplePerms()
			{
				return IsMultiplePermutations;
			}
			public int GetPermutation()
			{
				return Permutation;
			}
			#endregion
		}
		/// <summary>
		/// Interface class to pass mesh include information to the Halo1 BSP exporter
		/// </summary>
		protected class BSPInfoInternal : ShaderInfoInternal, IHalo1BSPInterface
		{
			BSPObjectType type;

			/// <summary>
			/// Sets the bsp object type
			/// </summary>
			/// <param name="bsp_type"></param>
			public void SetType(BSPObjectType bsp_type)
			{
				type = bsp_type;
			}

			#region IHalo1BSPInterface Members
			public bool IncludeRenderMesh()
			{
				return ((type & BSPObjectType.RenderMesh) == BSPObjectType.RenderMesh);
			}
			public bool IncludePortalsMesh()
			{
				return ((type & BSPObjectType.Portals) == BSPObjectType.Portals);
			}
			public bool IncludeFogPlanesMesh()
			{
				return ((type & BSPObjectType.FogPlanes) == BSPObjectType.FogPlanes);
			}
			#endregion
		}
		#endregion

		#region Static Helper Classes
		protected static class Model
		{
			public static void AddGeometryInfos(ModelInfoInternal model_info, TagManager manager, int permutation, int lod)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

				foreach (var region in definition.Regions)
				{
					int permutation_index = permutation;

					if (permutation >= region.Permutations.Count)
						permutation_index = region.Permutations.Count - 1;

					string name = string.Format("{0}-{1}-lod{2}", 
						region.Name.Value, region.Permutations[permutation_index].Name, lod.ToString());

					int index = 0;
					switch (lod)
					{
						case 0: index = region.Permutations[permutation_index].SuperHigh; break;
						case 1: index = region.Permutations[permutation_index].High; break;
						case 2: index = region.Permutations[permutation_index].Medium; break;
						case 3: index = region.Permutations[permutation_index].Low; break;
						case 4: index = region.Permutations[permutation_index].SuperLow; break;
					};

					model_info.AddGeometry(name, index);
				}
			}
			public static void AddShaderDatums(ModelInfoInternal model_info, TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

				for (int i = 0; i < model_info.GetGeometryCount(); i++)
				{
					foreach (var part in definition.Geometries[model_info.GetGeometryIndex(i)].Parts)
						model_info.AddShaderDatum(definition.Shaders[part.ShaderIndex.Value].Shader.Datum,
							definition.Shaders[part.ShaderIndex.Value].Shader.ToString());
				}
			}

			public static int GetPermutationCount(TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

				int permutation_count = 0;
				foreach (var region in definition.Regions)
					permutation_count = (region.Permutations.Count > permutation_count ? region.Permutations.Count : permutation_count);
				return permutation_count;
			}
			public static int GetVertexCount(ModelInfoInternal model_info, TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

				int vertex_count = 0;
				for (int i = 0; i < model_info.GetGeometryCount(); i++)
				{
					foreach (var part in definition.Geometries[model_info.GetGeometryIndex(i)].Parts)
						vertex_count += part.UncompressedVertices.Count;
				}
				return vertex_count;
			}
			public static int GetTriangleCount(ModelInfoInternal model_info, TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

				int triangle_count = 0;
				for (int i = 0; i < model_info.GetGeometryCount(); i++)
				{
					foreach (var part in definition.Geometries[model_info.GetGeometryIndex(i)].Parts)
						triangle_count += part.Triangles.Count;
				}
				return triangle_count;
			}
		};
		protected static class BSP
		{
			public static void AddShaderDatums(BSPInfoInternal bsp_info, TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.structure_bsp_group;

				foreach (var lightmap in definition.Lightmaps)
				{
					foreach (var material in lightmap.Materials)
						bsp_info.AddShaderDatum(material.Shader.Datum,
							material.Shader.ToString());
				}
			}

			public static int GetVertexCount(TagManager manager, BSPObjectType type)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.structure_bsp_group;
				int count = 0;
				switch (type)
				{
					case BSPObjectType.RenderMesh:
						foreach (var lightmap in definition.Lightmaps)
						{
							foreach (var material in lightmap.Materials)
								count += material.VertexBuffersCount1;
						}
						break;
					case BSPObjectType.Portals:
						foreach (var portal in definition.ClusterPortals)
							count += portal.Vertices.Count;
						break;
					case BSPObjectType.FogPlanes:
						foreach (var fogplane in definition.FogPlanes)
							count += fogplane.Vertices.Count;
						break;
				};
				return count;
			}
			public static int GetTriangleCount(TagManager manager, BSPObjectType type)
			{
				var definition = manager.TagDefinition as Blam.Halo1.Tags.structure_bsp_group;
				int count = 0;
				switch (type)
				{
					case BSPObjectType.RenderMesh:
						foreach (var lightmap in definition.Lightmaps)
						{
							foreach (var material in lightmap.Materials)
								count += material.SurfaceCount;
						}
						break;
					case BSPObjectType.Portals:
						foreach (var portal in definition.ClusterPortals)
							count += portal.Vertices.Count - 2;
						break;
					case BSPObjectType.FogPlanes:
						foreach (var fogplane in definition.FogPlanes)
							count += fogplane.Vertices.Count - 2;
						break;
				};
				return count;
			}
		};
		#endregion

		#region Class Members
		TagIndex tagIndex;
		TagManager tagManager;
		#endregion

		#region Constructors
		/// <summary>
		/// Halo1 export interface class constructor
		/// </summary>
		/// <param name="tag_index">Tag index containing the tag being exported</param>
		/// <param name="tag_datum">DatumIndex of the tag being exported</param>
		public ColladaHalo1(TagIndex tag_index, DatumIndex tag_datum)
		{
			tagIndex = tag_index;
			tagManager = tag_index[tag_datum];

			GenerateInfoList();
		}
		#endregion

		#region Info List Generation
		/// <summary>
		/// Generic branch point for adding info classes
		/// </summary>
		protected override void GenerateInfoList()
		{
			// separate methods for model, bsp
			if (tagManager.GroupTag.Equals(Blam.Halo1.TagGroups.mod2))
				GenerateInfoListModel();
			else if (tagManager.GroupTag.Equals(Blam.Halo1.TagGroups.sbsp))
				GenerateInfoListBsp();
		}
		/// <summary>
		/// Creates info classes for a gbxmodel
		/// </summary>
		void GenerateInfoListModel()
		{
			int permutation_count = Model.GetPermutationCount(tagManager);

			for (int i = 0; i < permutation_count; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					ModelInfoInternal model_info_internal = new ModelInfoInternal();

					model_info_internal.Permutation = i;
					model_info_internal.IsMultiplePermutations = false;
					Model.AddGeometryInfos(model_info_internal, tagManager, i, j);
					Model.AddShaderDatums(model_info_internal, tagManager);

					internalInfoList.Add(model_info_internal);

					ColladaHalo1ModelInfo model_info = new ColladaHalo1ModelInfo(
						internalInfoList.Count - 1,
						Path.GetFileNameWithoutExtension(tagManager.Name),
						Model.GetVertexCount(model_info_internal, tagManager),
						Model.GetTriangleCount(model_info_internal, tagManager),
						i,
						j);

					Add(model_info);
				}
			}
		}
		/// <summary>
		/// Creates info classes for a structure BSP
		/// </summary>
		void GenerateInfoListBsp()
		{
			string bsp_name = Path.GetFileNameWithoutExtension(tagManager.Name);

			int vertex_count, triangle_count;

			BSPInfoInternal bsp_info_internal;

			// create an info object representing the render mesh
			vertex_count = BSP.GetVertexCount(tagManager, BSPObjectType.RenderMesh);
			triangle_count = BSP.GetTriangleCount(tagManager, BSPObjectType.RenderMesh);

			bsp_info_internal = new BSPInfoInternal();
			bsp_info_internal.SetType(BSPObjectType.RenderMesh);
			BSP.AddShaderDatums(bsp_info_internal, tagManager);
			internalInfoList.Add(bsp_info_internal);

			Add(new ColladaHalo1BSPInfo(internalInfoList.Count - 1,
				bsp_name,
				vertex_count,
				triangle_count,
				BSPObjectType.RenderMesh));

			// create an info object representing the portals mesh
			vertex_count = BSP.GetVertexCount(tagManager, BSPObjectType.Portals);
			triangle_count = BSP.GetTriangleCount(tagManager, BSPObjectType.Portals);

			if ((vertex_count > 0) && (triangle_count > 0))
			{
				bsp_info_internal = new BSPInfoInternal();
				bsp_info_internal.SetType(BSPObjectType.Portals);
				internalInfoList.Add(bsp_info_internal);

				Add(new ColladaHalo1BSPInfo(internalInfoList.Count - 1,
					bsp_name,
					vertex_count,
					triangle_count,
					BSPObjectType.Portals));
			}

			// create an info object representing the fogplane mesh
			vertex_count = BSP.GetVertexCount(tagManager, BSPObjectType.FogPlanes);
			triangle_count = BSP.GetTriangleCount(tagManager, BSPObjectType.FogPlanes);

			if ((vertex_count > 0) && (triangle_count > 0))
			{
				bsp_info_internal = new BSPInfoInternal();
				bsp_info_internal.SetType(BSPObjectType.FogPlanes);
				internalInfoList.Add(bsp_info_internal);

				Add(new ColladaHalo1BSPInfo(internalInfoList.Count - 1,
					bsp_name,
					vertex_count,
					triangle_count,
					BSPObjectType.FogPlanes));
			}
		}
		#endregion

		public override void Export(string file_name)
		{
			if (registeredInfos.Count == 0)
			{
				AddReport("COLLADAINTERFACE : invalid info count on export");
				return;
			}

			// if model create model exporter
			if (tagManager.GroupTag.Equals(Blam.Halo1.TagGroups.mod2))
				ExportModel(file_name);
			// otherwise bsp exporter
			else if (tagManager.GroupTag.Equals(Blam.Halo1.TagGroups.sbsp))
				ExportBSP(file_name);
		}

		#region Export Functions
		void ExportModel(string file_name)
		{
			ModelInfoInternal model_info = new ModelInfoInternal();

			List<int> added_permutations = new List<int>();

			// create an info object with all of the registered infos combined
			foreach (int index in registeredInfos)
			{
				ModelInfoInternal info = internalInfoList[index] as ModelInfoInternal;

				if (!added_permutations.Contains(info.Permutation))
					added_permutations.Add(info.Permutation);

				for (int i = 0; i < info.GetShaderCount(); i++)
					model_info.AddShaderDatum(info.GetShaderDatum(i), info.GetShaderName(i));
				for (int i = 0; i < info.GetGeometryCount(); i++)
					model_info.AddGeometry(info.GetGeometryName(i), info.GetGeometryIndex(i));
			}

			if (added_permutations.Count == 1)
				model_info.Permutation = added_permutations[0];
			else
				model_info.IsMultiplePermutations = true;

			ColladaExportArgs arguments = new ColladaExportArgs(Overwrite, RelativeFilePath, BitmapFormat);
			var exporter = new Halo1.ColladaModelExporter(arguments, model_info, tagIndex, tagManager);

			ExportSave(exporter, RelativeFilePath + file_name + ".dae");
		}

		void ExportBSP(string file_name)
		{
			BSPInfoInternal bsp_info = new BSPInfoInternal();

			BSPObjectType bsp_type = BSPObjectType.None;

			// create an info object with all of the registered infos combined
			foreach (int index in registeredInfos)
			{
				BSPInfoInternal info = internalInfoList[index] as BSPInfoInternal;

				if (info.IncludeRenderMesh()) { bsp_type |= BSPObjectType.RenderMesh; }
				if (info.IncludePortalsMesh()) { bsp_type |= BSPObjectType.Portals; }
				if (info.IncludeFogPlanesMesh()) { bsp_type |= BSPObjectType.FogPlanes; }

				for (int i = 0; i < info.GetShaderCount(); i++)
					bsp_info.AddShaderDatum(info.GetShaderDatum(i), info.GetShaderName(i));
			}

			bsp_info.SetType(bsp_type);

			ColladaExportArgs arguments = new ColladaExportArgs(Overwrite, RelativeFilePath, BitmapFormat);
			var exporter = new Halo1.ColladaBSPExporter(arguments, bsp_info, tagIndex, tagManager);

			ExportSave(exporter, RelativeFilePath + file_name + ".dae");
		}
		#endregion
	};
}
