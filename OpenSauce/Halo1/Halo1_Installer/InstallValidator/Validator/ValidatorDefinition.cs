/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace InstallValidator.Validator
{
	public enum ValidationEventEnum
	{
		[XmlEnum(Name = "continue")]
		Continue,
		[XmlEnum(Name = "warn")]
		Warn,
		[XmlEnum(Name = "error")]
		Error
	}

	[XmlRoot(ElementName = "Installation")]
	public class ValidatorDefinition
	{
		public class Message
		{
			[XmlAttribute(AttributeName = "id")]
			public string ID;

			[XmlAttribute(AttributeName = "type")]
			public ValidationEventEnum Type;

			[XmlText]
			public string MessageValue;
		}

		public abstract class MessageClass
		{
			[XmlElement(ElementName = "Message")]
			public List<Message> Messages = new List<Message>();

			protected Message GetMessage(string ID)
			{
				if ((ID == null) || (ID.Length == 0))
					return null;

				return Messages.Find(
					delegate(Message value)
					{
						if (value.ID.Equals(ID))
							return true;
						return false;
					}
				);
			}
		}

		public class Version : MessageClass
		{
			[XmlAttribute(AttributeName = "md5")]
			public string MD5;

			[XmlAttribute(AttributeName = "ondiff")]
			public string OnDiffMessageID;

			[XmlAttribute(AttributeName = "onmatch")]
			public string OnMatchMessageID;

			[XmlIgnore]
			public Message OnDiffMessage
			{
				get
				{
					return GetMessage(OnDiffMessageID);
				}
			}

			[XmlIgnore]
			public Message OnMatchMessage
			{
				get
				{
					return GetMessage(OnMatchMessageID);
				}
			}
		}

		public class File : MessageClass
		{
			[XmlAttribute(AttributeName = "id")]
			public string ID;

			[XmlAttribute(AttributeName = "name")]
			public string Name;

			[XmlAttribute(AttributeName = "onmissing")]
			public string OnMissingMessageID;

			[XmlAttribute(AttributeName = "onpresent")]
			public string OnPresentMessageID;

			[XmlAttribute(AttributeName = "onunknown")]
			public string OnUnknownMessageID;

			[XmlElement(ElementName = "Version")]
			public List<Version> Versions = new List<Version>();

			[XmlIgnore]
			public Message OnMissingMessage
			{
				get
				{
					return GetMessage(OnMissingMessageID);
				}
			}

			[XmlIgnore]
			public Message OnPresentMessage
			{
				get
				{
					return GetMessage(OnPresentMessageID);
				}
			}
		}

		public class Directory : MessageClass
		{
			[XmlAttribute(AttributeName = "id")]
			public string ID;

			[XmlAttribute(AttributeName = "name")]
			public string Name;

			[XmlAttribute(AttributeName = "onmissing")]
			public string OnMissingMessageID;

			[XmlAttribute(AttributeName = "onpresent")]
			public string OnPresentMessageID;

			[XmlElement(ElementName = "Directory")]
			public List<Directory> SubDirectories = new List<Directory>();

			[XmlElement(ElementName = "File")]
			public List<File> Files = new List<File>();

			[XmlIgnore]
			public Message OnMissingMessage
			{
				get
				{
					return GetMessage(OnMissingMessageID);
				}
			}

			[XmlIgnore]
			public Message OnPresentMessage
			{
				get
				{
					return GetMessage(OnPresentMessageID);
				}
			}
		}

		[XmlAttribute(AttributeName = "definition")]
		public string Definition;

		[XmlAttribute(AttributeName = "registry_key")]
		public string RegistryKey;

		[XmlAttribute(AttributeName = "registry_name")]
		public string RegistryName;

		[XmlElement(ElementName = "Directory")]
		public Directory RootDirectory;
	}
}
