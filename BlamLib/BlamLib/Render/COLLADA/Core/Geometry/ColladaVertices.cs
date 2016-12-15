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
	public partial class ColladaVertices : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaVertices>> _id;
		ColladaObjectAttribute<ColladaNCName> _name;

		ColladaObjectElementList<ColladaInputUnshared> _input;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Attributes
		[XmlAttribute("id"), ColladaID("vertices-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }

		[XmlAttribute("name"), DefaultValue("")]
		public string Name
		{ get { return _name.Value; } set { _name.Value = value; } }
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputUnshared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaVertices() : base(Enums.ColladaElementType.Core_Vertices)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaVertices>>(""));
			Fields.Add(_name = new ColladaObjectAttribute<ColladaNCName>(""));
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputUnshared>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _id));
			ValidationTests.Add(new ColladaEmptyString(Enums.ColladaElementType.All, _id));
			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _input));
			ValidationTests.Add(new ColladaListMinCount<ColladaInputUnshared>(Enums.ColladaElementType.All, _input, 1));
			ValidationTests.Add(new ColladaListHasValue<ColladaInputUnshared, Enums.ColladaInputSharedSemantic>(Enums.ColladaElementType.All,
				_input, "Semantic", Enums.ColladaInputSharedSemantic.POSITION));
		}
	}
}