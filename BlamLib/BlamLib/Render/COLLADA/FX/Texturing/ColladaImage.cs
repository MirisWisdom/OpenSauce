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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaImage : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaImage>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		ColladaObjectAttribute<string> _format;
		ColladaObjectAttribute<uint> _height;
		ColladaObjectAttribute<uint> _width;
		ColladaObjectAttribute<uint> _depth;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElement<ColladaInitFrom> _initFrom;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("image-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }

		[XmlAttribute("format"), DefaultValue("")]
		public string Format
		{ get { return _format.Value; } set { _format.Value = value; } }

		[XmlAttribute("height"), DefaultValue(0)]
		public uint Height
		{ get { return _height.Value; } set { _height.Value = value; } }

		[XmlAttribute("width"), DefaultValue(0)]
		public uint Width
		{ get { return _width.Value; } set { _width.Value = value; } }

		[XmlAttribute("depth"), DefaultValue(1)]
		public uint Depth
		{ get { return _depth.Value; } set { _depth.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public Core.ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("init_from")]
		public ColladaInitFrom InitFrom
		{ get { return _initFrom.Value; } set { _initFrom.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaImage() : base(Enums.ColladaElementType.Fx_Image)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaImage>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_format = new ColladaObjectAttribute<string>(""));
			Fields.Add(_height = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_width = new ColladaObjectAttribute<uint>(0));
			Fields.Add(_depth = new ColladaObjectAttribute<uint>(1));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_initFrom = new ColladaObjectElement<ColladaInitFrom>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _initFrom));
		}
	}
}