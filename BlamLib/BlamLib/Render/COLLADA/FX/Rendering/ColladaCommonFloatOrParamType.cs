/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaCommonFloatOrParamType : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaSIDValue<float>> _float;
		ColladaObjectElement<ColladaParam> _param;
		#endregion

		#region Children
		[XmlElement("float")]
		public ColladaSIDValue<float> Float
		{ get { return _float.Value; } set { _float.Value = value; } }

		[XmlElement("param")]
		public ColladaParam Param
		{ get { return _param.Value; } set { _param.Value = value; } }
		#endregion

		public ColladaCommonFloatOrParamType() : base(Enums.ColladaElementType.Fx_CommonFloatOrParamType)
		{
			Fields.Add(_float = new ColladaObjectElement<ColladaSIDValue<float>>());
			Fields.Add(_param = new ColladaObjectElement<ColladaParam>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_float);
			field_list.Add(_param);

			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
		}
	}
}