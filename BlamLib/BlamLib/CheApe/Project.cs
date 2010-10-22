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
using System.Text;

namespace BlamLib.CheApe
{
	/// <summary>
	/// CheApe project file interface
	/// </summary>
	[IO.Class((int)IO.TagGroups.Enumerated.CheApeProject, 1)]
	public sealed class Project : IO.FileManageable, IDisposable
	{
		#region Interface
		/// <summary>
		/// Game specific implementation interface for CheApe projects
		/// </summary>
		public interface Interface
		{
			/// <summary>
			/// Create a new project file and object
			/// </summary>
			/// <param name="file">Path to the project file</param>
			/// <returns>Read to use Project object</returns>
			Project Create(string file);
			/// <summary>
			/// Open an existing project file
			/// </summary>
			/// <param name="file">Path to the project file</param>
			/// <returns>Project using data loaded from <paramref name="file"/></returns>
			Project Open(string file);
			/// <summary>
			/// Save an existing project
			/// </summary>
			/// <param name="save_as">If not null, then it saves the project to this file and updates the project settings</param>
			/// <param name="project">Project object in question</param>
			void Save(string save_as, Project project);
			/// <summary>
			/// Close an existing project
			/// </summary>
			/// <param name="save">Should we save the project data before closing?</param>
			/// <param name="project">Project object in question</param>
			void Close(bool save, Project project);
		};

		public static Interface GetInterface(BlamVersion v)
		{
			if ((v & BlamVersion.Halo1) != 0) return new Blam.Halo1.CheApe.Project.Interface();
#if !NO_HALO2
			else if ((v & BlamVersion.Halo2) != 0) return new Blam.Halo2.CheApe.Project.Interface();
#endif

			throw new Debug.Exceptions.UnreachableException(v);
		}
		#endregion

		internal ProjectState OwnerState = null;

		internal Project(BlamVersion v, string file_name)
		{
			engine = v;
			fileName = file_name;
			folder = System.IO.Path.GetDirectoryName(file_name) + "\\";
		}

		/// <summary>
		/// Call after Project object is no longer in use
		/// </summary>
		public void Dispose()
		{
			OwnerState.Dispose();
		}

		#region Engine
		BlamVersion engine;
		/// <summary>
		/// 
		/// </summary>
		public BlamVersion Engine	{ get { return engine; } }
		#endregion

		#region FileName
		string fileName = string.Empty;
		/// <summary>
		/// The absolute file path of this project
		/// </summary>
		public string FileName
		{
			get { return fileName; }
			internal set { fileName = value; }
		}
		#endregion

		#region Folder
		string folder = string.Empty;
		/// <summary>
		/// The absolute path to the folder the project file is located in
		/// </summary>
		public string Folder { get { return folder; } }
		#endregion

		#region Files
		List<string> files = new List<string>();
		/// <summary>
		/// Files referenced by this project
		/// </summary>
		public List<string> Files	{ get { return files; } }
		#endregion

		#region Manifest files
		/// <summary>
		/// All embedded files should be added at runtime, we should NEVER 
		/// have to stream these to the project file
		/// </summary>
		List<string> embeddedFiles = new List<string>();

		/// <summary>
		/// Add a manifest file
		/// </summary>
		/// <param name="path">Type name path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public void AddEmbedded(string path, string name)
		{
			embeddedFiles.Add(System.IO.Path.Combine(path, name));
		}

		/// <summary>
		/// Remove an existing manifest file
		/// </summary>
		/// <param name="path">Type name path to the file</param>
		/// <param name="name">File name (with extension)</param>
		public void RemoveEmbedded(string path, string name)
		{
			embeddedFiles.Remove(System.IO.Path.Combine(path, name));
		}
		#endregion

		/// <summary>
		/// Determines if <paramref name="path"/> is capable of being added to the project
		/// </summary>
		/// <param name="path">Path to the file in question</param>
		/// <param name="reason">If this is not a valid file this holds the reason, otherwise <c>null</c></param>
		/// <returns>True if a valid file, false otherwise</returns>
		public bool ValidFile(string path, out string reason)
		{
			reason = null;

			if(!path.Contains(folder))
			{
				reason = string.Format("'{0}' isn't in this project's immediate directory.", path);
			}
			else if (files.Contains(System.IO.Path.GetFileName(path)))
				reason = string.Format("'{0}' already exists in the project.", path);
			else if (embeddedFiles.Contains(System.IO.Path.GetFileName(path)))
				reason = string.Format("'{0}' already exists in the project (Embedded).", path);
			else
				return true;

			return false;
		}

		/// <summary>
		/// Add the file <paramref name="path"/> to the project
		/// </summary>
		/// <param name="path">Name of the file we want to add to the project</param>
		/// <param name="create">Should we create the file on the disk first?</param>
		/// <returns>Name of the file relative to the actual project file</returns>
		public string Add(string path, bool create)
		{
			if(!path.Contains(folder))
				throw new Debug.ExceptionLog("Files for this project must be in it's immediate directory: '{0}'", folder);

			if(create)
			{
				if (!System.IO.Directory.Exists(System.IO.Path.GetDirectoryName(path)))
					System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(path));
				if (!System.IO.File.Exists(path))
					System.IO.File.Create(path).Close();
				CheApe.Import.WriteBlankDocument(path, OwnerState.Definition.Engine);
			}

			files.Add(System.IO.Path.GetFileName(path));
			
			return files[files.Count-1];
		}

		/// <summary>
		/// Remove the file named <paramref name="path"/> from the project
		/// </summary>
		/// <param name="path">Name of the file we want to remove from the project</param>
		/// <param name="delete">Should we delete the file from disk too?</param>
		/// <remarks>Does nothing if <paramref name="path"/> doesn't exist</remarks>
		public void Remove(string path, bool delete)
		{
			bool result = files.Remove(path);

			if (result && delete)
			{
				path = System.IO.Path.Combine(folder, path);
				if(System.IO.File.Exists(path))
					System.IO.File.Delete(path);
			}
		}

		/// <summary>
		/// Remove the file located at <paramref name="path_index"/> in <see cref="Project.Files"/>
		/// </summary>
		/// <param name="path_index">Index of the path we want to remove from the project</param>
		/// <param name="delete">Should we delete the file from disk too?</param>
		/// <remarks>DEBUG ONLY: Exceptions if <paramref name="path_index"/> is out of bounds</remarks>
		public void Remove(int path_index, bool delete)
		{
			Debug.Assert.If(path_index >= 0 && path_index < files.Count, "Path index was out of range: [{0} !{1}]", files.Count, path_index);

			string path = System.IO.Path.Combine(folder, files[path_index]);
			files.RemoveAt(path_index);

			if (delete) System.IO.File.Delete(path);
		}


		bool imported = false; // have we even ran one import yet?

		/// <summary>
		/// Import the project files
		/// </summary>
		public void Import()
		{
			const string k_error = "A parsing error (in {0}) has been encountered, please check the debug.log file.{1}";

			OwnerState.Importer.Reset();
			bool is_embedded = true;
			string dbg_name = "BORK BORK BORK!";

			try
			{
				foreach (string f in embeddedFiles)
				{
					string path = System.IO.Path.GetDirectoryName(f) + "\\";
					string name = System.IO.Path.GetFileName(f);

					OwnerState.Importer.ProcessFile(OwnerState, path, dbg_name = name);
				}

				is_embedded = false;
				foreach (string path in files)
					OwnerState.Importer.ProcessFile(OwnerState, folder + (dbg_name = path));

			}catch(Debug.ExceptionLog)
			{
				System.Windows.Forms.MessageBox.Show(string.Format(k_error,
					"CheApe", is_embedded ? " (Embedded)" : ""), dbg_name);
				return;
			}catch(System.Exception)
			{
				System.Windows.Forms.MessageBox.Show(string.Format(k_error,
					".NET", is_embedded ? " (Embedded)" : ""), dbg_name);
				return;
			}

			imported = true;
		}

		/// <summary>
		/// Build the whole project
		/// </summary>
		/// <param name="reimport">Should we re-import the project files first?</param>
		public void Build(bool reimport)
		{
			if (imported && reimport)
				OwnerState.Compiler.Reset();

			if(!imported || reimport)				
				Import();

			try { OwnerState.Compiler.Write(folder, fileName); }
			catch (Debug.ExceptionLog)
			{
				System.Windows.Forms.MessageBox.Show("A compiler error has been encountered, please check the debug.log file", fileName);
				return;
			}

			System.Windows.Forms.MessageBox.Show("Build Finished!");
		}

		/// <summary>
		/// Build the whole project
		/// </summary>
		/// <remarks>Re-imports all project files</remarks>
		public void Build() { Build(true); }


		#region IStreamable Members
		public override void Read(IO.EndianReader s)
		{
			BlamVersion ver = (BlamVersion)s.ReadUInt16();
			// HACK: this is a hack
			if (ver == BlamVersion.Halo1)
				ver = BlamVersion.Halo1_CE;
			Debug.Assert.If(ver == engine, "Engine version mismatch: expected {0}, but got {1}", engine, ver);
			s.Seek(2, System.IO.SeekOrigin.Current);
			#region FileNames
			int file_count = s.ReadInt32();
			for (int x = 0; x < file_count; x++) files.Add(s.ReadCString());
			#endregion
		}

		public override void Write(IO.EndianWriter s)
		{
			s.Write((ushort)engine);
			s.Write(ushort.MinValue);
			#region FileNames
			s.Write(files.Count);
			foreach (string str in files) s.Write(str, true);
			#endregion
		}
		#endregion
	};

	/// <summary>
	/// Game specific implementation base class for CheApe project states
	/// </summary>
	internal abstract class ProjectState : IDisposable
	{
		#region Engine
		BlamVersion engine;
		/// <summary>
		/// 
		/// </summary>
		public BlamVersion Engine	{ get { return engine; } }
		#endregion

		#region Definition
		protected XmlInterface definition;
		/// <summary>
		/// CheApe definition data
		/// </summary>
		public XmlInterface Definition	{ get { return definition; } }
		#endregion

		#region Project
		protected Project project;
		/// <summary>
		/// CheApe project
		/// </summary>
		public Project Project			{ get { return project; } }
		#endregion

		#region Compiler
		protected Compiler compiler;
		/// <summary>
		/// CheApe compiler implementations
		/// </summary>
		public Compiler Compiler		{ get { return compiler; } }
		#endregion

		#region ImportedBlocks
		protected Dictionary<string, Import.Block> importedBlocks = new Dictionary<string, Import.Block>();
		/// <summary>
		/// 
		/// </summary>
		public Dictionary<string, Import.Block> ImportedBlocks { get { return importedBlocks; } }
		#endregion

		#region Importer
		protected Import importer;
		/// <summary>
		/// CheApe xml definition implementation
		/// </summary>
		public Import Importer	{ get { return importer; } }
		#endregion

		internal Scripting.XmlInterface scriptingInterface = null;

		protected ProjectState(BlamVersion engine, Project proj)
		{
			this.engine = engine;

			Managers.GameManager.Namespace nspace;
			Managers.GameManager.Platform plat;
			// Get the namespace of the engine we're using
			Managers.GameManager.FromBlamVersion(engine, out nspace, out plat);

			// Read the CheApe engine definition data we need for importing
			definition = new XmlInterface(engine);
			definition.Read(Managers.GameManager.GetRelativePath(nspace), "CheApe.xml");

			InitializeTypeIndicies();

			proj.OwnerState = this;
			project = proj;

			// TODO: uncomment this for scripting support
 			Managers.BlamDefinition gd = Program.GetManager(engine);
 			(gd as Managers.IScriptingController).ScriptingCacheOpen(engine);
			scriptingInterface = gd[engine].GetResource<Scripting.XmlInterface>(Managers.BlamDefinition.ResourceScripts);
		}

		public void Dispose()
		{
			compiler.Dispose();

			if (scriptingInterface != null)
			{
				scriptingInterface = null;
				Managers.BlamDefinition gd = Program.GetManager(this.engine);
				(gd as Managers.IScriptingController).ScriptingCacheClose(this.engine);
			}
		}

		#region Type Indicies
		internal int kTypeIndexPad = -1;
		internal int kTypeIndexSkip = -1;
		internal int kTypeIndexArrayStart = -1;
		internal int kTypeIndexArrayEnd = -1;
		internal int kTypeIndexTerminator = -1;

		protected void InitializeTypeIndicies()
		{
			kTypeIndexPad = definition.GetTypeIndex("Pad");
			kTypeIndexSkip = definition.GetTypeIndex("Skip");
			kTypeIndexArrayStart = definition.GetTypeIndex("ArrayStart");
			kTypeIndexArrayEnd = definition.GetTypeIndex("ArrayEnd");
			kTypeIndexTerminator = definition.GetTypeIndex("Terminator");
		}
		#endregion

		/// <summary>
		/// Get the size of a single tag field based on it's enumeration index
		/// </summary>
		/// <param name="type_index"></param>
		/// <returns></returns>
		public int GetFieldSize(int type_index)
		{
			if (type_index == kTypeIndexArrayStart ||
				type_index == kTypeIndexArrayEnd ||
				type_index == kTypeIndexTerminator)
				return 0;
			else
				return definition.FieldTypes[type_index].SizeOf;
		}
	};
}