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
	public partial class ColladaLight : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaLight>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElement<ColladaTechniqueCommon> _techniqueCommon;
		ColladaObjectElementList<ColladaTechnique> _technique;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("light-{0}")]
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

		[XmlElement("technique_common")]
		public ColladaTechniqueCommon TechniqueCommon
		{ get { return _techniqueCommon.Value; } set { _techniqueCommon.Value = value; } }

		[XmlElement("technique")]
		public List<ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaLight() : base(Enums.ColladaElementType.Core_Light)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaLight>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_techniqueCommon = new ColladaObjectElement<ColladaTechniqueCommon>());
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _techniqueCommon));
		}
	}
}