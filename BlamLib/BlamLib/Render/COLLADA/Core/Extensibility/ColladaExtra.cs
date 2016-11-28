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
	public partial class ColladaExtra : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaExtra>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
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
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaExtra>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_type = new ColladaObjectAttribute<string>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _technique));
			ValidationTests.Add(new ColladaListMinCount<ColladaTechnique>(Enums.ColladaElementType.All, _technique, 1));
		}
	}
}