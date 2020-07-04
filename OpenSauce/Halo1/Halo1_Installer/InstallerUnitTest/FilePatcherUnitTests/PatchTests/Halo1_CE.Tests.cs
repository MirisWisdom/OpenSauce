/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Microsoft.Win32;

namespace InstallerUnitTest.FilePatcherUnitTests.PatchTests
{
	[TestClass]
	[DeploymentItem(@"FilePatcherUnitTests\PatchTests\Resources\OS_haloceded.exe", "Resources")]
	public class Halo1_CE
	{
		public TestContext TestContext { get; set; }

		private string mHCEInstallationDir;
		private string mHEKInstallationDir;

		[TestInitialize]
		public void Initialize()
		{
			mHCEInstallationDir = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo CE", "EXE Path", "") as string;
			mHEKInstallationDir = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo HEK", "EXE Path", "") as string;
			
			Assert.IsTrue(Directory.Exists(mHCEInstallationDir), "Halo CE is not installed");
			Assert.IsTrue(Directory.Exists(mHEKInstallationDir), "Halo Editing Kit is not installed");
		}

		[TestMethod]
		public void FilePatcher_WithHCEDediPatch_PatchesTheExe()
		{
			var errorOccurred = false;
			var exePath = Path.Combine(mHCEInstallationDir, "haloceded.exe");

			FilePatcher.FilePatcherCustomAction.FilePatcher(String.Join(";", "Halo1_HCE_Dedi", exePath, TestContext.TestRunResultsDirectory),
				(value) => { },
				(value, showMessage) =>
				{
					errorOccurred = true;
				}
			);

			Assert.IsFalse(errorOccurred, "Failed to patch the HCE dedicated server");
		}

		[TestMethod]
		public void FilePatcher_WithHEKGuerillaPatch_PatchesTheExe()
		{
			var filePath = Path.Combine(mHEKInstallationDir, "guerilla.exe");
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher(String.Join(";", "Halo1_HEK_Guerilla", filePath, TestContext.TestRunResultsDirectory),
				(value) => { },
				(value, showMessage) =>
				{
					errorOccurred = true;
				}
			);

			Assert.IsFalse(errorOccurred, "Failed to patch HEK gueriila");
		}

		[TestMethod]
		public void FilePatcher_WithHEKSapienPatch_PatchesTheExe()
		{
			var filePath = Path.Combine(mHEKInstallationDir, "sapien.exe");
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher(String.Join(";", "Halo1_HEK_Sapien", filePath, TestContext.TestRunResultsDirectory),
				(value) => { },
				(value, showMessage) =>
				{
					errorOccurred = true;
				}
			);

			Assert.IsFalse(errorOccurred, "Failed to patch HEK sapien");
		}

		[TestMethod]
		public void FilePatcher_WithHEKToolPatch_PatchesTheExe()
		{
			var filePath = Path.Combine(mHEKInstallationDir, "tool.exe");
			var errorOccurred = false;

			FilePatcher.FilePatcherCustomAction.FilePatcher(String.Join(";", "Halo1_HEK_Tool", filePath, TestContext.TestRunResultsDirectory),
				(value) => { },
				(value, showMessage) =>
				{
					errorOccurred = true;
				}
			);

			Assert.IsFalse(errorOccurred, "Failed to patch HEK tool");
		}
	}
}
