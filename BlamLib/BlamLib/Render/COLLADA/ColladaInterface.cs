/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	[Flags]
	public enum BSPObjectType
	{
		None,

		RenderMesh = 1<<0,
		Portals = 1<<1,
		FogPlanes = 1<<2,
	};

	public interface IHaloShaderDatumList
	{
		int GetShaderCount();
		Blam.DatumIndex GetShaderDatum(int index);
		string GetShaderName(int index);
	};

	/// <summary>
	/// Base external info class containing a name property and a index reference to the internal info list
	/// </summary>
	public abstract class ColladaInfo
	{
		public string Name { get; private set; }
		public int InternalIndex { get; private set; }

		protected ColladaInfo(int index, string info_name)
		{
			InternalIndex = index;
			Name = info_name;
		}
	};

	public abstract class ColladaHaloModelInfoBase : ColladaInfo
	{
		public int VertexCount { get; private set; }
		public int FaceCount { get; private set; }

		protected ColladaHaloModelInfoBase(int internal_index, string name, 
			int vertex_count, int face_count)
			: base(internal_index, name)
		{
			VertexCount = vertex_count;
			FaceCount = face_count;
		}
	};

	abstract class ColladaInterface : List<ColladaInfo>
	{
		public const string kDefaultBitmapFormat = "tif";

		#region Internal Classes
		/// <summary>
		/// Empty abstract class so that the internal info list can be in the ColladaInterface class
		/// </summary>
		protected abstract class ColladaInfoInternal
		{
		}
		#endregion

		#region Class Members
		public bool Overwrite = false;
		public string BitmapFormat = kDefaultBitmapFormat;
		public string RelativeFilePath = "";

		protected List<int> registeredInfos = new List<int>();
		protected List<ColladaInfoInternal> internalInfoList = new List<ColladaInfoInternal>();
		#endregion

		#region Error Reporting
		List<string> colladaReports;
		/// <summary>
		/// Add a string that contains information about an event, to the report list
		/// </summary>
		/// <param name="report">The string to add to the report array</param>
		public void AddReport(string report)
		{
			// if the array is null, create it
			if (colladaReports == null)
				colladaReports = new List<string>();

			// add the string to the array
			colladaReports.Add(report);
		}
		/// <summary>
		/// Enumerates the COLLADA reports, then clears the report list when finished
		/// </summary>
		/// <returns></returns>
		public IEnumerable<string> Reports()
		{
			if (colladaReports != null)
			{
				foreach (string r in colladaReports)
					yield return r;

				colladaReports.Clear();
			}
		}
		/// <summary>
		/// Collada error event function
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		protected void ExporterErrorOccured(object sender, ColladaErrorEventArgs e)
		{
			AddReport(e.ErrorMessage);
		}
		#endregion

		#region Export Registration
		/// <summary>
		/// Registers an info element for export
		/// </summary>
		/// <param name="info">An info object representing a object to export</param>
		public void RegisterForExport(ColladaInfo info)
		{
			if (!registeredInfos.Contains(info.InternalIndex))
				registeredInfos.Add(info.InternalIndex);
		}
		/// <summary>
		/// Removes all registered export objects from the export list
		/// </summary>
		public void ClearRegister()
		{
			registeredInfos.Clear();
		}
		#endregion

		protected abstract void GenerateInfoList();

		protected void ExportSave(ColladaExporter exporter, string file_name)
		{
			exporter.ErrorOccured += new EventHandler<ColladaErrorEventArgs>(ExporterErrorOccured);
			bool success = exporter.BuildColladaInstance();
			exporter.ErrorOccured -= new EventHandler<ColladaErrorEventArgs>(ExporterErrorOccured);

			if (success)
				exporter.SaveDAE(file_name);
		}

		public abstract void Export(string file_name);
	};
}