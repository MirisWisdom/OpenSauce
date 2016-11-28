/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using CPUQuery.QueryFunctions;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.CPUQueryUnitTests.QueryFunctions
{
	[TestClass]
	public class IsProcessorFeaturePresentQueryTests
	{
		[TestMethod]
		public void Execute_WithAValidProcessorFeatureString_ReturnsTrueOrFalse()
		{
			var result = IsProcessorFeaturePresentQuery.Execute("InstructionsXMMIAvailable");

			Assert.IsTrue((result == "True") || (result == "False"), "Failed to return a true/false value when using a valud processor feature type");
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw an exception when passed an unknown processor feature string")]
		public void Execute_WithAnUnknownProcessorFeatureString_Throws()
		{
			IsProcessorFeaturePresentQuery.Execute("UnknownProcessorFeature");
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw an exception when passed an empty processor feature string")]
		public void Execute_WithAnEmptyProcessorFeatureString_Throws()
		{
			IsProcessorFeaturePresentQuery.Execute("");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw an exception when passed a null processor feature string")]
		public void Execute_WithANullProcessorFeatureString_Throws()
		{
			IsProcessorFeaturePresentQuery.Execute(null);
		}
	}
}
