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
	public partial class ColladaEffect : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaEffect>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;
		ColladaObjectElement<Core.ColladaAsset> _asset;
		ColladaObjectElementList<ColladaAnnotate> _annotate;
		ColladaObjectElementList<ColladaImage> _image;
		ColladaObjectElementList<ColladaNewparam> _newparam;
		ColladaObjectElementList<ColladaProfileCOMMON> _profileCOMMON;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), ColladaID("effect-{0}")]
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

		[XmlElement("annotate")]
		public List<ColladaAnnotate> Annotate
		{ get { return _annotate.Value; } set { _annotate.Value = value; } }

		[XmlElement("image")]
		public List<ColladaImage> Image
		{ get { return _image.Value; } set { _image.Value = value; } }

		[XmlElement("newparam")]
		public List<ColladaNewparam> Newparam
		{ get { return _newparam.Value; } set { _newparam.Value = value; } }

		[XmlElement("profile_COMMON")]
		public List<ColladaProfileCOMMON> ProfileCOMMON
		{ get { return _profileCOMMON.Value; } set { _profileCOMMON.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaEffect() : base(Enums.ColladaElementType.Fx_Effect)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaEffect>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<Core.ColladaAsset>());
			Fields.Add(_annotate = new ColladaObjectElementList<ColladaAnnotate>());
			Fields.Add(_image = new ColladaObjectElementList<ColladaImage>());
			Fields.Add(_newparam = new ColladaObjectElementList<ColladaNewparam>());
			Fields.Add(_profileCOMMON = new ColladaObjectElementList<ColladaProfileCOMMON>());
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _id));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _id));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _profileCOMMON));
			ValidationTests.Add(new ColladaListMinCount<ColladaProfileCOMMON>(Enums.ColladaElementType.All, _profileCOMMON, 1));
		}
	}
}