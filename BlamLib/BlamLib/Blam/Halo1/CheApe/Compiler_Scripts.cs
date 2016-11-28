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
		/// <summary>
		/// short return_type;
		/// pad16;
		/// cstring name;
		/// void* parse;
		/// void* evaluate;
		/// cstring info;
		/// cstring param_info;
		/// short access;
		/// short argc;
		/// short args[]; (pad16 if odd count)
		/// </summary>
		internal sealed class ScriptFunction : Compiler.Object
		{
			public const int Size = 2 + 2 + 4 + 4 + 4 + 4 + 4 + 2 + 2 + 2
				+ 2;

			Import.ScriptFunction func = null;
			public void Reset(Import.ScriptFunction def) { func = def; }
			public ScriptFunction() { }
			public ScriptFunction(Import.ScriptFunction def) { func = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				comp.MarkLocationFixup(func.Name, stream, false);
				stream.Write(func.returnType.Opcode);
				stream.Write((short)(func.IsInternal ? 1 : 0));//stream.Write(ushort.MinValue);
				stream.Write(func.Name);
				stream.Write(uint.MaxValue);
				stream.Write(uint.MaxValue);
				stream.Write(func.Help);
				stream.Write(func.HelpArg);
				stream.Write(ushort.MinValue);
				short count; stream.Write(count = (short)func.Arguments.Count);
				foreach (Import.ScriptFunctionArg arg in func.Arguments) stream.Write(arg.type.Opcode);
				if ((count % 2) > 0) stream.Write(ushort.MinValue);
			}
			#endregion
		};

		/// <summary>
		/// cstring name;
		/// short type;
		/// pad16;
		/// void* address;
		/// short access;
		/// pad16;
		/// </summary>
		internal sealed class ScriptGlobal : Compiler.Object
		{
			public const int Size = 4 + 2 + 2 + 4 + 2 + 2;

			Import.ScriptGlobal glob = null;
			public void Reset(Import.ScriptGlobal def) { glob = def; }
			public ScriptGlobal() { }
			public ScriptGlobal(Import.ScriptGlobal def) { glob = def; }

			#region IStreamable Members
			public override void Write(IO.EndianWriter stream)
			{
				Compiler comp = stream.Owner as Compiler;

				comp.MarkLocationFixup(glob.Name, stream, false);
				stream.Write(glob.Name);
				stream.Write(glob.type.Opcode);
				stream.Write((short)(glob.IsInternal ? 1 : 0));//stream.Write(ushort.MinValue);
				stream.Write(uint.MaxValue);
				stream.Write(uint.MinValue);
			}
			#endregion
		};

		void ScriptingHeadersToStream(Import import)
		{
			string name;
			uint base_address = OwnerState.Definition.MemoryInfo.BaseAddress;

			if ((Head.ScriptFunctionsCount = import.ScriptFunctions.Count) > 0)
			{
				Head.ScriptFunctionsAddress = base_address + MemoryStream.PositionUnsigned;
				// allocate script functions pointers
				foreach (Import.ScriptFunction sc in import.ScriptFunctions.Values)
				{
					name = sc.ToString();
					AddLocationFixup(name, MemoryStream);
					MemoryStream.Write(uint.MinValue);
				}

				AlignMemoryStream(Compiler.kDefaultAlignment);
			}

			if ((Head.ScriptGlobalsCount = import.ScriptGlobals.Count) > 0)
			{
				Head.ScriptGlobalsAddress = base_address + MemoryStream.PositionUnsigned;
				// allocate script global pointers
				foreach (Import.ScriptGlobal sc in import.ScriptGlobals.Values)
				{
					name = sc.ToString();
					AddLocationFixup(name, MemoryStream);
					MemoryStream.Write(uint.MinValue);
				}

				AlignMemoryStream(Compiler.kDefaultAlignment);
			}
		}

		void ScriptingDefinitionsToStream(Import import)
		{
			if (Head.ScriptFunctionsCount > 0)
			{
				var sfunc = new ScriptFunction();
				foreach (Import.ScriptFunction sc in import.ScriptFunctions.Values)
				{
					sfunc.Reset(sc);
					sfunc.Write(MemoryStream);
				}
			}

			if (Head.ScriptGlobalsCount > 0)
			{
				var sglob = new ScriptGlobal();
				foreach (Import.ScriptGlobal sc in import.ScriptGlobals.Values)
				{
					sglob.Reset(sc);
					sglob.Write(MemoryStream);
				}
			}
		}

		void ScriptingToStream(Import import)
		{
			ScriptingHeadersToStream(import);
			ScriptingDefinitionsToStream(import);
		}
	};
}