/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Xml;
using FilePatcher.Patcher;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace InstallerUnitTest.FilePatcherUnitTests.Patcher
{
	[TestClass]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.txt")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidString.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidUInt32.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidUInt16.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidByte.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidBytes.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexUInt32.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexUInt16.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexByte.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexBytes.xml")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.OutOfBoundsString.xml")]
	public class PatchApplierTests
	{
		public TestContext TestContext { get; set; }

		private bool TryApplyPatch(string patchFile, string targetFile)
		{
			using (var xmlReader = XmlReader.Create(patchFile))
			{
				var patcher = new PatchApplier();
				var definition = patcher.DeserializeDefinition(xmlReader);

				return patcher.Patch(definition, targetFile, TestContext.DeploymentDirectory);
			}
		}

		#region Valid Patch
		[TestMethod]
		public void Patch_WithAValidStringPatch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.ValidString.xml", "DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidUInt32Patch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.ValidUInt32.xml", "DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidUInt16Patch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.ValidUInt16.xml", "DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidBytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.ValidByte.xml", "DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidBytesPatch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.ValidBytes.xml", "DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}
		#endregion

		#region Invalid Patch
		[TestMethod]
		public void Patch_WithAnOutOfBoundsPatch_ReturnsFalse()
		{
			var result = TryApplyPatch("DummyFileTwo.OutOfBoundsString.xml", "DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAUInt32Patch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.InvalidHexUInt32.xml", "DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAUInt16Patch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.InvalidHexUInt16.xml", "DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForABytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.InvalidHexByte.xml", "DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAMultiBytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch("DummyFileTwo.InvalidHexBytes.xml", "DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}
		#endregion

		#region Invalid Arguments
		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw an exception when no definition was supplied")]
		public void Patch_WithANullDefinition_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch((PatchDefinition)null, "DummyFileTwo.txt", TestContext.DeploymentDirectory);
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw an exception when no target file was supplied")]
		public void Patch_WithANullTargetFile_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch(new PatchDefinition(), null, TestContext.DeploymentDirectory);
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw an exception when no target file was supplied")]
		public void Patch_WithAnEmptyTargetFile_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch(new PatchDefinition(), "", TestContext.DeploymentDirectory);
		}

		[TestMethod]
		public void Patch_WithAMissingTargetFile_ReturnsFalse()
		{
			var patcher = new PatchApplier();

			var result = patcher.Patch(new PatchDefinition(), "MissingFile.txt", TestContext.DeploymentDirectory);

			Assert.IsFalse(result, "Returned true when attempting to patch a missing file");
		}

		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw an exception when no output directory was supplied")]
		public void Patch_WithANullOutputDirectory_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch(new PatchDefinition(), "DummyFileTwo.txt", null);
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw an exception when no output directory was supplied")]
		public void Patch_WithAnEmptyOutputDirectory_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch(new PatchDefinition(), "DummyFileTwo.txt", "");
		}
		#endregion
	}
}
