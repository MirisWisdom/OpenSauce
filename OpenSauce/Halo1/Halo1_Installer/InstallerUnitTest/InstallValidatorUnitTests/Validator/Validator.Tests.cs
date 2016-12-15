/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Xml;
using InstallValidator.Validator;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.InstallValidatorUnitTests.Validator
{
	[TestClass]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\TestFileOne.txt", @"Resources\TestDirectoryOne")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\TestFileTwo.txt", @"Resources\TestDirectoryOne\TestDirectoryTwo")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateValidDirectory.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMissingDirectoryError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMissingDirectoryWarning.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidatePresentDirectoryError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidatePresentDirectoryWarning.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMissingFileError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMissingFileWarning.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidatePresentFileError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidatePresentFileWarning.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMatchingFileError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMatchingFileWarning.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMismatchedFileError.xml", @"Definitions")]
	[DeploymentItem(@"InstallValidatorUnitTests\Resources\ValidateMismatchedFileWarning.xml", @"Definitions")]
	public class ValidatorTests
	{
		private enum ExpectedResult
		{
			Valid,
			Warning,
			Error
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tests the validation of a directory. </summary>
		///
		/// <exception cref="NotSupportedException">
		/// 	Thrown when the requested operation is not supported.
		/// </exception>
		///
		/// <param name="definitionFile">	The validator definition file. </param>
		/// <param name="path">			 	Full pathname of the directory to validate. </param>
		/// <param name="expectedResult">	The expected validation result. </param>
		///
		/// <returns>	true if the test passes, false if the test fails. </returns>
		private bool TestValidateDirectory(string definitionFile, string path, ExpectedResult expectedResult)
		{
			using (var xmlReader = XmlReader.Create(definitionFile))
			{
				var definition = PathValidator.DeserializeDefinition(xmlReader);
				var validator = new PathValidator();

				validator.Validate(definition, path);

				switch (expectedResult)
				{
					case ExpectedResult.Valid:
						return (validator.ErrorMessages.Count == 0) && (validator.WarningMessages.Count == 0);
					case ExpectedResult.Warning:
						return (validator.ErrorMessages.Count == 0) && (validator.WarningMessages.Count > 0);
					case ExpectedResult.Error:
						return (validator.ErrorMessages.Count > 0) && (validator.WarningMessages.Count == 0);
					default:
						throw new NotSupportedException("Unknown result type");
				}
			}
		}

		[TestMethod]
		public void Validate_WithAValidDirectory_HasNoErrorsOrWarnings()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateValidDirectory.xml", @"Resources\TestDirectoryOne", ExpectedResult.Valid);

			Assert.IsTrue(result, "Validation failed for a directory known to be valid");
		}

		#region Directories
		[TestMethod]
		public void Validate_WithAPresentDirectoryWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidatePresentDirectoryWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAPresentDirectoryError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidatePresentDirectoryError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}

		[TestMethod]
		public void Validate_WithAMissingDirectoryWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMissingDirectoryWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAMissingDirectoryError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMissingDirectoryError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}
		#endregion

		#region Files
		[TestMethod]
		public void Validate_WithAPresentFileWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidatePresentFileWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAPresentFileError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidatePresentFileError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}

		[TestMethod]
		public void Validate_WithAMissingFileWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMissingFileWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAMissingFileError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMissingFileError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}

		[TestMethod]
		public void Validate_WithAMatchingFileWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMatchingFileWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAMatchingFileError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMatchingFileError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}

		[TestMethod]
		public void Validate_WithAMismatchedFileWarning_ProducesAWarning()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMismatchedFileWarning.xml", @"Resources\TestDirectoryOne", ExpectedResult.Warning);

			Assert.IsTrue(result, "Validation failed for a directory known to contain a warning");
		}

		[TestMethod]
		public void Validate_WithAMismatchedFileError_ProducesAnError()
		{
			var result = TestValidateDirectory(@"Definitions\ValidateMismatchedFileError.xml", @"Resources\TestDirectoryOne", ExpectedResult.Error);

			Assert.IsTrue(result, "Validation failed for a directory known to contain an error");
		}
		#endregion
	}
}
