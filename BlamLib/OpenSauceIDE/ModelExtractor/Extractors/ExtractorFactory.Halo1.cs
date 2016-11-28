using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   An extractor factory. </summary>
	public partial class ExtractorFactory
		: IExtractorFactory
	{
		private void AddTypesHalo1()
		{
			var modelExtractor = AddType("GbxModel"
				, "gbxmodel"
				, typeof(Halo1.Gbxmodel.ExtractorGbxmodel)
				, typeof(Halo1.Gbxmodel.ExtractionJobGbxmodel));

			AddType("Structure BSP"
				, "scenario_structure_bsp"
				, typeof(Halo1.StructureBSP.ExtractorStructureBSP)
				, typeof(Halo1.StructureBSP.ExtractionJobStructureBSP));

			AddType("Scenario"
				, "scenario"
				, typeof(Halo1.Scenario.ExtractorScenario)
				, typeof(Halo1.Scenario.ExtractionJobScenario)
				, modelExtractor);
		}
	}
}
