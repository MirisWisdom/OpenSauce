using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;

namespace OpenSauceIDE.ServerMapDownloadTool.MapDownloadList
{
	public enum MapDownloadCompressionFormat
	{
		[XmlEnum(Name = "zip")]
		Zip,
		[XmlEnum(Name = "7zip")]
		SevenZip
	};

	public class MapDownloadClass
	{
		[XmlAttribute(AttributeName = "algorithm")]
		public MapDownloadCompressionFormat Algorithm;

		[XmlAttribute(AttributeName = "name")]
		public string Name;

		[XmlAttribute(AttributeName = "md5")]
		public string MD5;

		[XmlAttribute(AttributeName = "uncompressed_size")]
		public uint UncompressedSize;

		[XmlAttribute(AttributeName = "compressed_size")]
		public uint CompressedSize;

		[XmlAttribute(AttributeName = "host_directory")]
		public string HostDirectory;

		[XmlElement(ElementName="part")]
		public List<MapPartClass> Parts = new List<MapPartClass>();
	}
}
