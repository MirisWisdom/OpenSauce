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

namespace BlamLib.Blam.Halo1.CheApe
{
	internal sealed class Import : BlamLib.CheApe.Import
	{
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
			private List<Field> fields = new List<Field>();
			/// <summary>
			/// 
			/// </summary>
			public List<Field> Fields	{ get { return fields; } }

			int CalculateSize(BlamLib.CheApe.ProjectState state, Field f)
			{
				if (f.TypeIndex == state.kTypeIndexPad || f.TypeIndex == state.kTypeIndexSkip)
					return f.ToInt();
				else
					return state.GetFieldSize(f.TypeIndex);
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
			#endregion

			public TagBlock() : base() {}
			public TagBlock(BlamLib.CheApe.ProjectState state, IO.XmlStream s) : base(state, s)
			{
				s.ReadAttribute("maxElements", 10, ref maxElements);
				s.ReadAttributeOpt("dontReadChildren", ref DontReadChildren);

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "field") continue;

					s.SaveCursor(n);
					fields.Add(new Field(state, s));
					s.RestoreCursor();
				}
			}

			public override void Read(BlamLib.CheApe.ProjectState state, IO.XmlStream s)
			{
				base.Read(state, s);

				foreach(XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "field") continue;

					s.SaveCursor(n);
					fields.Add(new Field(state, s));
					s.RestoreCursor();
				}
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