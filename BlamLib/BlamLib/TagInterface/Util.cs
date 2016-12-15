/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using GameMan = BlamLib.Managers.GameManager;

namespace BlamLib.TagInterface
{
	#region Definition File
	[IO.Class((int)IO.TagGroups.Enumerated.DefinitionFile, 0)]
	public sealed class DefinitionFile
	{
		public const string Ext = "deff";

		#region Item
		public struct Item : IO.IStreamable
		{
			#region Type
			private FieldType type;
			public FieldType Type { get { return type; } }
			#endregion

			#region Flags
			private int flags;
			public int Flags { get { return flags; } }
			#endregion

			public static readonly Item Null = new Item(FieldType.None, -1);
			public Item(FieldType t, int f) { type = t; flags = f; }

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				type = (FieldType)s.ReadByte();
				flags = s.ReadInt32();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write((byte)type);
				s.Write(flags);
			}
			#endregion
		};
		#endregion

		private Definition def = null;
		private BlamVersion engine = BlamVersion.Unknown;
		private short header = 0;
		private short version = 0;
		private TagGroup groupTag = null;

		private IO.EndianReader InputStream = null;
		private IO.EndianWriter OutputStream = null;


		public DefinitionFile(TagGroup tag, GameMan.Namespace g, GameMan.Platform p)
		{
			engine = GameMan.ToBlamVersion(g, p);
			groupTag = tag;
		}

		public DefinitionFile(TagGroup tag, BlamVersion g)
		{
			engine = g;
			groupTag = tag;
		}

		public void Setup(Definition definition, TagGroup tag, short head, short ver)
		{
			def = definition;
			groupTag = tag;
			header = head;
			version = ver;
		}

		public void SetupCreate()
		{
			GameMan.Namespace n;
			GameMan.Platform p;
			GameMan.FromBlamVersion(engine, out n, out p);

			OutputStream = new BlamLib.IO.EndianWriter(
				GameMan.GetPlatformFolderPath(
					n, 
					p, 
					BlamLib.Managers.GameManager.PlatformFolder.Definitions_TDF
					) + groupTag.Name + "." + Ext, 
				BlamLib.IO.EndianState.Little, this, true);
		}

		public void SetupParse()
		{
			GameMan.Namespace n;
			GameMan.Platform p;
			GameMan.FromBlamVersion(engine, out n, out p);

			InputStream = new BlamLib.IO.EndianReader(
				GameMan.GetPlatformFolderPath(
				n, 
				p, 
				BlamLib.Managers.GameManager.PlatformFolder.Definitions_TDF
				) + groupTag.Name + "." + Ext, 
				BlamLib.IO.EndianState.Little, this);
		}

		public void Close()
		{
			if (InputStream != null) { InputStream.Close(); InputStream = null; }
			if (OutputStream != null) { OutputStream.Close(); OutputStream = null; }
		}

		#region Creating
		public void Create()
		{
			Debug.Assert.If(OutputStream != null,
				"Can't create a definition file when we're not initialized for that mode");

			OutputStream.WriteTag(Ext);
			groupTag.Write(OutputStream);
			OutputStream.Write((uint)engine);
			OutputStream.Write(version);
			OutputStream.Write(header);

			Create(def);
		}

		private void Create(Definition def)
		{
			foreach (Field fi in def)
			{
				fi.ToDefinitionItem().Write(OutputStream);
				if (fi.GetType().IsGenericType)
				{
					//if (fi.GetType().GetGenericTypeDefinition() == typeof(Block<>) ||
					//	fi.GetType().GetGenericTypeDefinition() == typeof(Struct<>))
					if (fi.FieldType == FieldType.Block ||
						fi.FieldType == FieldType.Struct)
						Create(fi.FieldValue as Definition); // Block<T>\Struct<T>'s FieldValue is its Definition, remember
				}
			}

			// Marks the end of a definition
			new Item(FieldType.Terminator, 0).Write(OutputStream);
		}
		#endregion

		#region Parsing
		Item currentField = new Item();
		public Definition Parse()
		{
			Debug.Assert.If(InputStream != null,
				"Can't parse a definition file when we're not initialized for that mode");

			InputStream.ReadTag(); // Ext
			//groupTag = InputStream.ReadTag();
			engine = (BlamVersion)InputStream.ReadUInt32();
			version = InputStream.ReadInt16();
			header = InputStream.ReadInt16();

			this.def = new Definition();
			//this.def.SetOwnerObject(this.def); // we want fields of this definition to point to the definition as their parent
			return Parse(null);
		}

		private Definition Parse(Definition parent)
		{
			Definition def;
			if (parent == null)
				def = this.def;
			else
				def = new Definition();

			Field temp = null;
			currentField.Read(InputStream);
			IStructureOwner struct_owner = (IStructureOwner)def;
			while (currentField.Type != FieldType.Terminator)
			{
				def.Add(temp = FieldUtil.CreateFromDefinitionItem(engine, struct_owner.OwnerObject, currentField));
				
				if (currentField.Type == FieldType.Block ||
					currentField.Type == FieldType.Struct)
					Parse(temp.FieldValue as Definition);

				currentField.Read(InputStream);
			}

			return def;
		}
		#endregion
	};
	#endregion

#if !DEBUG_PUBLIC
	#region Definition Conversion File
	[IO.Class((int)IO.TagGroups.Enumerated.DefinitionConversionFile, 0)]
	public sealed class DefinitionConversionFile
	{
		public const string Ext = "decf";

		#region Item
		public class Item : IO.IStreamable
		{
			#region Desc
			public struct Desc : IO.IStreamable
			{
				#region Type
				private FieldType type;
				public FieldType Type { get { return type; } }
				#endregion

				#region FieldIndex
				private byte fieldIndex;
				public byte FieldIndex { get { return fieldIndex; } }
				#endregion

				#region Flags
				private int flags;
				public int Flags { get { return flags; } }
				#endregion

				/// <summary>
				/// parameterless constructor...mb
				/// </summary>
				/// <param name="dummy"></param>
				public Desc(bool dummy) { type = FieldType.None; fieldIndex = 0xFF; flags = 0; }
				public Desc(FieldType t, byte field, int flags) { type = t; fieldIndex = field; this.flags = flags; }

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					type = (FieldType)s.ReadByte();
					fieldIndex = s.ReadByte();
					flags = s.ReadInt32();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write((byte)type);
					s.Write(fieldIndex);
					s.Write(flags);
				}
				#endregion
			};
			#endregion

			public Desc Stubbs;
			public Desc Halo1;
			public Desc Halo2;
			public Desc Halo3;

			/// <summary>
			/// Gets the field index of this item
			/// to use based on the conversion from
			/// <paramref name="source"/> to <paramref name="target"/>
			/// </summary>
			/// <param name="source">currently unused</param>
			/// <param name="target">Engine version the definition is being converted to</param>
			/// <returns>Field index to use</returns>
			internal int IndexOf(BlamVersion source, BlamVersion target)
			{
				if ((target & BlamVersion.Halo1) != 0)		return Halo1.FieldIndex;
				else if ((target & BlamVersion.Halo2) != 0)	return Halo2.FieldIndex;
				else if ((target & BlamVersion.Halo3) != 0)	return Halo3.FieldIndex;
				else if ((target & BlamVersion.Stubbs) != 0)return Stubbs.FieldIndex;

				return -1;
			}

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				Stubbs.Read(s);
				Halo1.Read(s);
				Halo2.Read(s);
				Halo3.Read(s);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				Stubbs.Write(s);
				Halo1.Write(s);
				Halo2.Write(s);
				Halo3.Write(s);
			}
			#endregion
		};

		public class ItemList : System.Collections.ArrayList
		{
			#region Add
			public int Add(Item value) { return base.Add(value); }

			public int Add(ItemList value) { return base.Add(value); }
			#endregion

			#region Insert
			public void Insert(int index, Item value) { base.Insert(index, value); }

			public void Insert(int index, ItemList value) { base.Insert(index, value); }
			#endregion

			#region Indexers
			public new Item this[int index]
			{
				get { return base[index] as Item; }
				set { base[index] = value; }
			}

			public ItemList GetList(int index) { return base[index] as ItemList; }
			public Item GetItem(int index) { return base[index] as Item; }
			#endregion

			#region Util
			public bool IsItem(int index) { return base[index] is Item; }
			public bool IsItemList(int index) { return base[index] is ItemList; }
			#endregion
		};
		#endregion

		//private BlamVersion engineSource = BlamVersion.Unknown;
		//private Definition defSource = null;

		//private BlamVersion engineTarget = BlamVersion.Unknown;
		//private Definition defTarget = null;

		IO.EndianReader InputStream = null;
		private ItemList commands = new ItemList();

		#region Create
		#endregion

		#region Parse
		public void Parse()
		{
			InputStream.ReadTag(); // Ext tag header
			InputStream.ReadInt32(); // Flags

			InputStream.ReadTag(); // Stubbs
			InputStream.ReadTag(); // Halo1
			InputStream.ReadTag(); // Halo2
			InputStream.ReadTag(); // Halo3

			InputStream.ReadInt16(); // IDs count
			InputStream.ReadInt16(); // Blocks count

			Parse(commands);
		}

		Item field = null;
		ItemList block = null;
		public void Parse(ItemList list)
		{
			FieldType ft = FieldType.None;
			while ((ft = (FieldType)InputStream.ReadByte()) != FieldType.ArrayEnd) // DECF terminator
			{
				switch (ft)
				{
					case FieldType.Custom: // DECF field
						field = new Item();
						list.Add(field);
						field.Read(InputStream);
						break;

					case FieldType.Block:
						block = new ItemList();
						list.Add(block);
						Parse(block);
						break;

					case FieldType.Data:
						break;

					default:
						break;
				}
			}
		}
		#endregion

		public void Convert()
		{
		}
	};
	#endregion
#endif
}