/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using CPUQuery;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.CPUQueryUnitTests
{
	[TestClass]
	public class CPUQueryTests
	{
		#region IsProcessorFeaturePresent
		[TestMethod]
		public void CPUQuery_WithAValidIsProcessorFeaturePresentQuery_SetsTheSpecifiedProperty()
		{
			var propertySet = false;
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=InstructionsMMXAvailable;OutputProp=MMX_SUPPORTED_PROP",
				(property, value) =>
				{
					propertySet = property == "MMX_SUPPORTED_PROP";
				},
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(propertySet && !errorOccurred, "Failed to execute a CPUQuery with a valid input string");
		}

		[TestMethod]
		public void CPUQuery_IsProcessorFeaturePresentQueryWithNoFeatureType_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent;OutputProp=MMX_SUPPORTED_PROP",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no feature type is defined");
		}

		[TestMethod]
		public void CPUQuery_IsProcessorFeaturePresentQueryWithEmptyFeatureType_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=;OutputProp=MMX_SUPPORTED_PROP",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when an empty feature type is defined");
		}

		[TestMethod]
		public void CPUQuery_IsProcessorFeaturePresentQueryWithInvalidFeatureType_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=UNKNOWN_FEATURE_TYPE;OutputProp=MMX_SUPPORTED_PROP",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when an invalid feature type is defined");
		}
		#endregion

		#region Argument Validation
		[TestMethod]
		public void CPUQuery_WithAValidQueryAndOutputPropDefinedFirst_SetsTheSpecifiedProperty()
		{
			var propertySet = false;
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("OutputProp=MMX_SUPPORTED_PROP;IsProcessorFeaturePresent=InstructionsMMXAvailable",
				(property, value) =>
				{
					propertySet = property == "MMX_SUPPORTED_PROP";
				},
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(propertySet && !errorOccurred, "Failed to execute a CPUQuery with a valid input string");
		}

		[TestMethod]
		public void CPUQuery_WithAnEmptyString_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no data string is provided");
		}

		[TestMethod]
		public void CPUQuery_WithANullString_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery(null,
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error a null data string is provided");
		}

		[TestMethod]
		public void CPUQuery_WithNoQueryType_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("OutputProp=MMX_SUPPORTED_PROP",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no query type is provided");
		}

		[TestMethod]
		public void CPUQuery_WithNoOutputProperty_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=InstructionsMMXAvailable",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no output property is provided");
		}

		[TestMethod]
		public void CPUQuery_WithNoOutputPropertyValue_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=InstructionsMMXAvailable;OutputProp",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no output property value is provided");
		}

		[TestMethod]
		public void CPUQuery_WithEmptyOutputPropertyValue_FlagsAnError()
		{
			var errorOccurred = false;

			CPUQueryCustomAction.CPUQuery("IsProcessorFeaturePresent=InstructionsMMXAvailable;OutputProp=",
				(property, value) => { },
				(value) => { },
				(value) =>
				{
					errorOccurred = true;
				});

			Assert.IsTrue(errorOccurred, "Failed to flag an error when no output property value is provided");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no set property callback is supplied")]
		public void CPUQuery_WithNoSetPropertyCallback_Throws()
		{
			CPUQueryCustomAction.CPUQuery("OutputProp=MMX_SUPPORTED_PROP;IsProcessorFeaturePresent=InstructionsMMXAvailable",
				null,
				(value) => { },
				(value) => { });
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no logging callback is supplied")]
		public void CPUQuery_WithNoLoggingCallback_Throws()
		{
			CPUQueryCustomAction.CPUQuery("OutputProp=MMX_SUPPORTED_PROP;IsProcessorFeaturePresent=InstructionsMMXAvailable",
				(property, value) => { },
				null,
				(value) => { });
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw when no error callback is supplied")]
		public void CPUQuery_WithNoErrorCallback_Throws()
		{
			CPUQueryCustomAction.CPUQuery("OutputProp=MMX_SUPPORTED_PROP;IsProcessorFeaturePresent=InstructionsMMXAvailable",
				(property, value) => { },
				(value) => { },
				null);
		}
		#endregion
	}
}