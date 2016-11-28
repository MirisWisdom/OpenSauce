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

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Scenario
{
	public partial class ExtractorScenarioUI : UserControl
	{
		public ExtractorScenarioUI()
		{
			InitializeComponent();
		}

		public bool ExportScenery
		{
			get { return mExtractionOptionFlags.GetItemChecked(0); }
		}

		public bool ExportDeviceMachines
		{
			get { return mExtractionOptionFlags.GetItemChecked(1); }
		}

		public bool ExportControls
		{
			get { return mExtractionOptionFlags.GetItemChecked(2); }
		}

		public bool ExportLightFixtures
		{
			get { return mExtractionOptionFlags.GetItemChecked(3); }
		}

		public bool ExportSoundScenery
		{
			get { return mExtractionOptionFlags.GetItemChecked(4); }
		}

		public bool ExportObjectMeshes
		{
			get { return mExtractionOptionFlags.GetItemChecked(5); }
		}
	}
}
