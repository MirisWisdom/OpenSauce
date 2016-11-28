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
	//TODO: may be wrong setting the enum element name to "enum"
	//though it's doubtful this will be an issue for the time being
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSetparam : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _ref;
		ColladaObjectAttribute<string> _program;
		ColladaObjectElementList<ColladaAnnotate> _annotate;
		ColladaObjectValue<bool?> _bool;
		ColladaObjectElement<ColladaValueArray<bool>> _bool2;
		ColladaObjectElement<ColladaValueArray<bool>> _bool3;
		ColladaObjectElement<ColladaValueArray<bool>> _bool4;
		ColladaObjectValue<int?> _int;
		ColladaObjectElement<ColladaValueArray<int>> _int2;
		ColladaObjectElement<ColladaValueArray<int>> _int3;
		ColladaObjectElement<ColladaValueArray<int>> _int4;
		ColladaObjectValue<float?> _float;
		ColladaObjectElement<ColladaValueArray<float>> _float2;
		ColladaObjectElement<ColladaValueArray<float>> _float3;
		ColladaObjectElement<ColladaValueArray<float>> _float4;
		ColladaObjectElement<ColladaValueArray<float>> _float1x1;
		ColladaObjectElement<ColladaValueArray<float>> _float1x2;
		ColladaObjectElement<ColladaValueArray<float>> _float1x3;
		ColladaObjectElement<ColladaValueArray<float>> _float1x4;
		ColladaObjectElement<ColladaValueArray<float>> _float2x1;
		ColladaObjectElement<ColladaValueArray<float>> _float2x2;
		ColladaObjectElement<ColladaValueArray<float>> _float2x3;
		ColladaObjectElement<ColladaValueArray<float>> _float2x4;
		ColladaObjectElement<ColladaValueArray<float>> _float3x1;
		ColladaObjectElement<ColladaValueArray<float>> _float3x2;
		ColladaObjectElement<ColladaValueArray<float>> _float3x3;
		ColladaObjectElement<ColladaValueArray<float>> _float3x4;
		ColladaObjectElement<ColladaValueArray<float>> _float4x1;
		ColladaObjectElement<ColladaValueArray<float>> _float4x2;
		ColladaObjectElement<ColladaValueArray<float>> _float4x3;
		ColladaObjectElement<ColladaValueArray<float>> _float4x4;
		ColladaObjectElement<ColladaSurface> _surface;
		ColladaObjectElement<ColladaSampler1D> _sampler1D;
		ColladaObjectElement<ColladaSampler2D> _sampler2D;
		ColladaObjectElement<ColladaSampler3D> _sampler3D;
		ColladaObjectElement<ColladaSamplerCUBE> _samplerCUBE;
		ColladaObjectValue<string> __enum;
		#endregion

		#region Attributes
		[XmlAttribute("ref")]
		public string Ref
		{ get { return _ref.Value; } set { _ref.Value = value; } }

		[XmlAttribute("program"), DefaultValue("")]
		public string Program
		{ get { return _program.Value; } set { _program.Value = value; } }
		#endregion

		#region Children
		[XmlElement("annotate")]
		public List<ColladaAnnotate> Annotate
		{ get { return _annotate.Value; } set { _annotate.Value = value; } }

		#region CORE_PARAM_TYPE
		#region bools
		[XmlElement("bool")]
		public bool? Bool
		{ get { return _bool.Value; } set { _bool.Value = value; } }

		[XmlElement("bool2")]
		public ColladaValueArray<bool> Bool2
		{ get { return _bool2.Value; } set { _bool2.Value = value; } }

		[XmlElement("bool3")]
		public ColladaValueArray<bool> Bool3
		{ get { return _bool3.Value; } set { _bool3.Value = value; } }

		[XmlElement("bool4")]
		public ColladaValueArray<bool> Bool4
		{ get { return _bool4.Value; } set { _bool4.Value = value; } }
		#endregion

		#region ints
		[XmlElement("int")]
		public int? Int
		{ get { return _int.Value; } set { _int.Value = value; } }

		[XmlElement("int2")]
		public ColladaValueArray<int> Int2
		{ get { return _int2.Value; } set { _int2.Value = value; } }

		[XmlElement("int3")]
		public ColladaValueArray<int> Int3
		{ get { return _int3.Value; } set { _int3.Value = value; } }

		[XmlElement("int4")]
		public ColladaValueArray<int> Int4
		{ get { return _int4.Value; } set { _int4.Value = value; } }
		#endregion

		#region floats
		[XmlElement("float")]
		public float? Float
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
		#endregion

		#region matrices
		[XmlElement("float1x1")]
		public ColladaValueArray<float> Float1x1
		{ get { return _float1x1.Value; } set { _float1x1.Value = value; } }

		[XmlElement("float1x2")]
		public ColladaValueArray<float> Float1x2
		{ get { return _float1x2.Value; } set { _float1x2.Value = value; } }

		[XmlElement("float1x3")]
		public ColladaValueArray<float> Float1x3
		{ get { return _float1x3.Value; } set { _float1x3.Value = value; } }

		[XmlElement("float1x4")]
		public ColladaValueArray<float> Float1x4
		{ get { return _float1x4.Value; } set { _float1x4.Value = value; } }

		[XmlElement("float2x1")]
		public ColladaValueArray<float> Float2x1
		{ get { return _float2x1.Value; } set { _float2x1.Value = value; } }

		[XmlElement("float2x2")]
		public ColladaValueArray<float> Float2x2
		{ get { return _float2x2.Value; } set { _float2x2.Value = value; } }

		[XmlElement("float2x3")]
		public ColladaValueArray<float> Float2x3
		{ get { return _float2x3.Value; } set { _float2x3.Value = value; } }

		[XmlElement("float2x4")]
		public ColladaValueArray<float> Float2x4
		{ get { return _float2x4.Value; } set { _float2x4.Value = value; } }

		[XmlElement("float3x1")]
		public ColladaValueArray<float> Float3x1
		{ get { return _float3x1.Value; } set { _float3x1.Value = value; } }

		[XmlElement("float3x2")]
		public ColladaValueArray<float> Float3x2
		{ get { return _float3x2.Value; } set { _float3x2.Value = value; } }

		[XmlElement("float3x3")]
		public ColladaValueArray<float> Float3x3
		{ get { return _float3x3.Value; } set { _float3x3.Value = value; } }

		[XmlElement("float3x4")]
		public ColladaValueArray<float> Float3x4
		{ get { return _float3x4.Value; } set { _float3x4.Value = value; } }

		[XmlElement("float4x1")]
		public ColladaValueArray<float> Float4x1
		{ get { return _float4x1.Value; } set { _float4x1.Value = value; } }

		[XmlElement("float4x2")]
		public ColladaValueArray<float> Float4x2
		{ get { return _float4x2.Value; } set { _float4x2.Value = value; } }

		[XmlElement("float4x3")]
		public ColladaValueArray<float> Float4x3
		{ get { return _float4x3.Value; } set { _float4x3.Value = value; } }

		[XmlElement("float4x4")]
		public ColladaValueArray<float> Float4x4
		{ get { return _float4x4.Value; } set { _float4x4.Value = value; } }
		#endregion

		#region surfaces
		[XmlElement("surface")]
		public ColladaSurface Surface
		{ get { return _surface.Value; } set { _surface.Value = value; } }
		#endregion

		#region samplers
		[XmlElement("sampler1D")]
		public ColladaSampler1D Sampler1D
		{ get { return _sampler1D.Value; } set { _sampler1D.Value = value; } }

		[XmlElement("sampler2D")]
		public ColladaSampler2D Sampler2D
		{ get { return _sampler2D.Value; } set { _sampler2D.Value = value; } }

		[XmlElement("sampler3D")]
		public ColladaSampler3D Sampler3D
		{ get { return _sampler3D.Value; } set { _sampler3D.Value = value; } }

		[XmlElement("samplerCUBE")]
		public ColladaSamplerCUBE SamplerCUBE
		{ get { return _samplerCUBE.Value; } set { _samplerCUBE.Value = value; } }
		#endregion

		#region enums
		[XmlElement("enum")]
		public string Enum
		{ get { return __enum.Value; } set { __enum.Value = value; } }
		#endregion
		#endregion
		#endregion

		public ColladaSetparam() : base(Enums.ColladaElementType.Fx_Setparam)
		{
			Fields.Add(_ref = new ColladaObjectAttribute<string>(""));
			Fields.Add(_program = new ColladaObjectAttribute<string>(""));
			Fields.Add(_annotate = new ColladaObjectElementList<ColladaAnnotate>());
			Fields.Add(_bool = new ColladaObjectValue<bool?>());
			Fields.Add(_bool2 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_bool3 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_bool4 = new ColladaObjectElement<ColladaValueArray<bool>>());
			Fields.Add(_int = new ColladaObjectValue<int?>());
			Fields.Add(_int2 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_int3 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_int4 = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_float = new ColladaObjectValue<float?>());
			Fields.Add(_float2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float1x1 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float1x2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float1x3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float1x4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2x1 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2x2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2x3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float2x4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3x1 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3x2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3x3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float3x4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4x1 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4x2 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4x3 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_float4x4 = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_surface = new ColladaObjectElement<ColladaSurface>());
			Fields.Add(_sampler1D = new ColladaObjectElement<ColladaSampler1D>());
			Fields.Add(_sampler2D = new ColladaObjectElement<ColladaSampler2D>());
			Fields.Add(_sampler3D = new ColladaObjectElement<ColladaSampler3D>());
			Fields.Add(_samplerCUBE = new ColladaObjectElement<ColladaSamplerCUBE>());
			Fields.Add(__enum = new ColladaObjectValue<string>());

			List<ColladaObject> field_list = new List<ColladaObject>();
			field_list.Add(_bool);
			field_list.Add(_bool2);
			field_list.Add(_bool3);
			field_list.Add(_bool4);
			field_list.Add(_int);
			field_list.Add(_int2);
			field_list.Add(_int3);
			field_list.Add(_int4);
			field_list.Add(_float);
			field_list.Add(_float2);
			field_list.Add(_float3);
			field_list.Add(_float4);
			field_list.Add(_float1x1);
			field_list.Add(_float1x2);
			field_list.Add(_float1x3);
			field_list.Add(_float1x4);
			field_list.Add(_float2x1);
			field_list.Add(_float2x2);
			field_list.Add(_float2x3);
			field_list.Add(_float2x4);
			field_list.Add(_float3x1);
			field_list.Add(_float3x2);
			field_list.Add(_float3x3);
			field_list.Add(_float3x4);
			field_list.Add(_float4x1);
			field_list.Add(_float4x2);
			field_list.Add(_float4x3);
			field_list.Add(_float4x4);
			field_list.Add(_surface);
			field_list.Add(_sampler1D);
			field_list.Add(_sampler2D);
			field_list.Add(_sampler3D);
			field_list.Add(_samplerCUBE);
			field_list.Add(__enum);

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _ref));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _ref));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _program));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.Fx_CommonColorOrTextureType, _program));
			ValidationTests.Add(new ColladaMutuallyExclusive(Enums.ColladaElementType.All, field_list));
			ValidationTests.Add(new ColladaOneRequired(Enums.ColladaElementType.All, field_list));
		}
	}
}