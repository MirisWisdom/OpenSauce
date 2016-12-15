/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace SetProperties.PropertySetter
{
	public class PropertySetter
	{
		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Additional information for set property error events. </summary>
		public class SetPropertiesErrorEventArgs : EventArgs
		{
			public string Message;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event queue for all listeners interested in ErrorOccurred events. </summary>
		public event EventHandler<SetPropertiesErrorEventArgs> ErrorOccurred;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the error occurred action. </summary>
		///
		/// <param name="message">	The message. </param>
		private void OnErrorOccurred(string message)
		{
			var handler = ErrorOccurred;

			if (handler != null)
			{
				handler(this, new SetPropertiesErrorEventArgs() { Message = message });
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Executes the error occurred action. </summary>
		///
		/// <param name="format">	The message format. </param>
		/// <param name="args">  	A variable-length parameters list containing arguments. </param>
		private void OnErrorOccurred(string format, params object[] args)
		{
			OnErrorOccurred(String.Format(format, args));
		}
		#endregion

		#region Definition
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Deserialize a property set definition. </summary>
		///
		/// <param name="xmlStream">	The XML stream. </param>
		///
		/// <returns>	A property set definition. </returns>
		public static PropertySetDefinition DeserializeDefinition(XmlReader xmlStream)
		{
			if (xmlStream == null)
			{
				return null;
			}

			// Deserialize the definition xml
			var serializer = new XmlSerializer(typeof(PropertySetDefinition));
			if (!serializer.CanDeserialize(xmlStream))
			{
				return null;
			}

			return serializer.Deserialize(xmlStream) as PropertySetDefinition;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a property set definition from an embedded resource. </summary>
		///
		/// <param name="definitionId">	Identifier for the property set definition. </param>
		///
		/// <returns>	The property set definition. </returns>
		private PropertySetDefinition GetDefinition(string definitionId)
		{
			// Build the location of the property set definition resource
			// Open the resource for reading
			var definitionResourceId = String.Format("SetProperties.Definitions.{0}.xml", definitionId);

			using (var xmlStream = XmlReader.Create(GetType().Assembly.GetManifestResourceStream(definitionResourceId)))
			{
				return DeserializeDefinition(xmlStream);
			}
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets properties according to the provided definition. </summary>
		///
		/// <param name="propertySetDefinition">	The property set definition. </param>
		/// <param name="onSetProperty">			The set property callback. </param>
		/// <param name="onGetProperty">			The get property callback. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public bool SetProperties(PropertySetDefinition propertySetDefinition,
			Action<string, string> onSetProperty,
			Func<string, string> onGetProperty)
		{
			// Set the properties
			foreach (var property in propertySetDefinition.Properties)
			{
				var splitValues = property.Value.Split(new char[] { ';' });

				// Expand properties as needed
				for (int i = 0; i < splitValues.Length; ++i)
				{
					if (splitValues[i].StartsWith("[") && splitValues[i].EndsWith("]"))
					{
						var propertyValue = onGetProperty(splitValues[i].Trim('[', ']'));
						if(propertyValue == null)
						{
							return false;
						}

						splitValues[i] = propertyValue;
					}
				}

				// Set the property
				switch(property.Type)
				{
					case PropertyType.String:
						onSetProperty(property.Name, property.Value);
						break;
					case PropertyType.Path:
						{
							var pathResult = "";
							foreach (var path in splitValues)
							{
								pathResult = Path.Combine(pathResult, path);
							}
							onSetProperty(property.Name, pathResult);
						}
						break;
					case PropertyType.FileExists:
						{
							var expandedPath = Environment.ExpandEnvironmentVariables(splitValues[0]);
							var exists = File.Exists(expandedPath);
							onSetProperty(property.Name, exists.ToString());
						}
						break;
					case PropertyType.DirectoryExists:
						{
							var expandedPath = Environment.ExpandEnvironmentVariables(splitValues[0]);
							var exists = Directory.Exists(expandedPath);
							onSetProperty(property.Name, exists.ToString());
						}
						break;
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets properties according to the provided definition. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="propertySetDefinitionID">	Identifier for the property set definition. </param>
		public bool SetProperties(string propertySetDefinitionID,
			Action<string, string> onSetProperty,
			Func<string, string> onGetProperty)
		{
			if (propertySetDefinitionID == null)
			{
				throw new ArgumentNullException("Property set definition id cannot be null");
			}

			if (propertySetDefinitionID == String.Empty)
			{
				throw new ArgumentNullException("Property set definition id cannot be empty");
			}

			// Get the property set definition
			var definition = GetDefinition(propertySetDefinitionID);
			if (definition == null)
			{
				OnErrorOccurred("Failed to find a property set definition. Inform the developers of this.");
				return false;
			}

			return SetProperties(definition, onSetProperty, onGetProperty);
		}
	}
}
