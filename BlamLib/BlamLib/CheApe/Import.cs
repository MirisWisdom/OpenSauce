/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace BlamLib.CheApe
{
	// TODO: catch un-terminated ArrayStart fields
	// ^ Halo1 & Halo2 have this support in FieldContainer and FieldSet (respectively)

	internal abstract partial class Import
	{
		/// <summary>
		/// Writes a blank xml document for someone to start filling in
		/// </summary>
		/// <param name="file"></param>
		/// <param name="engine"></param>
		public static void WriteBlankDocument(string file, BlamVersion engine)
		{
			// maybe I should use XmlTextWriter, but I rly don't care right now...
			using (var io = new System.IO.StreamWriter(file))
			{
				io.WriteLine("<?xml version=\"1.0\" encoding=\"us-ascii\" standalone=\"yes\"?>");
				io.WriteLine("<definitions game=\"{0}\">", engine);
				io.Write("</definitions>");
			}
		}

		#region Export tags
		internal static readonly TagInterface.TagGroup ExportStringListTag = new TagInterface.TagGroup(
			"estr", "exported_string_list");

		internal static readonly TagInterface.TagGroup ExportFieldSetTag = new TagInterface.TagGroup(
			"etfd", "exported_field_set");
		internal static readonly TagInterface.TagGroup ExportTagReferenceTag = new TagInterface.TagGroup(
			"etrf", "exported_tag_reference");
		internal static readonly TagInterface.TagGroup ExportTagDataTag = new TagInterface.TagGroup(
			"etda", "exported_tag_data");
		internal static readonly TagInterface.TagGroup ExportTagBlockTag = new TagInterface.TagGroup(
			"etbk", "exported_tag_block");
		internal static readonly TagInterface.TagGroup ExportTagStructTag = new TagInterface.TagGroup(
			"etst", "exported_tag_struct");

		internal static readonly TagInterface.TagGroup ExportScriptFunctionTag = new TagInterface.TagGroup(
			"ehsf", "exported_script_function");
		internal static readonly TagInterface.TagGroup ExportScriptGlobalTag = new TagInterface.TagGroup(
			"ehsg", "exported_script_global");
		#endregion

		public abstract class Object
		{
			#region Name
			protected uint name = 0;
			protected string nameString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint Name
			{
				get { return name; }
				set { name = value; }
			}

			public override string ToString() { return nameString; }
			#endregion

			#region Export APIs
			bool mIsExported;
			public bool IsExported { get { return mIsExported; } }

			internal virtual TagInterface.TagGroup ExportTag { get { return TagInterface.TagGroup.Null; } }

			internal void ExplicitlyExport(ProjectState state)
			{
//				mIsExported = true;
//				state.Compiler.AddExport(this);
			}
			protected void InitializeForExport(ProjectState state, IO.XmlStream s)
			{
				s.ReadAttributeOpt("export", ref mIsExported);

//				if (IsExported)
//					state.Compiler.AddExport(this);
			}
			#endregion

			protected Object() { }
			protected Object(ProjectState state, IO.XmlStream s)
			{
				s.ReadAttribute("name", ref nameString);
				InitializeForExport(state, s);

				name = state.Compiler.Strings.Add(nameString);
			}
		};

		public abstract class ObjectWithDebugName : Object
		{
			protected ObjectWithDebugName(ProjectState state, IO.XmlStream s) : base()
			{
				s.ReadAttribute("name", ref nameString);
				InitializeForExport(state, s);

				name = state.Compiler.DebugStrings.Add(nameString);
			}

			protected ObjectWithDebugName(ProjectState state, string nameString) : base()
			{
				name = state.Compiler.DebugStrings.Add(nameString);
			}
		};

		#region Fixups
		/// <summary>
		/// Fixups found in the XML files
		/// </summary>
		public Dictionary<string, Fixup> Fixups = new Dictionary<string, Fixup>();
		#endregion

		#region Tag Interface
		/// <summary>
		/// Enum Definitions found in the XML files
		/// </summary>
		public Dictionary<string, StringList> Enums = new Dictionary<string, StringList>();
		/// <summary>
		/// Flag Definitions found in the XML files
		/// </summary>
		public Dictionary<string, StringList> Flags = new Dictionary<string, StringList>();
		/// <summary>
		/// Tag Reference Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagReference> References = new Dictionary<string, TagReference>();
		/// <summary>
		/// Tag Data Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagData> Data = new Dictionary<string, TagData>();
		#endregion

		#region Script Interface
		/// <summary>
		/// Script Functions found in the XML files
		/// </summary>
		public Dictionary<string, ScriptFunction> ScriptFunctions = new Dictionary<string, ScriptFunction>();
		/// <summary>
		/// Script Globals found in the XML files
		/// </summary>
		public Dictionary<string, ScriptGlobal> ScriptGlobals = new Dictionary<string, ScriptGlobal>();
		#endregion


		/// <summary>
		/// Reset the import state so there are no present definitions
		/// </summary>
		public virtual void Reset()
		{
			Fixups.Clear();
			Enums.Clear();
			Flags.Clear();
			References.Clear();
			Data.Clear();
			ScriptFunctions.Clear();
			ScriptGlobals.Clear();
		}

		#region XmlNodeComplexity
		/// <summary>
		/// The maximum complexity level which this object can achieve.
		/// </summary>
		const int XmlNodeComplexity = 2;

		protected abstract int PreprocessXmlNodeComplexity(int complexity);

		private int PreprocessXmlNodeComplexity()
		{
			int complexity = 0;
			complexity = Math.Max(complexity, Fixup.XmlNodeComplexity);

			complexity = Math.Max(complexity, StringList.XmlNodeComplexity);
			complexity = Math.Max(complexity, TagReference.XmlNodeComplexity);
			complexity = Math.Max(complexity, TagData.XmlNodeComplexity);

			complexity = Math.Max(complexity, ScriptFunction.XmlNodeComplexity);
			complexity = Math.Max(complexity, ScriptGlobal.XmlNodeComplexity);

			complexity = PreprocessXmlNodeComplexity(complexity);

			return complexity + Import.XmlNodeComplexity;
		}
		#endregion

		protected const string kDuplicateErrorStr = "Project already contains a {0} called '{1}', ignoring...";
		protected abstract void ProcessDefinition(XmlNode node, ProjectState state, IO.XmlStream s);

		/// <summary>
		/// Process a XML file containing CheApe tag definitions
		/// </summary>
		/// <param name="state"></param>
		/// <param name="file"></param>
		public void ProcessFile(ProjectState state, string file)
		{
			using (IO.XmlStream s = new BlamLib.IO.XmlStream(file, this))
			{
				ProcessFile(state, s);
			}
		}

		/// <summary>
		/// Process a XML manifest file containing CheApe tag definitions
		/// </summary>
		/// <param name="state"></param>
		/// <param name="path"></param>
		/// <param name="name"></param>
		public void ProcessFile(ProjectState state, string path, string name)
		{
			using (IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
			{
				ProcessFile(state, s);
			}
		}

		/// <summary>
		/// Process a XML file containing CheApe definitions
		/// </summary>
		/// <param name="state"></param>
		/// <param name="s"></param>
		private void ProcessFile(ProjectState state, BlamLib.IO.XmlStream s)
		{
			int complexity = 1 + 
				PreprocessXmlNodeComplexity();

			BlamVersion def_engine = BlamVersion.Unknown;
			s.ReadAttribute("game", ref def_engine);
			if (def_engine != state.Definition.Engine)
			{
				Debug.Assert.If(false, "CheApe definition '{0}' is for {1}. Expected a {2} definition.", s.FileName, def_engine, state.Definition.Engine);
			}
			else
			{
				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					switch (n.Name)
					{
						#region Enums
						case "enums":
							s.SaveCursor(n);
							ProcessEnums(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Flags
						case "flags":
							s.SaveCursor(n);
							ProcessFlags(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Tag References
						case "references":
							s.SaveCursor(n);
							ProcessTagReferences(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Tag Data
						case "data":
							s.SaveCursor(n);
							ProcessTagDatas(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Script Functions
						case "scriptFunctions":
							if (state.scriptingInterface == null) break;

							s.SaveCursor(n);
							ProcessScriptFunctions(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Script Globals
						case "scriptGlobals":
							if (state.scriptingInterface == null) break;

							s.SaveCursor(n);
							ProcessScriptGlobals(state, s);
							s.RestoreCursor();
							break;
						#endregion

						#region Fix-ups
						case "fixups":
							s.SaveCursor(n);
							ProcessFixups(state, s);
							s.RestoreCursor();
							break;
						#endregion

						default:
							ProcessDefinition(n, state, s);
							break;
					}
				}
			}
		}
	};
}