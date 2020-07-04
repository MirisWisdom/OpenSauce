/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;

namespace BlamLib.CheApe
{
	partial class Import
	{
		/// <summary>
		/// XmlNode interface for a tag field
		/// [enum: type]
		/// [string: name], [bool: locked], [bool: hidden], [bool: blockname], [bool: deprecated], [string: tooltip], [string: units], [bool: hiddenAlways]
		/// [string: definition]
		/// [string: extra]
		/// </summary>
		public sealed class Field : Import.Object
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

			bool TypeIndexIsExplanation(ProjectState state)
			{
				return state.Definition.FieldTypes[typeIndex].Name == "Explanation";
			}

			/// <summary>
			/// HACK! DON'T USE ME UNLESS YOU KNOW WHAT YOU'RE DOING!
			/// </summary>
			/// <param name="value"></param>
			internal void ChangeTypeIndex(int value)
			{
				typeIndex = value;
			}
			#endregion

			#region Definition
			private string definition = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public string Definition	{ get { return definition; } }

			/// <summary>
			/// HACK! DON'T USE ME UNLESS YOU KNOW WHAT YOU'RE DOING!
			/// </summary>
			/// <param name="value"></param>
			internal void ChangeDefinition(string value)
			{
				definition = value;
			}

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

			void ValidateDefinition(ProjectState state)
			{
				if (string.IsNullOrEmpty(definition) && state.Definition.FieldTypes[typeIndex].RequiresDefinition)
					throw new Debug.ExceptionLog("\"{0}\" of type {1} has no definition", nameString, state.Definition.FieldTypes[typeIndex].Name);
			}
			void SyncDefinitionWithState(ProjectState state)
			{
				if (TypeIndexIsExplanation(state))
				{
					if (definition == null) // Explanation requires a definition, so we'll implicitly use an empty string
						definition = "";
					else
					{
						// since halo internally uses "|n" and "|t", we'll just use these >=D
						definition = definition.Replace("~N", "\n").Replace("~T", "    ");
					}

					definition = state.Compiler.Strings.Add(definition).ToString("X"); // set definition to be the value of string's address in the pool
				}
				else ValidateDefinition(state);
			}
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
				if (s.ReadAttributeOpt("definition", ref definition) || TypeIndexIsExplanation(state))
				{
					SyncDefinitionWithState(state);
				}
				else if(state.Definition.FieldTypes[typeIndex].RequiresDefinition)
					throw new Debug.ExceptionLog("\"{0}\" of type {1} has no definition", nameString, state.Definition.FieldTypes[typeIndex].Name);
				#endregion
			}

			/// <summary>
			/// Explicitly construct a tag field
			/// </summary>
			/// <param name="state"></param>
			/// <param name="type_index"></param>
			/// <param name="name_string"></param>
			/// <param name="def"></param>
			internal Field(ProjectState state, int type_index, string name_string, string def)
			{
				Debug.Assert.If(type_index != -1, "Invalid field type given to explicit Import.Field");
				typeIndex = type_index;

				nameString = name_string;
				name = state.Compiler.Strings.Add(nameString);

				definition = def;
				SyncDefinitionWithState(state);
			}
		};

		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// [list: fields]
		/// </summary>
		public sealed class StringList : Import.ObjectWithDebugName
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
		public sealed class TagReference : Import.ObjectWithDebugName
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

			/// <summary>
			/// Explicitly construct a tag reference definition
			/// </summary>
			/// <param name="state"></param>
			/// <param name="nameString"></param>
			/// <param name="isNonResolving"></param>
			/// <param name="tags"></param>
			/// <remarks>Adds itself to <paramref name="state"/>'s Importer's References list</remarks>
			internal TagReference(ProjectState state, string nameString, bool isNonResolving, params string[] tags) : base(state, nameString)
			{
				IsNonResolving = isNonResolving;
				elements.AddRange(tags);

				state.Importer.References.Add(nameString, this);
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
		public sealed class TagData : Import.Object
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
			}
		};

		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// </summary>
		public abstract class Block : Import.Object
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
		public abstract class TagGroup : Import.Object
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

		static void ProcessStringList(ProjectState state, IO.XmlStream s, Dictionary<string, StringList> listDic,
			string elementName, string listTypeName)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != elementName) continue;

				s.SaveCursor(n);
				var list = new StringList(state, s);
				s.RestoreCursor();
				string name_str = list.ToString();

				try { listDic.Add(name_str, list); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, listTypeName, name_str); }
			}
		}
		void ProcessEnums(ProjectState state, IO.XmlStream s)
		{
			ProcessStringList(state, s, Enums, "Enum", "enum definition");
		}
		void ProcessFlags(ProjectState state, IO.XmlStream s)
		{
			ProcessStringList(state, s, Enums, "Flag", "flag definition");
		}

		void ProcessTagReferences(ProjectState state, IO.XmlStream s)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "Reference") continue;

				s.SaveCursor(n);
				var tagref = new TagReference(state, s);
				s.RestoreCursor();
				string name_str = tagref.ToString();

				try { References.Add(name_str, tagref); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "tag reference definition", name_str); }
			}
		}

		void ProcessTagDatas(ProjectState state, IO.XmlStream s)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "TagData") continue;

				s.SaveCursor(n);
				var tagdata = new TagData(state, s);
				s.RestoreCursor();
				string name_str = tagdata.ToString();

				try { Data.Add(name_str, tagdata); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "tag data definition", name_str); }
			}
		}
	};
}