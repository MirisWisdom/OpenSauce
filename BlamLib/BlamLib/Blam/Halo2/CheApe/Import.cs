/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;

namespace BlamLib.Blam.Halo2.CheApe
{
	internal sealed class Import : BlamLib.CheApe.Import
	{
		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// [list: fields]
		/// </summary>
		public sealed class FieldSet : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 + 
				Field.XmlNodeComplexity;

			#region Fields
			private List<Field> fields = new List<Field>();
			/// <summary>
			/// 
			/// </summary>
			public List<Field> Fields	{ get { return fields; } }

			int CalculateSize(BlamLib.CheApe.ProjectState state, Field f, int kUselessPadTypeIndex)
			{
				if (f.TypeIndex == state.kTypeIndexPad ||
					f.TypeIndex == kUselessPadTypeIndex ||
					f.TypeIndex == state.kTypeIndexSkip)
					return f.ToInt();
				//else if (f.TypeIndex == kStructTypeIndex)
				//	((Struct)state.ImportedBlocks[f.ToString()]).Block.Latest.CalculateSize();
				else
					return state.GetFieldSize(f.TypeIndex);
			}

			int CalculateSizeFromArrayStart(BlamLib.CheApe.ProjectState state, ref int current_index, int kUselessPadTypeIndex)
			{
				int array_size = 0;
				int array_count = fields[current_index++].ToInt();

				for (; ; current_index++)
					if (fields[current_index].TypeIndex == state.kTypeIndexArrayStart)
						array_size += CalculateSizeFromArrayStart(state, ref current_index, kUselessPadTypeIndex);
					else if (fields[current_index].TypeIndex == state.kTypeIndexArrayEnd)
						break;
					else
						array_size += CalculateSize(state, fields[current_index], kUselessPadTypeIndex);

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

				int kUselessPadTypeIndex = state.Definition.GetTypeIndex("UselessPad");
				//int kStructTypeIndex = state.Definition.GetTypeIndex("Struct");

				for (int x = 0; x < fields.Count; x++)
				{
					if (fields[x].TypeIndex == state.kTypeIndexArrayStart)
						size += CalculateSizeFromArrayStart(state, ref x, kUselessPadTypeIndex);
					else
						size += CalculateSize(state, fields[x], kUselessPadTypeIndex);
				}

				return size;
			}
			#endregion

			public FieldSet(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
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

		/// <summary>
		/// XmlNode interface for a abstract data block definition
		/// [string: name]
		/// [list: fieldsSets]
		/// </summary>
		new public abstract class Block : BlamLib.CheApe.Import.Block
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 + 
				1 + 
				FieldSet.XmlNodeComplexity;

			#region Fields
			private List<FieldSet> fieldSets = new List<FieldSet>();
			/// <summary>
			/// 
			/// </summary>
			public List<FieldSet> FieldSets	{ get { return fieldSets; } }

			/// <summary>
			/// Gets the latest field set
			/// </summary>
			public FieldSet Latest { get { return fieldSets[fieldSets.Count - 1]; } }
			#endregion

			public Block() {}
			public Block(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "FieldSet") continue;

					s.SaveCursor(n);
					fieldSets.Add(new FieldSet(state, s));
					s.RestoreCursor();
				}
			}

			public override void Read(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				base.Read(state, s);

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "FieldSet") continue;

					s.SaveCursor(n);
					fieldSets.Add(new FieldSet(state, s));
					s.RestoreCursor();
				}
			}
		};

		/// <summary>
		/// XmlNode interface for a tag block definition
		/// [string: name]
		/// [int: maxElements]
		/// [list: fieldSets]
		/// </summary>
		public sealed class TagBlock : Block
		{
			#region MaxElements
			private int maxElements = 1;
			/// <summary>
			/// 
			/// </summary>
			public int MaxElements { get { return maxElements; } }
			#endregion

			public TagBlock() {}
			public TagBlock(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttribute("maxElements", 10, ref maxElements);
			}
		};

		/// <summary>
		/// XmlNode interface for a tag group definition
		/// [string: name]
		/// [string: groupTag]
		/// [string: parentTag]
		/// [int: version]
		/// [list: fieldSets]
		/// </summary>
		new public sealed class TagGroup : BlamLib.CheApe.Import.TagGroup
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = Import.Block.XmlNodeComplexity;

			/// <summary>
			/// Constructs a tag group definition from an xml definition node
			/// </summary>
			/// <param name="state"></param>
			/// <param name="s"></param>
			public TagGroup(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				block = new TagBlock();
				block.DisplayName = name;
				string tempName = nameString + "_block";
				block.Name = blockName = state.Compiler.Strings.Add(tempName);
				block.Read(state, s);
			}
		};

		/// <summary>
		/// XmlNode interface for a struct definition
		/// [string: name]
		/// [string: groupTag]
		/// [list: fieldSets]
		/// </summary>
		public sealed class Struct : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = Import.Block.XmlNodeComplexity;

			#region GroupTag
			private string groupTag = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public string GroupTag	{ get { return groupTag; } }
			#endregion

			#region Block
			private TagBlock block = null;
			/// <summary>
			/// 
			/// </summary>
			public TagBlock Block { get { return block; } }
			#endregion

			public Struct(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttribute("groupTag", ref groupTag);

				block = new TagBlock();
				block.DisplayName = name;
				string tempName = nameString + "_block";
				block.Name = state.Compiler.Strings.Add(tempName);
				block.Read(state, s);
			}
		};


		/// <summary>
		/// Tag Block Definitions found in the XML files
		/// </summary>
		public Dictionary<string, TagBlock> Blocks = new Dictionary<string, TagBlock>();
		/// <summary>
		/// Tag Struct Definitions found in the XML files
		/// </summary>
		public Dictionary<string, Struct> Structs = new Dictionary<string, Struct>();
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

			Blocks.Clear();
			Structs.Clear();
			Groups.Clear();
		}

		protected override int PreprocessXmlNodeComplexity(int complexity)
		{
			complexity = Math.Max(complexity, TagBlock.XmlNodeComplexity);
			complexity = Math.Max(complexity, Struct.XmlNodeComplexity);
			complexity = Math.Max(complexity, TagGroup.XmlNodeComplexity);

			return complexity;
		}

		protected override void ProcessDefinition(XmlNode node, BlamLib.CheApe.ProjectState state, BlamLib.IO.XmlStream s)
		{
			string name_str;

			switch (node.Name)
			{
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

				#region Tag Structs
				case "structs":
					s.SaveCursor(node);
					foreach (XmlNode n in s.Cursor.ChildNodes)
					{
						if (n.Name != "Struct") continue;

						s.SaveCursor(n);
						Struct str = new Struct(state, s);
						s.RestoreCursor();
						name_str = str.ToString();

						try { Structs.Add(name_str, str); }
						catch (ArgumentException) { Debug.LogFile.WriteLine(CheApe.Import.kDuplicateErrorStr, "tag struct definition", name_str); }
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