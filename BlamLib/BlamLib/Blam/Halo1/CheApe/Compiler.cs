/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Blam.Halo1.CheApe
{
	public sealed partial class Compiler : BlamLib.CheApe.Compiler
	{
		class DataArray : IO.IStreamable
		{
			#region Constants
			public const int MaxValue = 64;
			public const int Size = 0x38;
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

				int real_count = import.Groups.Count;
				stream.Write("dynamic tag groups", false);
				stream.Write((short)DataArray.MaxValue);
				stream.Write((short)Item.Size);
				stream.Write(true);
				stream.Write(true);
				stream.Write((short)0);
				MiscGroups.data.Write(stream);
				stream.Write((short)real_count);//stream.Write((short)Datums.Count);
				stream.Write((short)real_count);//stream.Write((short)Datums.Count);
				stream.Write(real_count);
				stream.WritePointer(stream.PositionUnsigned + 4);

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

		internal override IField ConstructField(Import.Field def)
		{
			return new Field(def);
		}


		DataArray DynamicTagGroups;

		uint CalculateStringPoolBaseAddress()
		{
			return OwnerState.Definition.MemoryInfo.BaseAddress + DataArray.TotalSize + 8; // we pad by 8 bytes in the cache after the data array, so + 8
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

				ScriptingToStream(import);

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