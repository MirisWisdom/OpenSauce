/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaModifier : ColladaElement
	{
		#region Fields
		ColladaObjectValue<Enums.ColladaModifierValue> _value;
		#endregion

		[XmlTextAttribute()]
		public Enums.ColladaModifierValue Value
		{ get { return _value.Value; } set { _value.Value = value; } }

		public ColladaModifier() : base(Enums.ColladaElementType.Fx_Modifier)
		{
			Fields.Add(_value = new ColladaObjectValue<Enums.ColladaModifierValue>(Enums.ColladaModifierValue.CONST));
		}
	}
}