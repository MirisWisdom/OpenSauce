/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
	public partial class ExtractorStructureBSPUI : UserControl
	{
		public ExtractorStructureBSPUI()
		{
			InitializeComponent();
		}

		public bool ExportRenderMesh
		{
			get { return mExtractionOptionFlags.GetItemChecked(0); }
		}

		public bool ExportPortals
		{
			get { return mExtractionOptionFlags.GetItemChecked(1); }
		}

		public bool ExportFogPlanes
		{
			get { return mExtractionOptionFlags.GetItemChecked(2); }
		}

		public bool ExportBSPMarkers
		{
			get { return mExtractionOptionFlags.GetItemChecked(3); }
		}

		public bool ExportLightmapSizes
		{
			get { return mExtractionOptionFlags.GetItemChecked(4); }
		}

		public int LightmapSizeMultiplier
		{
			get { return (int)mSizeMultiplierNumeric.Value; }
		}
	}
}
