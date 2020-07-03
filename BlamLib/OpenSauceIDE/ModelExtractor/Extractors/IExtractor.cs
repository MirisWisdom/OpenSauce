/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   Interface for a model extractor. </summary>
	public interface IExtractor
	{
		void Initialise();
		void Dispose();

		System.Windows.Forms.Control GetFormControl();

		object GetExtractionData();
	}
}
