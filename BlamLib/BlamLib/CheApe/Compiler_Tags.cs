/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.CheApe
{
	partial class Compiler
	{
		/// <summary>
		/// long length;
		/// cstring* elements;
		/// </summary>
		internal sealed class StringList : Object
		{
			public const int Size = 4 * 2;

			Import.StringList stringList = null;
			public void Reset(Import.StringList def) { stringList = def; }
			public StringList() { }
			public StringList(Import.StringList def) { stringList = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				uint elementsAddress = stream.PositionUnsigned;
				// write string list's element's addresses
				foreach (uint i in stringList.Elements) stream.Write(i);

				comp.MarkLocationFixup(stringList.Name, stream, true);

				stream.Write(stringList.Elements.Count);
				stream.WritePointer(elementsAddress);
			}
			#endregion
		};

		/// <summary>
		/// long flags;
		/// tag group_tag;
		/// tag* group_tag_list;
		/// </summary>
		internal sealed class TagReference : Object
		{
			public const int Size = 4 * 3;

			Import.TagReference tagRef = null;
			public void Reset(Import.TagReference def) { tagRef = def; }
			public TagReference() { }
			public TagReference(Import.TagReference def) { tagRef = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				uint elementsAddress = 0;
				int flags = (
						(tagRef.IsNonResolving ? 1<<1 : 0)
					);

				if (tagRef.Elements.Count > 1)
				{
					elementsAddress = stream.PositionUnsigned;
					foreach (string i in tagRef.Elements)
						stream.WriteTag(i);

					comp.MarkLocationFixup(tagRef.Name, stream, true);
					stream.Write(flags);
					stream.Write((int)-1);
					stream.WritePointer(elementsAddress);
				}
				else
				{
					comp.MarkLocationFixup(tagRef.Name, stream, true);
					stream.Write(flags);
					stream.WriteTag(tagRef.Elements[0]);
					stream.Write((int)0);
				}
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// long flags;
		/// long max_size;
		/// void* proc_byte_swap;
		/// </summary>
		internal class TagData : Object
		{
			public const int Size = 4 * 4;

			protected Import.TagData tagData;
			public void Reset(Import.TagData def) { tagData = def; }
			public TagData() { }
			public TagData(Import.TagData def) { tagData = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				comp.MarkLocationFixup(tagData.Name, stream, false);

				int flags = (
						(tagData.IsNeverStreamed ? 1 << 0 : 0) |
						(tagData.IsTextData ? 1 << 1 : 0) |
						(tagData.IsDebugData ? 1 << 2 : 0)
					);

				stream.Write(tagData.Name);
				stream.Write(flags);
				stream.Write(tagData.MaxSize);
				stream.Write((int)0); // should come back later and write the address of the proc via a lookup table from a xml or text file
			}
			#endregion
		};

		#region Compiler Write helpers
		protected void EnumerationsToMemoryStream()
		{
			var sl = new StringList();
			foreach (Import.StringList slist in OwnerState.Importer.Enums.Values)
			{
				sl.Reset(slist);
				sl.Write(MemoryStream);
			}

			foreach (Import.StringList slist in OwnerState.Importer.Flags.Values)
			{
				sl.Reset(slist);
				sl.Write(MemoryStream);
			}
		}

		protected void TagReferencesToMemoryStream()
		{
			var tr = new TagReference();
			foreach (Import.TagReference tagr in OwnerState.Importer.References.Values)
			{
				tr.Reset(tagr);
				tr.Write(MemoryStream);
			}
		}

		protected void TagDataToMemoryStream()
		{
			var td = new TagData();
			foreach (Import.TagData tagd in OwnerState.Importer.Data.Values)
			{
				td.Reset(tagd);
				td.Write(MemoryStream);
			}
		}
		#endregion
	};
}