/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Xml;
using System.Xml.Serialization;

namespace InstallValidator.Validator
{
	public class PathValidator
	{
		public List<string> WarningMessages { get; private set; }
		public List<string> ErrorMessages { get; private set; }

		#region Definition
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Deserialize a validator definition. </summary>
		///
		/// <param name="xmlStream">	The XML stream. </param>
		///
		/// <returns>	A ValidatorDefinition. </returns>
		public static ValidatorDefinition DeserializeDefinition(XmlReader xmlStream)
		{
			if (xmlStream == null)
			{
				return null;
			}

			// Deserialize the definition xml
			var serializer = new XmlSerializer(typeof(ValidatorDefinition));
			if (!serializer.CanDeserialize(xmlStream))
			{
				return null;
			}

			return serializer.Deserialize(xmlStream) as ValidatorDefinition;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a validator definition from an embedded resource. </summary>
		///
		/// <param name="definitionId">	Identifier for the validator definition. </param>
		///
		/// <returns>	The validator definition. </returns>
		private ValidatorDefinition GetDefinition(string definitionId)
		{
			// Build the location of the validator definition resource
			// Open the resource for reading
			var definitionResourceId = String.Format("InstallValidator.Definitions.{0}.xml", definitionId);

			using(var xmlStream = XmlReader.Create(GetType().Assembly.GetManifestResourceStream(definitionResourceId)))
			{
				return DeserializeDefinition(xmlStream);
			}
		}
		#endregion

		#region Validation
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates an MD5 checksum for a stream. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="streamIn">	The stream to read. </param>
		///
		/// <returns>	An MD5 checksum. </returns>
		private static string MD5Stream(Stream streamIn)
		{
			if (streamIn == null)
			{
				throw new ArgumentNullException("streamIn cannot be null");
			}

			if (!streamIn.CanRead)
			{
				throw new NotSupportedException("Cannot read from the input stream");
			}

			// Create an md5 hash for the stream and return it's string representation
			using (var md5 = MD5.Create())
			{
				if (md5 == null)
				{
					return null;
				}

				streamIn.Seek(0, SeekOrigin.Begin);

				var md5Hash = md5.ComputeHash(streamIn);

				return BitConverter.ToString(md5Hash).Replace("-", "");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates a file. </summary>
		///
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="file">	   	The file validation definition. </param>
		/// <param name="filePath">	Full pathname of the file. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool ValidateFile(ValidatorDefinition.File file, string filePath)
		{
			if (File.Exists(filePath))
			{
				// Process the file present event
				var onPresentMessage = file.OnPresentMessage;
				if (onPresentMessage != null)
				{
					switch (onPresentMessage.Type)
					{
						case ValidationEventEnum.Warn:
							WarningMessages.Add(onPresentMessage.MessageValue);
							return true;
						case ValidationEventEnum.Error:
							ErrorMessages.Add(onPresentMessage.MessageValue);
							return false;
						default:
							throw new NotSupportedException("Unknown event type");
					}
				}

				// Open the file and generate an MD5 hash for it
				string localMd5 = "";
				using(FileStream openedFile = File.OpenRead(filePath))
				{
					// Error if file could not be opened
					if ((openedFile == null) || !openedFile.CanRead)
					{
						ErrorMessages.Add(String.Format("Failed to open {0} for reading, close all programs before attempting to install", file.Name));
						return false;
					}

					localMd5 = MD5Stream(openedFile);
					if (localMd5 == null)
					{
						ErrorMessages.Add(String.Format("Failed to create an MD5 checksum for {0}. Inform the developers of this.", file.Name));
						return false;
					}
				}

				// Compare the MD5 hash with all known version
				foreach (var version in file.Versions)
				{
					if (String.Equals(version.MD5, localMd5, StringComparison.CurrentCultureIgnoreCase))
					{
						// Process file matches version event
						var onMatchMessage = version.OnMatchMessage;
						if (onMatchMessage != null)
						{
							switch (onMatchMessage.Type)
							{
								case ValidationEventEnum.Warn:
									WarningMessages.Add(onMatchMessage.MessageValue);
									return true;
								case ValidationEventEnum.Error:
									ErrorMessages.Add(onMatchMessage.MessageValue);
									return false;
							}
						}
					}
					else
					{
						// Process file differs from version event
						var onDiffMessage = version.OnDiffMessage;
						if (onDiffMessage != null)
						{
							switch (onDiffMessage.Type)
							{
								case ValidationEventEnum.Warn:
									WarningMessages.Add(onDiffMessage.MessageValue);
									return true;
								case ValidationEventEnum.Error:
									ErrorMessages.Add(onDiffMessage.MessageValue);
									return false;
							}
						}
					}
				}
			}
			else
			{
				// Process file missing event
				var onMissingMessage = file.OnMissingMessage;
				if (onMissingMessage != null)
				{
					switch (onMissingMessage.Type)
					{
						case ValidationEventEnum.Warn:
							WarningMessages.Add(onMissingMessage.MessageValue);
							break;
						case ValidationEventEnum.Error:
							ErrorMessages.Add(onMissingMessage.MessageValue);
							return false;
					}
				}
			}
			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates a directory. </summary>
		///
		/// <param name="directory">	 	The directory validation definition. </param>
		/// <param name="directoryPath">	Full pathname of the directory. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		private bool ValidateDirectory(ValidatorDefinition.Directory directory, string directoryPath)
		{
			if (Directory.Exists(directoryPath))
			{
				// Process directory present event
				var onPresentMessage = directory.OnPresentMessage;
				if (onPresentMessage != null)
				{
					switch (onPresentMessage.Type)
					{
						case ValidationEventEnum.Warn:
							WarningMessages.Add(onPresentMessage.MessageValue);
							return true;
						case ValidationEventEnum.Error:
							ErrorMessages.Add(onPresentMessage.MessageValue);
							return false;
					}
				}

				// Validate known subdirectories
				foreach (var subdirectory in directory.SubDirectories)
				{
					string subdirectoryPath = Path.Combine(directoryPath, subdirectory.Name);

					if (!ValidateDirectory(subdirectory, subdirectoryPath))
					{
						return false;
					}
				}

				// Validate known files
				foreach (var file in directory.Files)
				{
					string filePath = Path.Combine(directoryPath, file.Name);

					if (!ValidateFile(file, filePath))
					{
						return false;
					}
				}
			}
			else
			{
				// Process directory missing event
				var onMissingMessage = directory.OnMissingMessage;
				if (onMissingMessage != null)
				{
					switch (onMissingMessage.Type)
					{
						case ValidationEventEnum.Warn:
							WarningMessages.Add(onMissingMessage.MessageValue);
							break;
						case ValidationEventEnum.Error:
							ErrorMessages.Add(onMissingMessage.MessageValue);
							return false;
					}
				}
			}
			return true;
		}
		#endregion

		public PathValidator()
		{
			WarningMessages = new List<string>();
			ErrorMessages = new List<string>();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates a path according to the provided validation definition. </summary>
		///
		/// <param name="validatorDefinition">	The validator definition. </param>
		/// <param name="validatorPath">	  	The path to validate. </param>
		public void Validate(ValidatorDefinition validatorDefinition, string validatorPath)
		{
			ValidateDirectory(validatorDefinition.RootDirectory, validatorPath);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates a path according to the provided validation definition. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		///
		/// <param name="validatorDefinitionID">	The validator definition id. </param>
		/// <param name="validatorPath">			The path to validate. </param>
		public void Validate(string validatorDefinitionID, string validatorPath)
		{
			if (validatorDefinitionID == null)
			{
				throw new ArgumentNullException("Validator definition id cannot be null");
			}

			if (validatorDefinitionID == String.Empty)
			{
				throw new ArgumentNullException("Validator definition id cannot be empty");
			}

			// Get the validator definition
			var definition = GetDefinition(validatorDefinitionID);
			if (definition == null)
			{
				ErrorMessages.Add("Failed to find a validator definition. Inform the developers of this.");
				return;
			}

			ValidateDirectory(definition.RootDirectory, validatorPath);
		}
	}
}