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

namespace BlamLib.Render.COLLADA
{
	/// <summary>
	/// Base external info class containing a name property and a index reference to the internal info list
	/// </summary>
	public class ColladaInfo
	{
		protected string name;
		protected int internalIndex;

		public string Name
		{
			get { return name; }
		}
		public int InternalIndex
		{
			get { return internalIndex; }
		}

		protected ColladaInfo() { }
		public ColladaInfo(int index, string info_name)
		{
			internalIndex = index;
			name = info_name;
		}
	}
	abstract class ColladaInterface : List<ColladaInfo>
	{
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
		public string BitmapFormat = "tif";
		public string RelativeFilePath = "";

		protected List<int> registeredInfos = new List<int>();
		protected List<ColladaInfoInternal> internalInfoList = new List<ColladaInfoInternal>();
		#endregion

		#region Error Reporting
		private List<string> colladaReports;
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
		/// Gets the last report in the list then removes it
		/// </summary>
		/// <returns>The last report in the list</returns>
		public string GetReport()
		{
			// if the array is null or it has no elements, return null
			if ((colladaReports == null) || (colladaReports.Count == 0))
			{
				colladaReports = null;
				return null;
			}

			// copy the string to a local variable, then remove it from the array
			string return_string = colladaReports[0];
			colladaReports.RemoveAt(0);

			// return the rport string
			return return_string;
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

		public abstract void Export(string file_name);
	};
}
