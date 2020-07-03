/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaPolygons : ColladaGeometryCommonAttributes
	{
		#region Fields
		ColladaObjectElementList<ColladaInputShared> _input;
		ColladaObjectElementList<ColladaValueArray<uint>> _p;
		ColladaObjectElementList<ColladaPolyPH> _ph;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputShared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }

		[XmlElement("p")]
		public List<ColladaValueArray<uint>> P
		{ get { return _p.Value; } set { _p.Value = value; } }

		[XmlElement("ph")]
		public List<ColladaPolyPH> PH
		{ get { return _ph.Value; } set { _ph.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaPolygons() : base(Enums.ColladaElementType.Core_Polygons)
		{
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputShared>());
			Fields.Add(_p = new ColladaObjectElementList<ColladaValueArray<uint>>());
			Fields.Add(_ph = new ColladaObjectElementList<ColladaPolyPH>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());
		}
	}
}