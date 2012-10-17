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
using Microsoft.Win32;

namespace InstallerUnitTest
{
	/// <summary>
	/// Tests the installers validation definitions
	/// </summary>
	[TestClass]
	public class Test_InstallValidator
	{
		class ValidatorDefinition
		{
			public string InstallDefinition { get; private set; }
			public string InstallPath { get; private set; }

			public ValidatorDefinition(string definition, string path)
			{
				InstallDefinition = definition;
				InstallPath = path;
			}
		}

		List<ValidatorDefinition> ValidatorDefinitions = new List<ValidatorDefinition>();

		public Test_InstallValidator()
		{
			string hce_install_directory = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo CE", "EXE Path", "") as string;
			if (hce_install_directory == null || hce_install_directory.Length == 0)
				throw new Exception("HCE Install path registry entry is missing");

			ValidatorDefinition hce_validator = new ValidatorDefinition("Halo1_CE", hce_install_directory);

			string hek_install_directory = Registry.GetValue("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Microsoft Games\\Halo HEK", "EXE Path", "") as string;
			if (hek_install_directory == null || hek_install_directory.Length == 0)
				throw new Exception("HEK Install path registry entry is missing");

			ValidatorDefinition hek_validator = new ValidatorDefinition("Halo1_HEK", hek_install_directory);

			ValidatorDefinitions.Add(hce_validator);
			ValidatorDefinitions.Add(hek_validator);
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
		public void TestInstallValidator()
		{
			foreach (var definition in ValidatorDefinitions)
			{
				InstallValidator.CustomActions.ValidateInstallPath(definition.InstallDefinition, definition.InstallPath);
			}
		}
	}
}
