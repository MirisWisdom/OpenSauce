/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using TI = BlamLib.TagInterface;

namespace BlamLib.Scripting
{
	#region hs_scripts_block
	public abstract class hs_scripts_block : TI.Definition
	{
		#region hs_scripts_parameters_block
		public abstract class hs_scripts_parameters_base : TI.Definition
		{
			public TI.String Name;
			public TI.Enum Type;

			protected hs_scripts_parameters_base(int field_count) : base(field_count) { }
		};
		#endregion

		#region Fields
		public TI.Field Name; // TI.String or TI.StringId
		public TI.Enum ScriptType;
		public TI.Enum ReturnType;
		public TI.LongInteger RootExpressionIndex;
		#endregion

		protected hs_scripts_block(int field_count) : base(field_count) {}

		public virtual TI.IBlock GetParametersBlock() { return null; }
	};
	#endregion

	#region hs_globals_block
	public abstract class hs_globals_block : TI.Definition
	{
		#region Fields
		public TI.String Name;
		public TI.Enum Type;
		public TI.LongInteger InitExpressionIndex;
		#endregion

		protected hs_globals_block(int field_count) : base(field_count) { }
	};
	#endregion

	#region hs_syntax_datum_block
	public abstract class hs_syntax_datum_block : TI.Definition
	{
		#region Fields
		public TI.ShortInteger DatumHeader;
		public TI.ShortInteger TypeUnion;
		public TI.ShortInteger Type;
		public TI.ShortInteger Flags;
		public TI.LongInteger NextNodeIndex;
		public TI.LongInteger Pointer; // string offset to either: hs-strings or the character index from when the engine parsed the source script
		public TI.LongInteger Data;
		/// <summary>
		/// For Generation 3 engines
		/// </summary>
		public TI.ShortInteger LineNumber;
		protected TI.ShortInteger LineNumberPadding;
		#endregion

		protected hs_syntax_datum_block(int field_count) : base(field_count) { }
	}
	#endregion

#if !NO_SCRIPTING
	public abstract class ExpressionInfo
	{
		/// <summary>
		/// Name of the expression
		/// </summary>
		public string Name = "";

		/// <summary>
		/// Expression's begin statement offset
		/// </summary>
		public int NodeOffset = 0;

		/// <summary>
		/// How many nodes this expression takes up
		/// </summary>
		public int NodeCount;

		/// <summary>
		/// The real offset to where the expression begins
		/// </summary>
		public int RealOffset = 0;

		public ScriptNode Root = null;

		public Util.StringArray Code = new Util.StringArray();

		[System.Diagnostics.Conditional("DEBUG")]
		public void DumpGraph(System.IO.StreamWriter s, Util.StringPool strings)
		{
			s.WriteLine("{0}:{1}",
				this.GetType() == typeof(ScriptBlock) ? "Script" : "Global",
				Name);
			Root.DumpGraph(s, 1, strings);
		}
	};

	/// <summary>
	/// Scenario's Script Block definition
	/// </summary>
	public class ScriptBlock : ExpressionInfo
	{
		/// <summary>
		/// Script's script type
		/// </summary>
		public short ScriptType = 0;

		/// <summary>
		/// Script's return type
		/// </summary>
		public short ReturnType = 0;

		public static ScriptBlock[] FromBlock(TI.IBlock block)
		{
			TI.IElementArray ea = block.GetElements();
			ScriptBlock[] value = new ScriptBlock[ea.Count];

			hs_scripts_block hs;
			for (int x = 0; x < value.Length; x++)
			{
				hs = (hs_scripts_block)ea.GetElement(x);
				var v = value[x] = new ScriptBlock();

				v.Name = hs.Name.ToString();
				v.ScriptType = (short)hs.ScriptType.Value;
				v.ReturnType = (short)hs.ReturnType.Value;
				Blam.DatumIndex index = hs.RootExpressionIndex.Value;
				v.NodeOffset = index.Index;

				if (v.Name == TagInterface.StringId.kEncryptedResult)
					v.Name = (hs.Name as TagInterface.StringId).Handle.ToUInt32().ToString("X8");
			}

			return value;
		}
	};

	/// <summary>
	/// Scenario's Globals block definition
	/// </summary>
	public class GlobalsBlock : ExpressionInfo
	{
		/// <summary>
		/// Global's type
		/// </summary>
		public short Type = 0;

		public static GlobalsBlock[] FromBlock(TI.IBlock block)
		{
			TI.IElementArray ea = block.GetElements();
			GlobalsBlock[] value = new GlobalsBlock[ea.Count];

			hs_globals_block hs;
			for (int x = 0; x < value.Length; x++)
			{
				hs = (hs_globals_block)ea.GetElement(x);
				var v = value[x] = new GlobalsBlock();

				v.Name = hs.Name.Value;
				v.Type = (short)hs.Type.Value;
				Blam.DatumIndex index = hs.InitExpressionIndex.Value;
				v.NodeOffset = index.Index;

				// We have to dynamically caluclate the node counts as 
				// some globals are set at runtime, ie '(* 20 seconds)'
				v.NodeCount = -1;
			}

			return value;
		}
	};
#endif
}