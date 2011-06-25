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
using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaParam : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<string> _type;
		ColladaObjectAttribute<string> _semantic;
		#endregion

		[XmlIgnore]
		private string[] validTypesAccessor = { 
			"int", 
			"int2", "int2x2",
			"int3", "int3x3",
			"int4", "int4x4",

			"float", 
			"float2", "float2x2", "float2x3", "float2x4",
			"float3", "float3x2", "float3x3", "float3x4",
			"float4", "float4x2", "float4x3", "float4x4",
			"float7",

			"Name", 

			"bool", "bool2", "bool3", "bool4",

			"IDREF" };

		#region Attributes
		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("type")]
		public string Type
		{ get { return _type.Value; } set { _type.Value = value; } }

		[XmlAttribute("semantic"), DefaultValue("")]
		public string Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }
		#endregion

		public ColladaParam() : base(Enums.ColladaElementType.Core_Param)
		{
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_type = new ColladaObjectAttribute<string>(""));
			Fields.Add(_semantic = new ColladaObjectAttribute<string>(""));

			List<string> validTypes = new List<string>(validTypesAccessor);

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _type));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _type));
			ValidationTests.Add(new ColladaHasValidValue<string>(Enums.ColladaElementType.Core_Accessor, _type, validTypes));
		}
	}
}