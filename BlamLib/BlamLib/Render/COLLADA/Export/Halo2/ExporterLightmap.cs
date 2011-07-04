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

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaLightmapExporter : ColladaModelExporterHalo2
	{
		#region Class Fields
		IHalo2LightmapInterface lightmapInfo;

		List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		/// <summary>
		/// Halo2 Lightmap exporter class
		/// </summary>
		/// <param name="lightmap_info">An object implementing IHalo2LightmapInterface to define what meshes are to be included in the collada file</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaLightmapExporter(IHalo2LightmapInterface lightmap_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(lightmap_info, tag_index, tag_manager)
		{
			lightmapInfo = lightmap_info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates geometries for the relevant lightmap meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			H2.Tags.scenario_structure_lightmap_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_lightmap_group;

			for (int i = 0; i < definition.LightmapGroups.Count; i++)
			{
				for (int j = 0; j < definition.LightmapGroups[i].Clusters.Count; j++)
				{
					//TODO: figure out lightmap materials
					string name = String.Format("{0}-group{1}-cluster{2}", ColladaUtilities.FormatName(tagName, " ", "_"), i, j);
					listGeometry.Add(CreateGeometry(name,
						definition.LightmapGroups[i].Clusters[j].GeometryInfo,
						definition.LightmapGroups[i].Clusters[j].CacheData[0].Geometry.Value,
						new List<string>()));
				}
			}
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates a collada node for a single render geometry
		/// </summary>
		/// <param name="index">The render geometry index to create a node for</param>
		/// <returns></returns>
		Core.ColladaNode CreateNodeRender(int index)
		{
			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = listGeometry[index].Name;
			model_node.Name = ColladaUtilities.FormatName(model_node.Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceGeometry instance_geometry = new Core.ColladaInstanceGeometry();
			instance_geometry.URL = ColladaUtilities.BuildUri(listGeometry[index].ID);
			model_node.Add(instance_geometry);

			return model_node;
		}
		/// <summary>
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			for(int i = 0; i < listGeometry.Count; i++)
				listNode.Add(CreateNodeRender(i));
		}
		#endregion
		#endregion

		#region Library Creation
		/// <summary>
		/// Creates the library_geometries element in the collada file
		/// </summary>
		void AddLibraryGeometries()
		{
			COLLADAFile.LibraryGeometries = new Core.ColladaLibraryGeometries();
			COLLADAFile.LibraryGeometries.Geometry = new List<Core.ColladaGeometry>();
			COLLADAFile.LibraryGeometries.Geometry.AddRange(listGeometry);
		}
		/// <summary>
		/// Creates the library_visual_scenes element in the collada file. The node list is added under a node named "frame" since that is
		/// required when creating new BSPs.
		/// </summary>
		void AddLibraryVisualScenes()
		{
			// add the main scene node
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = new List<Core.ColladaNode>();

			Core.ColladaNode frame = new BlamLib.Render.COLLADA.Core.ColladaNode();
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

			CreateGeometryList();
			CreateNodeList();

			AddLibraryGeometries();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	};
}