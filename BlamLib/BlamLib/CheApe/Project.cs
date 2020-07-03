/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

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


		static void ShowMessageInForm(System.Windows.Forms.Form owner_form, string msg)
		{
			if (owner_form != null)
			{
				owner_form.BeginInvoke(new Action( () => 
						System.Windows.Forms.MessageBox.Show(owner_form, msg)
					));
			}
			else System.Windows.Forms.MessageBox.Show(msg);
		}
		static void ShowMessageInForm(System.Windows.Forms.Form owner_form, string caption, string fmt, params object[] args)
		{
			string msg = string.Format(fmt, args);
			if (owner_form != null)
			{
				owner_form.BeginInvoke(new Action( () => 
						System.Windows.Forms.MessageBox.Show(owner_form, msg, caption)
					));
			}
			else System.Windows.Forms.MessageBox.Show(msg, caption);
		}
		bool imported = false; // have we even ran one import yet?

		/// <summary>
		/// Import the project files
		/// </summary>
		/// <param name="owner_form"></param>
		public void Import(System.Windows.Forms.Form owner_form)
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
				ShowMessageInForm(owner_form, dbg_name, k_error, "CheApe", is_embedded ? " (Embedded)" : "");
				return;
			}catch(System.Exception ex)
			{
				ShowMessageInForm(owner_form, dbg_name, k_error, ".NET", is_embedded ? " (Embedded)" : "");
				Debug.LogFile.WriteLine("CheApe: .NET parsing error in {0}.{1}{2}", dbg_name, Program.NewLine, ex);
				return;
			}

			imported = true;
		}

		/// <summary>
		/// Build the whole project
		/// </summary>
		/// <param name="owner_form"></param>
		/// <param name="reimport">Should we re-import the project files first?</param>
		public void Build(System.Windows.Forms.Form owner_form, bool reimport)
		{
			if (imported && reimport)
				OwnerState.Compiler.Reset();

			if(!imported || reimport)
				Import(owner_form);

			// owner_form should be allowed to be null as the last time I checked MSBOX's implementation in Reflector, 
			// its code just calls GetActiveWindow() when [owner] is null

			try { OwnerState.Compiler.Write(folder, fileName); }
			catch (Debug.ExceptionLog)
			{
				ShowMessageInForm(owner_form, fileName, "A compiler error has been encountered, please check the debug.log file");
				return;
			}

			ShowMessageInForm(owner_form, "Build Finished!");
		}

		/// <summary>
		/// Build the whole project
		/// </summary>
		/// <param name="param">If calling from a GUI, this should be an <see cref="System.Windows.Forms.Form"/> instance, or null</param>
		/// <remarks>Re-imports all project files</remarks>
		public void Build(object param) { Build(param as System.Windows.Forms.Form, true); }


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
}