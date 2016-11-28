/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaPerspective : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaSIDValue<float>> _xFov;
		ColladaObjectElement<ColladaSIDValue<float>> _yFov;
		ColladaObjectElement<ColladaSIDValue<float>> _aspectRatio;
		ColladaObjectElement<ColladaSIDValue<float>> _zNear;
		ColladaObjectElement<ColladaSIDValue<float>> _zFar;
		#endregion

		#region Children
		[XmlElement("xfov")]
		public ColladaSIDValue<float> XFov
		{ get { return _xFov.Value; } set { _xFov.Value = value; } }

		[XmlElement("yfov")]
		public ColladaSIDValue<float> YFov
		{ get { return _yFov.Value; } set { _yFov.Value = value; } }

		[XmlElement("aspect_ratio")]
		public ColladaSIDValue<float> AspectRatio
		{ get { return _aspectRatio.Value; } set { _aspectRatio.Value = value; } }

		[XmlElement("znear")]
		public ColladaSIDValue<float> ZNear
		{ get { return _zNear.Value; } set { _zNear.Value = value; } }

		[XmlElement("zfar")]
		public ColladaSIDValue<float> ZFar
		{ get { return _zFar.Value; } set { _zFar.Value = value; } }
		#endregion

		public ColladaPerspective() : base(Enums.ColladaElementType.Core_Perspective)
		{
			Fields.Add(_xFov = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_yFov = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_aspectRatio = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_zNear = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_zFar = new ColladaObjectElement<ColladaSIDValue<float>>());

			List<ColladaObject> required_fields = new List<ColladaObject>();
			required_fields.Add(_xFov);
			required_fields.Add(_yFov);

			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, required_fields));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _aspectRatio));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _zNear));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _zFar));
		}
	}
}