/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;
using Microsoft.Win32;
using FilePatcher;

namespace InstallerUnitTest
{
	/// <summary>
	/// Tests the installers patching definitions
	/// </summary>
	[TestClass]
	public class FilePatcherTests
	{
		[TestMethod]
		public void FilePatcher_WithNullActionData_FlagsAnError()
		{
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher(null, message => { }, (message, displayError) => { errorOccurred = true; });

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void FilePatcher_WithEmptyActionData_FlagsAnError()
		{
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher("", message => { }, (message, displayError) => { errorOccurred = true; });

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void FilePatcher_WithIncorrectActionData_FlagsAnError()
		{
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher("ID;File", message => { }, (message, displayError) => { errorOccurred = true; });

			Assert.IsTrue(errorOccurred, "An error was not flagged when invalid action data was supplied");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no logging callback is supplied")]
		public void FilePatcher_WithNoLoggingCallback_Throws()
		{
			FilePatcher.FilePatcherCustomAction.FilePatcher("ID;File;Output",
				null,
				(message, displayError) => { });
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no error callback is supplied")]
		public void FilePatcher_WithNoErrorCallback_Throws()
		{
			FilePatcher.FilePatcherCustomAction.FilePatcher("ID;File;Output",
				message => { },
				null);
		}
	}
}
