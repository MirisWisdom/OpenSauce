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
	public partial class ColladaPolylist : ColladaGeometryCommonAttributes
	{
		#region Fields
		ColladaObjectElementList<ColladaInputShared> _input;
		ColladaObjectElement<ColladaValueArray<int>> _vCount;
		ColladaObjectElement<ColladaValueArray<int>> _p;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputShared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }

		[XmlElement("vcount")]
		public ColladaValueArray<int> VCount
		{ get { return _vCount.Value; } set { _vCount.Value = value; } }

		[XmlElement("p")]
		public ColladaValueArray<int> P
		{ get { return _p.Value; } set { _p.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaPolylist() : base(Enums.ColladaElementType.Core_Polylist)
		{
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputShared>());
			Fields.Add(_vCount = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_p = new ColladaObjectElement<ColladaValueArray<int>>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());
		}
	}
}