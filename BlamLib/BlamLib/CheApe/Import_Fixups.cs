/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;

namespace BlamLib.CheApe
{
	partial class Import
	{
		public enum FixupType : short
		{
			/// <summary>
			/// Replace an array of characters at a specific address
			/// </summary>
			String,
			/// <summary>
			/// Replace a pointer to a string with a pointer to a string internal to the cheape cache
			/// </summary>
			StringPtr,
			Memory,
			/// <summary>
			/// Replace a pointer to a different address, at a specific address
			/// </summary>
			Pointer,
			Field,
		};

		/// <summary>
		/// XmlNode interface for a data fixup for a tool executable
		/// [string: name]
		/// [address: guerilla], [address: tool], [address: sapien]
		/// [enum: type], [enum: fieldType]
		/// [string: definition]
		/// </summary>
		public sealed class Fixup : ObjectWithDebugName
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 + 
				1 +	// definition element
				1;		// field element

			#region Guerilla
			private uint addressGuerilla;
			/// <summary>
			/// Address of this fixup in the Guerilla.exe
			/// </summary>
			public uint AddressGuerilla		{ get { return addressGuerilla; } }

			private uint definitionGuerilla;
			/// <summary>
			/// Special fixup data for Guerilla.exe
			/// </summary>
			public uint DefinitionGuerilla	{ get { return definitionGuerilla; } }
			#endregion

			#region Tool
			private uint addressTool;
			/// <summary>
			/// Address of this fixup in the Tool.exe
			/// </summary>
			public uint AddressTool			{ get { return addressTool; } }

			private uint definitionTool;
			/// <summary>
			/// Special fixup data for Tool.exe
			/// </summary>
			public uint DefinitionTool		{ get { return definitionTool; } }
			#endregion

			#region Sapien
			private uint addressSapien;
			/// <summary>
			/// Address of this fixup in the Sapien.exe
			/// </summary>
			public uint AddressSapien		{ get { return addressSapien; } }

			private uint definitionSapien;
			/// <summary>
			/// Special fixup data for Sapien.exe
			/// </summary>
			public uint DefinitionSapien	{ get { return definitionSapien; } }
			#endregion


			#region Type
			private FixupType type;
			/// <summary>
			/// The type of fixup operation this object defines
			/// </summary>
			public FixupType Type	{ get { return type; } }
			#endregion

			#region Definition
			private string definition;
			/// <summary>
			/// 
			/// </summary>
			public string Definition { get { return definition; } }

			/// <summary>
			/// Parses the [definition] to an integer (base 10)
			/// </summary>
			/// <returns></returns>
			/*public*/ int ToInt() { return Convert.ToInt32(definition, 10); }

			/// <summary>
			/// Parses the [definition] to a pointer (base 16 integer)
			/// </summary>
			/// <returns></returns>
			public uint ToPointer() { return Convert.ToUInt32(definition, 16); }

			/// <summary>
			/// Parses the [definition] to a string
			/// </summary>
			/// <returns></returns>
			public string ToStringValue() { return definition; }

			#region DefinitionLength
			private int definitionLength;
			/// <summary>
			/// 
			/// </summary>
			public int DefinitionLength	{ get { return definitionLength; } }
			#endregion

			#endregion

			/// <summary>
			/// Is this fix-up's definition the same for all platforms?
			/// </summary>
			/// <returns></returns>
			public bool IsPlatformIndependent()
			{
				return definition != null;
			}

			#region Field
			private Field field;
			/// <summary>
			/// 
			/// </summary>
			public Field Field { get { return field; } }
			#endregion

			bool ReadSingleDefinition(ProjectState state, IO.XmlStream s, string attr_name, ref uint value)
			{
				string temp = null;
				bool add_def_memory = s.ReadAttributeOpt(attr_name, ref temp);
				if(add_def_memory)
				{
					if (type == FixupType.String || type == FixupType.StringPtr)
														value = state.Compiler.RamAddString(temp, out definitionLength);
					else if (type == FixupType.Memory)	value = state.Compiler.RamAddMemory(temp, out definitionLength);
					else if (type == FixupType.Pointer)	value = Convert.ToUInt32(definition, 16);
				}

				return add_def_memory;
			}

			public Fixup(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string temp = string.Empty;

				s.ReadAttribute("guerilla", 16, ref addressGuerilla);
				s.ReadAttribute("tool", 16, ref addressTool);
				s.ReadAttribute("sapien", 16, ref addressSapien);

				s.ReadAttribute("type", ref type);

				#region Definition
				if (type != FixupType.Field && s.ReadAttributeOpt("definition", ref temp))
				{
					if (type == FixupType.String || type == FixupType.StringPtr)
														definition = state.Compiler.RamAddString(temp, out definitionLength).ToString("X8");
					else if (type == FixupType.Memory)	definition = state.Compiler.RamAddMemory(temp, out definitionLength).ToString("X8");
					else if (type == FixupType.Pointer)	definition = temp;

					definitionGuerilla =
					definitionTool =
					definitionSapien =
						ToPointer();
				}
				else definition = null;
				#endregion

				if(definition == null) foreach (XmlNode node in s.Cursor.ChildNodes)
				{
					if (node.Name == "definition")
					{
						s.SaveCursor(node);

						if (type != FixupType.Field)
						{
							bool is_string = type == FixupType.String;

							// this will be set to false when not all platforms have a definition
							bool add_def_memory = true;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "guerilla", ref definitionGuerilla);
							else				definitionGuerilla = uint.MaxValue;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "tool", ref definitionTool);
							else				definitionTool = uint.MaxValue;
							if (add_def_memory)	add_def_memory = ReadSingleDefinition(state, s, "sapien", ref definitionSapien);
							else				definitionSapien = uint.MaxValue;

							// Houston, we forgot a platform...
							if (!add_def_memory && definitionGuerilla != uint.MaxValue)
							{
								// TODO: error here
							}
						}
						else
						{
							foreach (XmlNode fnode in s.Cursor.ChildNodes)
								if (fnode.Name == "field")
								{
									s.SaveCursor(fnode);
									field = new Field(state, s);
									s.RestoreCursor();
									break;
								}
						}

						s.RestoreCursor();
						break;
					}
				}
			}
		};

		void ProcessFixups(ProjectState state, IO.XmlStream s)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "fixup") continue;

				s.SaveCursor(n);
				var fu = new Fixup(state, s);
				s.RestoreCursor();
				string name_str = fu.ToString();

				try { Fixups.Add(name_str, fu); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "fix-up definition", name_str); }
			}
		}
	};
}