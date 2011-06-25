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

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaTrifans : ColladaGeometryCommonAttributes
	{
		#region Fields
		ColladaObjectElementList<ColladaInputShared> _input;
		ColladaObjectElementList<ColladaValueArray<int>> _p;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputShared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }

		[XmlElement("p")]
		public List<ColladaValueArray<int>> P
		{ get { return _p.Value; } set { _p.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaTrifans() : base(Enums.ColladaElementType.Core_Trifans)
		{
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputShared>());
			Fields.Add(_p = new ColladaObjectElementList<ColladaValueArray<int>>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());
		}
	}
}