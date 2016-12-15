/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Fx
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSemantic : ColladaElement
	{
		#region Fields
		ColladaObjectValue<string> _value;
		#endregion

		[XmlTextAttribute()]
		public string Value
		{ get { return _value.Value; } set { _value.Value = value; } }

		public ColladaSemantic() : base(Enums.ColladaElementType.Fx_Semantic)
		{
			Fields.Add(_value = new ColladaObjectValue<string>(""));
		}
	}
}