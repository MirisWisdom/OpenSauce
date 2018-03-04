using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.IO;

namespace OpenSauceIDE.Test
{
	[TestClass]
	public class LightmapImporterTest
	{
		[TestMethod]
		public void LightmapImporterImport()
		{
			var importer = new LightmapImporter.Importer.LightmapImporter();

			var result = importer.ImportTexcoords(@"D:\Program Files (x86)\Microsoft Games\Halo Custom Edition\tags"
				, @"levels\test\beavercreek\beavercreek"
				, @"D:\Program Files (x86)\Microsoft Games\Halo Custom Edition\data\levels\test\beavercreek\beavercreek.dae");

			Assert.IsTrue(result, "Failed to import texture coordinates");
		}
	}
}
