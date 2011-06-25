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
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaAnnotate : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _name;
		ColladaObjectValue<bool?> _bool;
		ColladaObjectElement<ColladaValueArray<bool>> _bool2;
		ColladaObjectElement<ColladaValueArray<bool>> _bool3;
		ColladaObjectElement<ColladaValueArray<bool>> _bool4;
		ColladaObjectValue<int?> _int;
		ColladaObjectElement<ColladaValueArray<int>> _int2;
		ColladaObjectElement<ColladaValueArray<int>> _int3;
		ColladaObjectElement<ColladaValueArray<int>> _int4;
		ColladaObjectValue<float?> _float;
		ColladaObjectElement<ColladaValueArray<float>> _float2;
		ColladaObjectElement<ColladaValueArray<float>> _float3;
		ColladaObjectElement<ColladaValueArray<float>> _float4;
		ColladaObjectElement<ColladaValueArray<float>> _float2x2;
		ColladaObjectElement<ColladaValueArray<float>> _float3x3;
		ColladaObjectElement<ColladaValueArray<float>> _float4x4;
		ColladaObjectValue<string> _string;
		#endregion

		#region Attributes
		[XmlAttribute("name")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		#region bools
		[XmlElement("bool")]
		public bool? Bool
		{ get { return _bool.Value; } set { _bool.Value = value; } }

		[XmlElement("bool2")]
		public ColladaValueArray<bool> Bool2
		{ get { return _bool2.Value; } set { _bool2.Value = value; } }

		[XmlElement("bool3")]
		public ColladaValueArray<bool> Bool3
		{ get { return _bool3.Value; } set { _bool3.Value = value; } }

		[XmlElement("bool4")]
		public ColladaValueArray<bool> Bool4
		{ get { return _bool4.Value; } set { _bool4.Value = value; } }
		#endregion

		#region ints
		[XmlElement("int")]
		public int? Int
		{ get { return _int.Value; } set { _int.Value = value; } }

		[XmlElement("int2")]
		public ColladaValueArray<int> Int2
		{ get { return _int2.Value; } set { _int2.Value = value; } }

		[XmlElement("int3")]
		public ColladaValueArray<int> Int3
		{ get { return _int3.Value; } set { _int3.Value = value; } }

		[XmlElement("int4")]
		public ColladaValueArray<int> Int4
		{ get { return _int4.Value; } set { _int4.Value = value; } }
		#endregion

		#region floats
		[XmlElement("float")]
		public float? Float
		{ get { return _float.Value; } set { _float.Value = value; } }

		[XmlElement("float2")]
		public ColladaValueArray<float> Float2
		{ get { return _float2.Value; } set { _float2.Value = value; } }

		[XmlElement("float3")]
		public ColladaValueArray<float> Float3
		{ get { return _float3.Value; } set { _float3.Value = value; } }

		[XmlElement("float4")]
		public ColladaValueArray<float> Float4
		{ get { return _float4.Value; } set { _float4.Value = value; } }

		[XmlElement("float2x2")]
		public ColladaValueArray<float> Float2x2
		{ get { return _float2x2.Value; } set { _float2x2.Value = value; } }

		[XmlElement("float3x3")]
		public ColladaValueArray<float> Float3x3
		{ get { return _float3x3.Value; } set { _float3x3.Value = value; } }

		[XmlElement("float4x4")]
		public ColladaValueArray<float> Float4x4
		{ get { return _float4x4.Value; } set { _float4x4.Value = value; } }
		#endregion

		#region strings
		[XmlElement("string")]
		public string String
		{ get { return _string.Value; } set { _string.Value = value; } }
		#endregion
		#endregion

		public ColladaAnnotate() : base(Enums.ColladaElementType.Fx_Annotate)
		{
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_bool = new ColladaObjectValue<bool?>());
			Fields.Add(_bool2 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_bool3 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_bool4 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_int = new ColladaObjectValue<int?>());
			Fields.Add(_int2 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_int3 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_int4 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_float = new ColladaObjectValue<float?>());
			Fields.Add(_float2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2x2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3x3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4x4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_string = new ColladaObjectValue<string>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_bool);
			field_list.Add(_bool2);
			field_list.Add(_bool3);
			field_list.Add(_bool4);
			field_list.Add(_int);
			field_list.Add(_int2);
			field_list.Add(_int3);
			field_list.Add(_int4);
			field_list.Add(_float);
			field_list.Add(_float2);
			field_list.Add(_float3);
			field_list.Add(_float4);
			field_list.Add(_float2x2);
			field_list.Add(_float3x3);
			field_list.Add(_float4x4);
			field_list.Add(_string);

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _name));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _name));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
		}
	}
}