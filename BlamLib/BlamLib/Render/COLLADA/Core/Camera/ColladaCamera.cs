/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
	public partial class ColladaCamera : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaCamera>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElement<ColladaOptics> _optics;
		ColladaObjectElement<ColladaImager> _imager;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("camera-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("optics")]
		public ColladaOptics Optics
		{ get { return _optics.Value; } set { _optics.Value = value; } }

		[XmlElement("imager")]
		public ColladaImager Imager
		{ get { return _imager.Value; } set { _imager.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaCamera() : base(Enums.ColladaElementType.Core_Camera)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaCamera>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_optics = new ColladaObjectElement<ColladaOptics>());
			Fields.Add(_imager = new ColladaObjectElement<ColladaImager>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

		}
	}
}