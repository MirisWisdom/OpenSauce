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
	public class Test_FilePatcher
	{
		class PatchDefinition
		{
			public string DefinitionName { get; private set; }
			public string FileName { get; private set; }

			public PatchDefinition(string name, string file)
			{
				DefinitionName = name;
				FileName = file;
			}
		};

		class PatchSet
		{
			public string FilesRootDirectory { get; private set; }
			public string OutputDirectory { get; private set; }
			public List<PatchDefinition> PatchDefinitions;

			public PatchSet(string files_root_dir, string output_dir)
			{
				FilesRootDirectory = files_root_dir;
				OutputDirectory = output_dir;
				PatchDefinitions = new List<PatchDefinition>();
			}
		}

		List<PatchSet> PatchSets = new List<PatchSet>();

		public Test_FilePatcher()
		{
			string DocumentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

			string hce_install_directory = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo CE", "EXE Path", "") as string;
			string hce_output_directory = Path.Combine(DocumentsPath, "OpenSauceUnitTest\\Halo1_Installer\\FilePatcher\\HCE");
			if (hce_install_directory == null || hce_install_directory.Length == 0)
				throw new Exception("HCE Install path registry entry is missing");

			PatchSet hce_patch_set = new PatchSet(hce_install_directory, hce_output_directory);
			hce_patch_set.PatchDefinitions.Add(new PatchDefinition("Halo1_HCE_Dedi", "haloceded.exe"));

			string hek_install_directory = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo HEK", "EXE Path", "") as string;
			string hek_output_directory = Path.Combine(DocumentsPath, "OpenSauceUnitTest\\Halo1_Installer\\FilePatcher\\HEK");
			if (hek_install_directory == null || hek_install_directory.Length == 0)
				throw new Exception("HEK Install path registry entry is missing");

			PatchSet hek_patch_set = new PatchSet(hek_install_directory, hek_output_directory);
			hek_patch_set.PatchDefinitions.Add(new PatchDefinition("Halo1_HEK_Guerilla", "guerilla.exe"));
			hek_patch_set.PatchDefinitions.Add(new PatchDefinition("Halo1_HEK_Sapien", "sapien.exe"));
			hek_patch_set.PatchDefinitions.Add(new PatchDefinition("Halo1_HEK_Tool", "tool.exe"));

			PatchSets.Add(hce_patch_set);
			PatchSets.Add(hek_patch_set);
		}

		private TestContext testContextInstance;

		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		public TestContext TestContext
		{
			get
			{
				return testContextInstance;
			}
			set
			{
				testContextInstance = value;
			}
		}

		#region Additional test attributes
		//
		// You can use the following additional attributes as you write your tests:
		//
		// Use ClassInitialize to run code before running the first test in the class
		// [ClassInitialize()]
		// public static void MyClassInitialize(TestContext testContext) { }
		//
		// Use ClassCleanup to run code after all tests in a class have run
		// [ClassCleanup()]
		// public static void MyClassCleanup() { }
		//
		// Use TestInitialize to run code before running each test 
		// [TestInitialize()]
		// public void MyTestInitialize() { }
		//
		// Use TestCleanup to run code after each test has run
		// [TestCleanup()]
		// public void MyTestCleanup() { }
		//
		#endregion

		[TestMethod]
		public void TestFilePatcher()
		{
			foreach (var patch_set in PatchSets)
			{
				foreach(var definition in patch_set.PatchDefinitions)
				{
					FilePatcher.CustomActions.ApplyPatch(definition.DefinitionName,
						Path.Combine(patch_set.FilesRootDirectory, definition.FileName),
						patch_set.OutputDirectory);
				}
			}
		}
	}
}
