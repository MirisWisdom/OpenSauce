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
	public partial class ColladaAnimationClip : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaAnimationClip>> _id;
		ColladaObjectAttribute<double> _start;
		ColladaObjectAttribute<double> _end;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaInstanceAnimation> _instanceAnimation;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("animclip-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("start"), DefaultValue(0.0)]
		public double Start
		{ get { return _start.Value; } set { _start.Value = value; } }

		[XmlAttribute("end"), DefaultValue(0.0)]
		public double End
		{ get { return _end.Value; } set { _end.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		[XmlElement("asset")]
		public ColladaAsset Asset
		{ get { return _asset.Value; } set { _asset.Value = value; } }

		[XmlElement("instance_animation")]
		public List<ColladaInstanceAnimation> InstanceAnimation
		{ get { return _instanceAnimation.Value; } set { _instanceAnimation.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaAnimationClip() : base(Enums.ColladaElementType.Core_AnimationClip)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaAnimationClip>>(""));
			Fields.Add(_start = new ColladaObjectAttribute<double>(0.0));
			Fields.Add(_end = new ColladaObjectAttribute<double>(0.0));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));

			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_instanceAnimation = new ColladaObjectElementList<ColladaInstanceAnimation>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _instanceAnimation));
			ValidationTests.Add(new ColladaListMinCount<ColladaInstanceAnimation>(Enums.ColladaElementType.All, _instanceAnimation, 1));
		}
	}
}