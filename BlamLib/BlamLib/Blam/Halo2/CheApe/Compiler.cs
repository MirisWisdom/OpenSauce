/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo2.CheApe
{
	public sealed class Compiler : BlamLib.CheApe.Compiler
	{
		private class DataArray : IO.IStreamable
		{
			#region Constants
			public const int MaxValue = 64;
			public const int Size = 0x4C;
			public const int TotalSize = (DataArray.MaxValue * Item.Size) + DataArray.Size;
			#endregion

			#region Items
			public class Item : IO.IStreamable
			{
				public const int Size = 2 + 2 + 4;

				public short Header = 0;
				public short Flags = 0;
				public uint Address = 0;

				#region IStreamable Members
				public void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }

				public void Write(IO.EndianWriter stream)
				{
					stream.Write(Header);
					stream.Write(Flags);
					stream.Write(Address);
				}
				#endregion
			};
			public List<Item> Datums = new List<Item>();
			#endregion

			#region IStreamable Members
			public void Read(IO.EndianReader stream) { throw new Exception("The method or operation is not implemented."); }

			public void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				Import import = comp.OwnerState.Importer as Import;

				short real_count = (short)import.Groups.Count;
				var next_index = new DatumIndex((ushort)real_count,
					0); // note that this default value isn't ideal
				stream.Write("dynamic tag groups", false);
				stream.Write(DataArray.MaxValue);
				stream.Write(Item.Size);
				stream.Write((byte)0); // alignment bit
				stream.Write(true); // is valid
				stream.Write((short)0); // flags
				MiscGroups.data.Write(stream);
				stream.Write(uint.MinValue); // allocator
				stream.Write((int)0); // bit vector next index
				stream.Write(Datums.Count); // bit vector length
				stream.Write(Datums.Count); // actual count
				next_index.Write(stream); // next index
				stream.WritePointer(stream.PositionUnsigned + 8);
				stream.Write((int)0); // bit vector pointer

				#region Write tag group datums
				foreach (Import.TagGroup tg in import.Groups.Values)
				{
					stream.Write((short)0); // Header
					stream.Write((short)0); // Flags
					comp.AddLocationFixup(tg.Name, stream, false);
					stream.Write((int)0);
				}
				#endregion

				#region Write null datums
				Item i = new Item();
				int count = DataArray.MaxValue - real_count;
				for (int x = 0; x < count; x++)
					i.Write(stream);
				#endregion
			}
			#endregion
		};

		#region Tag Interface
		#region Type Indicies
		internal int
			TypeIndexCharEnum = -1,
			TypeIndexEnum = -1,
			TypeIndexLongEnum = -1,
			TypeIndexLongFlags = -1,
			TypeIndexWordFlags = -1,
			TypeIndexByteFlags = -1,
			TypeIndexTagReference = -1,
			TypeIndexData = -1,
			TypeIndexBlock = -1,
			TypeIndexCharBlockIndex1 = -1,
			TypeIndexCharBlockIndex2 = -1,
			TypeIndexShortBlockIndex1 = -1,
			TypeIndexShortBlockIndex2 = -1,
			TypeIndexLongBlockIndex1 = -1,
			TypeIndexLongBlockIndex2 = -1,
			TypeIndexPad = -1,
			TypeIndexUselessPad = -1,
			TypeIndexSkip = -1,
			TypeIndexArrayStart = -1,
			TypeIndexArrayEnd = -1,
			TypeIndexExplanation = -1,
			TypeIndexCustom = -1,
			TypeIndexStruct = -1,
			TypeIndexTerminator = -1;

		private void InitializeTypeIndicies()
		{
			TypeIndexCharEnum = OwnerState.Definition.GetTypeIndex("CharEnum");
			TypeIndexEnum = OwnerState.Definition.GetTypeIndex("Enum");
			TypeIndexLongEnum = OwnerState.Definition.GetTypeIndex("LongEnum");
			TypeIndexLongFlags = OwnerState.Definition.GetTypeIndex("LongFlags");
			TypeIndexWordFlags = OwnerState.Definition.GetTypeIndex("WordFlags");
			TypeIndexByteFlags = OwnerState.Definition.GetTypeIndex("ByteFlags");
			TypeIndexTagReference = OwnerState.Definition.GetTypeIndex("TagReference");
			TypeIndexData = OwnerState.Definition.GetTypeIndex("Data");
			TypeIndexBlock = OwnerState.Definition.GetTypeIndex("Block");
			TypeIndexCharBlockIndex1 = OwnerState.Definition.GetTypeIndex("CharBlockIndex1");
			TypeIndexCharBlockIndex2 = OwnerState.Definition.GetTypeIndex("CharBlockIndex2");
			TypeIndexShortBlockIndex1 = OwnerState.Definition.GetTypeIndex("ShortBlockIndex1");
			TypeIndexShortBlockIndex2 = OwnerState.Definition.GetTypeIndex("ShortBlockIndex2");
			TypeIndexLongBlockIndex1 = OwnerState.Definition.GetTypeIndex("LongBlockIndex1");
			TypeIndexLongBlockIndex2 = OwnerState.Definition.GetTypeIndex("LongBlockIndex2");
			TypeIndexPad = OwnerState.Definition.GetTypeIndex("Pad");
			TypeIndexUselessPad = OwnerState.Definition.GetTypeIndex("UselessPad");
			TypeIndexSkip = OwnerState.Definition.GetTypeIndex("Skip");
			TypeIndexArrayStart = OwnerState.Definition.GetTypeIndex("ArrayStart");
			TypeIndexArrayEnd = OwnerState.Definition.GetTypeIndex("ArrayEnd");
			TypeIndexExplanation = OwnerState.Definition.GetTypeIndex("Explanation");
			TypeIndexCustom = OwnerState.Definition.GetTypeIndex("Custom");
			TypeIndexStruct = OwnerState.Definition.GetTypeIndex("Struct");
			TypeIndexTerminator = OwnerState.Definition.GetTypeIndex("Terminator");
		}
		#endregion

		/// <summary>
		/// cstring name;
		/// long flags;
		/// long alignment_bit;
		/// long max_size;
		/// void* proc_byte_swap;
		/// void* proc_copy;
		/// </summary>
		new sealed class TagData : BlamLib.CheApe.Compiler.TagData
		{
			public new const int Size = 4 * 6;

			public TagData() { }
			public TagData(Import.TagData def) : base(def) { }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				comp.MarkLocationFixup(tagData.Name, stream, false);

				stream.Write(tagData.Name);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write(tagData.MaxSize);
				stream.Write((int)0); // should come back later and write the address of the proc via a lookup table from a xml or text file
				stream.Write((int)0);
			}
			#endregion
		};

		/// <summary>
		/// short field_type;
		/// pad16;
		/// cstring name;
		/// void* definition;
		/// tag extra;
		/// </summary>
		internal sealed class Field : Object, Compiler.IField
		{
			public const int Size = 2 + 2 + 4 + 4 + 4;

			Import.Field field = null;
			public void Reset(Import.Field def) { field = def; }
			public Field() { }
			public Field(Import.Field def) { field = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				stream.Write(field.TypeIndex);

				#region name
				if (field.TypeIndex != comp.TypeIndexPad || field.TypeIndex != comp.TypeIndexUselessPad)
					stream.Write(field.Name);
				else
					stream.Write((int)0);
				#endregion

				#region definition
				if (field.TypeIndex == comp.TypeIndexArrayStart ||
					field.TypeIndex == comp.TypeIndexPad ||
					field.TypeIndex == comp.TypeIndexUselessPad ||
					field.TypeIndex == comp.TypeIndexSkip)
					stream.Write(field.ToInt());
				else if (field.TypeIndex == comp.TypeIndexCustom)
					stream.WriteTag(field.ToString());
				else if (field.TypeIndex == comp.TypeIndexExplanation)
				{
					if (field.Definition != string.Empty)
						stream.Write(field.ToPointer());
				}
				else if (	field.TypeIndex == comp.TypeIndexByteFlags ||
							field.TypeIndex == comp.TypeIndexCharEnum ||
							field.TypeIndex == comp.TypeIndexEnum ||
							field.TypeIndex == comp.TypeIndexLongEnum ||
							field.TypeIndex == comp.TypeIndexWordFlags ||
							field.TypeIndex == comp.TypeIndexCharBlockIndex1 ||
							field.TypeIndex == comp.TypeIndexCharBlockIndex2 ||
							field.TypeIndex == comp.TypeIndexShortBlockIndex1 ||
							field.TypeIndex == comp.TypeIndexShortBlockIndex2 ||
							field.TypeIndex == comp.TypeIndexLongFlags ||
							field.TypeIndex == comp.TypeIndexLongBlockIndex1 ||
							field.TypeIndex == comp.TypeIndexLongBlockIndex2 ||
							field.TypeIndex == comp.TypeIndexTagReference ||
							field.TypeIndex == comp.TypeIndexData ||
							field.TypeIndex == comp.TypeIndexBlock ||
							field.TypeIndex == comp.TypeIndexStruct)
				{
					comp.AddLocationFixup(field.Definition, stream);

					stream.Write((int)0); // should come back later and write the address
				}
				else
					stream.Write((int)0);
				#endregion

				stream.Write((int)0); // tag extra
			}
			#endregion
		};

		internal override IField ConstructField(Import.Field def)
		{
			return new Field(def);
		}

		/// <summary>
		/// void* version.fields;
		/// long version.index;
		/// void* version.upgrade_proc;
		/// pad32;
		/// size_t version.size_of; // set to -1 when unused, else set to a value to override the field-set's calculated size_of
		/// size_t size;
		/// int alignment_bit;
		/// int parent_version_index; // sometimes -1, like in animation
		/// void* fields;
		/// cstring sizeof_string;
		/// 
		/// cstring byteswap_name;
		/// size_t byteswap_size;
		/// long* byteswap_codes;
		/// tag byteswap_sig;
		/// bool init'd;
		/// pad24;
		/// 
		/// FLAG(0) = has_tag_blocks
		/// FLAG(1) = has_structs
		/// FLAG(2) = has_generate_default_proc (checked in tool @52D04B)
		/// FLAG(3) = has_old_string_id
		/// FLAG(4) = has_useless_pad_size
		/// FLAG(5) = has_cache_size
		/// ulong runtime_flags;
		/// void* runtime_fields;
		/// size_t runtime_size; // for cache builds and such
		/// size_t runtime_total_useless_pad;
		/// </summary>
		internal sealed class FieldSet : Object
		{
			public const int Size = (4 * 10) + (4 * 9);

			Import.FieldSet fieldSet = null;
			public void Reset(Import.FieldSet def) { fieldSet = def; }
			public FieldSet() { }
			public FieldSet(Import.FieldSet def) { fieldSet = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				#region Fields
				#region Byte swap codes
				/*
				uint fieldBSCodesAddress = stream.PositionUnsigned;
				stream.Write(comp.OwnerState.Definition.FieldTypes[comp.TypeIndexArrayStart].ByteSwapCodes[0]);
				stream.Write((int)1); // array count
				foreach (Import.Field f in fieldSet.Fields)
				{
					if (f.TypeIndex == comp.TypeIndexUselessPad) continue;
					else if (f.TypeIndex == comp.TypeIndexPad || f.TypeIndex == comp.TypeIndexSkip)
						stream.Write(f.ToInt());
					else
						foreach (int x in comp.OwnerState.Definition.FieldTypes[f.TypeIndex].ByteSwapCodes)
							if (x != 0)
								stream.Write(x);
				}
				stream.Write(comp.OwnerState.Definition.FieldTypes[comp.TypeIndexArrayEnd].ByteSwapCodes[0]);
				*/
				#endregion

				uint fieldsAddress = stream.PositionUnsigned;
				Field temp = new Field();
				foreach (Import.Field f in fieldSet.Fields)
				{
					temp.Reset(f);
					temp.Write(stream);
				}
				stream.Write(comp.TypeIndexTerminator); stream.Write((int)0); stream.Write((int)0); stream.Write((int)0);
				#endregion

				int field_set_size = fieldSet.CalculateSize(comp.OwnerState);

				stream.WritePointer(fieldsAddress);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write(uint.MaxValue);
				stream.Write(field_set_size);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.WritePointer(fieldsAddress);
				stream.Write(comp.Strings.GetNull());

				stream.Write(comp.Strings.GetNull());
				stream.Write(field_set_size);
				stream.Write((int)0);//stream.WritePointer(fieldBSCodesAddress);
				MiscGroups.bysw.Write(stream);
				stream.Write((int)0/*1*/);

				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
			}
			#endregion
		};

		/// <summary>
		/// cstring display_name;
		/// cstring name;
		/// long flags;
		/// long max_elements;
		/// cstring max_elements_name;
		/// void* field_sets;
		/// long field_set_count;
		/// void* fields_set_latest;
		/// pad32;
		/// void* proc_postprocess;
		/// void* proc_format;
		/// pad32; // proc generate default block
		/// pad32; // proc dispose
		/// pad32; // proc handle invalid
		/// </summary>
		internal sealed class TagBlock : Object
		{
			public const int Size = 4 * 14;

			Import.TagBlock tagBlock = null;
			public uint RuntimeAddress;
			public void Reset(Import.TagBlock def) { tagBlock = def; }
			public TagBlock() { }
			public TagBlock(Import.TagBlock def) { tagBlock = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				#region FieldSets
				uint fieldSetsAddress = stream.PositionUnsigned;
				FieldSet temp = new FieldSet();
				Import.FieldSet fs;
				int x;
				for (x = 0; x < tagBlock.FieldSets.Count - 1; x++)
				{
					fs = tagBlock.FieldSets[x];
					temp.Reset(fs);
					temp.Write(stream);
				}

				uint fieldSetLatestAddress = stream.PositionUnsigned;
				fs = tagBlock.FieldSets[x];
				temp.Reset(fs);
				temp.Write(stream);
				#endregion

				comp.MarkLocationFixup(tagBlock.Name, stream, false);

				RuntimeAddress = stream.PositionUnsigned;
				stream.Write(tagBlock.Name);
				stream.Write(tagBlock.DisplayName);
				stream.Write((int)0);
				stream.Write(tagBlock.MaxElements);
				stream.Write(comp.Strings.GetNull());
				stream.WritePointer(fieldSetsAddress);
				stream.Write(tagBlock.FieldSets.Count);
				stream.WritePointer(fieldSetLatestAddress);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// tag group;
		/// cstring display name;
		/// void* block_definition;
		/// </summary>
		internal sealed class Struct : Object
		{
			public const int Size = 4 * 4;

			Import.Struct tagStruct = null;
			public void Reset(Import.Struct def) { tagStruct = def; }
			public Struct() { }
			public Struct(Import.Struct def) { tagStruct = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				TagBlock tb = new TagBlock((Import.TagBlock)tagStruct.Block);
				tb.Write(stream);

				comp.MarkLocationFixup(tagStruct.Name, stream, false);

				stream.Write(tagStruct.Name);
				stream.WriteTag(tagStruct.GroupTag);
				stream.Write(tagStruct.Block.DisplayName);
				stream.WritePointer(tb.RuntimeAddress);
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// long flags;
		/// tag group_tag;
		/// tag parent_tag;
		/// short version;
		/// bool init'd;
		/// pad8;
		/// pad32; // proc postprocess
		/// pad32; // proc save preprocess
		/// pad32; // proc postprocess for sync
		/// pad32; // flags
		/// void* header_block_definition; 
		/// tag child_group_tags[16]; 
		/// short child_count;
		/// pad16;
		/// cstring default_tag_path;
		/// </summary>
		internal sealed class TagGroup : Object
		{
			public const int Size = (4 * 4) + (2 + 2) + (4 * 4) + (4 * 1) +
				((4 * 16) + 2 + 2) + 4;

			Import.TagGroup tagGroup = null;
			public void Reset(Import.TagGroup def) { tagGroup = def; }
			public TagGroup() { }
			public TagGroup(Import.TagGroup def) { tagGroup = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				TagBlock tb = new TagBlock((Import.TagBlock)tagGroup.Block);
				tb.Write(stream);

				comp.MarkLocationFixup(tagGroup.Name, stream, false);

				stream.Write(tagGroup.Name);
				stream.Write((int)0);
				stream.WriteTag(tagGroup.GroupTag);
				if (tagGroup.ParentTag != null)
					stream.WriteTag(tagGroup.ParentTag);
				else
					stream.Write((int)-1);
				stream.Write(tagGroup.Version);
				stream.Write((short)1); // init'd
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.Write((int)0);
				stream.WritePointer(tb.RuntimeAddress);
				for (int x = 0; x < 17; x++) stream.Write((int)0); // child group tags
				stream.Write((int)0); // we don't support that shit, gtfo
				stream.Write(comp.Strings.GetNull());
			}
			#endregion
		};
		#endregion

		#region Script Interface
		#endregion


		DataArray DynamicTagGroups;

		uint CalculateStringPoolBaseAddress()
		{
			return OwnerState.Definition.MemoryInfo.BaseAddress + DataArray.TotalSize + 4; // we pad by 4 bytes in the cache after the data array, so + 4
		}

		internal Compiler(Project.ProjectState state) : base(state.Engine)
		{
			OwnerState = state;

			DynamicTagGroups = new DataArray();
			Strings = new Util.StringPool(true, CalculateStringPoolBaseAddress());

			InitializeTypeIndicies();
		}

		public override void Reset()
		{
			base.Reset();

			DynamicTagGroups = new DataArray();
			Strings = new Util.StringPool(true, CalculateStringPoolBaseAddress());
		}

		public override void Write(IO.EndianWriter stream)
		{
			const int k_alignment = Compiler.kDefaultAlignment;
			int align;
			using (var mem = InitializeMemoryStream())
			{
				DynamicTagGroups.Write(MemoryStream); // write the data array to the stream
				MemoryStream.Write((int)0); MemoryStream.Write((int)0); // alignment

				StringPoolToMemoryStream();

				Import import = OwnerState.Importer as Import;

				FixupsToMemoryStream();

				#region Scripting
				{
				}
				#endregion

				EnumerationsToMemoryStream();

				TagReferencesToMemoryStream();

				TagDataToMemoryStream();

				#region TagBlock
				TagBlock tb = new TagBlock();
				foreach (Import.TagBlock tagb in import.Blocks.Values)
				{
					tb.Reset(tagb);
					tb.Write(MemoryStream);
				}
				#endregion

				#region Struct
				Struct strct = new Struct();
				foreach (Import.Struct tags in import.Structs.Values)
				{
					strct.Reset(tags);
					strct.Write(MemoryStream);
				}
				#endregion

				#region TagGroup
				TagGroup tg = new TagGroup();
				foreach (Import.TagGroup tagg in import.Groups.Values)
				{
					tg.Reset(tagg);
					tg.Write(MemoryStream);
				}
				#endregion

				PostprocessWritebacks();

				// Create header
				PostprocessHeaderThenStream(stream, CalculateStringPoolBaseAddress());

				mem.WriteTo(stream.BaseStream); // write all the data that will be read into memory from a tool to the file
			}

			align = k_alignment - (stream.Length % k_alignment);
			if (align != k_alignment) stream.Write(new byte[align]);
		}
	};
}