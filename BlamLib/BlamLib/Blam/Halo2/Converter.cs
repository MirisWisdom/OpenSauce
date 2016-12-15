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

namespace BlamLib.Blam.Halo2
{
	public class Converter
	{
		public enum FieldType
		{
			None,

			Byte,
			Word,
			Dword,

			StringId,
			Block,
			Reference,
			Data,
			Struct,

			GeometryBlock,
			AnimationCache,
			BitmapCache,
			SoundCache,
		};

		public class Extra
		{
			public string Name;
			public int Size;
			public char[] Tag;

			public Block _Block;
		};

		public class Field
		{
			private static bool in_geometry_block = false;

			public FieldType Type;
			public int Offset;
			public int OffsetCache;
			public int OffsetAlpha;
			public int OffsetAlphaCache;

			Extra _Extra;

			public void Import(XmlElement node)
			{
				//string name = node.Name;
				string type = node.Attributes["type"].InnerText;

				switch (type)
				{
					case "Byte":
						Type = FieldType.Byte;
						break;

					case "Word":
						Type = FieldType.Word;
						break;

					case "Dword":
						Type = FieldType.Dword;
						break;

					case "StringId":
						Type = FieldType.StringId;
						break;

					case "Block":
						Type = FieldType.Block;
						_Extra = new Extra();
						_Extra._Block = new Block();
						_Extra._Block.Import(node["block"]);
						break;

					case "TagReference":
						Type = FieldType.Reference;
						break;

					case "Data":
						Type = FieldType.Data;
						break;

					case "GeometryBlock":
						in_geometry_block = true;
						Type = FieldType.GeometryBlock;
						break;

					case "AnimationCache":
						Type = FieldType.AnimationCache;
						break;

					case "BitmapCache":
						Type = FieldType.BitmapCache;
						break;

					case "SoundCache":
						Type = FieldType.SoundCache;
						break;

					case "Struct":
						Type = FieldType.Struct;
						_Extra = new Extra();
						_Extra.Name = node.Attributes["name"].InnerText;
						_Extra.Size = Convert.ToInt32(node.Attributes["size"].InnerText);
						_Extra.Tag = node.Attributes["definition"].InnerText.ToCharArray();
						break;

					case "StructEnd":
						in_geometry_block = false;
						Type = FieldType.None;
						break;

					case "ArrayStart":
						Type = FieldType.None;
						break;

					case "ArrayEnd":
						Type = FieldType.None;
						break;
				}

				if(Type == FieldType.Byte ||
					Type == FieldType.Word ||
					Type == FieldType.Dword)
				{
					if(node.Attributes["definition"] != null)
						(_Extra = new Extra()).Size = Convert.ToInt32(node.Attributes["definition"].InnerText);
				}

				if (Type != FieldType.None)
				{
					Offset = Convert.ToInt32(node.Attributes["offsetFile"].InnerText);
					OffsetCache = Convert.ToInt32(node.Attributes["offsetCache"].InnerText);
					OffsetAlpha = Convert.ToInt32(node.Attributes["offsetAlphaFile"].InnerText);
					OffsetAlphaCache = Convert.ToInt32(node.Attributes["offsetAlphaCache"].InnerText);
				}
			}
		};

		public class Block
		{
			public string Name;
			public int Version;
			public int Size;
			public int SizeAlpha;
			public List<Field> Fields = new List<Field>();

			public void Import(XmlElement node)
			{
				Name = node.Attributes["name"].InnerText;
				Version = Convert.ToInt32(node.Attributes["version"].InnerText);
				Size = Convert.ToInt32(node.Attributes["size"].InnerText);
				SizeAlpha = Convert.ToInt32(node.Attributes["sizeAlpha"].InnerText);
				
				XmlElement fields = node["fields"];
				Field f;
				for(int x = 0; x < fields.ChildNodes.Count; x++)
				{
					f = new Field();
					f.Import((XmlElement)fields.ChildNodes[x]);
					if(f.Type != FieldType.None)
						Fields.Add(f);
				}
			}
		};

		public class Group
		{
			public string Name;
			public char[] GroupTag;
			public char[] GroupTagParent;

			public Block _Block;

			public void Import(string path)
			{
				XmlDocument doc = new XmlDocument();
				System.IO.StreamReader sr = new System.IO.StreamReader(path);
				doc.Load(sr);
				sr.Close();

				XmlElement e = doc["tagGroup"];
				Name = e.Attributes["name"].InnerText;
				GroupTag = e.Attributes["groupTag"].InnerText.ToCharArray();

				string tmp = e.Attributes["groupTagParent"].InnerText;
				if (tmp != "????") GroupTagParent = tmp.ToCharArray();
				else GroupTagParent = null;

				_Block = new Block();
				_Block.Import(e["block"]);
			}
		};

		// make a class that can create abstract objects to describe
		// a block\tag that has been streamed from a cache file.
		// Using this we can store tags for later re-use (ie, user interface tags)

		public class BlockAbstraction
		{
			Block desc;

			byte[][] Elements;
			List<BlockAbstraction> Children;

			public BlockAbstraction(Block description)
			{
				desc = description;
			}

			public void Read(Halo2.CacheFile c)
			{
			}

			public void Write(IO.EndianWriter io)
			{
				io.Write(desc.Name, 64);
				io.Write(desc.Size);
			}
		};

		public class Migrate
		{
			#region Cache IO
			Halo2.CacheFile PC_MM;
			Halo2.CacheFile PC_SM;
			Halo2.CacheFile PC_SS;
			Halo2.CacheFile PC;

			Halo2.CacheFile Alpha_MM;
			Halo2.CacheFile Alpha_SM;
			Halo2.CacheFile Alpha;

			Halo2.CacheFile Xbox_MM;
			Halo2.CacheFile Xbox_SM;
			Halo2.CacheFile Xbox_SS;
			Halo2.CacheFile Xbox;

			/// <summary>
			/// Setup file IO
			/// </summary>
			/// <param name="pc">path to pc map file</param>
			/// <param name="alpha">path to alpha map file</param>
			/// <param name="path">path to place xbox map file</param>
			public void Setup(string pc, string alpha, string path)
			{
				PC = new CacheFile(pc);
				Alpha = new CacheFile(alpha);
			}

			public void Setup(BlamPlatform platform, string mm, string sm, string ss)
			{
				if(platform == BlamPlatform.PC)
				{
					PC_MM = new CacheFile(mm);
					PC_SM = new CacheFile(sm);
					PC_SS = new CacheFile(ss);
				}
				else if(platform == BlamPlatform.Xbox)
				{
					Xbox_MM = new CacheFile(mm);
					Xbox_SM = new CacheFile(sm);
					Xbox_SS = new CacheFile(ss);
				}
				else
				{
					Alpha_MM = new CacheFile(mm);
					Alpha_SM = new CacheFile(sm);
				}
			}
			#endregion

			#region Build
			/*
			 * Build process:
			 * - Copy deterministic header fields to xbox header
			 * - Calculate new string id table and data needed for tag fixing
			 * - Calculate new tag paths
			 * - Calculate new unicode strings
			 * 
			 * - Reconstruct cache blocks
			 * 
			 * - Calculate new tag index with tag items
			 * - Fix tags
			 *	@ Rebase pointers
			 *	@ Correct references
			 *	@ Correct string ids
			 *	@ Correct predicted resources
			 *	@ Correct geometry cache blocks
			 * 
			 * - Build nondeterministic header fields
			*/

			#region Build Header
			void BuildHeaderBefore()
			{
			}

			void BuildHeaderAfter()
			{
			}
			#endregion

			#region Build resource data
			void BuildStringIds()
			{
			}

			void BuildTagPaths()
			{
			}

			void BuildMultilingualStrings()
			{
			}
			#endregion

			#region Build Cache Blocks
			void BuildBlocksSound()
			{
			}

			void BuildBlocksRenderModel()
			{
			}

			void BuildBlocksLightmap()
			{
			}

			void BuildBlocksAnimation()
			{
			}

			void BuildBlocksBitmap()
			{
			}
			#endregion

			#region Build Tag buffer
			void BuildSoundGestalt()
			{
			}

			void BuildIndexItems()
			{
			}

			void BuildIndex()
			{
			}

			void BuildTagsPredictedResources()
			{
			}

			void BuildTags()
			{
			}
			#endregion

			/// <summary>
			/// Build the required data
			/// </summary>
			void Build()
			{
				BuildHeaderBefore();

				BuildStringIds();
				BuildTagPaths();
				BuildMultilingualStrings();

				BuildBlocksSound();
				BuildBlocksRenderModel();
				BuildBlocksLightmap();
				BuildBlocksAnimation();
				BuildBlocksBitmap();

				BuildIndex();
				BuildIndexItems();
				BuildSoundGestalt();
				BuildTags();


				BuildHeaderAfter();
			}
			#endregion

			#region Stream
			void StreamResources()
			{
			}

			void StreamCachBlocks()
			{
			}

			void StreamBspTagBuffer()
			{
			}

			void StreamTagBuffer()
			{
			}

			/// <summary>
			/// Stream the built data to a single cache
			/// </summary>
			void Stream()
			{
			}
			#endregion

			/// <summary>
			/// Performs the actual migration
			/// </summary>
			public void Process()
			{
			}
		};
	};
}