/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace OpenSauceIDE.LightmapImporter.Settings
{
	/// <summary>	Interface for lightmap importer settings. </summary>
	public interface ILightmapImporterSettings
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the pathname of the tags folder. </summary>
		///
		/// <value>	The pathname of the tags folder. </value>
		string TagsFolder { get; set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the pathname of the data folder. </summary>
		///
		/// <value>	The pathname of the data folder. </value>
		string DataFolder { get; set; }
	}
}
