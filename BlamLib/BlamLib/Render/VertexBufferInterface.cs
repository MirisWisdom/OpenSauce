/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using BlamLib.Managers;
using D3D = SlimDX.Direct3D9;

namespace BlamLib.Render
{
	public enum DeclarationType
	{
		/// <summary>
		/// Skip a stride of bytes
		/// </summary>
		Skip = -2,
		/// <summary>
		/// Invalid
		/// </summary>
		Unused = -1,

		Float1 = 0,	// 0
		Float2,		// 1
		Float3,		// 2
		Float4,		// 3

		Ubyte4,		// 4
		UbyteN4,	// 5

		Color,		// 6

		UShort2,	// 7
		UShort4,	// 8, educated guess

		UShortN2,	// 9
		UShortN4,	// A

		UShortN2_2,	// B, IDK why this translated to UShortN2?
		UShortN4_2,	// C, IDK why this translated to UShortN4?

		UDHen3N,	// D, educated guess
		DHen3N,		// E

		Float16Two,	// F
		Float16Four,// 10

		Dec3N,		// 11
		UHenD3N,	// 12
		UDec4N,		// 13

		ByteN4, // added for support for 's_decorator_vertex' in H3.

		#region Legacy
		/// <remarks>Halo 2</remarks>
		UbyteN,
		/// <remarks>Halo 2</remarks>
		UbyteN2,
		/// <remarks>Halo 2</remarks>
		UbyteN3,

		/// <remarks>Halo 2</remarks>
		UShortN,
		/// <remarks>Halo 2</remarks>
		UShortN3,

		/// <remarks>Halo 2</remarks>
		ShortN,
		/// <remarks>Halo 2</remarks>
		ShortN2,
		/// <remarks>Halo 2</remarks>
		ShortN3,
		/// <remarks>Halo 2</remarks>
		ShortN4,

		/// <remarks>Halo 1\2</remarks>
		HenD3N,
		#endregion
	};


	internal partial class VertexBufferInterface
	{
		public const string
			kTypePosition =		"Position",
			kTypeNodeIndices =	"NodeIndices",
			kTypeNodeWeights =	"NodeWeights",
			kTypeTexCoord =		"TexCoord",
			kTypeNormal =		"Normal",
			kTypeBinormal =		"Binormal",
			kTypeTangent =		"Tangent",

			kTypeColor =		"Color"
			;

		/// <summary>
		/// Base Vertex Buffer definitions interface
		/// </summary>
		public abstract class VertexBuffers : Managers.BlamDefinition.IGameResource
		{
			#region Type base
			public abstract class TypeBase
			{
				public readonly short Opcode;
				/// <summary>
				/// Name of the Type
				/// </summary>
				public readonly string Name;

				protected TypeBase(IO.XmlStream s)
				{
					if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Empty;
					s.ReadAttribute("opcode", 16, ref Opcode);
				}
			};

			protected abstract void TypesRead(IO.XmlStream s);

			/// <summary>
			/// Find a type definition based on its name
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			public abstract TypeBase TypeFind(string name);

			/// <summary>
			/// Find a type definition based on its opcode
			/// </summary>
			/// <param name="opcode"></param>
			/// <returns></returns>
			public abstract TypeBase TypeFind(short opcode);
			#endregion

			#region Definition base
			public abstract class ElementBase
			{
				public readonly string Name;
				public abstract TypeBase GetTypeBase();
				public readonly DeclarationType DeclarationType;

				protected ElementBase(DefinitionBase parent, IO.XmlStream s)
				{
					Name = string.Empty;

					s.ReadAttributeOpt("name", ref Name);
					s.ReadAttribute("declType", ref DeclarationType);
				}

				public abstract byte GetUsageData();
			};

			public abstract class DefinitionBase
			{
				public readonly short Opcode;
				public readonly string Name;
				public abstract int GetSize();
				public abstract ElementBase[] GetElements();

				protected DefinitionBase(VertexBuffers owner, IO.XmlStream s)
				{
					if (!s.ReadAttributeOpt("name", ref Name)) Name = string.Empty;
					s.ReadAttribute("opcode", 16, ref Opcode);
					if (Name == "") Name = Opcode.ToString();
				}

				protected static int GetElementSize(ElementBase e)
				{
					if (e.DeclarationType != DeclarationType.Skip)
						return e.DeclarationType.GetSize();

					return e.GetUsageData();
				}
			};

			protected abstract void DefinitionsRead(VertexBuffers owner, IO.XmlStream s);

			/// <summary>
			/// Find a vertex buffer definition based on its name
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			public abstract DefinitionBase DefinitionFind(string name);

			/// <summary>
			/// Find a vertex buffer definition based on its opcode
			/// </summary>
			/// <param name="opcode"></param>
			/// <returns></returns>
			public abstract DefinitionBase DefinitionFind(short opcode);
			#endregion


			#region Engine
			protected BlamVersion engine;
			/// <summary>
			/// Engine these vertex buffer definitions belong to
			/// </summary>
			public BlamVersion Engine	{ get { return engine; } }
			#endregion

			#region IGameResource Members
			public abstract bool Load(string path, string name);
			public abstract void Close();
			#endregion
		};


		public sealed class StreamReader : IO.IStreamable
		{
			readonly VertexBuffers.DefinitionBase definition;
			readonly VertexBuffers.ElementBase[] elements;
			readonly DeclarationTypes.IDeclType[] declTypes;
			LowLevel.Math.real_quaternion[] streamedElements;

			//bool denormalized = false;

			/// <summary>
			/// Is the underlying vertex definition actually a platform's 
			/// "Null" vertex?
			/// </summary>
			public bool UsesNullDefinition { get {
				return definition.Name == "Null";
			} }

			public StreamReader(VertexBuffers.DefinitionBase stream_def)
			{
				definition = stream_def;
				if (UsesNullDefinition) return;

				elements = stream_def.GetElements();

				declTypes = new DeclarationTypes.IDeclType[elements.Length];
				streamedElements = new LowLevel.Math.real_quaternion[elements.Length];

				for (int x = 0; x < elements.Length; x++)
				{
					var dt = (declTypes[x] = elements[x].DeclarationType.AllocateDeclType());

					if (dt is DeclarationTypes.Skip)
						(dt as DeclarationTypes.Skip).ByteCount = elements[x].GetUsageData();
				}
			}

			#region IStreamable Members
			public void Read(IO.EndianReader s)
			{
				//denormalized = false;

				for (int x = 0; x < streamedElements.Length; x++)
					declTypes[x].Read(s);
			}

			public void Write(IO.EndianWriter s)	{ throw new NotSupportedException(); }
			#endregion

			public void Denormalize()
			{
				for (int x = 0; x < declTypes.Length; x++)
					declTypes[x].Denormalize(out streamedElements[x]);

				//denormalized = true;
			}

			// TODO: Have each engine's VBI's Definition have a bit-vector which maps with a 
			// string list with all possible Element names on that engine. Could enable faster 
			// and more complex Element querying later on

			/// <summary>
			/// Get an element streamed from a buffer by its element name (ie, 
			/// <see cref="kTypePosition"/>)
			/// </summary>
			/// <param name="element_type_name"></param>
			/// <param name="value"></param>
			/// <returns></returns>
			public bool FindStreamedElement(string element_type_name, ref LowLevel.Math.real_quaternion value)
			{
				int se_index = Array.FindIndex(elements, e => e.GetTypeBase().Name == element_type_name);

				if(se_index != -1)
					value = streamedElements[se_index];

				return se_index != -1;
			}

			public void GetStreamedElement(int element_index, ref LowLevel.Math.real_quaternion value)
			{
				value = streamedElements[element_index];
			}

			#region Util
			/// <summary>
			/// Get each element's source data as a string (before de-normalization)
			/// </summary>
			/// <returns></returns>
			public IEnumerable<string> GetNormalizedStrings()
			{
				for (int x = 0; x < declTypes.Length; x++)
					yield return declTypes[x].ToString();

				yield break;
			}

			/// <summary>
			/// Get each element as a formatted <see cref="LowLevel.Math.real_quaternion"/> string
			/// </summary>
			/// <returns></returns>
			/// <remarks>Calls <see cref="Denormalize"/> for you. You were probably lazy anyway, high-five.</remarks>
			public IEnumerable<string> GetDenormalizedStrings()
			{
				Denormalize();

				for (int x = 0; x < declTypes.Length; x++)
					yield return declTypes[x].ToString(streamedElements[x]);

				yield break;
			}
			#endregion
		};
	};
}