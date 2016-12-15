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
	public partial class ColladaTargets : ColladaElement
	{
		#region Fields
		ColladaObjectElementList<ColladaInputUnshared> _input;
		ColladaObjectElementList<ColladaExtra> _extra;
		#endregion

		#region Children
		[XmlElement("input")]
		public List<ColladaInputUnshared> Input
		{ get { return _input.Value; } set { _input.Value = value; } }

		[XmlElement("extra")]
		public List<ColladaExtra> Extra
		{ get { return _extra.Value; } set { _extra.Value = value; } }
		#endregion

		public ColladaTargets() : base(Enums.ColladaElementType.Core_Targets)
		{
			Fields.Add(_input = new ColladaObjectElementList<ColladaInputUnshared>());
			Fields.Add(_extra = new ColladaObjectElementList<ColladaExtra>());

			ValidationTests.Add(new ColladaIsNull(Enums.ColladaElementType.All, _input));
			ValidationTests.Add(new ColladaListMinCount<ColladaInputUnshared>(Enums.ColladaElementType.All, _input, 2));
			ValidationTests.Add(new ColladaListHasValue<ColladaInputUnshared, Enums.ColladaInputSharedSemantic>(Enums.ColladaElementType.All,
				_input, "Semantic", Enums.ColladaInputSharedSemantic.MORPH_TARGET));
			ValidationTests.Add(new ColladaListHasValue<ColladaInputUnshared, Enums.ColladaInputSharedSemantic>(Enums.ColladaElementType.All,
				_input, "Semantic", Enums.ColladaInputSharedSemantic.MORPH_WEIGHT));
		}
	}
}