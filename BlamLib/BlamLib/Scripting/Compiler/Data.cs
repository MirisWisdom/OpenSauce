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
	public class CompileException : Debug.ExceptionLog
	{
		public CompileException(string msg) : base(msg) { }
		public CompileException(string fmt, params object[] args) : base(string.Format(fmt, args)) { }
	};

	/// <summary>
	/// Execution state for the compiler
	/// </summary>
	public class Data
	{
		Scripting.XmlInterface definition;
		/// <summary>
		/// Backing scripting definition for this execution state
		/// </summary>
		internal Scripting.XmlInterface Definition { get { return definition; } }

		/// <summary>
		/// Script nodes data
		/// </summary>
		public List<Scripting.ScriptNode> Nodes;
		/// <summary>
		/// Script strings buffer
		/// </summary>
		public Util.StringPool StringData;
		/// <summary>
		/// State's scripts
		/// </summary>
		public List<ScriptBlock> Scripts;
		/// <summary>
		/// State's globals
		/// </summary>
		public List<GlobalsBlock> Globals;

		/// <summary>
		/// Initialize the execution state for the compiler using a blam-script definition
		/// </summary>
		/// <param name="definition"></param>
		internal Data(Scripting.XmlInterface definition)
		{
			this.definition = definition;
			Nodes = new List<ScriptNode>(this.definition.MaxNodes);
			StringData = new Util.StringPool(false);
			Scripts = new List<ScriptBlock>();
			Globals = new List<GlobalsBlock>();
		}

		public Scripting.ScriptNode New()
		{
			Scripting.ScriptNode node = new ScriptNode();
			node.Index = (short)(Definition.NodeSalt - (short)Nodes.Count);
			//node.NextExpression = Definition.DatumFromIndex(SyntaxData.Nodes.Count);

			Nodes.Add(node);
			return node;
		}

		public Scripting.ScriptNode Get(Blam.DatumIndex index, bool throw_exception)
		{
			if (index < 0 || index > Nodes.Count)
				if (throw_exception)
					throw new Debug.ExceptionLog("Index is not valid. {0:X} {1:X}", index);
				else
					return null;

			return Nodes[index];
		}

		public Scripting.ScriptNode Get(Blam.DatumIndex index) { return Get(index, false); }

		public Scripting.ScriptNode GetNext(Scripting.ScriptNode node, bool throw_exception)
		{
			if (node.NextExpression.Index < 0 || node.NextExpression.Index > Nodes.Count)
				if (throw_exception)
					throw new Debug.ExceptionLog("NextExpression is not valid. {0:X} {1:X}", node.Index, node.NextExpression.Index);
				else
					return null;

			return Nodes[node.NextExpression];
		}

		public Scripting.ScriptNode GetNext(Scripting.ScriptNode node) { return GetNext(node, false); }
	};
}
#endif