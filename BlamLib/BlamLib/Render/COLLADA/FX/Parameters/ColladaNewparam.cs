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
	public partial class ColladaNewparam : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sid;
		ColladaObjectElement<ColladaSemantic> _semantic;
		ColladaObjectElement<ColladaValueArray<float>> _float;
		ColladaObjectElement<ColladaValueArray<float>> _float2;
		ColladaObjectElement<ColladaValueArray<float>> _float3;
		ColladaObjectElement<ColladaValueArray<float>> _float4;
		ColladaObjectElement<ColladaSurface> _surface;
		ColladaObjectElement<ColladaSampler2D> _sampler2D;
		#endregion

		#region Attributes
		[XmlAttribute("sid")]
		public string sID
		{ get { return _sid.Value; } set { _sid.Value = value; } }
		#endregion

		#region Children
		[XmlElement("semantic")]
		public ColladaSemantic Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlElement("float")]
		public ColladaValueArray<float> Float
		{ get { return _float.Value; } set { _float.Value = value; } }

		[XmlElement("float2")]
		public ColladaValueArray<float> Float2
		{ get { return _float2.Value; } set { _float2.Value = value; } }

		[XmlElement("float3")]
		public ColladaValueArray<float> Float3
		{ get { return _float3.Value; } set { _float3.Value = value; } }

		[XmlElement("float4")]
		public ColladaValueArray<float> Float4
		{ get { return _float4.Value; } set { _float4.Value = value; } }

		[XmlElement("surface")]
		public ColladaSurface Surface
		{ get { return _surface.Value; } set { _surface.Value = value; } }

		[XmlElement("sampler2D")]
		public ColladaSampler2D Sampler2D
		{ get { return _sampler2D.Value; } set { _sampler2D.Value = value; } }
		#endregion

		public ColladaNewparam() : base(Enums.ColladaElementType.Fx_Newparam)
		{
			Fields.Add(_sid = new ColladaObjectAttribute<string>(""));
			Fields.Add(_semantic = new ColladaObjectElement<ColladaSemantic>());
			Fields.Add(_float = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_surface = new ColladaObjectElement<ColladaSurface>());
			Fields.Add(_sampler2D = new ColladaObjectElement<ColladaSampler2D>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_float);
			field_list.Add(_float2);
			field_list.Add(_float3);
			field_list.Add(_float4);
			field_list.Add(_surface);
			field_list.Add(_sampler2D);

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sid));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
		}
	}
}