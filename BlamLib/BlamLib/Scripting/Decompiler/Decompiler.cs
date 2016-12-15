/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#if !NO_SCRIPTING
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Scripting.Decompiler
{
	/// <summary>
	/// Decompiler delegate
	/// </summary>
	/// <param name="decomp">string buffer to receive the decompiled code text</param>
	/// <param name="nodes">script nodes to decompile</param>
	/// <param name="index">index inside the nodes</param>
	/// <returns></returns>
	public delegate bool Delegate(StringBuilder decomp, ScriptNode[] nodes, ref short index);

	public class DecompilerException : Debug.ExceptionLog
	{
		public DecompilerException(string exp_name, int exp_index) : base("{0} <bad index> {1:X8}", exp_name, exp_index) {}

		public DecompilerException(string exp_name, int exp_index, ScriptNode node) :
			base("{0} {1} {2} {3} {4}", exp_name, node.NodeIndex, exp_index, Program.NewLine, node.HexDump()) {}

		public DecompilerException(string exp_name, int exp_index, ScriptNode node, Exception inner)
			:
			base("{0} {1} {2} {3} {4} {5}", exp_name, node.NodeIndex, exp_index, Program.NewLine, node.HexDump(), inner) { }
	};

	public class Decompiler
	{
	};
}
#endif