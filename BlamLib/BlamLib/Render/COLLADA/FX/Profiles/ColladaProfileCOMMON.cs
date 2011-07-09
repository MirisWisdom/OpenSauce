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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaProfileCOMMON : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _id;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElementList<ColladaImage> _image;
		ColladaObjectElementList<ColladaNewparam> _newparam;
		ColladaObjectElement<ColladaTechnique> _technique;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("profilecommon-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public Core.ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("image")]
		public List<ColladaImage> Image
		{ get { return _image.Value; } set { _image.Value = value; } }

		[XmlElement("newparam")]
		public List<ColladaNewparam> Newparam
		{ get { return _newparam.Value; } set { _newparam.Value = value; } }

		[XmlElement("technique")]
		public ColladaTechnique Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaProfileCOMMON() : base(Enums.ColladaElementType.Fx_ProfileCOMMON)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_image = new ColladaObjectElementList<ColladaImage>());
			Fields.Add(_newparam = new ColladaObjectElementList<ColladaNewparam>());
			Fields.Add(_technique = new ColladaObjectElement<ColladaTechnique>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _technique));
		}
	}
}