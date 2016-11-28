/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render
{
	namespace COLLADA
	{
		[SerializableAttribute()]
		[XmlTypeAttribute(AnonymousType = true)]
		public partial class ColladaValue<T>
		{
			public ColladaValue() { }
			public ColladaValue(T value) { Value = value; }

			[XmlTextAttribute()]
			public T Value;
		}
	}
}