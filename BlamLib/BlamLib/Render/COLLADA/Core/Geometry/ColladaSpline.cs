/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSpline : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<bool> _closed;

		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaControlVertices> _controlVertices;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("closed"), DefaultValue(false)]
		public bool Closed
		{ get { return _closed.Value; } set { _closed.Value = value; } }
		#endregion

		#region Children
		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("control_vertices")]
		public ColladaControlVertices ControlVertices
		{ get { return _controlVertices.Value; } set { _controlVertices.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaSpline() : base(Enums.ColladaElementType.Core_Spline)
		{
			Fields.Add(_closed = new ColladaObjectAttribute<bool>(false));
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_controlVertices = new ColladaObjectElement<ColladaControlVertices>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 1));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _controlVertices));
		}
	}
}