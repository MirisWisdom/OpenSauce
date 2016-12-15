/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.CheApe
{
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