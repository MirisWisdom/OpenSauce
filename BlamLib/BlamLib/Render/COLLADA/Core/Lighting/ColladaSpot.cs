/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSpot : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaColor> _color;
		ColladaObjectElement<ColladaSIDValue<float>> _constantAttenuation;
		ColladaObjectElement<ColladaSIDValue<float>> _linearAttenuation;
		ColladaObjectElement<ColladaSIDValue<float>> _quadraticAttenuation;
		ColladaObjectElement<ColladaSIDValue<float>> _FalloffAngle;
		ColladaObjectElement<ColladaSIDValue<float>> _FalloffExponent;
		#endregion

		#region Children
		[XmlElement("color")]
		public ColladaColor Color
		{ get { return _color.Value; } set { _color.Value = value; } }

		[XmlElement("constant_attenuation")]
		public ColladaSIDValue<float> ConstantAttenuation
		{ get { return _constantAttenuation.Value; } set { _constantAttenuation.Value = value; } }

		[XmlElement("linear_attenuation")]
		public ColladaSIDValue<float> LinearAttenuation
		{ get { return _linearAttenuation.Value; } set { _linearAttenuation.Value = value; } }

		[XmlElement("quadratic_attenuation")]
		public ColladaSIDValue<float> QuadraticAttenuation
		{ get { return _quadraticAttenuation.Value; } set { _quadraticAttenuation.Value = value; } }

		[XmlElement("falloff_angle")]
		public ColladaSIDValue<float> FalloffAngle
		{ get { return _FalloffAngle.Value; } set { _FalloffAngle.Value = value; } }

		[XmlElement("falloff_exponent")]
		public ColladaSIDValue<float> FalloffExponent
		{ get { return _FalloffExponent.Value; } set { _FalloffExponent.Value = value; } }
		#endregion

		public ColladaSpot() : base(Enums.ColladaElementType.Core_Spot)
		{
			Fields.Add(_color = new ColladaObjectElement<ColladaColor>());
			Fields.Add(_constantAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_linearAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_quadraticAttenuation = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_FalloffAngle = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_FalloffExponent = new ColladaObjectElement<ColladaSIDValue<float>>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _color));
		}
	}
}