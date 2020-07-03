/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Windows.Forms;
using BlamLib;

namespace OpenSauceIDE.LightmapImporter
{
	public partial class LightmapImporter : Form
	{
		#region Fields
		private ILightmapImporterController mController;
		#endregion
		
		#region Construction/Destruction
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="gameVersion">	The game version. </param>
		public LightmapImporter(BlamVersion gameVersion)
		{
			InitializeComponent();

			mController = new LightmapImporterController(gameVersion);

			mController.StateChanged += ControllerStateChanged;
			SetState(LightmapImporterStateEnum.ImporterReady);

			mTagsPathControl.BindPath("TagsFolder", mController.GetImporterSettings());
			mDataPathControl.BindPath("DataFolder", mController.GetImporterSettings());
			mMessageListControl.Attach(mController);
		}
		
		/// <summary>	Default constructor. </summary>
		public LightmapImporter()
			: this(BlamVersion.Halo1_PC)
		{ }
		#endregion

		#region State
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Updates the UI according to the controller's state. </summary>
		///
		/// <param name="state">    The controllers state. </param>
		private void SetState(LightmapImporterStateEnum state)
		{
			switch (state)
			{
				case LightmapImporterStateEnum.ImporterReady:
					mImportUVsButton.Enabled = true;
					break;
				case LightmapImporterStateEnum.ImporterImporting:
					mImportUVsButton.Enabled = false;
					break;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Handles state changes in the importer controller. </summary>
		///
		/// <remarks>   This can be called from other threads. </remarks>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        The LightmapImporterStateChangeArgs to process. </param>
		private void ControllerStateChanged(object sender, LightmapImporterStateChangeArgs e)
		{
			if (InvokeRequired)
			{
				BeginInvoke(new Action(() => SetState(e.State)));
			}
			else
			{
				SetState(e.State);
			}
		}
		#endregion

		private void ImportUVsClick(object sender, EventArgs e)
		{
			mController.ImportLightmap();
		}
	}
}
