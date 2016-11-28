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
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.txt", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidString.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidUInt32.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidUInt16.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidByte.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.ValidBytes.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexUInt32.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexUInt16.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexByte.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.InvalidHexBytes.xml", "Resources")]
	[DeploymentItem(@"FilePatcherUnitTests\Resources\DummyFileTwo.OutOfBoundsString.xml", "Resources")]
	public class PatchApplierTests
	{
		public TestContext TestContext { get; set; }

		private bool TryApplyPatch(string patchFile, string targetFile)
		{
			using (var xmlReader = XmlReader.Create(patchFile))
			{
				var definition = PatchApplier.DeserializeDefinition(xmlReader);
				var patcher = new PatchApplier();

				return patcher.Patch(definition, targetFile, TestContext.DeploymentDirectory);
			}
		}

		#region Valid Patch
		[TestMethod]
		public void Patch_WithAValidStringPatch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.ValidString.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidUInt32Patch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.ValidUInt32.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidUInt16Patch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.ValidUInt16.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidBytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.ValidByte.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}

		[TestMethod]
		public void Patch_WithAValidBytesPatch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.ValidBytes.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsTrue(result, "Failed to patch a file with a valid patch");
		}
		#endregion

		#region Invalid Patch
		[TestMethod]
		public void Patch_WithAnOutOfBoundsPatch_ReturnsFalse()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.OutOfBoundsString.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAUInt32Patch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.InvalidHexUInt32.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAUInt16Patch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.InvalidHexUInt16.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForABytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.InvalidHexByte.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}

		[TestMethod]
		public void Patch_WithInvalidHexForAMultiBytePatch_PatchesTheFile()
		{
			var result = TryApplyPatch(@"Resources\DummyFileTwo.InvalidHexBytes.xml", @"Resources\DummyFileTwo.txt");

			Assert.IsFalse(result, "Returned true for a patch that is invalid");
		}
		#endregion

		#region Invalid Arguments
		[TestMethod, ExpectedException(typeof(ArgumentNullException), "Did not throw an exception when no definition was supplied")]
		public void Patch_WithANullDefinition_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch((PatchDefinition)null, @"Resources\DummyFileTwo.txt", TestContext.DeploymentDirectory);
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

			patcher.Patch(new PatchDefinition(), @"Resources\DummyFileTwo.txt", null);
		}

		[TestMethod, ExpectedException(typeof(ArgumentException), "Did not throw an exception when no output directory was supplied")]
		public void Patch_WithAnEmptyOutputDirectory_Throws()
		{
			var patcher = new PatchApplier();

			patcher.Patch(new PatchDefinition(), @"Resources\DummyFileTwo.txt", "");
		}
		#endregion
	}
}
