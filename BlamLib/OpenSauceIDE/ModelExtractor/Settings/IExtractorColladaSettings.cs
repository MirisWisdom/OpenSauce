/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Bitmaps;

namespace OpenSauceIDE.ModelExtractor.Settings
{
	/// <summary>	Interface for extractor collada settings. </summary>
	public interface IExtractorColladaSettings
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether models should be overwritten. </summary>
		///
		/// <value>	true if overwrite, false if not. </value>
		bool Overwrite { get; set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets the collada bitmap format. </summary>
		///
		/// <value>	The bitmap format. </value>
		AssetFormat BitmapFormat { get; set; }
	}
}
