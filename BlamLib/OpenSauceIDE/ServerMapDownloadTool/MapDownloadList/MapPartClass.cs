using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.ComponentModel;

namespace OpenSauceIDE.ServerMapDownloadTool.MapDownloadList
{
	public class MapPartClass
	{
		[XmlAttribute(AttributeName = "name")]
		public string Name = "";

		[XmlAttribute(AttributeName = "index")]
		public int Index = 0;

		[XmlAttribute(AttributeName = "md5")]
		public string MD5 = "";

		[XmlAttribute(AttributeName = "size")]
		public uint Size = 0;

		[XmlAttribute(AttributeName = "encrypted"), DefaultValue(false)]
		public bool Encrypted = false;

		[XmlAttribute(AttributeName = "unencrypted_md5"), DefaultValue("")]
		public string UnencryptedMD5 = "";
	}
}
