/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace BlamLib.Blam.Halo1.CheApe
{
	internal sealed class Import : BlamLib.CheApe.Import
	{
		internal const string kStringIdFieldDefinitionName = "string_id_yelo_non_resolving";
		internal const string kStringIdGroupTag = "sidy";
		internal const int kStringIdPadSize = StringId.kSizeOf - StringIdDesc.kSizeOf;

		internal TagReference StringIdFieldDefinition;
		internal Field StringIdFieldHandlePadding;

		void StringIdFieldsInitialize(BlamLib.CheApe.ProjectState state)
		{
			if (StringIdFieldDefinition != null) return;

			// Will add itself to the import state in the ctor
			StringIdFieldDefinition = new TagReference(state, kStringIdFieldDefinitionName, true, kStringIdGroupTag);
			StringIdFieldHandlePadding = new Field(state, state.kTypeIndexPad, "", kStringIdPadSize.ToString());
		}

		public sealed class FieldContainer
		{
			private List<Field> fields = new List<Field>();
			/// <summary>
			/// 
			/// </summary>
			public List<Field> Fields	{ get { return fields; } }

			int CalculateSize(BlamLib.CheApe.ProjectState state, Field f)
			{
				int size = 0;

				if (f.TypeIndex == state.kTypeIndexPad || f.TypeIndex == state.kTypeIndexSkip)
					size = f.ToInt();
				else
					size = state.GetFieldSize(f.TypeIndex);

				return size;
			}

			int CalculateSizeFromArrayStart(BlamLib.CheApe.ProjectState state, ref int current_index)
			{
				int array_size = 0;
				int array_count = fields[current_index++].ToInt();

				for (; ; current_index++)
					if (fields[current_index].TypeIndex == state.kTypeIndexArrayStart)
						array_size += CalculateSizeFromArrayStart(state, ref current_index);
					else if (fields[current_index].TypeIndex == state.kTypeIndexArrayEnd)
						break;
					else
						array_size += CalculateSize(state, fields[current_index]);

				return array_size * array_count;
			}

			/// <summary>
			/// Calculates the block size in bytes via the field for this block
			/// eliminating the need for programmers to aid designers in creating
			/// the tag group definition files
			/// </summary>
			/// <returns></returns>
			public int CalculateSize(BlamLib.CheApe.ProjectState state)
			{
				int size = 0;

				for (int x = 0; x < fields.Count; x++)
				{
					if (fields[x].TypeIndex == state.kTypeIndexArrayStart)
						size += CalculateSizeFromArrayStart(state, ref x);
					else
						size += CalculateSize(state, fields[x]);
				}

				return size;
			}

			public void Read(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				int array_start_depth = 0;

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "field") continue;

					Field f;
					s.SaveCursor(n);
					fields.Add(f = new Field(state, s));
					s.RestoreCursor();

					if (f.TypeIndex == state.kTypeIndexArrayStart) array_start_depth++;
					else if (f.TypeIndex == state.kTypeIndexArrayEnd) array_start_depth--;
				}

				if (array_start_depth != 0)
					throw new Debug.ExceptionLog("Unterminated ArrayStart or extra ArrayEnd in '{0}'.{1}{2}", 
						s.FileName, Program.NewLine, s.Cursor.OuterXml);
			}
		};

		public sealed class TagStruct
		{
			public string Name { get; private set; }

			public FieldContainer Fields { get; private set; }

			public TagStruct(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				string name_string = null;
				s.ReadAttribute("name", ref name_string);
				Name = name_string;

				Fields = new FieldContainer();
			}

			public void Read(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				Fields.Read(state, s);
			}
		};

		/// <summary>
		/// XmlNode interface for a tag block definition
		/// [string: name]
		/// [int: maxElements]
		/// [boolean: dontReadChildren]
		/// [list: fields]
		/// </summary>
		public sealed class TagBlock : BlamLib.CheApe.Import.Block
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 + 
				Field.XmlNodeComplexity;

			#region MaxElements
			private int maxElements = 1;
			/// <summary>
			/// 
			/// </summary>
			public int MaxElements	{ get { return maxElements; } }
			#endregion

			public readonly bool DontReadChildren;

			#region Fields
			FieldContainer mFields = new FieldContainer();

			public List<Field> Fields { get { return mFields.Fields; } }

			/// <summary>
			/// Calculates the block size in bytes via the field for this block
			/// eliminating the need for programmers to aid designers in creating
			/// the tag group definition files
			/// </summary>
			/// <returns></returns>
			public int CalculateSize(BlamLib.CheApe.ProjectState state)
			{
				return mFields.CalculateSize(state);
			}
			#endregion

			public TagBlock() : base() {}
			public TagBlock(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttribute("maxElements", 10, ref maxElements);
				s.ReadAttributeOpt("dontReadChildren", ref DontReadChildren);

				mFields.Read(state, s);
			}

			public override void Read(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				base.Read(state, s);

				mFields.Read(state, s);
			}
		};

		/// <summary>
		/// XmlNode interface for a tag group definition
		/// [string: name]
		/// [string: groupTag]
		/// [string: parentTag]
		/// [int: version]
		/// [boolean: isIncludedInTagGroupsChecksum]
		/// [list: fields]
		/// </summary>
		new public sealed class TagGroup : BlamLib.CheApe.Import.TagGroup
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = TagBlock.XmlNodeComplexity;

			public readonly bool IsIncludedInTagGroupsChecksum;

			/// <summary>
			/// Constructs a tag group definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public TagGroup(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttributeOpt("isIncludedInTagGroupsChecksum", ref IsIncludedInTagGroupsChecksum);

				block = new TagBlock();
				block.DisplayName = name;
				string tempName = nameString + "_block";
				block.Name = blockName = state.Compiler.Strings.Add(tempName);
				block.Read(state, s);
			}
		};


		/// <summary>
		/// Tag Struct Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagStruct> Structs = new Dictionary<string, TagStruct>();
		/// <summary>
		/// Tag Block Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagBlock> Blocks = new Dictionary<string, TagBlock>();
		/// <summary>
		/// Tag Group Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagGroup> Groups = new Dictionary<string, TagGroup>();

		/// <summary>
		/// Reset the import state so there are no present definitions
		/// </summary>
		public override void Reset()
		{
			base.Reset();

			StringIdFieldDefinition = null;
			StringIdFieldHandlePadding = null;

			Structs.Clear();
			Blocks.Clear();
			Groups.Clear();
		}

		protected override int PreprocessXmlNodeComplexity(int complexity)
		{
			complexity = Math.Max(complexity, TagBlock.XmlNodeComplexity);
			complexity = Math.Max(complexity, TagGroup.XmlNodeComplexity);

			return complexity;
		}

		protected override void ProcessDefinition(XmlNode node, BlamLib.CheApe.ProjectState state, BlamLib.IO.XmlStream s)
		{
			StringIdFieldsInitialize(state);

			string name_str;

			switch (node.Name)
			{
				#region Tag Structs
				case "structs":
					s.SaveCursor(node);
					foreach (XmlNode n in s.Cursor.ChildNodes)
					{
						if (n.Name != "Struct") continue;

						s.SaveCursor(n);
						TagStruct block = new TagStruct(state, s);
						s.RestoreCursor();
						name_str = block.ToString();

						try { Structs.Add(name_str, block); }
						catch (ArgumentException) { Debug.LogFile.WriteLine(CheApe.Import.kDuplicateErrorStr, "tag struct definition", name_str); }
					}
					s.RestoreCursor();
					break;
				#endregion

				#region Tag Blocks
				case "blocks":
					s.SaveCursor(node);
					foreach (XmlNode n in s.Cursor.ChildNodes)
					{
						if (n.Name != "TagBlock") continue;

						s.SaveCursor(n);
						TagBlock block = new TagBlock(state, s);
						s.RestoreCursor();
						name_str = block.ToString();

						try { Blocks.Add(name_str, block); }
						catch (ArgumentException) { Debug.LogFile.WriteLine(CheApe.Import.kDuplicateErrorStr, "tag block definition", name_str); }
					}
					s.RestoreCursor();
					break;
				#endregion

				#region Tag Groups
				case "groups":
					s.SaveCursor(node);
					foreach (XmlNode n in s.Cursor.ChildNodes)
					{
						if (n.Name != "TagGroup") continue;

						s.SaveCursor(n);
						TagGroup group = new TagGroup(state, s);
						s.RestoreCursor();
						name_str = group.ToString();

						try { Groups.Add(name_str, group); }
						catch (ArgumentException) { Debug.LogFile.WriteLine(CheApe.Import.kDuplicateErrorStr, "tag group definition", name_str); }
					}
					s.RestoreCursor();
					break;
				#endregion
			}
		}
	};
}