/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using BlamLib;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	public struct ExtractorFileType
	{
		public string TypeName;
		public string TypeExtension;
		public string TypeFilter;
	}

	/// <summary>   Interface for an extractor factory. </summary>
	public interface IExtractorFactory
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Registers extractors made for gameVersion. </summary>
		///
		/// <param name="gameVersion">  The game version. </param>
		void RegisterExtractors(BlamVersion gameVersion);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the extractors for a type according to the provided extension. </summary>
		///
		/// <param name="extension">	The extension. </param>
		///
		/// <returns>	The extensions extractors. </returns>
		IEnumerable<IExtractor> GetExtractors(string extension);
	
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets an extraction job. </summary>
		///
		/// <param name="extension">	 	The files extension. </param>
		/// <param name="extractionData">	Information describing the extraction. </param>
		///
		/// <returns>	The extraction job. </returns>
		IExtractionJob GetExtractionJob(string extension, IExtractionData extractionData);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the file types in the factory. </summary>
		///
		/// <returns>
		///     An enumerator that allows foreach to be used to process the file types in this factory.
		/// </returns>
		IEnumerable<ExtractorFileType> GetFileTypes();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets file type information. </summary>
		///
		/// <param name="extension">	The extension to get info for. </param>
		///
		/// <returns>	The file type information. </returns>
		ExtractorFileType GetFileType(string extension);
	}
}
