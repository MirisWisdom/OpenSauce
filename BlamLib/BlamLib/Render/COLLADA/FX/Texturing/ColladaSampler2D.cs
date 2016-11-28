/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSampler2D : ColladaElement
	{
		#region Fields
		ColladaObjectValue<string> _source;
		ColladaObjectElementList<Core.ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("source")]
		public string Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("extra")]
		public List<Core.ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaSampler2D() : base(Enums.ColladaElementType.Fx_Sampler2D)
		{
			Fields.Add(_source = new ColladaObjectValue<string>(""));
			Fields.Add(_extra = new ColladaObjectElementList<Core.ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _source));
		}
	}
}