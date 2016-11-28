/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Managers;

namespace BlamLib.Scripting
{
	// TODO: create a new 'key' system for external script 
	// expression macros so that different names can be used 
	// in different contexts. This is required for Halo 2 which 
	// has multiple macro functions with the same literal name 
	// however which one is used depends on their supplied 
	// arguments. So we'll have to create our own 'decoration' 
	// scheme possibly.
	// This should of course be applied to globals not to just 
	// be consistent, but because 'decals' is defined twice in 
	// Halo1_CE (maybe Xbox too?)...not sure if that was on 
	// purpose.

	internal class DictionaryHack<K1, K2, V>
	{
		public Dictionary<K1, V> Dic1 = new Dictionary<K1, V>();
		public Dictionary<K2, V> Dic2 = new Dictionary<K2, V>();

		public DictionaryHack() { }

		public void Add(K1 key1, K2 key2, V value)
		{
			Dic1.Add(key1, value);
			Dic2.Add(key2, value);
		}

		public bool Contains(K1 key1)
		{
			return Dic1.ContainsKey(key1);
		}

		public bool Contains(K2 key2)
		{
			return Dic2.ContainsKey(key2);
		}

		public V this[K1 key] { get { return Dic1[key]; } }
		public V this[K2 key] { get { return Dic2[key]; } }

		public int Count { get { return Dic1.Count; } }
	};

	/// <summary>
	/// Provides a interface with a XML file that has all the
	/// definitions for a script that a variant of the Blam
	/// Engine uses.
	/// </summary>
	[XmlType(TypeName = "BlamScript")]
	internal class XmlInterface : Managers.BlamDefinition.IGameResource
	{
		#region FunctionArg
		[XmlType(TypeName = "arg")]
		public class FunctionArg
		{
			/// <summary>
			/// Owning function definition
			/// </summary>
			public readonly Function Owner;

			/// <summary>
			/// Argument index in the owner function's signature
			/// </summary>
			[XmlElement("opcode")]
			public readonly int Opcode;

			/// <summary>
			/// Name of the Function
			/// </summary>
			[XmlElement("name", IsNullable=true)]
			public readonly string Name;

			/// <summary>
			/// The return type of script
			/// </summary>
			/// <remarks>Defined in xml using the string variant of the type (when constructed, the string is passed into a lookup function)</remarks>
			[XmlElement("type")]
			public readonly short Type;

			/// <summary>
			/// Help info on what this parameter is for
			/// </summary>
			[XmlElement("help", IsNullable=true)]
			public readonly string Help;

			/// <summary>
			/// Creates all the FunctionArg data members
			/// </summary>
			/// <param name="owner">owner of this argument</param>
			/// <param name="index">index of this argument in the owner function's argument list</param>
			/// <param name="s">data stream</param>
			public FunctionArg(Function owner, int index, IO.XmlStream s)
			{
				Owner = owner;
				Opcode = index;

				if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Empty;
				if (!s.ReadAttributeOpt("help", ref Help)) Help = string.Empty;

				string temp = null;
				s.ReadAttribute("type", ref temp);
				Type = ((XmlInterface)s.Owner).GetValueType(temp).Opcode;
			}
		};
		#endregion

		/// <summary>
		/// External (to the scenario; ie engine functions) function categorization
		/// </summary>
		public enum FunctionGroup : int
		{
			Macro,		// everything else

			Begin,
			If,
			Cond,
			Set,
			/// <remarks>and, or</remarks>
			Logical,
			/// <remarks>add, subtract, multiply, divide, min, max</remarks>
			Arithmetic,
			/// <remarks>equal, not equal</remarks>
			Equality,
			/// <remarks>gt, lt, gte, lte</remarks>
			Inequality,
			Sleep,
			/// <remarks>starting with halo 2</remarks>
			SleepForever,
			SleepUntil,
			Wake,
			Inspect,
			ObjectCast,
			/// <remarks>halo 1 only</remarks>
			DebugString,

			/// <remarks>halo pc \ ce</remarks>
			Networking,
		};

		#region Function
		/// <summary>
		/// Specifies a definition of a function
		/// </summary>
		[XmlType(TypeName = "function")]
		public class Function
		{
			[XmlElement("opcode")]
			public readonly short Opcode;

			/// <summary>
			/// The return type of script
			/// </summary>
			[XmlElement("returnType")]
			public readonly short ReturnType;

			/// <summary>
			/// Name of the Function
			/// </summary>
			[XmlElement("name", IsNullable=true)]
			public readonly string Name;

			/// <summary>
			/// The Argument count for the function
			/// </summary>
			[XmlElement("argc", IsNullable=true)]
			public readonly short Argc;

			/// <summary>
			/// # of Arguments who are optional
			/// </summary>
			[XmlElement("opArgc", IsNullable=true)]
			public readonly short OpArgc;

			/// <summary>
			/// The argument list for this function
			/// </summary>
			[XmlArray(ElementName = "arg", IsNullable=true)]
			public readonly FunctionArg[] Args;

			/// <summary>
			/// Engine dependent flags
			/// </summary>
			[XmlElement("flags", IsNullable=true)]
			public readonly Util.Flags Flags;

			/// <summary>
			/// Allows us to declare special case function entities (for handling 
			/// parsing of 'if's and 'cond's, etc)
			/// </summary>
			[XmlElement("group", IsNullable=true)]
			public readonly FunctionGroup Group;

			/// <summary>
			/// Help info for the purpose of this function
			/// </summary>
			[XmlElement("help", IsNullable=true)]
			public readonly string Help;

			/// <summary>
			/// Help info for the argument list
			/// </summary>
			[XmlElement("helpArg", IsNullable=true)]
			public readonly string HelpArg;

			/// <summary>
			/// Function effects are null in-game
			/// </summary>
			[XmlElement("null", IsNullable = true)]
			public readonly bool IsNull;

			/// <summary>
			/// Creates all the Function data members
			/// </summary>
			/// <param name="s"></param>
			public Function(IO.XmlStream s)
			{
				s.ReadAttribute("opcode", 16, ref Opcode);
				
				{
					string temp = null;
					s.ReadAttribute("returnType", ref temp);
					ReturnType = ((XmlInterface)s.Owner).GetValueType(temp).Opcode;
				}

				if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Format("function_{0:X}", Opcode);
				if (!s.ReadAttributeOpt("argc", 10, ref Argc)) Argc = 0;
				if (!s.ReadAttributeOpt("opArgc", 10, ref OpArgc)) OpArgc = 0;

				{
					uint temp = 0;
					s.ReadAttributeOpt("flags", 16, ref temp);
					Flags = new Util.Flags(temp);
				}

				if (!s.ReadAttributeOpt<FunctionGroup>("group", ref Group)) Group = FunctionGroup.Macro;

				#region Args
				if (Argc > 0)
				{
					int x = 0;
					Args = new FunctionArg[Argc];
					foreach (XmlNode n in s.Cursor.ChildNodes)
					{
						if (n.Name != "arg") continue;

						s.SaveCursor(n);
						Args[x] = new FunctionArg(this, x++, s);
						s.RestoreCursor();
					}
				}
				else
					Args = null;
				#endregion

				if (!s.ReadAttributeOpt("help", ref Help)) Help = string.Empty;
				if (!s.ReadAttributeOpt("helpArg", ref HelpArg)) HelpArg = string.Empty;

				if (!s.ReadAttributeOpt("null", ref IsNull)) IsNull = false;
			}
		};
		#endregion

		#region Global
		/// <summary>
		/// Specifies a definition of a global
		/// </summary>
		[XmlType(TypeName = "global")]
		public class Global
		{
			[XmlElement("opcode")]
			public readonly short Opcode;

			/// <summary>
			/// The global's type
			/// </summary>
			[XmlElement("Type")]
			public readonly short Type;

			/// <summary>
			/// Name of the Global
			/// </summary>
			[XmlElement("name", IsNullable=true)]
			public readonly string Name;

			/// <summary>
			/// Help info for the purpose of this global
			/// </summary>
			[XmlElement("help", IsNullable=true)]
			public readonly string Help;

			/// <summary>
			/// Engine dependent flags
			/// </summary>
			[XmlElement("flags", IsNullable = true)]
			public readonly Util.Flags Flags;

			/// <summary>
			/// Global data is null in-game
			/// </summary>
			[XmlElement("null", IsNullable = true)]
			public readonly bool IsNull;

			/// <summary>
			/// Creates all the Global data members
			/// </summary>
			/// <param name="s"></param>
			public Global(IO.XmlStream s)
			{
				s.ReadAttribute("opcode", 16, ref Opcode);

				{
					string temp = null;
					s.ReadAttribute("type", ref temp);
					Type = ((XmlInterface)s.Owner).GetValueType(temp).Opcode;
				}

				if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Format("global_{0:X}", Opcode);
				if (!s.ReadAttributeOpt("help", ref Help)) Help = string.Empty;

				if (!s.ReadAttributeOpt("null", ref IsNull)) IsNull = false;

				{
					uint temp = 0;
					s.ReadAttributeOpt("flags", 16, ref temp);
					Flags = new Util.Flags(temp);
				}
			}
		};
		#endregion

		#region ScriptType
		/// <summary>
		/// Provides a definition for a script type in the blam engine
		/// </summary>
		[XmlType(TypeName = "type")]
		public class ScriptType
		{
			/// <summary>
			/// The opcode value of the script type
			/// </summary>
			[XmlElement("opcode")]
			public readonly short Opcode;

			/// <summary>
			/// The string value of the script type
			/// </summary>
			[XmlElement("name", IsNullable=true)]
			public readonly string Name;

			/// <summary>
			/// Help info for the purpose of this script type
			/// </summary>
			[XmlElement("help", IsNullable=true)]
			public readonly string Help;

			/// <summary>
			/// Creates all the ScriptType data members
			/// </summary>
			/// <param name="s"></param>
			public ScriptType(IO.XmlStream s)
			{
				s.ReadAttribute("opcode", 16, ref Opcode);

				if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Format("script_type_{0:X}", Opcode);
				if (!s.ReadAttributeOpt("help", ref Help)) Help = string.Empty;
			}
		};
		#endregion

		#region Type
		/// <summary>
		/// Provides a definition for a scripted value type in blam engine
		/// </summary>
		[XmlType(TypeName = "type")]
		public class Type
		{
			public const string TagReferenceAnyTag = "BLAM";

			#region Opcode
			/// <summary>
			/// The opcode value of the value type
			/// </summary>
			[XmlElement("opcode")]
			public readonly short Opcode;
			#endregion

			#region Size
			/// <summary>
			/// Size of an instance of this type
			/// </summary>
			[XmlElement("size")]
			public readonly short Size;
			#endregion

			#region Name
			/// <summary>
			/// The string value of the value type
			/// </summary>
			[XmlElement("name", IsNullable=true)]
			public readonly string Name;
			#endregion

			#region Help
			/// <summary>
			/// Help info for the purpose of this script type
			/// </summary>
			[XmlElement("help", IsNullable = true)]
			public readonly string Help;
			#endregion

			#region Default
// 			[XmlElement("DefaultInt", typeof(int)),
// 			XmlElement("DefaultString", typeof(string)),
// 			XmlElement("DefaultReal", typeof(float)),
// 			XmlElement("DefaultBool", typeof(bool))]
			public readonly string Default;
			#endregion

			#region Type specific
			[XmlElement("tagReference", IsNullable=true)]
			public readonly bool IsTagReference;
			[XmlElement("tag", IsNullable=true)]
			public readonly uint Tag;

			[XmlElement("enum", IsNullable=true)]
			public readonly bool IsEnum;
			private readonly string[] Enums;

			/// <summary>
			/// Value is suppose to be enclosed in quotes
			/// </summary>
			[XmlElement("quoted", IsNullable=true)]
			public readonly bool Quoted;

			/// <summary>
			/// Value is some kind of object
			/// </summary>
			[XmlElement("object", IsNullable=true)]
			public readonly bool IsObject;

			/// <summary>
			/// Value is a specific object type (ie "scenery")
			/// </summary>
			public bool IsObjectType { get { return IsObject && !Quoted; } }
			/// <summary>
			/// Value is a name value to a object (ie, "scenery_name")
			/// </summary>
			public bool IsObjectName { get { return IsObject && Quoted; } }
			#endregion

			public short ToEnum(string value)
			{
				if (Enums != null)
				{
					for (short x = 0; x < Enums.Length; x++)
						if (value == Enums[x])
							return x;
				}

				return -1;
			}

			public string ToEnum(short value)
			{
				if (Enums != null && !(Enums.Length > value))
					return Enums[value];

				return "<enum error>";
			}

			/// <summary>
			/// Creates all the Type data members
			/// </summary>
			/// <param name="s"></param>
			public Type(IO.XmlStream s)
			{
				s.ReadAttribute("opcode", 16, ref Opcode);
				s.ReadAttribute("size", 16, ref Size);

				if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Format("type_{0:X}", Opcode);
				if (!s.ReadAttributeOpt("help", ref Help)) Help = string.Empty;
				if (!s.ReadAttributeOpt("default", ref Default)) Default = string.Empty;

				string temp = null;
				#region TagReference
				if (s.ReadAttributeOpt("tag", ref temp))
				{
					IsTagReference = true;
					if(temp == TagReferenceAnyTag)
						Tag = uint.MaxValue;
					else
						Tag = TagInterface.TagGroup.ToUInt(temp);
				}
				else
				{
					IsTagReference = false;
					Tag = uint.MinValue;
				}
				#endregion

				#region Enum
				if(s.ElementsExist)
				{
					IsEnum = true;
					Enums = new string[s.Cursor.ChildNodes.Count];
					int x = 0;
					foreach(XmlNode n in s.Cursor.ChildNodes)
						if (n.Name == "enum")
							Enums[x++] = n.InnerText;
				}
				else
				{
					IsEnum = false;
					Enums = null;
				}
				#endregion

				if (!s.ReadAttributeOpt("quoted", ref Quoted)) Quoted = false;
				if (!s.ReadAttributeOpt("object", ref IsObject)) IsObject = false;
			}
		};
		#endregion


		#region Members
		#region Engine
		BlamVersion engine = BlamVersion.Unknown;
		/// <summary>
		/// The engine version this definition is for
		/// </summary>
		public BlamVersion Engine { get { return engine; } }
		#endregion

		#region FileEx
		string fileEx = "";
		/// <summary>
		/// The File Extension of the scripts
		/// </summary>
		public string FileEx { get { return fileEx; } }
		#endregion

		#region MaxNodes
		int maxNodes;
		/// <summary>
		/// Maximum amount of nodes a script graph can use for this definition
		/// </summary>
		public int MaxNodes { get { return maxNodes; } }
		#endregion

		#region MaxDynamicSourceDataBytes
		int maxDynamicSourceDataBytes;
		/// <summary>
		/// Maximum amount of string data, in bytes, the engine sets aside for 
		/// dynamic (read: runtime) operations
		/// </summary>
		/// <remarks>
		/// Gen 1 & 2 = 0x400
		/// Gen 3 = 0x1000
		/// </remarks>
		public int MaxDynamicSourceDataBytes { get { return maxDynamicSourceDataBytes; } }
		#endregion

		#region NodeSalt
		short nodeSalt;
		/// <summary>
		/// Salt value used in the script node datum indices
		/// </summary>
		public short NodeSalt { get { return nodeSalt; } }
		#endregion

		/// <summary>
		/// File to inherit definitions from. The file that defines this 
		/// will only have functions and globals defined.
		/// </summary>
		string inheritFrom = string.Empty;

		/// <summary>
		/// This BlamScripts Functions
		/// </summary>
		public DictionaryHack<string, short, Function> Functions = new DictionaryHack<string, short, Function>();
		/// <summary>
		/// The amount of Functions
		/// </summary>
		public int FunctionCount { get { return Functions.Count; } }

		#region Globals
		/// <summary>
		/// This BlamScripts Globals
		/// </summary>
		public DictionaryHack<string, short, Global> Globals = new DictionaryHack<string, short, Global>();
		/// <summary>
		/// The amount of Globals
		/// </summary>
		public int GlobalCount { get { return Globals.Count; } }
		#endregion

		#region Scripts
		/// <summary>
		/// This BlamScripts Script Types
		/// </summary>
		public DictionaryHack<string, short, ScriptType> Scripts = new DictionaryHack<string, short, ScriptType>();

		/// <summary>
		/// Number of different script types in this definition
		/// </summary>
		public int ScriptTypeCount { get { return Scripts.Count; } }
		#endregion

		#region Types
		/// <summary>
		/// This BlamScripts value types
		/// </summary>
		public DictionaryHack<string, short, Type> Types = new DictionaryHack<string, short, Type>();

		/// <summary>
		/// Number of different value types in this definition
		/// </summary>
		public int ValueTypeCount { get { return Types.Count; } }

		public Dictionary<string, uint> ObjectTypeMasks = new Dictionary<string, uint>();
		public List<int> ObjectTypeMasksList = new List<int>();

		public Dictionary<short, Dictionary<short, bool>> ValueTypeCasting;

		short objectTypeFirst = -1;
		/// <summary>
		/// Opcode of the first object value type
		/// </summary>
		public short ObjectTypeFirst { get { return objectTypeFirst; } }

		short objectTypeLast = -1;
		/// <summary>
		/// Opcode of the last object value type
		/// </summary>
		public short ObjectTypeLast { get { return objectTypeLast; } }

		short objectTypeNameFirst = -1;
		/// <summary>
		/// Opcode of the first object name value type
		/// </summary>
		public short ObjectTypeNameFirst { get { return objectTypeNameFirst; } }

		short objectTypeNameLast = -1;
		/// <summary>
		/// Opcode of the last object name value type
		/// </summary>
		public short ObjectTypeNameLast { get { return objectTypeNameLast; } }
		#endregion
		#endregion

		internal XmlInterface() {}

		#region Read
		/// <summary>
		/// Loads the necessary values to define a BlamScript
		/// </summary>
		/// <param name="s"></param>
		private void Read(IO.XmlStream s)
		{
			s.ReadAttribute("ext", ref fileEx);
			s.ReadAttribute("maxNodes", 10, ref maxNodes);
			s.ReadAttribute("maxDynamicSourceDataBytes", 16, ref maxDynamicSourceDataBytes);
			s.ReadAttribute("salt", 16, ref nodeSalt);

			foreach(XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name == "objectTypeMasks")
				{
					s.SaveCursor(n);
					ReadValueTypesObjectMasks(s);
					s.RestoreCursor();
				}
				else if (n.Name == "valueTypeCasting")
				{
					s.SaveCursor(n);
					ReadValueTypesObjectMasks(s);
					s.RestoreCursor();
				}
			}
		}
		#endregion

		#region Game Script Types
		private void ReadScripts(IO.XmlStream s)
		{
			ScriptType script;

			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "type") continue;

				s.SaveCursor(n);
				script = new ScriptType(s);
				s.RestoreCursor();

				Scripts.Add(script.Name, script.Opcode, script);
			}
		}

		/// <summary>
		/// Gets a <see cref="ScriptType"/> based on a string value
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		public ScriptType GetScriptType(string name) { return Scripts[name]; }

		/// <summary>
		/// Gets a <see cref="ScriptType"/> based on a 16 bit integer value
		/// </summary>
		/// <param name="opcode"></param>
		/// <returns></returns>
		public ScriptType GetScriptType(short opcode) { return Scripts[opcode]; }
		#endregion

		#region Game Value Types
		private void ReadValueTypes(IO.XmlStream s)
		{
			Type type;

			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "type") continue;

				s.SaveCursor(n);
				type = new Type(s);
				s.RestoreCursor();

				// find the range of object types
				if (objectTypeFirst == -1 && type.IsObjectType)			objectTypeFirst = type.Opcode;
				else if (objectTypeLast == -1 || type.IsObjectType)		objectTypeLast = type.Opcode;

				// find the range of types representing object naming types
				if (objectTypeNameFirst == -1 && type.IsObjectName)		objectTypeNameFirst = type.Opcode;
				else if (objectTypeNameLast == -1 || type.IsObjectName)	objectTypeNameLast = type.Opcode;

				Types.Add(type.Name, type.Opcode, type);
			}
		}

		/// <summary>
		/// Gets a <see cref="Type"/> based on a string value
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		public Type GetValueType(string name) { return Types[name]; }

		/// <summary>
		/// Gets a <see cref="Type"/> based on a 16 bit integer value
		/// </summary>
		/// <param name="opcode"></param>
		/// <returns></returns>
		public Type GetValueType(short opcode) { return Types[opcode]; }


		/// <summary>
		/// Masks MUST be in the order of the object types in the value types list
		/// </summary>
		/// <param name="s"></param>
		/// <example>
		///		<Object Type="object" Mask="0xFFFF" />
		/// </example>
		private void ReadValueTypesObjectMasks(IO.XmlStream s)
		{
			string temp = null;
			uint value = 0;
			XmlNode nodes = s.Cursor;
			foreach (XmlNode n in nodes.ChildNodes)
			{
				if (n.Name != "object") continue;

				s.Cursor = n;
				s.ReadAttribute("type", ref temp);
				s.ReadAttribute("mask", 16, ref value);

				ObjectTypeMasks.Add(temp, value);
				ObjectTypeMasksList.Add(unchecked((int)value));
			}
		}


		/// <summary>
		/// 
		/// </summary>
		/// <param name="s"></param>
		/// <example>
		///		<Cast From="object" To="void" />
		/// </example>
		private void ReadValueTypeCasting(IO.XmlStream s)
		{
			ValueTypeCasting = new Dictionary<short, Dictionary<short, bool>>(ValueTypeCount);
			for (short x = 0; x < ValueTypeCount; x++)
				ValueTypeCasting[x] = new Dictionary<short, bool>(ValueTypeCount);

			string temp = null;
			short from_type;
			short to_type;
			XmlNode nodes = s.Cursor;
			foreach (XmlNode n in nodes.ChildNodes)
			{
				if (n.Name != "cast") continue;

				s.Cursor = n;
				s.ReadAttribute("from", ref temp);
				from_type = GetValueType(temp).Opcode;
				s.ReadAttribute("to", ref temp);
				to_type = GetValueType(temp).Opcode;

				ValueTypeCasting[from_type][to_type] = true;
			}
		}
		#endregion

		#region Game Functions
		private void ReadFunctions(IO.XmlStream s)
		{
			Function func;

			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "function") continue;

				s.SaveCursor(n);
				func = new Function(s);
				s.RestoreCursor();

				Functions.Add(func.Name, func.Opcode, func);
			}
		}

		/// <summary>
		/// Returns a function definition from a string representing its name
		/// </summary>
		/// <param name="name">Name of the function</param>
		/// <returns>the function definiton</returns>
		public Function GetFunction(string name) { return Functions[name]; }

		/// <summary>
		/// Returns a function definition from a opcode representing a function
		/// </summary>
		/// <param name="opcode">Opcode of a function</param>
		/// <returns>the function definiton</returns>
		public Function GetFunction(short opcode) { return Functions[opcode]; }
		#endregion

		#region Game Globals
		private void ReadGlobals(IO.XmlStream s)
		{
			Global global;

			foreach (XmlNode n in s.Cursor.ChildNodes)
			{
				if (n.Name != "global") continue;

				s.SaveCursor(n);
				global = new Global(s);
				s.RestoreCursor();

				Globals.Add(global.Name, global.Opcode, global);
			}
		}

		/// <summary>
		/// Returns a global definition from a string representing its name
		/// </summary>
		/// <param name="name">Name of the global</param>
		/// <returns>the global definition</returns>
		public Global GetGlobal(string name) { return Globals[name]; }

		/// <summary>
		/// Returns a global definition from a opcode representing a global
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		public Global GetGlobal(short index) { return Globals[index]; }
		#endregion

		public Blam.DatumIndex DatumFromIndex(int index) { return new Blam.DatumIndex((ushort)index, (short)((index - nodeSalt) & 0xFFFF)); }

		#region IGameResource Members
		private bool LoadProcessNode(IO.XmlStream s, XmlNode n)
		{
			switch(n.Name)
			{
				case "definition":
					s.SaveCursor(n);
					Read(s); // Load the Main Data
					s.RestoreCursor();
					return true;

				case "scriptTypes":
					s.SaveCursor(n);
					ReadScripts(s); // Load the definition's script types
					s.RestoreCursor();
					return true;

				case "valueTypes":
					s.SaveCursor(n);
					ReadValueTypes(s); // Load the definition's value types
					s.RestoreCursor();
					return true;
			}

			return false;
		}

		private bool LoadInherited(string path, string name)
		{
			using (IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
			{
				BlamVersion inherited_engine = BlamVersion.Unknown;
				s.ReadAttribute("game", ref inherited_engine);

				Debug.Assert.If(inherited_engine.ToBuild() == engine.ToBuild(), "Inherit failed: expected '{0}', got '{1}' in '{2}{3}'",
					engine.ToBuild().ToString(), inherited_engine.ToBuild().ToString(), path, name);

				foreach (XmlNode n in s.Cursor.ChildNodes)
					LoadProcessNode(s, n);
			}
			return true;
		}

		public bool Load(string path, string name)
		{
			using (IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
			{
				s.ReadAttribute("game", ref engine);

				bool inherited;
				if ( inherited = s.ReadAttributeOpt("inheritFrom", ref inheritFrom) )
				{
					path = System.IO.Path.GetDirectoryName(inheritFrom);
					name = System.IO.Path.GetFileName(inheritFrom);

					if (!LoadInherited(path, name))
						return false;
				}

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (!inherited && LoadProcessNode(s, n))
						continue;
					else if (n.Name == "functions")
					{
						s.SaveCursor(n);
						ReadFunctions(s); // Load the definition's functions
						s.RestoreCursor();
					}
					else if (n.Name == "globals")
					{
						s.SaveCursor(n);
						ReadGlobals(s); // Load the definition's globals
						s.RestoreCursor();
					}
				}
			}

			return true;
		}

		public void Close()
		{
		}
		#endregion
	};
}