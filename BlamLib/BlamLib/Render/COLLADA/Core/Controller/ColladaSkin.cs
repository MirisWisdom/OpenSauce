/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSkin : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<string> _sourceAttrib;

		ColladaObjectElement<ColladaValueArray<float>> _bindShapeMatrix;
		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaJoints> _joints;
		ColladaObjectElement<ColladaVertexWeights> _vertexWeights;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("source"), ColladaURI]
		public string SourceAttrib
		{ get { return _sourceAttrib.Value; } set { _sourceAttrib.Value = value; } }
		#endregion

		#region Children
		[XmlElement("bind_shape_matrix")]
		public ColladaValueArray<float> BindShapeMatrix
		{ get { return _bindShapeMatrix.Value; } set { _bindShapeMatrix.Value = value; } }

		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("joints")]
		public ColladaJoints Joints
		{ get { return _joints.Value; } set { _joints.Value = value; } }

		[XmlElement("vertex_weights")]
		public ColladaVertexWeights VertexWeights
		{ get { return _vertexWeights.Value; } set { _vertexWeights.Value = value; } }
		
		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaSkin() : base(Enums.ColladaElementType.Core_Skin)
		{
			Fields.Add(_sourceAttrib = new ColladaObjectAttribute<string>(""));
			Fields.Add(_bindShapeMatrix = new ColladaObjectElement<ColladaValueArray<float>>());
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_joints = new ColladaObjectElement<ColladaJoints>());
			Fields.Add(_vertexWeights = new ColladaObjectElement<ColladaVertexWeights>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _sourceAttrib));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 3));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _joints));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _vertexWeights));
		}
	}
}