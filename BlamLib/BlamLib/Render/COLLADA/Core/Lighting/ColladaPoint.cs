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

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaPoint : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaColor> _color;
		ColladaObjectElement<ColladaSIDValue<float>> _constantAttenuation;
		ColladaObjectElement<ColladaSIDValue<float>> _linearAttenuation;
		ColladaObjectElement<ColladaSIDValue<float>> _quadraticAttenuation;
		#endregion

		#region Children
		[XmlElement("color")]
		public ColladaColor Color
		{ get { return _color.Value; } set { _color.Value = value; } }

		[XmlElement("constant_attenuation")]
		public ColladaSIDValue<float> ConstantAttenuation
		{ get { return _constantAttenuation.Value; } set { _constantAttenuation.Value = value; } }

		[XmlElement("linear_attenuation")]
		public ColladaSIDValue<float> LinearAttenuation
		{ get { return _linearAttenuation.Value; } set { _linearAttenuation.Value = value; } }

		[XmlElement("quadratic_attenuation")]
		public ColladaSIDValue<float> QuadraticAttenuation
		{ get { return _quadraticAttenuation.Value; } set { _quadraticAttenuation.Value = value; } }
		#endregion

		public ColladaPoint() : base(Enums.ColladaElementType.Core_Point)
		{
			Fields.Add(_color = new ColladaObjectElement<ColladaColor>());
			Fields.Add(_constantAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_linearAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_quadraticAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _color));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _quadraticAttenuation));
		}
	}
}