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
		[XmlAttribute("source")]
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