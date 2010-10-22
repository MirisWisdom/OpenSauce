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
using REF = System.Reflection;
using System.Text;

namespace BlamLib.IO
{
	public static class Assembly
	{
		const string BasePath = "BlamLib.Games.";

		static REF.Assembly ThisAssembly;

		static Assembly()
		{
			ThisAssembly = REF.Assembly.GetExecutingAssembly();
		}

		public static class File
		{
			public static bool Exists(string path, string name)
			{
				try { using (Stream s = Open(path, name)) { } }
				catch (FileNotFoundException) { return false; }

				return true;
			}

			public static bool Exists(REF.Assembly ass, string path, string name)
			{
				try { using(Stream s = Open(ass, path, name)) {} }
				catch (FileNotFoundException) { return false; }

				return true;
			}

			/// <summary>
			/// Open a manifest file from this assembly
			/// </summary>
			/// <param name="path">Type path to the file</param>
			/// <param name="name">File name (with extension)</param>
			/// <returns>Manifest file's stream</returns>
			/// <exception cref="Debug.ExceptionLog">When a <see cref="System.IO.FileNotFoundException"/> is encountered, it is caught and rethrown as a this type of exception</exception>
			public static Stream Open(string path, string name)
			{
				string manifest_path = string.Format("{0}{1}{2}", BasePath, path.Replace('\\', '.'), name);
				Stream s = null;
				try { s = ThisAssembly.GetManifestResourceStream(manifest_path); }
				catch (FileNotFoundException) { throw new Debug.ExceptionLog("Manifest not found! {0}", manifest_path); }
				return s;
			}

			/// <summary>
			/// Open a manifest file from <paramref name="ass"/>
			/// </summary>
			/// <param name="ass">Assembly to get the manifest file from</param>
			/// <param name="path">Type path to the file</param>
			/// <param name="name">File name (with extension)</param>
			/// <returns>Manifest file's stream</returns>
			/// <exception cref="Debug.ExceptionLog">When a <see cref="System.IO.FileNotFoundException"/> is encountered, it is caught and rethrown as a this type of exception</exception>
			public static Stream Open(REF.Assembly ass, string path, string name)
			{
				string manifest_path = string.Format("{0}{1}{2}", BasePath, path.Replace('\\', '.'), name);
				Stream s = null;
				try { s = ass.GetManifestResourceStream(manifest_path); }
				catch (FileNotFoundException) { throw new Debug.ExceptionLog("Manifest not found! {0}+{1}", ass.FullName, manifest_path); }
				return s;
			}
		};
	};
}