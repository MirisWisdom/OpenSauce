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
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Blam;

namespace BlamLib.Render.COLLADA
{
	/// <summary>
	/// Base class for exporting a COLLADA file
	/// </summary>
	public abstract class ColladaExporter
	{
		#region Settings
		protected bool bOverwrite = false;
		protected string relativeDataPath = "";
		protected string bitmapFormatString = "";

		/// <summary>
		/// When true, existing files are overwritten when saving
		/// </summary>
		public bool Overwrite
		{
			set { bOverwrite = value; }
		}
		/// <summary>
		/// Set this to the absoluts path that relative data references should start at
		/// </summary>
		public string RelativeDataPath
		{
			set { relativeDataPath = value; }
		}
		/// <summary>
		/// Set this to the bitmap extension the materials should use (tga, jpg, tif, etc)
		/// </summary>
		public string BitmapFormat
		{
			set { bitmapFormatString = value; }
		}
		#endregion

		#region Events
		public class ColladaErrorEventArgs : EventArgs
		{
			private string errorMessage;

			public string ErrorMessage
			{
				get { return errorMessage; }
			}

			private ColladaErrorEventArgs() { }
			public ColladaErrorEventArgs(string message)
			{
				errorMessage = message;
			}
		}

		/// <summary>
		/// This event is fired when an error has occured, with an error string describing the error in detail.
		/// </summary>
		public event EventHandler<ColladaErrorEventArgs> ErrorOccured;
		
		protected void OnErrorOccured(string message)
		{
			if (ErrorOccured != null)
				ErrorOccured(this, new ColladaErrorEventArgs(message));
		}
		#endregion

		#region Class Members
		protected ColladaFile COLLADAFile = new ColladaFile();
		#endregion

		#region Element Creation
		/// <summary>
		/// Adds an asset element to the current collada file
		/// </summary>
		/// <param name="author">String containing the authors name</param>
		/// <param name="authoring_tool">String containing the name of the program that created the file</param>
		/// <param name="unit_name">The name of the units the scene is scaled against</param>
		/// <param name="unit_meter_scale">The ratio of the units in relation to metres</param>
		/// <param name="up_axis">The axis that is considered to be "up"</param>
		public void AddAsset(string author,
			string authoring_tool,
			string unit_name,
			double unit_meter_scale,
			Enums.ColladaUpAxisEnum up_axis)
		{
			COLLADAFile.Asset = new COLLADA.Core.ColladaAsset();
			COLLADAFile.Asset.Contributor = new List<COLLADA.Core.ColladaContributor>();
			COLLADAFile.Asset.Contributor.Add(new COLLADA.Core.ColladaContributor());
			COLLADAFile.Asset.Contributor[0].Author = author;
			COLLADAFile.Asset.Contributor[0].AuthoringTool = authoring_tool;

			COLLADAFile.Asset.Created = DateTime.Now;
			COLLADAFile.Asset.Modified = DateTime.Now;
			COLLADAFile.Asset.Unit = new Core.ColladaAssetUnit();
			COLLADAFile.Asset.Unit.Meter = unit_meter_scale;
			COLLADAFile.Asset.Unit.Name = unit_name;
			COLLADAFile.Asset.UpAxis = up_axis;
		}
		/// <summary>
		/// Creates a generic source element for geometric data
		/// </summary>
		/// <param name="semantic">The use for the data in the source element</param>
		/// <param name="geometry_id">Name of the geometry the element contains data for</param>
		/// <param name="set">Data set that the sources data is intended for</param>
		/// <returns>A ColladaSource element</returns>
		protected Core.ColladaSource CreateSource(Enums.ColladaInputSharedSemantic semantic,
			string geometry_id,
			uint set)
		{
			Core.ColladaSource source = new Core.ColladaSource();

			source.FloatArray = new Core.ColladaFloatArray();
			source.TechniqueCommon = new Core.ColladaTechniqueCommon();
			source.TechniqueCommon.Accessor = new Core.ColladaAccessor();
			source.TechniqueCommon.Accessor.Param = new List<Core.ColladaParam>();

			string source_id = "";
			switch (semantic)
			{
				case Enums.ColladaInputSharedSemantic.POSITION:
					source_id = String.Concat(geometry_id, "-positions");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.NORMAL:
					source_id = String.Concat(geometry_id, "-normals");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.BINORMAL:
					source_id = String.Concat(geometry_id, "-binormals");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.TANGENT:
					source_id = String.Concat(geometry_id, "-tangents");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.TEXCOORD:
					source_id = String.Format("{0}-texcoord{1}", geometry_id, set);
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "S";
					source.TechniqueCommon.Accessor.Param[1].Name = "T";
					break;
				default:
					throw new ColladaException(String.Format(
						ColladaExceptionStrings.ImplimentationBug,
						"invalid semantic enum passed to CreateSource function"));
			}
			source.ID = source_id;
			source.FloatArray.ID = String.Concat(source.ID, "-array");
			source.TechniqueCommon.Accessor.Source = ColladaUtilities.BuildUri(source.FloatArray.ID);

			foreach (Core.ColladaParam param in source.TechniqueCommon.Accessor.Param)
				param.Type = "float";

			return source;
		}
		/// <summary>
		/// Creates a vertices element that defines which source element contains the position data
		/// </summary>
		/// <param name="geometry_id">The id of the geometry the element is associated with</param>
		/// <param name="source_id">The id of the source element that contains the position data</param>
		/// <returns>A ColladaVertices element</returns>
		protected Core.ColladaVertices CreateVertices(string geometry_id, string source_id)
		{
			Core.ColladaVertices vertices = new Core.ColladaVertices();
			vertices.ID = String.Concat(geometry_id, "-vertices");
			vertices.Input = new List<Core.ColladaInputUnshared>();
			vertices.Input.Add(new Core.ColladaInputUnshared());
			vertices.Input[0].Semantic = Enums.ColladaInputSharedSemantic.POSITION;
			vertices.Input[0].Source = ColladaUtilities.BuildUri(source_id);

			return vertices;
		}
		/// <summary>
		/// Creates a polygon index list for a set of planar, convex vertices
		/// </summary>
		/// <param name="vertex_count">Number of vertices in the plane</param>
		/// <returns></returns>
		protected Core.ColladaTriangles BuildFaceIndices(int vertex_count)
		{
			// create an array of vertex indices
			List<ushort> index_array = new List<ushort>();
			for (ushort j = 0; j < vertex_count; j++) { index_array.Add(j); }

			// in a triangle list without degenerates face count = vertex count - 2
			int vertex_index = 0;
			int surface_count = vertex_count - 2;

			// the triangle data is calculated by assuming the surface is coplanar and convex,
			// which means we can make the triangles by using 3 consecutive indices, removing the
			// middle index and repeating until only 3 indices are left, which makes the final triangle

			COLLADA.Core.ColladaTriangles triangles = new COLLADA.Core.ColladaTriangles();
			triangles.P = new COLLADA.ColladaValueArray<int>();
			for (int i = 0; i < surface_count; i++)
			{
				// create a triangle
				triangles.P.Add(
					index_array[vertex_index + 2],
					index_array[vertex_index + 1],
					index_array[vertex_index]);

				index_array.RemoveAt(vertex_index + 1);
				vertex_index++;

				if (vertex_index + 2 >= index_array.Count)
					vertex_index = 0;
			}
			return triangles;
		}
		#endregion

		#region Scene Creation
		/// <summary>
		/// Adds a scene to the collada file that instances a visual scene with the specified ID
		/// </summary>
		protected void AddScene(string scene_id)
		{
			COLLADAFile.Scene = new Core.ColladaScene();
			COLLADAFile.Scene.InstanceVisualScene = new Core.ColladaInstanceVisualScene();
			COLLADAFile.Scene.InstanceVisualScene.URL = ColladaUtilities.BuildUri(scene_id);
		}
		#endregion

		/// <summary>
		/// Saves the Collada file in its current state to file.
		/// </summary>
		/// <param name="location">Location to save the Collada file to</param>
		public void SaveDAE(string location)
		{
			// check whether the COLLADA file is valid
			try
			{
				COLLADAFile.Validate();
			}
			catch (Exception exception)
			{
				// if the collada file is not valid, add a report detailing why it is not valid
				OnErrorOccured(ColladaExceptionStrings.ValidationFailed);

				// report an error for all inner exceptions
				Exception exception_temp = exception;
				while (exception_temp != null)
				{
					OnErrorOccured(exception_temp.Message);

					// if the exception was a vaildation exception, report the element details
					ColladaValidationException validation_exception = (exception_temp as ColladaValidationException);
					if ((validation_exception != null) && (validation_exception.ElementDetails != null))
					{
						foreach (string detail in validation_exception.ElementDetails)
							OnErrorOccured(String.Format("COLLADA_DETAIL: {0}", detail));
					}
					exception_temp = exception_temp.InnerException;
				}
				return;
			}

			// if the file exists but overwritting is disabled, report this and return
			if (System.IO.File.Exists(location) && !bOverwrite)
			{
				OnErrorOccured(String.Format(ColladaExceptionStrings.FileExists, location));
				return;
			}

			// serialize the COLLADA file to the xml file
			System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(location));

			XmlSerializer serializer = new XmlSerializer(typeof(ColladaFile));

			XmlTextWriter writer = new XmlTextWriter(location, null);
			writer.Formatting = Formatting.Indented;
			serializer.Serialize(writer, COLLADAFile);
			writer.Close();
		}
		/// <summary>
		/// Performs the actual collada file population. Override this in derived classes to define how this is done.
		/// </summary>
		/// <returns>True if no errors occured</returns>
		protected virtual bool BuildColladaInstanceImpl() { return true; }
		/// <summary>
		/// Populates the collada file object with the current data set
		/// </summary>
		/// <returns>True if no errors occured</returns>
		public bool BuildColladaInstance()
		{
			// make sure the data path ends with a '\'
			if (!relativeDataPath.EndsWith("\\"))
				relativeDataPath += "\\";

			try
			{
				return BuildColladaInstanceImpl();
			}
			catch (Exception e)
			{
				// if an exception occured, report it and return gracefully
				OnErrorOccured(e.Message);
				OnErrorOccured(e.StackTrace);

				Exception exception = e.InnerException;
				while (exception != null)
				{
					OnErrorOccured(exception.Message);
					exception = exception.InnerException;
				}
			}
			return false;
		}
	}
}