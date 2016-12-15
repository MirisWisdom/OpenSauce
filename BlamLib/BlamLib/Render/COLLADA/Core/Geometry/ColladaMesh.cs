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
	public partial class ColladaMesh : ColladaElement
	{
		#region Fields
		ColladaObjectElementList<ColladaSource> _source;
		ColladaObjectElement<ColladaVertices> _vertices;
		ColladaObjectElementList<ColladaLines> _lines;
		ColladaObjectElementList<ColladaLinestrips> _linestrips;
		ColladaObjectElementList<ColladaPolygons> _polygons;
		ColladaObjectElementList<ColladaPolylist> _polylist;
		ColladaObjectElementList<ColladaTriangles> _triangles;
		ColladaObjectElementList<ColladaTrifans> _trifans;
		ColladaObjectElementList<ColladaTristrips> _tristrips;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("source")]
		public List<ColladaSource> Source
		{ get { return _source.Value; } set { _source.Value = value; } }

		[XmlElement("vertices")]
		public ColladaVertices Vertices
		{ get { return _vertices.Value; } set { _vertices.Value = value; } }

		[XmlElement("lines")]
		public List<ColladaLines> Lines
		{ get { return _lines.Value; } set { _lines.Value = value; } }

		[XmlElement("linestrips")]
		public List<ColladaLinestrips> Linestrips
		{ get { return _linestrips.Value; } set { _linestrips.Value = value; } }

		[XmlElement("polygons")]
		public List<ColladaPolygons> Polygons
		{ get { return _polygons.Value; } set { _polygons.Value = value; } }

		[XmlElement("polylist")]
		public List<ColladaPolylist> Polylist
		{ get { return _polylist.Value; } set { _polylist.Value = value; } }

		[XmlElement("triangles")]
		public List<ColladaTriangles> Triangles
		{ get { return _triangles.Value; } set { _triangles.Value = value; } }

		[XmlElement("trifans")]
		public List<ColladaTrifans> Trifans
		{ get { return _trifans.Value; } set { _trifans.Value = value; } }

		[XmlElement("tristrips")]
		public List<ColladaTristrips> Tristrips
		{ get { return _tristrips.Value; } set { _tristrips.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaMesh() : base(Enums.ColladaElementType.Core_Mesh)
		{
			Fields.Add(_source = new ColladaObjectElementList<ColladaSource>());
			Fields.Add(_vertices = new ColladaObjectElement<ColladaVertices>());
			Fields.Add(_lines = new ColladaObjectElementList<ColladaLines>());
			Fields.Add(_linestrips = new ColladaObjectElementList<ColladaLinestrips>());
			Fields.Add(_polygons = new ColladaObjectElementList<ColladaPolygons>());
			Fields.Add(_polylist = new ColladaObjectElementList<ColladaPolylist>());
			Fields.Add(_triangles = new ColladaObjectElementList<ColladaTriangles>());
			Fields.Add(_trifans = new ColladaObjectElementList<ColladaTrifans>());
			Fields.Add(_tristrips = new ColladaObjectElementList<ColladaTristrips>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _source));
			ValidationTests.Add(new ColladaListMinCount<ColladaSource>(Enums.ColladaElementType.All, _source, 1));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _vertices));
		}
	}
}