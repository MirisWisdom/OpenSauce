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
	public partial class ColladaIntArray : ColladaValueArray<int>
	{
		#region Fields
		ColladaObjectAttribute<string> _id;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<int> _minInclusive;
		ColladaObjectAttribute<int> _maxInclusive;
		#endregion

		#region Attributes
		[XmlAttribute("count")]
		public uint Count
		{
			get
			{
				if (Values == null)
					return 0;
				else
					return (uint)Values.Count;
			}
			set { }
		}

		[XmlAttribute("id"), DefaultValue(""), ColladaID("intarray-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("minInclusive"), DefaultValue(-2147483648)]
		public int MinInclusive
		{ get { return _minInclusive.Value; } set { _minInclusive.Value = value; } }

		[XmlAttribute("maxInclusive"), DefaultValue(2147483647)]
		public int MaxInclusive
		{ get { return _maxInclusive.Value; } set { _maxInclusive.Value = value; } }
		#endregion

		public ColladaIntArray() : base(Enums.ColladaElementType.Core_IntArray)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_minInclusive = new ColladaObjectAttribute<int>(-2147483648));
			Fields.Add(_maxInclusive = new ColladaObjectAttribute<int>(2147483647));
		}
	}
}