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

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaParam : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _ref;
		ColladaObjectValue<string> _text;
		#endregion

		#region Attributes
		[XmlAttribute("ref"), DefaultValue("")]
		public string Ref
		{ get { return _ref.Value; } set { _ref.Value = value; } }
		#endregion

		[XmlTextAttribute()]
		public string Text
		{ get { return _text.Value; } set { _text.Value = value; } }

		public ColladaParam() : base(Enums.ColladaElementType.Fx_Param)
		{
			Fields.Add(_ref = new ColladaObjectAttribute<string>(""));
			Fields.Add(_text = new ColladaObjectValue<string>(""));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonFloatOrParamType, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonFloatOrParamType, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_Technique, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_Technique, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_Bind, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_Bind, _ref));
		}
	}
}