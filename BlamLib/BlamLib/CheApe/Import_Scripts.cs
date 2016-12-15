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
		/// <summary>
		/// XmlNode interface for a script function argument
		/// [string: type]
		/// [string: name] (unused)
		/// </summary>
		public sealed class ScriptFunctionArg
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			internal Scripting.XmlInterface.Type type = null;

			public ScriptFunctionArg(ScriptFunction func, ProjectState state, IO.XmlStream s)
			{
				string type_name = null;
				s.ReadAttribute("type", ref type_name);

				if (!state.scriptingInterface.Types.Contains(type_name))
					throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", type_name);

				type = state.scriptingInterface.GetValueType(type_name);
			}
		};

		/// <summary>
		/// XmlNode interface for a script function
		/// [string: name]
		/// [string: returnType]
		/// [string: help] (optional)
		/// [string: helpArg] (optional)
		/// [list: args]
		/// </summary>
		public sealed class ScriptFunction : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1 +
				ScriptFunctionArg.XmlNodeComplexity;

			internal Scripting.XmlInterface.Type returnType = null;

			#region Help
			uint help = 0;
			string helpString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint Help { get { return help; } }
			#endregion

			#region HelpArg
			uint helpArg = 0;
			string helpArgString = string.Empty;
			/// <summary>
			/// 
			/// </summary>
			public uint HelpArg { get { return helpArg; } }
			#endregion

			#region Arguments
			private List<ScriptFunctionArg> args = new List<ScriptFunctionArg>();
			/// <summary>
			/// 
			/// </summary>
			public List<ScriptFunctionArg> Arguments { get { return args; } }
			#endregion

			public readonly bool IsInternal = false;

			public ScriptFunction(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string return_type_name = null;
				s.ReadAttribute("returnType", ref return_type_name);
				s.ReadAttributeOpt("help", ref helpString);
				s.ReadAttributeOpt("helpArg", ref helpArgString);
				s.ReadAttributeOpt("internal", ref IsInternal);

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "arg") continue;

					s.SaveCursor(n);
					args.Add(new ScriptFunctionArg(this, state, s));
					s.RestoreCursor();
				}

				if (!state.scriptingInterface.Types.Contains(return_type_name))
					throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", return_type_name);

				returnType = state.scriptingInterface.GetValueType(return_type_name);
				help = state.Compiler.Strings.Add(helpString);
				helpArg = state.Compiler.Strings.Add(helpArgString);
			}
		};

		/// <summary>
		/// XmlNode interface for a script global
		/// [string: name]
		/// [string: type]
		/// </summary>
		public sealed class ScriptGlobal : Object
		{
			/// <summary>
			/// The maximum complexity level which this object can achieve.
			/// </summary>
			public const int XmlNodeComplexity = 1;

			internal Scripting.XmlInterface.Type type = null;

			public readonly bool IsInternal = false;

			public ScriptGlobal(ProjectState state, IO.XmlStream s) : base(state, s)
			{
				string type_name = null;
				s.ReadAttribute("type", ref type_name);
				s.ReadAttributeOpt("internal", ref IsInternal);

				if (!state.scriptingInterface.Types.Contains(type_name))
					throw new Debug.ExceptionLog("CheApe: value type '{0}' does not exist", type_name);

				type = state.scriptingInterface.GetValueType(type_name);
			}
		};

		void ProcessScriptFunctions(ProjectState state, IO.XmlStream s)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "function") continue;

				s.SaveCursor(n);
				var sc = new ScriptFunction(state, s);
				s.RestoreCursor();
				string name_str = sc.ToString();

				if (state.scriptingInterface.Functions.Contains(name_str))
				{
					Debug.LogFile.WriteLine("Engine already contains a {0} named '{1}', skipping...", "script function", name_str);
					continue;
				}

				try { ScriptFunctions.Add(name_str, sc); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "script function definition", name_str); }
			}
		}

		void ProcessScriptGlobals(ProjectState state, IO.XmlStream s)
		{
			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "global") continue;

				s.SaveCursor(n);
				var sg = new ScriptGlobal(state, s);
				s.RestoreCursor();
				string name_str = sg.ToString();

				if (state.scriptingInterface.Globals.Contains(name_str))
				{
					Debug.LogFile.WriteLine("Engine already contains a {0} named '{1}', ignoring...", "script global", name_str);
					continue;
				}

				try { ScriptGlobals.Add(name_str, sg); }
				catch (ArgumentException) { Debug.LogFile.WriteLine(kDuplicateErrorStr, "script global definition", name_str); }
			}
		}
	};
}