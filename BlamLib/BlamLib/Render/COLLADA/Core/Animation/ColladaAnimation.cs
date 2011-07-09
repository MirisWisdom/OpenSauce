/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		ColladaObjectAttribute<string> _id;
		ColladaObjectAttribute<string> _name;

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
			Fields.Add(_id = new ColladaObjectAttribute<string>(""));
			Fields.Add(_name = new ColladaObjectAttribute<string>(""));
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