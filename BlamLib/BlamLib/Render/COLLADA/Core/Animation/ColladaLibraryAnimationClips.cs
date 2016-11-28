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
	public partial class ColladaLibraryAnimationClips : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaLibraryAnimationClips>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaAnimationClip> _animationClip;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("libanimclip-{0}")]
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

		[XmlElement("animation_clip")]
		public List<ColladaAnimationClip> AnimationClip
		{ get { return _animationClip.Value; } set { _animationClip.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaLibraryAnimationClips() : base(Enums.ColladaElementType.Core_LibraryAnimationClips)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaLibraryAnimationClips>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));

			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_animationClip = new ColladaObjectElementList<ColladaAnimationClip>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _animationClip));
			ValidationTests.Add(new ColladaListMinCount<ColladaAnimationClip>(Enums.ColladaElementType.All, _animationClip, 1));
		}
	}
}