/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Serialization;
using System.IO;
using System.Xml;
using System.ComponentModel;
using System.Security.Cryptography;

namespace InstallValidator
{
	public enum ValidationEventEnum
	{
		[XmlEnum(Name="continue")]
		Continue,
		[XmlEnum(Name = "warn")]
		Warn,
		[XmlEnum(Name = "error")]
		Error
	}

	[XmlRoot(ElementName="Installation")]
	public class Installation
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
			[XmlAttribute(AttributeName="id")]
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

	public class Validator
	{
		public List<string> WarningMessages { get; private set; }
		public List<string> ErrorMessages { get; private set; }

		Installation m_installation = null;

		private static string MD5Stream(Stream stream_in)
		{
			string md5_string = "";

			// check the source files md5 matches before patching
			using (MD5 md5 = MD5.Create())
			{
				if (md5 == null)
					return null;

				stream_in.Seek(0, SeekOrigin.Begin);
				byte[] md5_hash = md5.ComputeHash(stream_in);

				for (int x = 0; x < md5_hash.Length; x++)
					md5_string += md5_hash[x].ToString("X2");
			}

			return md5_string;
		}

		public Validator(string definition_id)
		{
			// build the location of the game definition resource
			// open the resource for reading
			Stream xml_stream = null;
			xml_stream = GetType().Assembly.GetManifestResourceStream(String.Format("InstallValidator.InstallationDefinitions.{0}.xml", definition_id));
			if (xml_stream == null)
				throw new Exception(String.Format("Matching installation definition for {0} was not found. Inform the developers of the problem", definition_id));
			
			// deserialize the definition xml
			XmlSerializer serializer = new XmlSerializer(typeof(Installation));
			m_installation = serializer.Deserialize(xml_stream) as Installation;

			if (m_installation == null)
				throw new Exception(String.Format("Failed to deserialize a installation definition xml {0}. Inform the developers of the problem.", definition_id));

			// create the message lists
			WarningMessages = new List<string>();
			ErrorMessages = new List<string>();
		}

		private bool ValidateFile(Installation.File file, string file_path)
		{
			if (File.Exists(file_path))
			{
				// process the file present event
				Installation.Message on_present_message = file.OnPresentMessage;
				if (on_present_message != null)
				{
					switch (on_present_message.Type)
					{
						case ValidationEventEnum.Warn:
							// add warning
							WarningMessages.Add(on_present_message.MessageValue);
							return true;
						case ValidationEventEnum.Error:
							//add error
							ErrorMessages.Add(on_present_message.MessageValue);
							return false;
						default:
							throw new Exception("Unknown event type");
					}
				}

				// open the file and generate an MD5 hash for it
				string local_md5 = "";
				using(FileStream opened_file = File.OpenRead(file_path))
				{
					// error if file could not be opened
					if ((opened_file == null) || !opened_file.CanRead)
					{
						ErrorMessages.Add(String.Format("Failed to open {0} for reading, close all programs before attempting to install", file.Name));
						return false;
					}

					local_md5 = MD5Stream(opened_file);
					if (local_md5 == null)
					{
						ErrorMessages.Add(String.Format("Failed to create an MD5 checksum for {0}. Inform the developers of this.", file.Name));
						return false;
					}
				}

				// compare the MD5 hash with all known version
				foreach (var version in file.Versions)
				{
					if (String.Equals(version.MD5, local_md5, StringComparison.CurrentCultureIgnoreCase))
					{
						// process file matches version event
						Installation.Message on_match_message = version.OnMatchMessage;
						if (on_match_message != null)
						{
							switch (on_match_message.Type)
							{
								case ValidationEventEnum.Warn:
									// add warning
									WarningMessages.Add(on_match_message.MessageValue);
									return true;
								case ValidationEventEnum.Error:
									//add error
									ErrorMessages.Add(on_match_message.MessageValue);
									return false;
							}
						}
					}
					else
					{
						// process file differs from version event
						Installation.Message on_diff_message = version.OnDiffMessage;
						if (on_diff_message != null)
						{
							switch (on_diff_message.Type)
							{
								case ValidationEventEnum.Warn:
									// add warning
									WarningMessages.Add(on_diff_message.MessageValue);
									return true;
								case ValidationEventEnum.Error:
									//add error
									ErrorMessages.Add(on_diff_message.MessageValue);
									return false;
							}
						}
					}
				}
			}
			else
			{
				// process file missing event
				Installation.Message on_missing_message = file.OnMissingMessage;
				if (on_missing_message != null)
				{
					switch (on_missing_message.Type)
					{
						case ValidationEventEnum.Warn:
							// add warning
							WarningMessages.Add(on_missing_message.MessageValue);
							break;
						case ValidationEventEnum.Error:
							//add error
							ErrorMessages.Add(on_missing_message.MessageValue);
							return false;
					}
				}
			}
			return true;
		}

		private bool ValidateDirectory(Installation.Directory directory, string directory_path)
		{
			if (Directory.Exists(directory_path))
			{
				// process directory present event
				Installation.Message on_present_message = directory.OnPresentMessage;
				if (on_present_message != null)
				{
					switch (on_present_message.Type)
					{
						case ValidationEventEnum.Warn:
							// add warning
							WarningMessages.Add(on_present_message.MessageValue);
							return true;
						case ValidationEventEnum.Error:
							//add error
							ErrorMessages.Add(on_present_message.MessageValue);
							return false;
					}
				}

				// validate known subdirectories
				foreach (var sub_directory in directory.SubDirectories)
				{
					string sub_directory_path = Path.Combine(directory_path, sub_directory.Name);
					if (!ValidateDirectory(sub_directory, sub_directory_path))
						return false;
				}

				// validate known files
				foreach (var file in directory.Files)
				{
					string file_path = Path.Combine(directory_path, file.Name);
					if (!ValidateFile(file, file_path))
						return false;
				}
			}
			else
			{
				// process diretory missing event
				Installation.Message on_missing_message = directory.OnMissingMessage;
				if (on_missing_message != null)
				{
					switch (on_missing_message.Type)
					{
						case ValidationEventEnum.Warn:
							// add warning
							WarningMessages.Add(on_missing_message.MessageValue);
							break;
						case ValidationEventEnum.Error:
							//add error
							ErrorMessages.Add(on_missing_message.MessageValue);
							return false;
					}
				}
			}
			return true;
		}
		
		public void Validate(string installation_path)
		{
			ValidateDirectory(m_installation.RootDirectory, installation_path);
		}
	}
}