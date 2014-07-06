using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace SetPathProperties
{
    [XmlRoot(ElementName = "PathSet")]
    public class PathSet
    {
        public class Property
        {
            [XmlAttribute("Name")]
            public string Name;

            [XmlAttribute("Value")]
            public string Value;
        }

        [XmlAttribute("Id")]
        public string ID;

        [XmlAttribute("RootProperty")]
        public string RootProperty;

        [XmlElement("Property")]
        public List<Property> Properties = new List<Property>();
    }
}
