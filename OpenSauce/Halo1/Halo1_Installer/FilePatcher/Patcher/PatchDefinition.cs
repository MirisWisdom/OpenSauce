/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Xml.Serialization;

namespace FilePatcher.Patcher
{
	[XmlRoot(ElementName = "FilePatch")]
	public class PatchDefinition
	{
		public enum ValueType
		{
			[XmlEnum(Name = "string")]
			String,
			[XmlEnum(Name = "uint32")]
			UInteger32,
			[XmlEnum(Name = "uint16")]
			UInteger16,
			[XmlEnum(Name = "byte")]
			Byte,
			[XmlEnum(Name = "bytes")]
			Bytes
		}

		public class Address
		{
			[XmlAttribute(AttributeName = "ptr")]
			public string PTRString = "";

			[XmlAttribute(AttributeName = "value")]
			public string Value = "";
		}

		public class Data
		{
			[XmlAttribute(AttributeName = "id")]
			public string ID = "";

			[XmlAttribute(AttributeName = "type")]
			public ValueType Type;

			[XmlAttribute(AttributeName = "length"), DefaultValue(0)]
			public int Length = 0;

			[XmlAttribute(AttributeName = "swapendian"), DefaultValue(false)]
			public bool SwapEndian = false;

			[XmlAttribute(AttributeName = "writenull"), DefaultValue(true)]
			public bool WriteNull = true;

			[XmlText]
			public string Value;
		}

		public class Patch
		{
			[XmlAttribute(AttributeName = "id")]
			public string ID = "";

			[XmlElement(ElementName = "Data")]
			public List<Data> Data = new List<Data>();

			[XmlElement(ElementName = "Address")]
			public List<Address> Addresses = new List<Address>();
		}

		[XmlAttribute(AttributeName = "inputfile")]
		public string InputFile = "";

		[XmlAttribute(AttributeName = "outputfile")]
		public string OutputFile = "";

		[XmlAttribute(AttributeName = "peaddressmask")]
		public string PEAddressMaskString = "";

		[XmlAttribute(AttributeName = "md5_pre")]
		public string MD5Pre = "";

		[XmlAttribute(AttributeName = "md5_post")]
		public string MD5Post = "";

		[XmlElement(ElementName = "Patch")]
		public List<Patch> Patches = new List<Patch>();
	}
}
