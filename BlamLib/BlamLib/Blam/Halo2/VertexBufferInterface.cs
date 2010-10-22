/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
﻿using System;
using System.Collections.Generic;
using System.Xml;
using BlamLib.Managers;
using D3D = SlimDX.Direct3D9;

namespace BlamLib.Render
{
	partial class VertexBufferInterface
	{
		/// <summary>
		/// Halo 2 Vertex definitions
		/// </summary>
		public class VertexBuffersHalo2 : VertexBuffers
		{
			public const string
				kTypeAnisoBinormal =		"AnisoBinormal",
				kTypeIncidentRadiosity =	"IncidentRadiosity",

				kTypeSecondaryTexCoord =	"SecondaryTexCoord",
				kTypeSecondaryPosition =	"SecondaryPosition",
				kTypeSecondaryNodeIndices =	"SecondaryNodeIndices",
				kTypeSecondaryNodeWeights =	"SecondaryNodeWeights",
				kTypeSecondaryIsqSelect =	"SecondaryIsqSelect",

				kTypeTintFactor =			"TintFactor",
				kTypeDsqPlane =				"DsqPlane",
				kTypeBillboardOffset =		"BillboardOffset",
				kTypeBillboardAxis =		"BillboardAxis",
				kTypePcaClusterId =			"PcaClusterId",
				kTypePcaVertexWeights =		"PcaVertexWeights"
				;

			#region Type
			public sealed class Type : TypeBase
			{
				public readonly D3D.DeclarationMethod DeclarationMethod;
				public readonly D3D.DeclarationUsage DeclarationUsage;
				public readonly byte UsageIndex;

				public Type(IO.XmlStream s) : base(s)
				{
					s.ReadAttributeOpt("declMethod", ref DeclarationMethod);
					s.ReadAttribute("declUsage", ref DeclarationUsage);
					s.ReadAttributeOpt("usageIndex", 10, ref UsageIndex);
				}
			};
			List<Type> Types = null;

			protected override void TypesRead(IO.XmlStream s)
			{
				Types = new List<Type>(s.Cursor.ChildNodes.Count);

				foreach (XmlNode node in s.Cursor.ChildNodes)
				{
					if (node.Name != "type") continue;

					s.SaveCursor(node);
					Types.Add(new Type(s));
					s.RestoreCursor();
				}
				Types.TrimExcess();
			}

			/// <summary>
			/// Find a type definition based on its name
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			public override TypeBase TypeFind(string name)
			{
				foreach (Type t in Types) if (t.Name == name) return t;

				return null;
			}

			/// <summary>
			/// Find a type definition based on its opcode
			/// </summary>
			/// <param name="opcode"></param>
			/// <returns></returns>
			public override TypeBase TypeFind(short opcode)
			{
				foreach (Type t in Types) if (t.Opcode == opcode) return t;

				return null;
			}
			#endregion

			#region Definition
			public sealed class Element : ElementBase
			{
				public readonly Type Type;
				public readonly byte UsageIndex;

				public Element(VertexBuffers owner, Definition parent, IO.XmlStream s) : base(parent, s)
				{
					string temp = string.Empty;
					s.ReadAttribute("type", ref temp);
					Type t = owner.TypeFind(temp) as Type;

					if (t != null) this.Type = t;
					else throw new Debug.ExceptionLog("'{0}' isn't a defined type (Owner: {1}/{2})", temp, parent.Opcode.ToString("X2"), parent.Name);

					if (base.DeclarationType == DeclarationType.Skip)
						s.ReadAttribute("usageIndex", 10, ref UsageIndex);
					else
						UsageIndex = 0;
				}

				public override TypeBase GetTypeBase()	{ return Type; }
				public override byte GetUsageData()		{ return UsageIndex; }
			};

			public sealed class Definition : DefinitionBase
			{
				public readonly int Size = 0;
				public readonly Element[] Elements;

				public Definition(VertexBuffers owner, IO.XmlStream s) : base(owner, s)
				{
					//s.ReadAttribute("size", 16, ref Size);

					if (s.Cursor.ChildNodes.Count > 0)
					{
						var elems = new List<Element>(s.Cursor.ChildNodes.Count);

						foreach (XmlNode node in s.Cursor.ChildNodes)
						{
							if (node.Name != "element") continue;

							s.SaveCursor(node);
							elems.Add(new Element(owner, this, s));
							s.RestoreCursor();
						}

						elems.TrimExcess();
						Elements = elems.ToArray();
						foreach (Element e in Elements) Size += GetElementSize(e);
					}
				}

				public override int GetSize()				{ return Size; }
				public override ElementBase[] GetElements()	{ return Elements; }
			};
			List<Definition> Definitions = null;

			protected override void DefinitionsRead(VertexBuffers owner, IO.XmlStream s)
			{
				Definitions = new List<Definition>(s.Cursor.ChildNodes.Count);

				foreach (XmlNode node in s.Cursor.ChildNodes)
				{
					if (node.Name != "definition") continue;

					s.SaveCursor(node);
					Definitions.Add(new Definition(owner, s));
					s.RestoreCursor();
				}
				Definitions.TrimExcess();
			}

			/// <summary>
			/// Find a vertex buffer definition based on its name
			/// </summary>
			/// <param name="name"></param>
			/// <returns></returns>
			public override DefinitionBase DefinitionFind(string name)
			{
				foreach (Definition t in Definitions) if (t.Name == name) return t;

				return null;
			}

			/// <summary>
			/// Find a vertex buffer definition based on its opcode
			/// </summary>
			/// <param name="opcode"></param>
			/// <returns></returns>
			public override DefinitionBase DefinitionFind(short opcode)
			{
				foreach (Definition t in Definitions) if (t.Opcode == opcode) return t;

				return null;
			}
			#endregion

			internal VertexBuffersHalo2() { }

			#region IGameResource Members
			public override bool Load(string path, string name)
			{
				using (IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
				{
					#region Game
					s.ReadAttribute("game", ref base.engine);

					//Debug.Assert.If(Engine == ver, "Version mismatch! '{0}' expected '{1}'", Engine.ToString(), ver.ToString());
					#endregion

					foreach (XmlNode n in s.Cursor.ChildNodes)
						if (n.Name == "types")
						{
							s.SaveCursor(n);
							TypesRead(s);
							s.RestoreCursor();
						}
						else if (n.Name == "definitions")
						{
							s.SaveCursor(n);
							DefinitionsRead(this, s);
							s.RestoreCursor();
						}						
				}

				return true;
			}

			public override void Close()
			{
			}
			#endregion
		};
	};
};