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

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaHalo2LightmapInfo : ColladaHaloModelInfoBase
	{
		public ColladaHalo2LightmapInfo(int internal_index, string name,
			int vertex_count, int face_count)
			: base(internal_index, name, vertex_count, face_count)
		{
		}
	};

	class ColladaHalo2 : ColladaInterface
	{
		#region Internal Classes
		/// <summary>
		/// Interface class to pass mesh include information to the Halo1 BSP exporter
		/// </summary>
		protected class LightmapInfoInternal : ColladaInfoInternal, IHalo2LightmapInterface, IHaloShaderDatumList
		{
			#region IHalo2ShaderDatumList Members
			public int GetShaderCount()
			{
				throw new NotImplementedException();
			}

			public DatumIndex GetShaderDatum(int index)
			{
				throw new NotImplementedException();
			}
			#endregion
		}
		#endregion

		#region Static Helper Classes
		protected static class BSP
		{
			public static int GetVertexCount(TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo2.Tags.scenario_structure_lightmap_group;

				int count = 0;
				foreach (var group in definition.LightmapGroups)
				{
					foreach (var cluster in group.Clusters)
						count += cluster.GeometryInfo.Value.TotalVertexCount.Value;
				}
				return count;
			}
			public static int GetTriangleCount(TagManager manager)
			{
				var definition = manager.TagDefinition as Blam.Halo2.Tags.scenario_structure_lightmap_group;

				int count = 0;
				foreach (var group in definition.LightmapGroups)
				{
					foreach (var cluster in group.Clusters)
						count += cluster.GeometryInfo.Value.TotalTriangleCount.Value;
				}
				return count;
			}
		};
		#endregion

		#region Class Members
		TagIndexBase tagIndex;
		TagManager tagManager;
		#endregion

		#region Constructor
		public ColladaHalo2(TagIndexBase tag_index, DatumIndex tag_datum)
		{
			tagIndex = tag_index;
			tagManager = tagIndex[tag_datum];

			GenerateInfoList();
		}
		#endregion

		#region Info List Generation
		protected override void GenerateInfoList()
		{
			if (tagManager.GroupTag.Equals(Blam.Halo2.TagGroups.ltmp))
				GenerateInfoListLightmap();
		}
		void GenerateInfoListLightmap()
		{
			string name = Path.GetFileNameWithoutExtension(tagManager.Name);

			int vertex_count, triangle_count;

			vertex_count = BSP.GetVertexCount(tagManager);
			triangle_count = BSP.GetTriangleCount(tagManager);

			if ((vertex_count > 0) && (triangle_count > 0))
			{
				LightmapInfoInternal lightmap_info_internal = new LightmapInfoInternal();
				internalInfoList.Add(lightmap_info_internal);

				Add(new ColladaHalo2LightmapInfo(internalInfoList.Count - 1,
					name,
					vertex_count,
					triangle_count));
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

			if (tagManager.GroupTag.Equals(Blam.Halo2.TagGroups.ltmp))
			{
				LightmapInfoInternal info = new LightmapInfoInternal();

				var exporter = new Halo2.ColladaLightmapExporter(info, tagIndex, tagManager);

				exporter.ErrorOccured += new EventHandler<ColladaExporter.ColladaErrorEventArgs>(ExporterErrorOccured);

				exporter.Overwrite = Overwrite;
				exporter.RelativeDataPath = RelativeFilePath;
				exporter.BitmapFormat = BitmapFormat;

				exporter.BuildColladaInstance();
				exporter.SaveDAE(RelativeFilePath + file_name + ".dae");

				exporter.ErrorOccured -= new EventHandler<ColladaExporter.ColladaErrorEventArgs>(ExporterErrorOccured);
			}
		}

		void ExporterErrorOccured(object sender, ColladaExporter.ColladaErrorEventArgs e)
		{
			AddReport(e.ErrorMessage);
		}
	};
}