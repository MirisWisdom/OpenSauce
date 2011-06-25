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
	public partial class ColladaTechnique : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _id;
		ColladaObjectAttribute<string> _sid;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElementList<ColladaNewparam> _newparam;
		ColladaObjectElementList<ColladaImage> _image;
		ColladaObjectElement<ColladaBlinn> _blinn;
		ColladaObjectElement<ColladaConstant> _constant;
		ColladaObjectElement<ColladaLambert> _lambert;
		ColladaObjectElement<ColladaPhong> _phong;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		[XmlIgnore]
		public new static readonly string ElementIDFormat = "technique-{0}";

		#region Attributes
		[XmlAttribute("id"), DefaultValue("")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("sid"), DefaultValue("")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public Core.ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("newparam")]
		public List<ColladaNewparam> Newparam
		{ get { return _newparam.Value; } set { _newparam.Value = value; } }

		[XmlElement("image")]
		public List<ColladaImage> Image
		{ get { return _image.Value; } set { _image.Value = value; } }

		[XmlElement("blinn")]
		public ColladaBlinn Blinn
		{ get { return _blinn.Value; } set { _blinn.Value = value; } }

		[XmlElement("constant")]
		public ColladaConstant Constant
		{ get { return _constant.Value; } set { _constant.Value = value; } }

		[XmlElement("lambert")]
		public ColladaLambert Lambert
		{ get { return _lambert.Value; } set { _lambert.Value = value; } }

		[XmlElement("phong")]
		public ColladaPhong Phong
		{ get { return _phong.Value; } set { _phong.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaTechnique() : base(Enums.ColladaElementType.Fx_Technique)
		{
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_newparam = new ColladaObjectElementList<ColladaNewparam>());
			Fields.Add(_image = new ColladaObjectElementList<ColladaImage>());
			Fields.Add(_blinn = new ColladaObjectElement<ColladaBlinn>());
			Fields.Add(_constant = new ColladaObjectElement<ColladaConstant>());
			Fields.Add(_lambert = new ColladaObjectElement<ColladaLambert>());
			Fields.Add(_phong = new ColladaObjectElement<ColladaPhong>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			List<ColladaObject> required = new List<ColladaObject>();
			required.Add(_blinn);
			required.Add(_constant);
			required.Add(_lambert);
			required.Add(_phong);

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, required));
		}
	}
}