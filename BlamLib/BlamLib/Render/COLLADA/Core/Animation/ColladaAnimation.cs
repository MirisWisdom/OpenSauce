/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml.Serialization;
using System.Collections.Generic;

using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaAnimation : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaAnimation>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElement<ColladaAsset> _asset;
		ColladaObjectElementList<ColladaAnimation> _animation;
		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElementList<ColladaSampler> _sampler;
		ColladaObjectElementList<ColladaChannel> _channel;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("animation-{0}")]
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

		[XmlElement("animation")]
		public List<ColladaAnimation> Animation
		{ get { return _animation.Value; } set { _animation.Value = value; } }

		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("sampler")]
		public List<ColladaSampler> Sampler
		{ get { return _sampler.Value; } set { _sampler.Value = value; } }

		[XmlElement("channel")]
		public List<ColladaChannel> Channel
		{ get { return _channel.Value; } set { _channel.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaAnimation() : base(Enums.ColladaElementType.Core_Animation) 
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaAnimation>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_asset = new ColladaObjectElement<ColladaAsset>());
			Fields.Add(_animation = new ColladaObjectElementList<ColladaAnimation>());
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_sampler = new ColladaObjectElementList<ColladaSampler>());
			Fields.Add(_channel = new ColladaObjectElementList<ColladaChannel>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			List<ColladaObject> mutually_inclusive = new List<ColladaObject>();
			mutually_inclusive.Add(_sampler);
			mutually_inclusive.Add(_channel);

			List<ColladaObject> mutually_exclusive1 = new List<ColladaObject>();
			mutually_inclusive.Add(_animation);
			mutually_inclusive.Add(_sampler);

			List<ColladaObject> mutually_exclusive2 = new List<ColladaObject>();
			mutually_inclusive.Add(_animation);
			mutually_inclusive.Add(_channel);

			ValidationTests.Add(new ColladaMutuallyInclusive(Enums.ColladaElementType.All, mutually_inclusive));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, mutually_exclusive1));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, mutually_exclusive2));
		}
	}
}