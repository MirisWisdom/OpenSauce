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
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSpline : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<bool> _closed;

		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaControlVertices> _controlVertices;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("closed"), DefaultValue(false)]
		public bool Closed
		{ get { return _closed.Value; } set { _closed.Value = value; } }
		#endregion

		#region Children
		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("control_vertices")]
		public ColladaControlVertices ControlVertices
		{ get { return _controlVertices.Value; } set { _controlVertices.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaSpline() : base(Enums.ColladaElementType.Core_Spline)
		{
			Fields.Add(_closed = new ColladaObjectAttribute<bool>(false));
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_controlVertices = new ColladaObjectElement<ColladaControlVertices>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 1));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _controlVertices));
		}
	}
}