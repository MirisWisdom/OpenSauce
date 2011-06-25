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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaInitFrom : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<uint> _mip;
		ColladaObjectAttribute<uint> _slice;
		ColladaObjectAttribute<Enums.ColladaFXSurfaceFaceEnum> _face;
		ColladaObjectValue<string> _text;
		#endregion

		#region Attributes
		[XmlAttribute("mip"), DefaultValue(0)]
		public uint Mip
		{ get { return _mip.Value; } set { _mip.Value = value; } }

		[XmlAttribute("slice"), DefaultValue(0)]
		public uint Slice
		{ get { return _slice.Value; } set { _slice.Value = value; } }

		[XmlAttribute("face"), DefaultValue(Enums.ColladaFXSurfaceFaceEnum.POSITIVE_X)]
		public Enums.ColladaFXSurfaceFaceEnum Face
		{ get { return _face.Value; } set { _face.Value = value; } }
		#endregion

		[XmlTextAttribute()]
		public string Text
		{ get { return _text.Value; } set { _text.Value = value; } }
		
		public ColladaInitFrom() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_mip = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_slice = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_face = new ColladaObjectAttribute<Enums.ColladaFXSurfaceFaceEnum>(Enums.ColladaFXSurfaceFaceEnum.POSITIVE_X));
			Fields.Add(_text = new ColladaObjectValue<string>(""));
		}
	}
}