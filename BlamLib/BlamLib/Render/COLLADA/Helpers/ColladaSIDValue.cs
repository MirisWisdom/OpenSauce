/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.Xml;
using System.Xml.Serialization;

namespace BlamLib.Render.COLLADA
{
	[SerializableAttribute()]
	[XmlTypeAttribute(AnonymousType = true)]
	public partial class ColladaSIDValue<T> : ColladaValue<T>
	{
		#region Attributes
		[XmlAttribute("sid"), DefaultValue("")]
		public string sID;
		#endregion

		public ColladaSIDValue() { }
		public ColladaSIDValue(T value) : base(value) { }
	}
}