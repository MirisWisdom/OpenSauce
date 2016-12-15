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
	public partial class ColladaBindVertexInput : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _semantic;
		ColladaObjectAttribute<string> _inputSemantic;
		ColladaObjectAttribute<uint> _inputSet;
		#endregion

		#region Attributes
		[XmlAttribute("semantic")]
		public string Semantic
		{ get { return _semantic.Value; } set { _semantic.Value = value; } }

		[XmlAttribute("input_semantic")]
		public string InputSemantic
		{ get { return _inputSemantic.Value; } set { _inputSemantic.Value = value; } }

		[XmlAttribute("input_set"), DefaultValue(0)]
		public uint InputSet
		{ get { return _inputSet.Value; } set { _inputSet.Value = value; } }
		#endregion

		public ColladaBindVertexInput() : base(Enums.ColladaElementType.Fx_BindVertexInput)
		{
			Fields.Add(_semantic = new ColladaObjectAttribute<string>(""));
			Fields.Add(_inputSemantic = new ColladaObjectAttribute<string>(""));
			Fields.Add(_inputSet = new ColladaObjectAttribute<uint>(0));

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _semantic));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _semantic));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _inputSemantic));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _inputSemantic));
		}
	}
}