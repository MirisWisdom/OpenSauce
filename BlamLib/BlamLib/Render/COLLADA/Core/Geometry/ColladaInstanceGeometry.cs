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
	public partial class ColladaInstanceGeometry : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sid;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _url;

		ColladaObjectElement<Fx.ColladaBindMaterial> _bindMaterial;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("url")]
		public string URL
		{ get { return _url.Value; } set { _url.Value = value; } }
		#endregion

		#region Children
		[XmlElement("bind_material")]
		public Fx.ColladaBindMaterial BindMaterial
		{ get { return _bindMaterial.Value; } set { _bindMaterial.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaInstanceGeometry() : base(Enums.ColladaElementType.Core_InstanceGeometry)
		{
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_url = new ColladaObjectAttribute<string>(""));
			Fields.Add(_bindMaterial = new ColladaObjectElement<Fx.ColladaBindMaterial>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _url));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _url));
		}
	}
}