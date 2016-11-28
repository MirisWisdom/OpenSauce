/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA.Core
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSkeleton : ColladaElement
	{
		#region Fields
		ColladaObjectValue<string> _value;
		#endregion

		[XmlTextAttribute()]
		public string Value
		{ get { return _value.Value; } set { _value.Value = value; } }

		public ColladaSkeleton() : base(Enums.ColladaElementType.Core_Skeleton) 
		{
			Fields.Add(_value = new ColladaObjectValue<string>("")); 
		}
	}
}