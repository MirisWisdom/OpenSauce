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
	public partial class ColladaOptics : ColladaElement
	{
		#region Fields
		ColladaObjectElement<ColladaTechniqueCommon> _techniqueCommon;
		ColladaObjectElementList<ColladaTechnique> _technique;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("technique_common")]
		public ColladaTechniqueCommon TechniqueCommon
		{ get { return _techniqueCommon.Value; } set { _techniqueCommon.Value = value; } }

		[XmlElement("technique")]
		public List<ColladaTechnique> Technique
		{ get { return _technique.Value; } set { _technique.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaOptics() : base(Enums.ColladaElementType.Core_Optics)
		{
			Fields.Add(_techniqueCommon = new ColladaObjectElement<ColladaTechniqueCommon>());
			Fields.Add(_technique = new ColladaObjectElementList<ColladaTechnique>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _techniqueCommon));
		}
	}
}