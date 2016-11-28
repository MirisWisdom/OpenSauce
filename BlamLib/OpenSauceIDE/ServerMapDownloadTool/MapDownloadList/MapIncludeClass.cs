using System;
using System.Xml.Serialization;

namespace OpenSauceIDE.ServerMapDownloadTool.MapDownloadList
{
	[XmlRoot(ElementName = "osHTTPServer", IsNullable = false)]
	public class MapIncludeClass
	{
		[XmlElement(ElementName = "map_download")]
		public MapDownloadClass MapDownload;
	}
}
