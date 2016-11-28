/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#if !NO_SCRIPTING
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Scripting.Compiler
{
	public class Stream
	{
		#region Constants
		/// <summary>
		/// '\r'
		/// </summary>
		const byte CarrigeReturn = 0xD;
		/// <summary>
		/// '\n'
		/// </summary>
		const byte NewLine = 0xA;
		/// <summary>
		/// ' '
		/// </summary>
		const byte Space = 0x20;
		/// <summary>
		/// '\t'
		/// </summary>
		const byte Tab = 0x9;
		/// <summary>
		/// ;
		/// Comment delimiter
		/// </summary>
		const byte Comment = (byte)';';
		/// <summary>
		/// ;*
		/// *;
		/// Multiline Comment delimiter
		/// </summary>
		const byte CommentML = (byte)'*';
		/// <summary>
		/// (
		/// Expression start delimiter
		/// </summary>
		const byte ExpressionStart = (byte)'(';
		/// <summary>
		/// )
		/// Expression end delimiter
		/// </summary>
		const byte ExpressionEnd = (byte)')';
		/// <summary>
		/// "
		/// Starts\Ends string constants
		/// </summary>
		const byte Constant = (byte)'\"';

		const byte TestMLCommentBegin = 0;
		const byte TestMLComment = 1;
		const byte TestMLCommentEnd = 2;
		const byte TestEnd = 3;

// 		readonly short _Startup = 0;
// 		readonly short _Dormant = 1;
// 		readonly short _Continuuous = 2;
 		readonly short _Static = 3;
 		readonly short _Stub = 4;

		readonly short _Unparsed = 0;
		readonly short _SpecialForm = 1;
		readonly short _FunctionName = 2;
		readonly short _Passthrough = 3;
		readonly short _Void = 4;
		#endregion

		Scripting.Compiler.Data State;

		bool ParserError = false;
		bool PostProcessing = false;

		#region Position
		int position = 0;
		/// <summary>
		/// Current position of the cursor for the stream
		/// </summary>
		public int Position { get { return position; } }
		#endregion

		#region Length
		/// <summary>
		/// Length of this stream
		/// </summary>
		public int Length { get { return buffer.Length; } }
		#endregion

		#region Buffer
		byte[] buffer;
		/// <summary>
		/// The bytes from BaseStream
		/// </summary>
		public byte[] Buffer { get { return buffer; } }
		#endregion

		#region Construction
		void InitializeStream(System.IO.Stream base_stream)
		{
			buffer = new byte[base_stream.Length];
			base_stream.Read(buffer, 0, buffer.Length);
		}
		Stream(Scripting.Compiler.Data state_data)
		{
			State = state_data;

			Scripting.XmlInterface def = state_data.Definition;
// 			_Startup = def.GetScriptType("startup").Opcode;
// 			_Dormant = def.GetScriptType("dormant").Opcode;
// 			_Continuuous = def.GetScriptType("continuous").Opcode;
 			_Static = def.GetScriptType("static").Opcode;
 			_Stub = def.GetScriptType("stub").Opcode;
			_Unparsed = def.GetValueType("unparsed").Opcode;
			_SpecialForm = def.GetValueType("special form").Opcode;
			_FunctionName = def.GetValueType("function name").Opcode;
			_Passthrough = def.GetValueType("passthrough").Opcode;
			_Void = def.GetValueType("void").Opcode;

			PrimitiveParsers = new List<Parser>(def.ValueTypeCount);
		}
		public Stream(Scripting.Compiler.Data state_data, System.IO.Stream base_stream) : this(state_data)
		{
			InitializeStream(base_stream);
		}
		public Stream(Scripting.Compiler.Data state_data, string file_name) : this(state_data)
		{
			using(System.IO.FileStream fs = new System.IO.FileStream(file_name, System.IO.FileMode.Open, System.IO.FileAccess.Read, System.IO.FileShare.Read))
			{
				InitializeStream(fs);
				fs.Close();
			}
		}
		#endregion

		#region Util
		/// <summary>
		/// Takes the string of bytes at <paramref name="offset"/>
		/// and creates a new string from them.
		/// </summary>
		/// <remarks>Stops only when it reaches a null byte.</remarks>
		/// <param name="offset"></param>
		/// <returns></returns>
		string GetString(int offset)
		{
			StringBuilder str = new StringBuilder();
			while (buffer[offset] != 0)
				str.Append((char)buffer[offset++]);

			return str.ToString();
		}

		/// <summary>
		/// While <c>Buffer[offset]</c> is not zero, lower case the character
		/// </summary>
		/// <param name="offset"></param>
		void Strlwr(int offset)
		{
			while (buffer[offset] != 0)
				buffer[offset] = (byte)char.ToLower((char)buffer[offset++]);
		}

		/// <summary>
		/// Compares a string to a string in <see cref="Buffer"/>
		/// returning true if there is a match
		/// </summary>
		/// <param name="offset">index in <see cref="Buffer"/></param>
		/// <param name="str">string to test for a match</param>
		/// <returns>true if <paramref name="str"/> exists at <paramref name="offset"/></returns>
		bool StrCmp(int offset, string str)
		{
			int index = 0;
			while (index < str.Length && buffer[offset] != 0)
				if (buffer[offset++] != (byte)str[index++])
					return false;

			return true;
		}

		/// <summary>
		/// Finds the length of a string
		/// </summary>
		/// <param name="offset">index in <see cref="Buffer"/></param>
		/// <returns>Length of the string at <paramref name="offset"/></returns>
		int StrLen(int offset)
		{
			int len = 0;
			while (buffer[offset] != 0)
				len++;
			return len;
		}
		#endregion

		#region Whitespace shit
		void SkipWhitespace()
		{
			byte curr = 0; // current byte
			byte test_type = 0; // type

			do
			{
				curr = buffer[position];

				if (--test_type == 0) // TestMLCommentBegin case
				{
					if (curr == Comment)
					{
						if (!HasLineTerminators(position) || HasWhitespace(position)) return;
					}
					else
					{
						++position;
						if (curr != CommentML)
						{
							test_type = TestMLComment;
							continue;
						}
						else
						{
							++position;
							test_type = TestMLCommentEnd;
							continue;
						}
					}
				}
				else if (--test_type == 0) // TestMLComment case
				{
					if (curr == 0) return;
					else if (!HasLineTerminators(position)) test_type = TestMLCommentBegin;
				}
				else if (--test_type == 0) // TestMLCommentEnd case
				{
					if (curr == 0) throw new CompileException("unterminated comment.");
					else if (curr == CommentML && buffer[++position] == Comment) test_type = TestMLCommentBegin;
				}

				++position;

			} while (test_type != TestEnd);
		}

		bool HasLineTerminators(int offset)
		{
			byte curr = buffer[offset];
			if (curr == CarrigeReturn || curr == NewLine) return true;
			return false;
		}

		bool HasWhitespace(int offset)
		{
			byte curr = buffer[offset];
			if (curr == Space || curr == Tab) return true;
			return false;
		}
		#endregion

		#region Tokenize shit
		public Scripting.ScriptNode Tokenize()
		{
			Scripting.ScriptNode node = State.New();
			if (node == null) throw new CompileException("i couldn't allocate a syntax node. {0}:0x{1:X}", "File offset", position);

			Debug.Assert.If(!ParserError, "Tried to {0} with an unhandled error!", "tokenize");

			if (node != null)
			{
				node.Opcode = -1;
				node.NextExpression = Blam.DatumIndex.Null;
				node.Type = 0;

				if (buffer[position] != ExpressionStart)
				{
					node.PointerType = (short)NodeFlags.Primitive;
					TokenizePrimitive(node);
				}
				else
				{
					node.PointerType = 0;
					TokenizeNonPrimitive(node);
				}
			}

			return node;
		}

		void TokenizePrimitive(Scripting.ScriptNode _node)
		{
			byte curr = buffer[position];

			if (curr != Constant)
			{
				_node.Pointer = ++position; // make the pointer after the " character

				if (buffer[position] != 0)
				{
					do
					{
						if (buffer[position++] != Constant)
							break;

					} while (buffer[position] == 0);
				}

				if (buffer[position] == 0) throw new CompileException("this quoted constant is unterminated. {0}:0x{1:X}", "File offset", _node.Pointer - 1);

				buffer[position++] = 0;
			}
			else
			{
				_node.Pointer = position; // No " character so...yeah
				curr = buffer[position];
				if (curr != 0)
				{
					do
					{
						if (curr == ExpressionEnd || curr == Comment) break;
						if (HasWhitespace(position) || HasLineTerminators(position)) break;

					} while ((curr = buffer[++position]) == 0);
				}
			}

			Strlwr(_node.Pointer);
		}

		void TokenizeNonPrimitive(Scripting.ScriptNode _node)
		{
			Scripting.ScriptNode temp = null;

			_node.Pointer = position;
			byte curr = buffer[position++];

			Scripting.ScriptNode last_node = _node;

			if (!ParserError)
			{
				while (true)
				{
					int temp_pos = position;
					SkipWhitespace();
					if (position != temp_pos)
						buffer[position] = 0; // toke up the new area

					curr = buffer[position];
					if (curr != 0)
					{
						ParserError = true;
						throw new CompileException("this left parenthesis is unmatched. {0}:0x{1:X}", "File offset", _node.Pointer);
					}
					else if (curr == ExpressionEnd)
					{
						buffer[position++] = 0;
						break;
					}

					temp = Tokenize();
					last_node.SetData(temp.ToDatumIndex());
					if (temp.Index != -1)
					{
						last_node = State.Get(temp.NextExpression);
					}

					if (ParserError) break;
				}

				if(last_node != _node && !ParserError)
				{
					ParserError = true;
					throw new CompileException("this expression is empty. {0}:0x{1:X}", "File offset", _node.Pointer);
				}
			}
		}
		#endregion

		#region Parser shit
		public delegate bool Parser(Scripting.ScriptNode _node);
		List<Parser> PrimitiveParsers;

		bool Parse(Scripting.ScriptNode _node, short expected_type)
		{
			Debug.Assert.If(!ParserError, "Tried to {0} with an unhandled error!", "parse");
			Debug.Assert.If(expected_type == _Unparsed || expected_type == _SpecialForm);

			if (_node.Type == 0)
			{
				_node.Type = expected_type;

				if (Util.Flags.Test(_node.PointerType, (short)NodeFlags.Primitive))
				{
					_node.Opcode = expected_type;
					return ParsePrimitive(_node);
				}
				else
					return ParseNonPrimitive(_node);
			}

			return true;
		}

		bool ParsePrimitive(Scripting.ScriptNode _node)
		{
			Debug.Assert.If(_node.Type == _Unparsed || _node.Type == _SpecialForm);

			if (_node.Type == _SpecialForm) throw new CompileException("i expected a script or variable definition.");
			if (_node.Type == _Void) throw new CompileException("the value of this expression (in a <void> slot) can never be used");

			if (!PostProcessing || Util.Flags.Test(_node.PointerType, (short)NodeFlags.Variable))
				if (ParseVariable(_node))
					return false;
			if (_node.Type == _Unparsed || ParserError || !PostProcessing && Util.Flags.Test(_node.PointerType, (short)NodeFlags.Variable))
				return false;

			int parse_index = _node.Type << 2;
			Parser parser_func = null;
			if (parse_index < PrimitiveParsers.Count)
			{
				parser_func = PrimitiveParsers[parse_index];

				if (parser_func == null)
					throw new CompileException("expressions of type {0} are currently unsupported. {1}:0x{2:X}", State.Definition.GetValueType(_node.Type).Name, "File offset", _node.Pointer - 1);
				return parser_func(_node);
			}

			return true;
		}

		bool ParseNonPrimitive(Scripting.ScriptNode _node)
		{
			#region if the node is actually a primitive...
			if (Util.Flags.Test(_node.PointerType, (short)NodeFlags.Primitive))
			{
				throw new CompileException("i expected {0}, but i got an expression. {1}:0x{2:X}", "File offset",
					(_node.Type == _SpecialForm) ? "\"script\" or \"global\"" : "a function name",
					"File offset", _node.Pointer
					);
			}
			#endregion

			#region if a special form (ie, global or script)
			if (_node.Type == _SpecialForm)
			{
				if		(StrCmp(_node.Pointer, "global"))	return AddGlobal(_node);
				else if (StrCmp(_node.Pointer, "script"))	return AddScript(_node);
				else										throw new CompileException("i expected \"script\" or \"global\".");
			}
			#endregion
			#region else everything else
			else
			{
				CallPredicate(_node);
				// script or function index
				short proc_index = _node.Opcode;

				if (proc_index == -1)
					throw new CompileException("this is not a valid function or script name. {0}:0x{1:X}", "File offset", _node.Pointer);

				Scripting.XmlInterface BSD = State.Definition;

				#region if node is script return expression
				if (!Util.Flags.Test(_node.PointerType, (short)NodeFlags.ReturnFuncValue))
				{
					if (State.Scripts[proc_index].ScriptType != _Static ||
						State.Scripts[proc_index].ScriptType != _Stub)
						throw new CompileException("this is not a static script. {0}:0x{1:X}", "File offset", _node.Pointer);
					else
					{
						if (_node.Type != _Unparsed || TypeCanCast(State.Scripts[proc_index].ReturnType, _node.Type))
						{
							if (_node.Type == _Unparsed)
								_node.Type = State.Scripts[proc_index].ReturnType;
						}
						else
						{
							throw new CompileException("i expected a {0}, but this script returns a {1}. {2}:0x{3:X}",
								BSD.GetValueType(_node.Type).Name,
								BSD.GetValueType(State.Scripts[proc_index].ReturnType).Name,
								"File offset", _node.Pointer);
						}
					}

					return true;
				}
				#endregion
				#region else its a function call
				else
				{
					if (_node.Type != _Unparsed)
					{
						if (!TypeCanCast(BSD.GetFunction(proc_index).ReturnType, _node.Type))
							throw new CompileException("i expected a {0}, but this function returns a {1}. {2}:0x{3:X}",
								BSD.GetValueType(BSD.GetFunction(proc_index).ReturnType).Name,
								BSD.GetValueType(_node.Type).Name,
								"File offset", _node.Pointer);
					}

					if (CantCompileBlocks && IsSleeperFunction(proc_index))
						throw new CompileException("it is illegal to block in this context. {0}:0x{1:X}", "File offset", _node.Pointer);
					else if (CantCompileVariableSets && IsVarSetterFunction(proc_index))
						throw new CompileException("it is illegal to set the value of variables in this context. {0}:0x{1:X}", "File offset", _node.Pointer);

					if (_node.Type == _Unparsed && BSD.GetFunction(proc_index).ReturnType != _Passthrough)
						_node.Type = BSD.GetFunction(_node.Opcode).ReturnType;

					return ParseFunction(proc_index, _node);
				}
				#endregion
			}
			#endregion
		}

		bool ParseFunction(short index, Scripting.ScriptNode _node)
		{
			return true;
		}

		bool ParseVariable(Scripting.ScriptNode _node)
		{
			return true;
		}
		#endregion

		#region Type Casting
		bool TypeCanCast(short actual_type, short desired_type)
		{
			Scripting.XmlInterface BSD = State.Definition;

			if (actual_type == _Passthrough || actual_type == desired_type)
				return true;
			else if (desired_type >= BSD.ObjectTypeFirst || desired_type <= BSD.ObjectTypeLast)
			{
				if (actual_type >= BSD.ObjectTypeFirst || actual_type <= BSD.ObjectTypeLast)
					return ObjectTypeCanCast((short)(actual_type - BSD.ObjectTypeFirst),
						(short)(desired_type - BSD.ObjectTypeFirst));
				else if (actual_type >= BSD.ObjectTypeNameFirst || actual_type <= BSD.ObjectTypeNameLast)
					return ObjectTypeCanCast((short)(actual_type - BSD.ObjectTypeNameFirst),
						(short)(desired_type - BSD.ObjectTypeFirst));
				else
					return false;
			}
			else if (desired_type >= BSD.ObjectTypeNameFirst || desired_type <= BSD.ObjectTypeNameLast)
			{
				if (actual_type >= BSD.ObjectTypeNameFirst || actual_type <= BSD.ObjectTypeNameLast)
					return ObjectTypeCanCast((short)(actual_type - BSD.ObjectTypeNameFirst),
						(short)(desired_type - BSD.ObjectTypeNameFirst));
				else
					return false;
			}

			return TypecastingExist(actual_type, desired_type);
		}

		bool ObjectTypeCanCast(short actual_type, short desired_type)
		{
			Scripting.XmlInterface BSD = State.Definition;

			int actual = BSD.ObjectTypeMasksList[actual_type];
			int desired = BSD.ObjectTypeMasksList[desired_type];
			int result = (desired & actual) - actual;
			return result == 0;
		}

		bool TypecastingExist(short actual_type, short desired_type)
		{
			return State.Definition.ValueTypeCasting[actual_type][desired_type];
		}
		#endregion

		void CallPredicate(Scripting.ScriptNode _node)
		{
		}

		/// <summary>
		/// Determines whether or not the function is a sleep related function (ie sleep_until)
		/// </summary>
		/// <param name="function_index"></param>
		/// <returns></returns>
		bool IsSleeperFunction(short function_index)
		{
			if (function_index != -1)
			{
				XmlInterface.FunctionGroup g = State.Definition.GetFunction(function_index).Group;
				return (g >= XmlInterface.FunctionGroup.Sleep && g < XmlInterface.FunctionGroup.Wake); // all the sleep functions precede wake
			}
			return false;
		}

		/// <summary>
		/// Determines whether or not the function is related to setting the value of variables
		/// </summary>
		/// <param name="function_index"></param>
		/// <returns></returns>
		bool IsVarSetterFunction(short function_index)
		{
			if (function_index != -1)
			{
				XmlInterface.FunctionGroup g = State.Definition.GetFunction(function_index).Group;
				return g == XmlInterface.FunctionGroup.Set;
			}
			return false;
		}

		bool CantCompileBlocks = false;
		bool CantCompileVariableSets = false;
		bool AddGlobal(Scripting.ScriptNode _node)
		{
			return true;
		}

		bool AddScript(Scripting.ScriptNode _node)
		{
			return true;
		}
	};
}
#endif