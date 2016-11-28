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
	public partial class ColladaMaterial : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaMaterial>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElement<ColladaInstanceEffect> _instanceEffect;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), ColladaID("material-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public Core.ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("instance_effect")]
		public ColladaInstanceEffect InstanceEffect
		{ get { return _instanceEffect.Value; } set { _instanceEffect.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaMaterial() : base(Enums.ColladaElementType.Fx_Material)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaMaterial>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_instanceEffect = new ColladaObjectElement<ColladaInstanceEffect>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _instanceEffect));
		}
	}
}