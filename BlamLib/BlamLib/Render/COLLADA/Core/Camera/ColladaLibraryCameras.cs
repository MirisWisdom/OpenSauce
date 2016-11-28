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
	public partial class ColladaLibraryCameras : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaLibraryCameras>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaCamera> _camera;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("libcameras-{0}")]
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

		[XmlElement("camera")]
		public List<ColladaCamera> Camera
		{ get { return _camera.Value; } set { _camera.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaLibraryCameras() : base(Enums.ColladaElementType.Core_LibraryCameras)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaLibraryCameras>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_camera = new ColladaObjectElementList<ColladaCamera>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _camera));
			ValidationTests.Add(new ColladaListMinCount<ColladaCamera>(Enums.ColladaElementType.All, _camera, 1));
		}
	}
}