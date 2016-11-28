/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region scenario
	partial class scenario_group
	{
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			//PredictedResources.DeleteAll();
			//EditorScenarioData.Delete();
			LevelData.DeleteAll();
			//SharedReferences.DeleteAll();
			//SimulationDefinitionTable.DeleteAll();

			return true;
		}
	};
	#endregion
}