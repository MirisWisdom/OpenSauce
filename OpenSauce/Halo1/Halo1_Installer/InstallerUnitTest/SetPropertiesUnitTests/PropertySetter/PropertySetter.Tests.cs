/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.SetPropertiesUnitTests.PropertySetter
{
	[TestClass]
	[DeploymentItem(@"SetPropertiesUnitTests\Resources\PropertySetValidPath.xml", "Resources")]
	[DeploymentItem(@"SetPropertiesUnitTests\Resources\PropertySetValidString.xml", "Resources")]
	[DeploymentItem(@"SetPropertiesUnitTests\Resources\PropertySetFileExists.xml", "Resources")]
	[DeploymentItem(@"SetPropertiesUnitTests\Resources\PropertySetDirectoryExists.xml", "Resources")]
	public class PropertySetterTests
	{
		private enum ExpectedResult
		{
			Success,
			Failure,
			Error
		}

		private bool TestSetProperties(string definitionFile,
			Dictionary<string, string> presentProperties,
			Dictionary<string, string> expectedProperties,
			ExpectedResult expectedResult)
		{
			using (var xmlReader = XmlReader.Create(definitionFile))
			{
				var definition = SetProperties.PropertySetter.PropertySetter.DeserializeDefinition(xmlReader);
				var propertySetter = new SetProperties.PropertySetter.PropertySetter();

				// Set the properties using the provided definition
				var errorOccurred = false;
				propertySetter.ErrorOccurred +=
					(sender, args) =>
					{
						errorOccurred = true;
					};

				Dictionary<string, string> setProperties = new Dictionary<string, string>();
				var result = propertySetter.SetProperties(definition,
					(property, value) =>
					{
						setProperties.Add(property, value);
					},
					(property) =>
					{
						if (!presentProperties.ContainsKey(property))
						{
							return null;
						}
						return presentProperties[property];
					}
				);

				// Check that the properties match the expected values
				var propertiesMatch = true;
				foreach(var property in expectedProperties)
				{
					if(!setProperties.ContainsKey(property.Key))
					{
						propertiesMatch = false;
						break;
					}

					if(setProperties[property.Key] != property.Value)
					{
						propertiesMatch = false;
						break;
					}
				}

				// Return whether the the expected result was encountered
				switch(expectedResult)
				{
					case ExpectedResult.Success:
						return (!errorOccurred && result && propertiesMatch);
					case ExpectedResult.Failure:
						return (!errorOccurred && !result);
					case ExpectedResult.Error:
						return (errorOccurred);
					default:
						throw new NotSupportedException("Unknown reuslt type");
				}
			}
		}

		[TestMethod]
		public void SetProperties_WithAStringProperty_SetsTheProperty()
		{
			var result = TestSetProperties(@"Resources\PropertySetValidString.xml", null,
				new Dictionary<string, string>()
				{
					{ "STRING_PROPERTY", "StringValue" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a valid string property");
		}

		[TestMethod]
		public void SetProperties_WithAPathProperty_SetsTheProperty()
		{
			var result = TestSetProperties(@"Resources\PropertySetValidPath.xml",
				new Dictionary<string, string>()
				{
					{ "PROPERTYSOURCE", @"C:\FakeDirectory" }
				},
				new Dictionary<string, string>()
				{
					{ "PATH_PROPERTY", @"C:\FakeDirectory\Filename.txt" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a valid string property");
		}

		[TestMethod]
		public void SetProperties_WithAPathPropertyAndMissingSourceProperty_ReturnsFalse()
		{
			var result = TestSetProperties(@"Resources\PropertySetValidPath.xml",
				new Dictionary<string, string>(),
				new Dictionary<string, string>()
				{
					{ "PATH_PROPERTY", @"C:\FakeDirectory\Filename.txt" }
				},
				ExpectedResult.Failure
			);

			Assert.IsTrue(result, "Failed to set a valid string property");
		}

		[TestMethod]
		public void SetProperties_WithAFileExistsPropertyAndAnExistingFile_SetsThePropertyToTrue()
		{
			var result = TestSetProperties(@"Resources\PropertySetFileExists.xml",
				new Dictionary<string, string>()
				{
					{ "PROPERTYSOURCE", @"%windir%\system32\notepad.exe" }
				},
				new Dictionary<string, string>()
				{
					{ "EXISTS_PROPERTY", "True" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a file exists property");
		}

		[TestMethod]
		public void SetProperties_WithAFileExistsPropertyAndAMissingFile_SetsThePropertyToFalse()
		{
			var result = TestSetProperties(@"Resources\PropertySetFileExists.xml",
				new Dictionary<string, string>()
				{
					{ "PROPERTYSOURCE", @"%windir%\system32\nopepad.exe" }
				},
				new Dictionary<string, string>()
				{
					{ "EXISTS_PROPERTY", "False" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a file exists property");
		}

		[TestMethod]
		public void SetProperties_WithADirectoryExistsPropertyAndAnExistingDirectory_SetsThePropertyToTrue()
		{
			var result = TestSetProperties(@"Resources\PropertySetDirectoryExists.xml",
				new Dictionary<string, string>()
				{
					{ "PROPERTYSOURCE", @"%windir%\system32" }
				},
				new Dictionary<string, string>()
				{
					{ "EXISTS_PROPERTY", "True" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a directory exists property");
		}

		[TestMethod]
		public void SetProperties_WithADirectoryExistsPropertyAndAMissingDirectory_SetsThePropertyToFalse()
		{
			var result = TestSetProperties(@"Resources\PropertySetDirectoryExists.xml",
				new Dictionary<string, string>()
				{
					{ "PROPERTYSOURCE", @"%windir%\sistem32" }
				},
				new Dictionary<string, string>()
				{
					{ "EXISTS_PROPERTY", "False" }
				},
				ExpectedResult.Success
			);

			Assert.IsTrue(result, "Failed to set a directory exists property");
		}
	}
}
