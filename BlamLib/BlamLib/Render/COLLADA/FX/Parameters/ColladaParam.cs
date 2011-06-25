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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaParam : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _ref;
		ColladaObjectValue<string> _text;
		#endregion

		#region Attributes
		[XmlAttribute("ref"), DefaultValue("")]
		public string Ref
		{ get { return _ref.Value; } set { _ref.Value = value; } }
		#endregion

		[XmlTextAttribute()]
		public string Text
		{ get { return _text.Value; } set { _text.Value = value; } }

		public ColladaParam() : base(Enums.ColladaElementType.Fx_Param)
		{
			Fields.Add(_ref = new ColladaObjectAttribute<string>(""));
			Fields.Add(_text = new ColladaObjectValue<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonFloatOrParamType, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonFloatOrParamType, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_Technique, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_Technique, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_Bind, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_Bind, _ref));
		}
	}
}