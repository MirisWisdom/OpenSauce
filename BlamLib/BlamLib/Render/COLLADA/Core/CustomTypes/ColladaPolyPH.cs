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
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaPolyPH : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaValueArray<uint>> _p;
		ColladaObjectElementList<ColladaValueArray<uint>> _h;
		#endregion

		#region Children
		[XmlElement("p")]
		public ColladaValueArray<uint> P
		{ get { return _p.Value; } set { _p.Value = value; } }

		[XmlElement("h")]
		public List<ColladaValueArray<uint>> H
		{ get { return _h.Value; } set { _h.Value = value; } }
		#endregion

		public ColladaPolyPH() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_p = new ColladaObjectElement<ColladaValueArray<uint>>());
			Fields.Add(_h = new ColladaObjectElementList<ColladaValueArray<uint>>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _p));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _h));
			ValidationTests.Add(new ColladaListMinCount<ColladaValueArray<uint>>(Enums.ColladaElementType.All, _h, 1));
		}
	}
}