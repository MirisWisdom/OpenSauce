/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#if !NO_SCRIPTING
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Scripting
{
	#region ScriptNode
	[Flags]
	public enum NodeFlags : short
	{
		/// <summary>
		/// Node describes a script-primitive value (ie, string_id, object_name, etc)
		/// </summary>
		Primitive =			1,
		/// <summary>
		/// Node is used as a return value
		/// </summary>
		ReturnFuncValue =	2, // Call predicate
		/// <summary>
		/// Node represents a variable reference to either a
		/// </summary>
		Variable =			4,
		/// <summary>
		/// The node is part of a range of expressions
		/// </summary>
		Concatenated =		8,
		/// <summary>
		/// This node actually represents a variable which is a parameter of the 
		/// node's owner script. Parameter index is stored as a Int32 in "data"
		/// </summary>
		/// <remarks>Added in Halo3</remarks>
		ScriptParameter =	16,
	};

	/// <summary>
	/// Single script node
	/// </summary>
	public class ScriptNode
	{
		public const short ValueObjectDatum = -6; // 0xFFFA
		public const short ObjectTypeDatum = -16512; // 0xBF80 test with (object type << 1)

		#region NodeIndex
		int nodeIndex;
		/// <summary>
		/// The node index
		/// </summary>
		public int NodeIndex
		{
			get { return nodeIndex; }
			set { nodeIndex = value; }
		}

		/// <summary>
		/// The node that precedes this one
		/// </summary>
		public ScriptNode Previous = null;
		/// <summary>
		/// The node that follows this one
		/// </summary>
		public ScriptNode Next = null;
		/// <summary>
		/// The first node in the expression that precedes this one 
		/// (assuming this node is the start of an expression itself)
		/// </summary>
		public ScriptNode PreviousSet = null;
		/// <summary>
		/// The first node in the expression that follows this one 
		/// (assuming this node is the start of an expression itself)
		/// </summary>
		public ScriptNode NextSet = null;

		[System.Diagnostics.Conditional("DEBUG")]
		public void DumpGraph(System.IO.StreamWriter s, int indent, Util.StringPool strings)
		{
			string indent_string = string.Empty;
			if (indent != 0)
			{
				System.Text.StringBuilder pad = new System.Text.StringBuilder(indent);
				for (int x = 0; x < indent; x++) pad.Append('\t');
				indent_string = pad.ToString();
			}
			s.Write("{0}Node:{1:X8}", indent_string, this.nodeIndex);
			s.Write("\tNext Set:{0:X4}", NextSet != null ? NextSet.nodeIndex : (short)-1);
			s.Write("\tPrev:{0:X4}", Previous != null ? Previous.nodeIndex : (short)-1);
			s.Write("\tNext:{0:X4}", Next != null ? Next.nodeIndex : (short)-1);
			s.WriteLine();
			s.Write("{0}  {1}", indent_string, HexDump());
			string string_data;
			if (Util.Flags.Test(pointerType, (short)1) && strings.TryAndGet((uint)pointer, out string_data))
				s.Write("\t{0}", string_data);
			s.WriteLine();

			if (Next != null) Next.DumpGraph(s, indent + 1, strings);
			if (NextSet != null) NextSet.DumpGraph(s, indent, strings);
		}
		#endregion

		#region Node body
		#region Index
		public const int OffsetIndex = 0;
		short index;
		/// <summary>
		/// 0x0: The node index (actually the salt header)
		/// </summary>
		public short Index
		{
			get { return index; }
			set { index = value; }
		}
		#endregion

		// expression->constant_type
		// predicate->function_index
		#region Opcode
		public const int OffsetOpcode = 0x2;
		short opcode;
		/// <summary>
		/// 0x2: The opcode of this node
		/// </summary>
		public short Opcode
		{
			get { return opcode; }
			set { opcode = value; }
		}
		#endregion

		// if this is a value
		// node, this is the enum value
		// of the type relative to all the types
		#region Type
		public const int OffsetType = 0x4;
		short type;
		/// <summary>
		/// 0x4: The type of the Data
		/// </summary>
		public short Type
		{
			get { return type; }
			set { type = value; }
		}
		#endregion

		// if this is a value node,
		// this is a enum value of the
		// type relative to all the values that
		// can be declared (ie, bool and so on)
		#region PointerType
		public const int OffsetPointerType = 0x6;
		short pointerType;
		/// <summary>
		/// 0x6: The type of the data that the pointer is pointing to
		/// </summary>
		public short PointerType
		{
			get { return pointerType; }
			set { pointerType = value; }
		}
		#endregion

		#region NextExpression
		public const int OffsetNextExp = 0x8;
		Blam.DatumIndex nextExpression;
		/// <summary>
		/// 0x8: The datum index of the next expression
		/// </summary>
		public Blam.DatumIndex NextExpression
		{
			get { return nextExpression; }
			set { nextExpression = value; }
		}
		#endregion

		#region Pointer
		public const int OffsetPointer = 0xC;
		int pointer;
		/// <summary>
		/// 0xC: A pointer to special data, ie string table
		/// 'flags' in the blam engine's code.
		/// 0x1 = _hs_syntax_node_primitive_bit
		/// </summary>
		public int Pointer
		{
			get { return pointer; }
			set { pointer = value; }
		}
		#endregion

		#region Data
		public const int OffsetData = 0x10;
		byte[] data = new byte[4];
		/// <summary>
		/// 0x10: The last 4 bytes of the node
		/// 
		/// If this is an object name, the data will be a short block index for
		/// the object names block. from there you can get the object's type, 
		/// and definition index
		/// 
		/// If its a tag ref, it will probably be a long block index to where 
		/// the tag ref is in the "references" block
		/// </summary>
		public byte[] Data
		{
			get { return data; }
			set { data = value; }
		}

		public void SetData(object value)
		{
			if (value is bool)
			{
				data[0] = (byte)(((bool)value) ? 1 : 0);
			}
			else if (value is short)
			{
				IO.ByteSwap.ReplaceBytes(data, 0, ((short)value));
			}
			else if (value is int)
			{
				IO.ByteSwap.ReplaceBytes(data, 0, ((int)value));
			}
			else if (value is Blam.DatumIndex)
			{
				IO.ByteSwap.ReplaceBytes(data, 0, (int)((Blam.DatumIndex)value));
			}
			else if (value is float)
			{
				IO.ByteSwap.ReplaceBytes(data, 0, ((float)value));
			}
		}
		#endregion

		public uint LineNumber = uint.MaxValue;

		string HexDump(string indent_string)
		{
			string fmt = "{8} {0} {1} {2} {3} {4}/{5} {6} {7}";
			string line_number = null;
			if (LineNumber != uint.MaxValue)
			{
				fmt = "{8} {0} {1} {2} {3} {4}/{5} {6} {7} {9}";
				line_number = LineNumber.ToString("X4");
			}

			return string.Format(fmt,
				index.ToString("X4"),
				opcode.ToString("X4"),
				type.ToString("X4"),
				pointerType.ToString("X4"),
				nextExpression.Salt.ToString("X4"),
				nextExpression.Index.ToString("X4"),
				pointer.ToString("X8"),
				ToLong().ToString("X8"),
				indent_string,
				line_number);
		}
		public string HexDump() { return HexDump(""); }
		#endregion

		#region Data Methods
		/// <summary>
		/// Translate 'Data' to a type based on the script type string
		/// </summary>
		/// <param name="type">script type name</param>
		/// <returns></returns>
		public object ToPrimitive(string type)
		{
			switch (type)
			{
				case "boolean": return ToBoolean();
				case "short": return ToShort();
				case "long": return ToLong();
				case "real": return ToReal();
			}

			return null;
		}

		/// <summary>
		/// Translates 'Data' to a boolean
		/// </summary>
		/// <returns></returns>
		public bool ToBoolean() { return BitConverter.ToBoolean(Data, 0); }

		/// <summary>
		/// Translates 'Data' to a short
		/// </summary>
		/// <returns></returns>
		public short ToShort() { return BitConverter.ToInt16(Data, 0); }

		/// <summary>
		/// Translates 'Data' to a int
		/// </summary>
		/// <returns></returns>
		public int ToLong() { return BitConverter.ToInt32(Data, 0); }

		/// <summary>
		/// Translates 'Data' to a float
		/// </summary>
		/// <returns></returns>
		public float ToReal() { return BitConverter.ToSingle(Data, 0); }

		/// <summary>
		/// Translates 'Data' to a expression index
		/// </summary>
		/// <returns></returns>
		public ushort ToDatumIndex() { return BitConverter.ToUInt16(Data, 2); }

		/// <summary>
		/// Translates 'Data' to a index
		/// </summary>
		/// <returns></returns>
		public short ToIndex() { return BitConverter.ToInt16(Data, 0); }
		#endregion

		#region Converters
		public static ScriptNode[] FromData(TagInterface.Data data)
		{
			int position = 56;
			int node_count = BitConverter.ToInt16(data.Value, 46);

			ScriptNode[] value = new ScriptNode[node_count]; // element count
			System.Diagnostics.Debug.WriteLine("node count: " + node_count);
			for (int x = 0; x < node_count; x++, position += 0x14)
			{
				var v = value[x] = new ScriptNode();
				v.nodeIndex = x;
				v.index = BitConverter.ToInt16(data.Value, position + OffsetIndex); //position += 2;
				v.opcode = BitConverter.ToInt16(data.Value, position + OffsetOpcode); //position += 2;
				v.type = BitConverter.ToInt16(data.Value, position + OffsetType); //position += 2;
				v.pointerType = BitConverter.ToInt16(data.Value, position + OffsetPointerType); //position += 2;
				// TODO: change this to ToInt32 to make it big endian compatible
				v.nextExpression.Index = BitConverter.ToUInt16(data.Value, position + OffsetNextExp); //position += 2;
				v.nextExpression.Salt = BitConverter.ToInt16(data.Value, position + OffsetNextExp + 2); //position += 2;
				v.pointer = BitConverter.ToInt32(data.Value, position + OffsetPointer); //position += 4;
				v.data[0] = data.Value[position + OffsetData]; //position += 1;
				v.data[1] = data.Value[position + OffsetData + 1]; //position += 1;
				v.data[2] = data.Value[position + OffsetData + 2]; //position += 1;
				v.data[3] = data.Value[position + OffsetData + 3]; //position += 1;
			}

			return value;
		}

		public static ScriptNode[] FromBlock(TagInterface.IBlock block)
		{
			TagInterface.IElementArray ea = block.GetElements();
			ScriptNode[] value = new ScriptNode[ea.Count];
			for (int x = 0; x < value.Length; x++)
			{
				var hs = (hs_syntax_datum_block)ea.GetElement(x);
				var v = value[x] = new ScriptNode();
				v.nodeIndex = x;
				v.index = (short)hs.DatumHeader.Value;
				v.opcode = (short)hs.TypeUnion.Value;
				v.type = (short)hs.Type.Value;
				v.pointerType = (short)hs.Flags.Value;
				v.nextExpression = hs.NextNodeIndex.Value;
				v.pointer = hs.Pointer.Value;
				int temp = hs.Data.Value;
				v.data[0] = (byte)(temp & 0x000000FF);
				v.data[1] = (byte)(temp & 0x0000FF00);
				v.data[2] = (byte)(temp & 0x00FF0000);
				v.data[3] = (byte)(temp & 0xFF000000);
			}

			return value;
		}
		#endregion
	};
	#endregion

	#region SyntaxNodeStack
	public class SyntaxNodeStack
	{
		#region Nodes
		Stack<ScriptNode> nodes = new Stack<ScriptNode>();
		public Stack<ScriptNode> Nodes { get { return nodes; } }
		#endregion

		public ScriptNode Pop() { return nodes.Pop(); }

		public void Pop(int count) { while (count-- > 0) nodes.Pop(); }

		public ScriptNode Peek() { return nodes.Peek(); }

		public int Push(ScriptNode node)
		{
			nodes.Push(node);
			return nodes.Count - 1;
		}

		public int Push(ScriptNode[] _nodes)
		{
			int count = 0;
			while (count++ < _nodes.Length)
				nodes.Push(_nodes[count]);

			return nodes.Count - _nodes.Length;
		}
	};
	#endregion

	#region ScriptStringEntry
	/// <summary>
	/// Represents a string and its position in the script syntax data
	/// </summary>
	public class ScriptStringEntry
	{
		/// <summary>
		/// Null version of this structure
		/// </summary>
		public static readonly ScriptStringEntry Null = new ScriptStringEntry("", -1);

		public readonly string Data;
		public readonly int Offset;

		public ScriptStringEntry(string entry, int offset)
		{
			Data = entry;
			Offset = offset;
		}

		/// <summary>
		/// Find the string that is expected to be located at <paramref name="offset"/> 
		/// in <paramref name="ste"/> and return its entry data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="ste"></param>
		/// <returns></returns>
		public static ScriptStringEntry Seach(int offset, ScriptStringEntry[] ste)
		{
			int low = 0, high = ste.Length;
			int mid;
			while (low <= high)
			{
				mid = (high + low) / 2;

				if (ste[mid].Offset < offset) low = mid;
				else if (offset < ste[mid].Offset) high = mid;
				else return ste[mid];
			}

			return Null;
		}

		public static ScriptStringEntry[] FromData(TagInterface.Data data, byte garbage_id)
		{
			int offset = 0;
			System.Text.StringBuilder stringEntry;
			ScriptStringEntry[] ste;
			List<ScriptStringEntry> value = new List<ScriptStringEntry>();

			for (int x = 0; x < data.Size; x++)
			{
				stringEntry = new StringBuilder();
				byte btchar = 0;

				#region (try to) get the string entry...
				try
				{
					do
					{
						if (offset < data.Size)
						{
							btchar = data[offset];

							if (btchar != 0) stringEntry.Append((char)btchar);
						}

						offset++;

					} while ((btchar != 0 || btchar != garbage_id) && offset < data.Size);
				}
				catch (IndexOutOfRangeException ex)
				{
					throw new Debug.ExceptionLog(ex, "Offset was outside the bounds of the data array.");
				}
				#endregion

				// ...and add it
				value.Add(new ScriptStringEntry(stringEntry.ToString(), offset - (stringEntry.Length + 1)));

				// we're in the padding area now
				if (btchar == garbage_id) break;
			}

			bool found_empty = false;
			for (int x = 0; x < value.Count; x++)
			{
				if (!found_empty) // do this shit until we find the first empty string...
				{
					found_empty = value[x].Data == "";
					continue;
				}

				// then do this for every empty string we may find
				if (value[x].Data == "" && found_empty)
					value.RemoveAt(x--);
			}

			// find the shit we only care for and return it
			ste = new ScriptStringEntry[value.Count];
			for (int x = 0; x < value.Count; x++)
				ste[x] = value[x];

			return ste;
		}
	};
	#endregion
}
#endif