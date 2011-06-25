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
	public partial class ColladaDirectional : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaColor> _color;
		#endregion

		#region Children
		[XmlElement("color")]
		public ColladaColor Color
		{ get { return _color.Value; } set { _color.Value = value; } }
		#endregion

		public ColladaDirectional() : base(Enums.ColladaElementType.Core_Directional)
		{
			Fields.Add(_color = new ColladaObjectElement<ColladaColor>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _color));
		}
	}
}