/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.SetPropertiesUnitTests
{
	public class SetPropertiesTests
	{
		[TestMethod]
		public void SetProperties_WithNullActionData_FlagsAnError()
		{
			var errorOccurred = false;

			SetProperties.SetPropertiesCustomAction.SetProperties(null,
				(value) => { },
				(property, value) => { },
				(property) => { return ""; },
				(value) => { errorOccurred = true; }
			);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod]
		public void SetProperties_WithEmptyActionData_FlagsAnError()
		{
			var errorOccurred = false;

			SetProperties.SetPropertiesCustomAction.SetProperties("",
				(value) => { },
				(property, value) => { },
				(property) => { return ""; },
				(value) => { errorOccurred = true; }
			);

			Assert.IsTrue(errorOccurred, "An error was not flagged when no action data was supplied");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no logging callback is supplied")]
		public void SetProperties_WithNoLoggingCallback_Throws()
		{
			SetProperties.SetPropertiesCustomAction.SetProperties("DefinitionID",
				null,
				(property, value) => { },
				(property) => { return ""; },
				(value) => { }
			);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no set property callback is supplied")]
		public void SetProperties_WithNoSetPropertyCallback_Throws()
		{
			SetProperties.SetPropertiesCustomAction.SetProperties("DefinitionID",
				(value) => { },
				null,
				(property) => { return ""; },
				(value) => { }
			);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no get property callback is supplied")]
		public void SetProperties_WithNoGetPropertyCallback_Throws()
		{
			SetProperties.SetPropertiesCustomAction.SetProperties("DefinitionID",
				(value) => { },
				(property, value) => { },
				null,
				(value) => { }
			);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no error callback is supplied")]
		public void SetProperties_WithNoErrorCallback_Throws()
		{
			SetProperties.SetPropertiesCustomAction.SetProperties("DefinitionID",
				(value) => { },
				(property, value) => { },
				(property) => { return ""; },
				null
			);
		}
	}
}
