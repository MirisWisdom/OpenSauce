/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam.Halo1.CheApe
{
	partial class Compiler
	{
		#region Type Indicies
		internal int
			TypeIndexString = -1,
			TypeIndexEnum = -1,
			TypeIndexLongFlags = -1,
			TypeIndexWordFlags = -1,
			TypeIndexByteFlags = -1,
			TypeIndexTagReference = -1,
			TypeIndexData = -1,
			TypeIndexBlock = -1,
			TypeIndexShortBlockIndex = -1,
			TypeIndexLongBlockIndex = -1,
			TypeIndexPad = -1,
			TypeIndexSkip = -1,
			TypeIndexArrayStart = -1,
			TypeIndexArrayEnd = -1,
			TypeIndexExplanation = -1,
			TypeIndexCustom = -1,
			TypeIndexTerminator = -1,
			TypeIndexStruct = -1,
			TypeIndexStringId = -1,
			TypeIndexOldStringId = -1;

		private void InitializeTypeIndicies()
		{
			TypeIndexString = OwnerState.Definition.GetTypeIndex("String");
			TypeIndexEnum = OwnerState.Definition.GetTypeIndex("Enum");
			TypeIndexLongFlags = OwnerState.Definition.GetTypeIndex("LongFlags");
			TypeIndexWordFlags = OwnerState.Definition.GetTypeIndex("WordFlags");
			TypeIndexByteFlags = OwnerState.Definition.GetTypeIndex("ByteFlags");
			TypeIndexTagReference = OwnerState.Definition.GetTypeIndex("TagReference");
			TypeIndexData = OwnerState.Definition.GetTypeIndex("Data");
			TypeIndexBlock = OwnerState.Definition.GetTypeIndex("Block");
			TypeIndexShortBlockIndex = OwnerState.Definition.GetTypeIndex("ShortBlockIndex");
			TypeIndexLongBlockIndex = OwnerState.Definition.GetTypeIndex("LongBlockIndex");
			TypeIndexPad = OwnerState.Definition.GetTypeIndex("Pad");
			TypeIndexSkip = OwnerState.Definition.GetTypeIndex("Skip");
			TypeIndexArrayStart = OwnerState.Definition.GetTypeIndex("ArrayStart");
			TypeIndexArrayEnd = OwnerState.Definition.GetTypeIndex("ArrayEnd");
			TypeIndexExplanation = OwnerState.Definition.GetTypeIndex("Explanation");
			TypeIndexCustom = OwnerState.Definition.GetTypeIndex("Custom");
			TypeIndexTerminator = OwnerState.Definition.GetTypeIndex("Terminator");
			TypeIndexStruct = OwnerState.Definition.GetTypeIndex("Struct");
			TypeIndexStringId = OwnerState.Definition.GetTypeIndex("StringId");
			TypeIndexOldStringId = OwnerState.Definition.GetTypeIndex("OldStringId");
		}
		#endregion

		/// <summary>
		/// short field_type;
		/// short pad;
		/// cstring name;
		/// void* definition;
		/// </summary>
		internal sealed class Field : Object, Compiler.IField
		{
			public const int Size = 2 + 2 + 4 + 4;

			Import.Field field = null;
			public void Reset(Import.Field def) { field = def; }
			public Field() { }
			public Field(Import.Field def)	{ field = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				#region TypeIndex
				if (field.TypeIndex == comp.TypeIndexStringId)
					stream.Write(comp.TypeIndexTagReference);
				else if (field.TypeIndex == comp.TypeIndexOldStringId)
					stream.Write(comp.TypeIndexString);
				else
					stream.Write(field.TypeIndex);
				#endregion

				if (field.TypeIndex != comp.TypeIndexPad)
					stream.Write(field.Name);
				else
					stream.Write((int)0);

				if (field.TypeIndex == comp.TypeIndexArrayStart ||
					field.TypeIndex == comp.TypeIndexPad ||
					field.TypeIndex == comp.TypeIndexSkip)
					stream.Write(field.ToInt());
				else if (field.TypeIndex == comp.TypeIndexCustom)
					stream.WriteTag(field.ToString());
				else if (field.TypeIndex == comp.TypeIndexExplanation)
				{
					if (field.Definition != string.Empty)
						stream.Write(field.ToPointer());
					else
						stream.Write(comp.Strings.GetNull());
				}
				else if (field.TypeIndex == comp.TypeIndexByteFlags ||
							field.TypeIndex == comp.TypeIndexEnum ||
							field.TypeIndex == comp.TypeIndexWordFlags ||
							field.TypeIndex == comp.TypeIndexShortBlockIndex ||
							field.TypeIndex == comp.TypeIndexLongFlags ||
							field.TypeIndex == comp.TypeIndexLongBlockIndex ||
							field.TypeIndex == comp.TypeIndexTagReference ||
							field.TypeIndex == comp.TypeIndexData ||
							field.TypeIndex == comp.TypeIndexBlock ||

							field.TypeIndex == comp.TypeIndexStringId)
				{
					comp.AddLocationFixup(field.Definition, stream);

					stream.Write((int)0); // should come back later and write the address
				}
				else if (field.TypeIndex == comp.TypeIndexOldStringId)
					stream.WriteTag(Import.kStringIdGroupTag);
				else if(field.TypeIndex == comp.TypeIndexString && !string.IsNullOrEmpty(field.Definition))
					stream.Write(field.ToInt()); // string's length
				else
					stream.Write((int)0);
			}
			#endregion
		};

		struct FieldsWriter
		{
			List<Import.Field> mFields;

			public FieldsWriter(List<Import.Field> fields) { mFields = fields; }

			static void WriteFieldsRecursive(IO.EndianWriter stream, Compiler comp, List<Import.Field> fields)
			{
				Field temp = new Field();
				foreach (Import.Field f in fields)
				{
					if (f.TypeIndex == comp.TypeIndexStruct)
					{
						var import = comp.OwnerState.Importer as Import;
						Import.TagStruct ts;
						// Assert that the field references a valid definition
						if (import.Structs.TryGetValue(f.Definition, out ts))
							WriteFieldsRecursive(stream, comp, ts.Fields.Fields);
						else
							Debug.Assert.If(false, "Field '{0}' references an undefined tag struct '{1}'",
								comp.Strings[f.Name], f.ToString());
					}
					else
					{
						// StringId support is a hack based on tag_reference fields
						// We internally define the tag reference definition, so we must internally add it
						if (f.TypeIndex == comp.TypeIndexStringId)
							f.ChangeDefinition(Import.kStringIdFieldDefinitionName);

						temp.Reset(f);
						temp.Write(stream);

						// While the initial part of the StringId hack is based on a tag_reference field,
						// we use an additional 32 bits for the string_id handle (instead of playing with
						// the tag_reference's tag_index member). This is seen as padding by the editor
						if (f.TypeIndex == comp.TypeIndexStringId)
						{
							temp.Reset((comp.OwnerState.Importer as Halo1.CheApe.Import).StringIdFieldHandlePadding);
							temp.Write(stream);
						}
					}
				}
			}
			public uint WriteFields(IO.EndianWriter stream, Compiler comp)
			{
				uint fieldsAddress = stream.PositionUnsigned;

				WriteFieldsRecursive(stream, comp, mFields);

				stream.Write(comp.TypeIndexTerminator); stream.Write((int)0); stream.Write((int)0);
				return fieldsAddress;
			}
		}

		/// <summary>
		/// string name;
		/// long flags;
		/// long max_elements;
		/// long element_size;
		/// long pad;
		/// void* fields;
		/// long pad;
		/// void* proc_postprocess;
		/// void* format;
		/// void* delete;
		/// long* byteswap_codes;
		/// </summary>
		internal sealed class TagBlock : Object
		{
			public const int Size = 4 * 11;

			Import.TagBlock tagBlock = null;
			public void Reset(Import.TagBlock def) { tagBlock = def; }
			public TagBlock() { }
			public TagBlock(Import.TagBlock def)	{ tagBlock = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				uint fieldsAddress = new FieldsWriter(tagBlock.Fields).WriteFields(stream, comp);
				comp.MarkLocationFixup(tagBlock.Name, stream, false);

				int flags = 0;

				stream.Write(tagBlock.Name);
				stream.Write(flags);
				stream.Write(tagBlock.MaxElements);
				stream.Write(tagBlock.CalculateSize(comp.OwnerState));
				stream.Write((int)0);
				stream.WritePointer(fieldsAddress);

				// procs
				stream.Write((int)0); stream.Write((int)0); stream.Write((int)0); stream.Write((int)0);

				stream.Write((int)0); // byte swap codes address
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// long flags;
		/// tag group_tag;
		/// tag parent_tag;
		/// short version;
		/// short pad;
		/// void* proc_postprocess;
		/// void* header_block_definition; 
		/// tag child_group_tags[16]; 
		/// short child_count;
		/// pad16;
		/// </summary>
		internal sealed class TagGroup : Object
		{
			public const int Size = (4 * 4) + (2 + 2) + (4 * 2) +
				((4 * 16) + 2 + 2);

			Import.TagGroup tagGroup = null;
			public void Reset(Import.TagGroup def) { tagGroup = def; }
			public TagGroup() { }
			public TagGroup(Import.TagGroup def)	{ tagGroup = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				int flags;

				#region Block
				var tag_block = tagGroup.Block as Import.TagBlock;
				uint fieldsAddress = new FieldsWriter(tag_block.Fields).WriteFields(stream, comp);
				//comp.MarkLocationFixup(tag_block.Name, stream, false);

				flags = 0;

				uint blockAddress = stream.PositionUnsigned;
				stream.Write(tagGroup.BlockName);
				stream.Write(flags);
				stream.Write((int)1); // max elements
				stream.Write(tag_block.CalculateSize(comp.OwnerState));
				stream.Write((int)0);
				stream.WritePointer(fieldsAddress);

				// procs
				stream.Write((int)0); stream.Write((int)0); stream.Write((int)0); stream.Write((int)0);

				stream.Write((int)0); // byte swap codes address
				#endregion

				comp.MarkLocationFixup(tagGroup.Name, stream, false);

				flags = (
						(tagGroup.IsIncludedInTagGroupsChecksum ? 1 << 0 : 0) |
						// 1 ?
						// 2 ?
						(tagGroup.Reloadable ? 1 << 3 : 0) |
						(tagGroup.DebugOnly ? 1 << 4 : 0)
					);

				stream.Write(tagGroup.Name);
				stream.Write(flags);
				#region GroupTag
				if (string.IsNullOrEmpty(tagGroup.GroupTag))
					Debug.LogFile.WriteLine("CheApe: tag_group '{0}' has a bad group-tag...check your XML?");
				stream.WriteTag(tagGroup.GroupTag);
				#endregion
				#region ParentTag
				if (tagGroup.ParentTag != null)
				{
					if (string.IsNullOrEmpty(tagGroup.GroupTag))
						Debug.LogFile.WriteLine("CheApe: tag_group '{0}' has a bad parent group-tag...check your XML?");
					stream.WriteTag(tagGroup.ParentTag);
				}
				else
					stream.Write((int)-1);
				#endregion
				stream.Write(tagGroup.Version); stream.Write((short)0);
				stream.Write((int)0); // post process proc
				stream.WritePointer(blockAddress);
				#region Child group tags
				for (int x = 0; x < 16; x++) stream.Write((int)0);
				stream.Write((int)0);
				#endregion
			}
			#endregion
		};
	};
}