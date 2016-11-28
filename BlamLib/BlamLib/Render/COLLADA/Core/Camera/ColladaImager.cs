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
	public partial class ColladaImager : ColladaElement
	{
		#region Fields
		ColladaObjectElementList<ColladaTechnique> _technique;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("technique")]
		public List<ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaImager() : base(Enums.ColladaElementType.Core_Imager)
		{
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _technique));
			ValidationTests.Add(new ColladaListMinCount<ColladaTechnique>(Enums.ColladaElementType.All, _technique, 1));
		}
	}
}