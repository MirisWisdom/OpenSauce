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
	public partial class ColladaSampler : ColladaElement
	{
		#region Fields
		ColladaObjectAttribute<ColladaID<ColladaSampler>> _id;

		ColladaObjectElementList<ColladaInputUnshared> _input;
		#endregion

		#region Attributes
		[XmlAttribute("id"), DefaultValue(""), ColladaID("sampler-{0}")]
		public string ID
		{ get { return _id.Value; } set { _id.Value = value; } }
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputUnshared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }
		#endregion

		public ColladaSampler() : base(Enums.ColladaElementType.Core_Sampler)
		{
			Fields.Add(_id = new ColladaObjectAttribute<ColladaID<ColladaSampler>>(""));
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputUnshared>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _input));
			ValidationTests.Add(new ColladaListMinCount<ColladaInputUnshared>(Enums.ColladaElementType.All, _input, 1));
			ValidationTests.Add(new ColladaListHasValue<ColladaInputUnshared, Enums.ColladaInputSharedSemantic>(Enums.ColladaElementType.All,
				_input, "Semantic", Enums.ColladaInputSharedSemantic.INTERPOLATION));
		}
	}
}