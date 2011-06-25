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

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaAssetUnit : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<double> _meter;
		ColladaObjectAttribute<string> _name;
		#endregion

		#region Attributes
		[XmlAttribute("meter"), DefaultValue(1.0)]
		public double Meter
		{ get { return _meter.Value; } set { _meter.Value = value; } }

		[XmlAttribute("name"), DefaultValue("meter")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		public ColladaAssetUnit() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_meter = new ColladaObjectAttribute<double>(1.0));
			Fields.Add(_name = new ColladaObjectAttribute<string>("meter"));
		}
	}
}