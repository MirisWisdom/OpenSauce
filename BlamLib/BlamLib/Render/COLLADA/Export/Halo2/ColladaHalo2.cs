using System;
using System.Collections.Generic;
using System.IO;
using BlamLib.Blam;
using BlamLib.Managers;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaHalo2LightmapInfo : ColladaInfo
	{
		private int vertexCount;
		private int faceCount;

		public int VertexCount
		{
			get { return vertexCount; }
		}
		public int FaceCount
		{
			get { return faceCount; }
		}

		private ColladaHalo2LightmapInfo() { }
		public ColladaHalo2LightmapInfo(int internal_index,
			string name,
			int vertex_count,
			int face_count)
			: base(internal_index, name)
		{
			vertexCount = vertex_count;
			faceCount = face_count;
		}
	}

	class ColladaHalo2 : ColladaInterface
	{
		#region Internal Classes
		/// <summary>
		/// Interface class to pass mesh include information to the Halo1 BSP exporter
		/// </summary>
		protected class LightmapInfoInternal : ColladaInfoInternal, IHalo2LightmapInterface, IHalo2ShaderDatumList
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
		static protected class BSP
		{
			static public int GetVertexCount(TagManager manager)
			{
				Blam.Halo2.Tags.scenario_structure_lightmap_group definition = manager.TagDefinition as Blam.Halo2.Tags.scenario_structure_lightmap_group;

				int count = 0;
				foreach (var group in definition.LightmapGroups)
				{
					foreach (var cluster in group.Clusters)
						count += cluster.GeometryInfo.Value.TotalVertexCount.Value;
				}
				return count;
			}
			static public int GetTriangleCount(TagManager manager)
			{
				Blam.Halo2.Tags.scenario_structure_lightmap_group definition = manager.TagDefinition as Blam.Halo2.Tags.scenario_structure_lightmap_group;

				int count = 0;
				foreach (var group in definition.LightmapGroups)
				{
					foreach (var cluster in group.Clusters)
						count += cluster.GeometryInfo.Value.TotalTriangleCount.Value;
				}
				return count;
			}
		}
		#endregion

		#region Class Members
		private TagIndexBase tagIndex;
		private TagManager tagManager;
		#endregion

		#region Constructor
		private ColladaHalo2() { }
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
		private void GenerateInfoListLightmap()
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

				COLLADA.Halo2.ColladaLightmapExporter exporter = new Halo2.ColladaLightmapExporter(info, tagIndex, tagManager);

				exporter.ErrorOccured += new EventHandler<ColladaExporter.ColladaErrorEventArgs>(ExporterErrorOccured);

				exporter.Overwrite = Overwrite;
				exporter.RelativeDataPath = RelativeFilePath;
				exporter.BitmapFormat = BitmapFormat;

				exporter.BuildColladaInstance();
				exporter.SaveDAE(RelativeFilePath + file_name + ".dae");

				exporter.ErrorOccured -= new EventHandler<ColladaExporter.ColladaErrorEventArgs>(ExporterErrorOccured);
			}
		}

		private void ExporterErrorOccured(object sender, ColladaExporter.ColladaErrorEventArgs e)
		{
			AddReport(e.ErrorMessage);
		}
	}
}
