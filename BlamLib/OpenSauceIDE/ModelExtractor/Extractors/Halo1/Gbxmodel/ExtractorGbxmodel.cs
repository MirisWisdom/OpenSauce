/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Gbxmodel
{
	public class ExtractorGbxmodel
		: IExtractor
	{
		private ExtractorGbxmodelUI mExtractorUI;

		#region Initialise/Dispose
		/// <summary>   Initialises this object. </summary>
		public void Initialise()
		{
			mExtractorUI = new ExtractorGbxmodelUI();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		///     Performs application-defined tasks associated with freeing, releasing, or resetting
		///     unmanaged resources.
		/// </summary>
		public void Dispose()
		{
			mExtractorUI = null;
		}
		#endregion

		#region UI
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets form control. </summary>
		///
		/// <returns>   The form control. </returns>
		public System.Windows.Forms.Control GetFormControl()
		{
			return mExtractorUI;
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Create an extraction data set. </summary>
		///
		/// <returns>	The extractors extraction data. </returns>
		public object GetExtractionData()
		{
			return null;
		}
	}
}
