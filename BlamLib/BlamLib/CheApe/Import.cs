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
using System.Xml;

namespace BlamLib.CheApe
{
	// TODO: catch un-terminated ArrayStart fields

	internal abstract class Import
	{
		/// <summary>
		/// Writes a blank xml document for someone to start filling in
		/// </summary>
		/// <param name="file"></param>
		/// <param name="engine"></param>
		public static void WriteBlankDocument(string file, BlamVersion engine)
		{
			// maybe I should use XmlTextWriter, but I rly don't care right now...
			System.IO.StreamWriter io = new System.IO.StreamWriter(file);
			io.WriteLine("<?xml version=\"1.0\" encoding=\"us-ascii\" standalone=\"yes\"?>");
			io.WriteLine("<definitions game=\"{0}\">", engine);
			io.Write("</definitions>");
			io.Flush();
			io.Close();
		}

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

			protected Object() { }
			protected Object(ProjectState state, IO.XmlStream s)
			{
				s.ReadAttribute("name", ref nameString);

				name = state.Compiler.Strings.Add(nameString);
			}
		};

		public abstract class ObjectWithDebugName : Object
		{
			protected ObjectWithDebugName(ProjectState state, IO.XmlStream s) : base()
			{
				s.ReadAttribute("name", ref nameString);

				name = state.Compiler.DebugStrings.Add(nameString);
			}
		};

		#region Fixups
		public enum FixupType : short
		{
			/// <summary>
			/// Replace an array of characters at a specific address
			/// </summary>
			String,
			Memory,
			/// <summary>
			/// Replace a pointer to a different address, at a specific address
			/// </summary>
			Pointer,
			Field,
		};

		/// <summary>
		/// XmlNode interface for a data fixup for a tool executable
		/// [string: name]
		/// [address: guerilla], [address: tool], [address: sapien]
		/// [enum: type], [enum: fieldType]
		/// [string: definition]
		/// </summary>
		public sealed class Fixup : ObjectWithDebugName
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 + 
				1 +	// definition element
				1;		// field element

			#region Guerilla
			private uint addressGuerilla;
			/// <summary>
			/// Address of this fixup in the Guerilla.exe
			/// </summary>
			public uint AddressGuerilla		{ get { return addressGuerilla; } }

			private uint definitionGuerilla;
			/// <summary>
			/// Special fixup data for Guerilla.exe
			/// </summary>
			public uint DefinitionGuerilla	{ get { return definitionGuerilla; } }
			#endregion

			#region Tool
			private uint addressTool;
			/// <summary>
			/// Address of this fixup in the Tool.exe
			/// </summary>
			public uint AddressTool			{ get { return addressTool; } }

			private uint definitionTool;
			/// <summary>
			/// Special fixup data for Tool.exe
			/// </summary>
			public uint DefinitionTool		{ get { return definitionTool; } }
			#endregion

			#region Sapien
			private uint addressSapien;
			/// <summary>
			/// Address of this fixup in the Sapien.exe
			/// </summary>
			public uint AddressSapien		{ get { return addressSapien; } }

			private uint definitionSapien;
			/// <summary>
			/// Special fixup data for Sapien.exe
			/// </summary>
			public uint DefinitionSapien	{ get { return definitionSapien; } }
			#endregion


			#region Type
			private FixupType type;
			/// <summary>
			/// The type of fixup operation this object defines
			/// </summary>
			public FixupType Type	{ get { return type; } }
			#endregion

			#region Definition
			private string definition;
			/// <summary>
			/// 
			/// </summary>
			public string Definition { get { return definition; } }

			/// <summary>
			/// Parses the [definition] to an integer (base 10)
			/// </summary>
			/// <returns></returns>
			/*public*/ int ToInt() { return Convert.ToInt32(definition, 10); }

			/// <summary>
			/// Parses the [definition] to a pointer (base 16 integer)
			/// </summary>
			/// <returns></returns>
			public uint ToPointer() { return Convert.ToUInt32(definition, 16); }

			/// <summary>
			/// Parses the [definition] to a string
			/// </summary>
			/// <returns></returns>
			public string ToStringValue() { return definition; }

			#region DefinitionLength
			private int definitionLength;
			/// <summary>
			/// 
			/// </summary>
			public int DefinitionLength	{ get { return definitionLength; } }
			#endregion

			#endregion

			/// <summary>
			/// Is this fix-up's definition the same for all platforms?
			/// </summary>
			/// <returns></returns>
			public bool IsPlatformIndependent()
			{
				return definition != null;
			}

			#region Field
			private Field field;
			/// <summary>
			/// 
			/// </summary>
			public Field Field { get { return field; } }
			#endregion

			bool ReadSingleDefinition(ProjectState state, IO.XmlStream s, string attr_name, ref uint value)
			{
				string temp = null;
				bool add_def_memory = s.ReadAttributeOpt(attr_name, ref temp);
				if(add_def_memory)
				{
					if (type == FixupType.String)		value = state.Compiler.RamAddString(temp, out definitionLength);
					else if (type == FixupType.Memory)	value = state.Compiler.RamAddMemory(temp, out definitionLength);
					else if (type == FixupType.Pointer)	value = Convert.ToUInt32(definition, 16);
				}

				return add_def_memory;
			}

			public Fixup(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string temp = string.Empty;

				s.ReadAttribute("guerilla", 16, ref addressGuerilla);
				s.ReadAttribute("tool", 16, ref addressTool);
				s.ReadAttribute("sapien", 16, ref addressSapien);

				s.ReadAttribute("type", ref type);

				#region Definition
				if (type != FixupType.Field && s.ReadAttributeOpt("definition", ref temp))
				{
					if (type == FixupType.String)		definition = state.Compiler.RamAddString(temp, out definitionLength).ToString("X8");
					else if (type == FixupType.Memory)	definition = state.Compiler.RamAddMemory(temp, out definitionLength).ToString("X8");
					else if (type == FixupType.Pointer)	definition = temp;

					definitionGuerilla =
					definitionTool =
					definitionSapien =
						ToPointer();
				}
				else definition = null;
				#endregion

				if(definition == null) foreach (XmlNode node in s.Cursor.ChildNodes)
				{
					if (node.Name == "definition")
					{
						s.SaveCursor(node);

						if (type != FixupType.Field)
						{
							bool is_string = type == FixupType.String;

							// this will be set to false when not all platforms have a definition
							bool add_def_memory = true;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "guerilla", ref definitionGuerilla);
							else				definitionGuerilla = uint.MaxValue;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "tool", ref definitionTool);
							else				definitionTool = uint.MaxValue;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "sapien", ref definitionSapien);
							else				definitionSapien = uint.MaxValue;

							// Houston, we forgot a platform...
							if (!add_def_memory && definitionGuerilla != uint.MaxValue)
							{
								// TODO: error here
							}
						}
						else
						{
							foreach (XmlNode fnode in s.Cursor.ChildNodes)
								if (fnode.Name == "field")
								{
									s.SaveCursor(fnode);
									field = new Field(state, s);
									s.RestoreCursor();
									break;
								}
						}

						s.RestoreCursor();
						break;
					}
				}
			}
		};

		/// <summary>
		/// Fixups found in the XML files
		/// </summary>
		public Dictionary<string, Fixup> Fixups = new Dictionary<string, Fixup>();
		#endregion

		#region Tag Interface
		/// <summary>
		/// XmlNode interface for a tag field
		/// [enum: type]
		/// [string: name], [bool: locked], [bool: hidden], [bool: blockname], [bool: deprecated], [string: tooltip], [string: units]
		/// [string: definition]
		/// [string: extra]
		/// </summary>
		public sealed class Field : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			#region TypeIndex
			private int typeIndex;
			/// <summary>
			/// 
			/// </summary>
			public int TypeIndex		{ get { return typeIndex; } }
			#endregion

			#region Definition
			private string definition = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public string Definition	{ get { return definition; } }

			/// <summary>
			/// Parses the [definition] to an integer (base 10)
			/// </summary>
			/// <returns></returns>
			public int ToInt() { return Convert.ToInt32(definition, 10); }

			/// <summary>
			/// Parses the [definition] to a pointer (base 16 integer)
			/// </summary>
			/// <returns></returns>
			public uint ToPointer() { return Convert.ToUInt32(definition, 16); }

			/// <summary>
			/// Parses the [definition] to a string
			/// </summary>
			/// <returns></returns>
			public override string ToString() { return definition; }
			#endregion

			/// <summary>
			/// Constructs a field from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public Field(ProjectState state, IO.XmlStream s)
			{
				string temp = string.Empty;

				#region Type
				s.ReadAttribute("type", ref temp);

				typeIndex = state.Definition.GetTypeIndex(temp);
				Debug.Assert.If(typeIndex != -1, "Not a usable field type: {0}", temp);
				#endregion

				#region Name
				nameString = temp = XmlInterface.BuildEditorNameString(s);

				name = state.Compiler.Strings.Add(temp);
				#endregion

				#region Definition
				if (s.ReadAttributeOpt("definition", ref definition))
				{						
					if(state.Definition.FieldTypes[typeIndex].Name == "Explanation")
					{
						// since halo internally uses "|n" and "|t", we'll just use these >=D
						definition = definition.Replace("~N", "\n").Replace("~T", "    ");

						if (definition != string.Empty)
							definition = state.Compiler.Strings.Add(definition).ToString("X"); // set definition to be the value of string's address in the pool
					}
					else if(definition == "" && state.Definition.FieldTypes[typeIndex].RequiresDefinition)
						throw new Debug.ExceptionLog("\"{0}\" of type {1} has no definition", nameString, state.Definition.FieldTypes[typeIndex].Name);
				}
				else if(state.Definition.FieldTypes[typeIndex].RequiresDefinition)
					throw new Debug.ExceptionLog("\"{0}\" of type {1} has no definition", nameString, state.Definition.FieldTypes[typeIndex].Name);
				#endregion
			}
		};

		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// [list: fields]
		/// </summary>
		public sealed class StringList : ObjectWithDebugName
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			#region Elements
			private List<uint> elements = new List<uint>();
			/// <summary>
			/// 
			/// </summary>
			public List<uint> Elements	{ get { return elements; } }
			#endregion

			/// <summary>
			/// Constructs a string list from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public StringList(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				foreach (XmlNode node in s.Cursor.ChildNodes)
					if (node.Name == "field")
						this.elements.Add(state.Compiler.Strings.Add(node.InnerText));
			}
		};

		/// <summary>
		/// XmlNode interface for a tag reference definition
		/// [string: name]
		/// [boolean: isNonResolving]
		/// [list: tags]
		/// </summary>
		public sealed class TagReference : ObjectWithDebugName
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			public readonly bool IsNonResolving;

			#region Elements
			private List<string> elements = new List<string>();
			/// <summary>
			/// 
			/// </summary>
			public List<string> Elements	{ get { return elements; } }
			#endregion

			/// <summary>
			/// Constructs a tag reference definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public TagReference(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttributeOpt("isNonResolving", ref IsNonResolving);

				foreach (XmlNode node in s.Cursor.ChildNodes)
					if (node.Name == "field")
						this.elements.Add(node.InnerText);
			}
		};

		/// <summary>
		/// XmlNode interface for a tag data definition
		/// [string: name]
		/// [int: maxSize]
		/// [boolean: isNeverStreamed]
		/// [boolean: isTextData]
		/// [boolean: isDebugData]
		/// [string: byteswapProc]
		/// </summary>
		public sealed class TagData : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			#region MaxSize
			private int maxSize = 0;
			/// <summary>
			/// 
			/// </summary>
			public int MaxSize	{ get { return maxSize; } }
			#endregion

			public readonly bool IsNeverStreamed;
			public readonly bool IsTextData;
			public readonly bool IsDebugData;

			#region ByteswapProc
// 			private string byteswapProc;
// 			/// <summary>
// 			/// 
// 			/// </summary>
// 			public string ByteswapProc	{ get { return byteswapProc; } }
			#endregion

			/// <summary>
			/// Constructs a data definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public TagData(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttributeOpt("maxSize", 16, ref maxSize);
				s.ReadAttributeOpt("isNeverStreamed", ref IsNeverStreamed);
				s.ReadAttributeOpt("isTextData", ref IsTextData);
				s.ReadAttributeOpt("isDebugData", ref IsDebugData);
//				s.ReadAttributeOpt("byteswapProc", ref byteswapProc);
			}
		};

		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// </summary>
		public abstract class Block : Object
		{
			#region DisplayName
			protected uint displayName = 0;
			protected string displayString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint DisplayName
			{
				get { return displayName; }
				set { displayName = value; }
			}
			#endregion

			/// <summary>
			/// Don't call this unless your name is <see cref="TagGroup"/>
			/// </summary>
			public Block() { }
			/// <summary>
			/// Constructs a block definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			protected Block(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				try { state.ImportedBlocks.Add(this.nameString, this); }
				catch (Exception) { }

				#region DisplayName
				if (s.ReadAttributeOpt("displayName", ref displayString)) displayName = state.Compiler.Strings.Add(displayString);
				else { displayName = name; displayString = nameString; }
				#endregion
			}

			public virtual void Read(ProjectState state, IO.XmlStream s)
			{
				try { state.ImportedBlocks.Add(this.nameString, this); }
				catch (Exception) { }
			}
		};

		/// <summary>
		/// XmlNode interface for a tag group definition
		/// [string: name]
		/// [string: groupTag]
		/// [string: parentTag]
		/// [int: version]
		/// </summary>
		public abstract class TagGroup : Object
		{
			#region BlockName
			protected uint blockName = 0;
			/// <summary>
			/// 
			/// </summary>
			public uint BlockName	{ get { return blockName; } }
			#endregion

			#region GroupTag
			protected string groupTag = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public string GroupTag	{ get { return groupTag; } }
			#endregion

			#region ParentTag
			protected string parentTag = null;
			/// <summary>
			/// 
			/// </summary>
			public string ParentTag	{ get { return parentTag; } }
			#endregion

			#region Version
			protected short version = -1;
			/// <summary>
			/// 
			/// </summary>
			public short Version	{ get { return version; } }
			#endregion

			#region Block
			protected Block block = null;
			/// <summary>
			/// 
			/// </summary>
			public Block Block { get { return block; } }
			#endregion

			/// <summary>
			/// Constructs a tag group definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			/// <remarks>Construct <see cref="TagGroup.Block"/> in the inheriting class's ctor</remarks>
			protected TagGroup(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttribute("groupTag", ref groupTag);
				s.ReadAttributeOpt("parentTag", ref parentTag);
				s.ReadAttribute("version", 10, ref version);
			}
		};

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
		/// XmlNode interface for a script function argument
		/// [string: type]
		/// [string: name] (unused)
		/// </summary>
		public sealed class ScriptFunctionArg
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			internal Scripting.XmlInterface.Type type = null;

			public ScriptFunctionArg(ScriptFunction func, ProjectState state, IO.XmlStream s)
			{
				string type_name = null;
				s.ReadAttribute("type", ref type_name);

				if (!state.scriptingInterface.Types.Contains(type_name)) throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", type_name);
				type = state.scriptingInterface.GetValueType(type_name);
			}
		};

		/// <summary>
		/// XmlNode interface for a script function
		/// [string: name]
		/// [string: returnType]
		/// [string: help] (optional)
		/// [string: helpArg] (optional)
		/// [list: args]
		/// </summary>
		public sealed class ScriptFunction : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 +
				ScriptFunctionArg.XmlNodeComplexity;

			internal Scripting.XmlInterface.Type returnType = null;

			#region Help
			uint help = 0;
			string helpString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint Help { get { return help; } }
			#endregion

			#region HelpArg
			uint helpArg = 0;
			string helpArgString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint HelpArg { get { return helpArg; } }
			#endregion

			#region Arguments
			private List<ScriptFunctionArg> args = new List<ScriptFunctionArg>();
			/// <summary>
			/// 
			/// </summary>
			public List<ScriptFunctionArg> Arguments { get { return args; } }
			#endregion

			public readonly bool IsInternal = false;

			public ScriptFunction(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string return_type_name = null;
				s.ReadAttribute("returnType", ref return_type_name);
				s.ReadAttributeOpt("help", ref helpString);
				s.ReadAttributeOpt("helpArg", ref helpArgString);
				s.ReadAttributeOpt("internal", ref IsInternal);

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "arg") continue;

					s.SaveCursor(n);
					args.Add(new ScriptFunctionArg(this, state, s));
					s.RestoreCursor();
				}

				if (!state.scriptingInterface.Types.Contains(return_type_name)) throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", return_type_name);
				returnType = state.scriptingInterface.GetValueType(return_type_name);
				help = state.Compiler.Strings.Add(helpString);
				helpArg = state.Compiler.Strings.Add(helpArgString);
			}
		};

		/// <summary>
		/// XmlNode interface for a script global
		/// [string: name]
		/// [string: type]
		/// </summary>
		public sealed class ScriptGlobal : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			internal Scripting.XmlInterface.Type type = null;

			public readonly bool IsInternal = false;

			public ScriptGlobal(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string type_name = null;
				s.ReadAttribute("type", ref type_name);
				s.ReadAttributeOpt("internal", ref IsInternal);

				if (!state.scriptingInterface.Types.Contains(type_name)) throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", type_name);
				type = state.scriptingInterface.GetValueType(type_name);
			}
		};

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
				string name_str;
				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					switch (n.Name)
					{
						#region Enums
						case "enums":
							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "Enum") continue;

								s.SaveCursor(n2);
								StringList list = new StringList(state, s);
								s.RestoreCursor();
								name_str = list.ToString();

								try { Enums.Add(name_str, list); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "enum definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Flags
						case "flags":
							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "Flag") continue;

								s.SaveCursor(n2);
								StringList list = new StringList(state, s);
								s.RestoreCursor();
								name_str = list.ToString();

								try { Flags.Add(name_str, list); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "flag definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Tag References
						case "references":
							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "Reference") continue;

								s.SaveCursor(n2);
								TagReference tagref = new TagReference(state, s);
								s.RestoreCursor();
								name_str = tagref.ToString();

								try { References.Add(name_str, tagref); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "tag reference definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Tag Data
						case "data":
							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "TagData") continue;

								s.SaveCursor(n2);
								TagData tagdata = new TagData(state, s);
								s.RestoreCursor();
								name_str = tagdata.ToString();

								try { Data.Add(name_str, tagdata); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "tag data definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Script Functions
						case "scriptFunctions":
							if (state.scriptingInterface == null) break;

							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "function") continue;

								s.SaveCursor(n2);
								ScriptFunction sc = new ScriptFunction(state, s);
								s.RestoreCursor();
								name_str = sc.ToString();

								if (state.scriptingInterface.Functions.Contains(name_str))
								{
									Debug.LogFile.WriteLine("Engine already contains a {0} named '{1}', skipping...", "script function", name_str);
									continue;
								}

								try { ScriptFunctions.Add(name_str, sc); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "script function definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Script Globals
						case "scriptGlobals":
							if (state.scriptingInterface == null) break;

							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "global") continue;

								s.SaveCursor(n2);
								ScriptGlobal sc = new ScriptGlobal(state, s);
								s.RestoreCursor();
								name_str = sc.ToString();

								if (state.scriptingInterface.Globals.Contains(name_str))
								{
									Debug.LogFile.WriteLine("Engine already contains a {0} named '{1}', ignoring...", "script global", name_str);
									continue;
								}

								try { ScriptGlobals.Add(name_str, sc); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "script global definition", name_str); }
							}
							s.RestoreCursor();
							break;
						#endregion

						#region Fix-ups
						case "fixups":
							s.SaveCursor(n);
							foreach (XmlNode n2 in s.Cursor.ChildNodes)
							{
								if (n2.Name != "fixup") continue;

								s.SaveCursor(n2);
								Fixup fu = new Fixup(state, s);
								s.RestoreCursor();
								name_str = fu.ToString();

								try { Fixups.Add(name_str, fu); }
								catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "fix-up definition", name_str); }
							}
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