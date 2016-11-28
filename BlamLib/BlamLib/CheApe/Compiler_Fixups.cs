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
		/// _enum type;
		/// int16 type_data;
		/// void* address[3];
		/// void* definition[3];
		/// PAD32;
		/// </summary>
		internal sealed class Fixup : Object
		{
			enum InternalFixupType : short
			{
				None,
				ReplaceMemory,
				ReplacePointer,
				ReplaceTagField,
			};

			public const int Size = 2 + 2 + (4*3) + (4*3) + 4;

			Import.Fixup fixup = null;
			public void Reset(Import.Fixup def) { fixup = def; }
			public Fixup() { }
			public Fixup(Import.Fixup def) { fixup = def; }

			InternalFixupType DetermineFixupType()
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.Memory:
						return InternalFixupType.ReplaceMemory;

					case Import.FixupType.StringPtr:
					case Import.FixupType.Pointer:
						return InternalFixupType.ReplacePointer;

					case Import.FixupType.Field:
						return InternalFixupType.ReplaceTagField;

					default: return InternalFixupType.None;
				}
			}

			short DetermineTypeData()
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.Memory:
						return (short)fixup.DefinitionLength;

//					case Import.FixupType.StringPtr:
// 					case Import.FixupType.Pointer:
// 						return 0;

					case Import.FixupType.Field:
						return (short)fixup.Field.TypeIndex;

					default: return 0;
				}
			}

			#region IStreamable Members
			void PlatformRamToStream(Compiler comp,
				IO.EndianWriter stream, 
				uint platform_reference,
				out uint result_reference)
			{
				switch (fixup.Type)
				{
					case Import.FixupType.String:
					case Import.FixupType.StringPtr:
					case Import.FixupType.Memory:
						result_reference = stream.BaseAddress + stream.PositionUnsigned;
						comp.RamToStream(stream, platform_reference, fixup.DefinitionLength);
						break;

					case Import.FixupType.Pointer:
						result_reference = platform_reference;
						break;

					default:
						result_reference = uint.MaxValue;
						break;
				}
			}

			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;
				uint data_pos_guerilla, data_pos_tool, data_pos_sapien;

				InternalFixupType real_type = DetermineFixupType();

				if (real_type == InternalFixupType.ReplaceTagField)
				{
					data_pos_guerilla =
					data_pos_tool =
					data_pos_sapien =
						stream.BaseAddress + stream.PositionUnsigned;
					IField f = comp.ConstructField(fixup.Field);
					f.Write(stream);
				}
				else if(!fixup.IsPlatformIndependent())
				{
					// if the platform address is NULL, the definition is 
					// assumed to be undefined (and thus not processed) 
					// for the target platform

					if (fixup.AddressGuerilla > 0)
						PlatformRamToStream(comp, stream, 
							fixup.DefinitionGuerilla, out data_pos_guerilla);
					else
						data_pos_guerilla = 0;

					if (fixup.AddressTool > 0)
						PlatformRamToStream(comp, stream, 
							fixup.DefinitionTool, out data_pos_tool);
					else
						data_pos_tool = 0;

					if (fixup.AddressSapien > 0)
						PlatformRamToStream(comp, stream, 
							fixup.AddressSapien, out data_pos_sapien);
					else
						data_pos_sapien = 0;
				}
				else
				{
					PlatformRamToStream(comp, stream, 
						fixup.ToPointer(), out data_pos_guerilla);

					data_pos_sapien = data_pos_tool =
						data_pos_guerilla;
				}

				comp.MarkLocationFixup(fixup.Name, stream, true);
				stream.Write((short)real_type);
				stream.Write(DetermineTypeData());
				stream.Write(fixup.AddressGuerilla);
				stream.Write(fixup.AddressTool);
				stream.Write(fixup.AddressSapien);
				stream.Write(data_pos_guerilla);
				stream.Write(data_pos_tool);
				stream.Write(data_pos_sapien);

				stream.Write(uint.MinValue);
			}
			#endregion
		};

		#region Compiler Write helpers
		protected void FixupsToMemoryStream()
		{
			uint base_address = OwnerState.Definition.MemoryInfo.BaseAddress;

			Head.FixupCount = OwnerState.Importer.Fixups.Count;
			if (Head.FixupCount > 0)
			{
				var f = new Fixup();
				Head.FixupAddress = base_address + MemoryStream.PositionUnsigned;
				foreach (Import.Fixup fu in OwnerState.Importer.Fixups.Values)
				{
					AddLocationFixup(fu.Name, MemoryStream, true);
					MemoryStream.Write((int)0);
				}
				foreach (Import.Fixup fu in OwnerState.Importer.Fixups.Values)
				{
					f.Reset(fu);
					f.Write(MemoryStream);
				}

				AlignMemoryStream(Compiler.kDefaultAlignment);
			}
		}
		#endregion
	};
}