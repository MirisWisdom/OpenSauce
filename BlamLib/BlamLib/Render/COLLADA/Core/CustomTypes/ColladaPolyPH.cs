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
	public partial class ColladaPolyPH : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaValueArray<uint>> _p;
		ColladaObjectElementList<ColladaValueArray<uint>> _h;
		#endregion

		#region Children
		[XmlElement("p")]
		public ColladaValueArray<uint> P
		{ get { return _p.Value; } set { _p.Value = value; } }

		[XmlElement("h")]
		public List<ColladaValueArray<uint>> H
		{ get { return _h.Value; } set { _h.Value = value; } }
		#endregion

		public ColladaPolyPH() : base(Enums.ColladaElementType.Undefined)
		{
			Fields.Add(_p = new ColladaObjectElement<ColladaValueArray<uint>>());
			Fields.Add(_h = new ColladaObjectElementList<ColladaValueArray<uint>>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _p));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _h));
			ValidationTests.Add(new ColladaListMinCount<ColladaValueArray<uint>>(Enums.ColladaElementType.All, _h, 1));
		}
	}
}