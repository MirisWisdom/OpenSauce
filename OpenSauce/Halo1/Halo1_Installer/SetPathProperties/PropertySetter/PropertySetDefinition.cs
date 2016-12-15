/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Xml.Serialization;

namespace SetProperties
{
	public enum PropertyType
	{
		[XmlEnum("string")]
		String,
		[XmlEnum("path")]
		Path,
		[XmlEnum("fileexists")]
		FileExists,
		[XmlEnum("directoryexists")]
		DirectoryExists
	}

	[XmlRoot(ElementName = "PropertySet")]
	public class PropertySetDefinition
	{
		public class Property
		{
			[XmlAttribute("Name")]
			public string Name;

			[XmlAttribute("Type"), DefaultValue(PropertyType.String)]
			public PropertyType Type = PropertyType.String;

			[XmlAttribute("Value")]
			public string Value;
		}

		[XmlElement("Property")]
		public List<Property> Properties = new List<Property>();
	}
}
