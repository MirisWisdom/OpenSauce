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
	public partial class ColladaExtra : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _id;
		ColladaObjectAttribute<string> _name;
		ColladaObjectAttribute<string> _type;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaTechnique> _technique;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("extra-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("type"), DefaultValue("")]
		public string Type
		{ get { return _type.Value; } set { _type.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("technique")]
		public List<ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }
		#endregion

		public ColladaExtra() : base(Enums.ColladaElementType.Core_Extra)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
			Fields.Add(_type = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _technique));
			ValidationTests.Add(new ColladaListMinCount<ColladaTechnique>(Enums.ColladaElementType.All, _technique, 1));
		}
	}
}