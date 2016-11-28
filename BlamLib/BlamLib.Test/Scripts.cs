/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using TI = BlamLib.TagInterface;

namespace BlamLib.Test
{
	class Scripts
	{
		public static void InteropReadStringData(Blam.CacheFile cf, int cache_offset, 
			out Util.StringPool strings_pool, byte k_pad_character)
		{
			strings_pool = new Util.StringPool();

			cf.InputStream.Seek(cache_offset, System.IO.SeekOrigin.Begin);
			var hs_strings = new BlamLib.TagInterface.Data();
			hs_strings.ReadHeader(cf);
			if (hs_strings.Size == 0) return;
			hs_strings.Read(cf);

			int offset = 0;
			byte btchar = 0;
			while (true)
			{
				var stringEntry = new System.Text.StringBuilder();
				try
				{
					do
					{
						if (offset < hs_strings.Size)
						{
							btchar = hs_strings[offset];

							if (btchar != 0) stringEntry.Append((char)btchar);
						}

						offset++;

					} while ((btchar != 0 && btchar != k_pad_character) && offset < hs_strings.Size);
				}
				catch (IndexOutOfRangeException ex)
				{
					throw new BlamLib.Debug.ExceptionLog("Offset was outside the bounds of the data array. {0}{1}", BlamLib.Program.NewLine, ex);
				}

				strings_pool.Add(stringEntry.ToString());

				if (btchar == k_pad_character) break;
			}
		}

		public static void InteropReadTagData(Blam.CacheFile cf,
			TagInterface.IBlock hs_scripts, int cache_offset_scripts, 
			TagInterface.IBlock hs_globals, int cache_offset_globals)
		{
			cf.InputStream.Position = cache_offset_scripts;
			hs_scripts.ReadHeader(cf);
			hs_scripts.Read(cf);

			cf.InputStream.Position = cache_offset_globals;
			hs_globals.ReadHeader(cf);
			hs_globals.Read(cf);
		}

		public static void InteropReadNodes(Blam.CacheFile cf, int cache_offset, 
			TagInterface.IBlock hs_nodes)
		{
			cf.InputStream.Seek(cache_offset, System.IO.SeekOrigin.Begin);
			hs_nodes.ReadHeader(cf);
			hs_nodes.Read(cf);
		}

		#region Output function names
		const int kScriptFunctionCountHalo3 = 0x5B9;
		const int kScriptFunctionCountHaloOdst = 0x698;
		const int kScriptFunctionCountReachBeta = 0x729;
		const int kScriptFunctionCountReachRetail = 0x7BD;
		public static void InitializeScriptFunctionsList(BlamVersion engine, out string[] script_functions)
		{
			switch (engine)
			{
				case BlamVersion.Halo3_Xbox:	script_functions = new string[kScriptFunctionCountHalo3]; break;

				case BlamVersion.HaloOdst_Xbox:	script_functions = new string[kScriptFunctionCountHaloOdst]; break;

				case BlamVersion.HaloReach_Beta: script_functions = new string[kScriptFunctionCountReachBeta]; break;
				case BlamVersion.HaloReach_Xbox: script_functions = new string[kScriptFunctionCountReachRetail]; break;
				default: script_functions = null; break;
			}
			for (int x = 0; x < script_functions.Length; x++)
				script_functions[x] = "123";
		}

		public static void OutputFunctionNames(bool compact, string k_test_results_path, string file_name, string[] script_functions)
		{
			using (var t = new System.Xml.XmlTextWriter(System.IO.Path.Combine(k_test_results_path, file_name), System.Text.Encoding.ASCII))
			{
				t.Indentation = 1;
				t.IndentChar = '\t';
				t.Formatting = System.Xml.Formatting.Indented;

				t.WriteStartDocument();
				t.WriteStartElement("Functions");
				for (int x = 0; x < script_functions.Length; x++)
				{
					if (script_functions[x] != "123")
					{
						t.WriteStartElement("entry");
						t.WriteAttributeString("key", "0x" + x.ToString("X3"));
						t.WriteAttributeString("value", script_functions[x]);
						t.WriteEndElement();
					}
					else if (!compact)
					{
						t.WriteStartElement("entry");
						t.WriteAttributeString("key", "0x" + x.ToString("X3"));
						t.WriteAttributeString("value", "UNKNOWN");
						t.WriteEndElement();
					}
				}
				t.WriteEndElement();
				t.WriteEndDocument();
			}
		}
		#endregion
	};

	abstract class ScenarioScriptInteropGen3
	{
		#region Fields
		int scnr_offset;

		protected Util.StringPool strings_pool;
		protected TI.IBlock sncr_hs_scripts;
		protected TI.Block<Blam.Halo3.Tags.hs_globals_block> hs_globals;
		protected TI.Block<Blam.Halo3.Tags.syntax_datum_block> hs_nodes;
		#endregion

		#region Ctor
		void ReadStringData(Blam.Cache.CacheFileGen3 cf, int hs_string_constants_offset)
		{
			Scripts.InteropReadStringData(cf, scnr_offset + hs_string_constants_offset, out strings_pool, 0xCD);
		}
		void ReadTagData(Blam.Cache.CacheFileGen3 cf, 
			int hs_scripts_offset, int hs_globals_offset,
			int hs_nodes_offset)
		{
			//hs_scripts = new TagInterface.Block<Blam.HaloReach.Tags.hs_scripts_block>(null, 0);
			hs_globals = new TagInterface.Block<Blam.Halo3.Tags.hs_globals_block>(null, 0);
			Scripts.InteropReadTagData(cf,
				sncr_hs_scripts,scnr_offset + hs_scripts_offset,
				hs_globals,		scnr_offset + hs_globals_offset);

			hs_nodes = new TagInterface.Block<Blam.Halo3.Tags.syntax_datum_block>(null, 0);
			Scripts.InteropReadNodes(cf, scnr_offset + hs_nodes_offset, hs_nodes);
		}
		protected ScenarioScriptInteropGen3(Blam.Cache.CacheFileGen3 cf, 
			TI.IBlock hs_scripts,
			int hs_string_constants_offset, 
			int hs_scripts_offset, int hs_globals_offset,
			int hs_nodes_offset)
		{
			scnr_offset = -1;
			foreach (var ci in cf.Index)
				// The scenario's group tag has never changed so this check is OK
				if (!ci.IsEmpty && ci.GroupTag.ID == Blam.Halo3.TagGroups.scnr.ID)
					scnr_offset = ci.Offset;

			sncr_hs_scripts = hs_scripts;
			ReadStringData(cf, hs_string_constants_offset);
			ReadTagData(cf, hs_scripts_offset, hs_globals_offset,
				hs_nodes_offset);
		}
		#endregion

		#region DumpScriptGraphs
		public void DumpScriptGraphs(Blam.Cache.CacheFileGen3 cf, System.IO.StreamWriter sw)
		{
			var node_interop = Scripting.ScriptNode.FromBlock(hs_nodes);
			var scripts_interop = Scripting.ScriptBlock.FromBlock(sncr_hs_scripts);
			var globals_interop = Scripting.GlobalsBlock.FromBlock(hs_globals);

			//foreach (var g in globals_interop) g.PostprocessNodes(node_interop);
			//foreach (var s in scripts_interop) s.PostprocessNodes(node_interop);

			foreach (var g in globals_interop) g.DumpGraph(sw, strings_pool);
			foreach (var s in scripts_interop) s.DumpGraph(sw, strings_pool);
		}
		#endregion

		#region ScanForScriptFunctions
		public void FindFunctionNames(string[] script_functions)
		{
			bool prev_was_script_predicate = false;
			List<string> funcs = new List<string>();
			foreach (Scripting.hs_syntax_datum_block n in hs_nodes.GetElements())
			{
				if (!prev_was_script_predicate && n.Type.Value == 2 && n.Flags.Value == 9)
				{
					var func = strings_pool[(uint)n.Pointer.Value];

					script_functions[n.TypeUnion] = func;
				}

				prev_was_script_predicate = n.Flags.Value == 0xA;
			}
		}
		#endregion
	};
}