/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib;
using BlamLib.Render.COLLADA;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
	public class ExtractorStructureBSP
		: IExtractor
	{		
		#region Initialise/Dispose
		/// <summary>   Initialises this object. </summary>
		public void Initialise()
		{
			mExtractorUI = new ExtractorStructureBSPUI();
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
		private ExtractorStructureBSPUI mExtractorUI;

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
			return new ExtractionJobStructureBSPData()
			{
				ExportRenderMesh = mExtractorUI.ExportRenderMesh,
				ExportPortals = mExtractorUI.ExportPortals,
				ExportFogPlanes = mExtractorUI.ExportFogPlanes,
				ExportBSPMarkers = mExtractorUI.ExportBSPMarkers,
				ExportLightmapSizes = mExtractorUI.ExportLightmapSizes,
				LightmapSizeMultiplier = mExtractorUI.LightmapSizeMultiplier
			};
		}
	}
}
