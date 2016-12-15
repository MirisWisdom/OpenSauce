/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.InstallValidatorUnitTests
{
	[TestClass]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\TestFileOne.txt", @"Resources\TestDirectoryOne")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\TestFileTwo.txt", @"Resources\TestDirectoryOne\TestDirectoryTwo")]
	public class InstallValidatorTests
	{
		public TestContext TestContext { get; set; }

		[TestMethod]
		public void InstallValidator_WithNullValidatorDefinition_FlagsAnError()
		{
			var errorOccurred = false;

			InstallValidator.InstallValidatorCustomAction.InstallValidator(null,
				@"Resources\TestDirectoryOne",
				message => { },
				(message, messageType) => { return true; },
				(message, messageType) => { errorOccurred = true; }
				);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void InstallValidator_WithEmptyValidatorDefinition_FlagsAnError()
		{
			var errorOccurred = false;

			InstallValidator.InstallValidatorCustomAction.InstallValidator("",
				@"Resources\TestDirectoryOne",
				message => { },
				(message, messageType) => { return true; },
				(message, messageType) => { errorOccurred = true; }
			);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void InstallValidator_WithNullValidatorPath_FlagsAnError()
		{
			var errorOccurred = false;

			InstallValidator.InstallValidatorCustomAction.InstallValidator("SomeDefinitionId",
				null,
				message => { },
				(message, messageType) => { return true; },
				(message, messageType) => { errorOccurred = true; }
			);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void InstallValidator_WithEmptyValidatorPath_FlagsAnError()
		{
			var errorOccurred = false;

			InstallValidator.InstallValidatorCustomAction.InstallValidator("SomeDefinitionId",
				"",
				message => { },
				(message, messageType) => { return true; },
				(message, messageType) => { errorOccurred = true; }
			);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no logging callback is supplied")]
		public void InstallValidator_WithNoLoggingCallback_Throws()
		{
			InstallValidator.InstallValidatorCustomAction.InstallValidator("SomeDefinitionId",
				@"Resources\TestDirectoryOne",
				null,
				(message, messageType) => { return true; },
				(message, messageType) => { }
			);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no user message callback is supplied")]
		public void InstallValidator_WithNoUserMessageCallback_Throws()
		{
			InstallValidator.InstallValidatorCustomAction.InstallValidator("SomeDefinitionId",
				@"Resources\TestDirectoryOne",
				message => { },
				null,
				(message, messageType) => { }
			);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no error callback is supplied")]
		public void InstallValidator_WithNoErrorCallback_Throws()
		{
			InstallValidator.InstallValidatorCustomAction.InstallValidator("SomeDefinitionId",
				@"Resources\TestDirectoryOne",
				message => { },
				(message, messageType) => { return true; },
				null
			);
		}
	}
}
