/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BlamLib;
using OpenSauceIDE.ModelExtractor.Extractors;

namespace OpenSauceIDE.ModelExtractor
{
	public partial class ModelExtractor : Form
	{
		#region Fields
		private IModelExtractorController mController;
		private List<string> mExtensions = new List<string>();
		#endregion

		#region Construction/Destruction
		/// <summary>   Default constructor. </summary>
		public ModelExtractor(BlamVersion gameVersion)
		{
			InitializeComponent();

			// Force controls added to the control panel to dock to the top
			mControlPanel.ControlAdded +=
				(sender, e) =>
				{
					e.Control.Dock = DockStyle.Top;
				};

			// Create the controller
			mController = new ModelExtractorController(gameVersion);
			
			// Attach to controller state changes
			mController.StateChanged += ControllerStateChanged;
			SetState(ModelExtractorStateEnum.ExtractorClosed);

			// Attach child controls to controller
			mTagsPathControl.BindPath("TagsFolder", mController.GetExtractorSettings());
			mDataPathControl.BindPath("DataFolder", mController.GetExtractorSettings());
			mMessageList.Attach(mController);
			mJobListControl.Attach(mController);

			// Populate the model type list
			foreach (var type in mController.GetExtractorFactory().GetFileTypes())
			{
				mModelTypeComboBox.Items.Add(String.Format("{0} (*.{1})", type.TypeName, type.TypeExtension));
				mExtensions.Add(type.TypeExtension);
			}

			// Set the initial extractor type
			mModelTypeComboBox.SelectedIndex = 0;
			mModelTypeComboBox.SelectedIndexChanged += SelectedExtensionChanged;
			SetExtractor();
		}

		public ModelExtractor()
			: this(BlamVersion.Halo1_PC)
		{ }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Event called when the form is closing. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        Form closing event information. </param>
		private void ModelExtractorClosed(object sender, FormClosedEventArgs e)
		{
			mController.Dispose();
		}
		#endregion

		#region Control Panel Setup
		/// <summary>   Sets the extractor controls. </summary>
		private void SetControls()
		{
			mControlPanel.Controls.Clear();

			var extractorControls = mController.GetControls();
			foreach (var control in extractorControls.Reverse())
			{
				mControlPanel.Controls.Add(control);
			}
		}
		#endregion

		#region Extension Selection
		/// <summary>   Sets the extractor to the currently selected. </summary>
		private void SetExtractor()
		{
			mController.CloseExtractor();
			mController.OpenExtractor(mExtensions[mModelTypeComboBox.SelectedIndex]);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Changes the current extractor to the selected one. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        Event information. </param>
		private void SelectedExtensionChanged(object sender, EventArgs e)
		{
			SetExtractor();
		}
		#endregion

		#region State
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Updates the UI according to the controller's state. </summary>
		///
		/// <param name="state">    The controllers state. </param>
		private void SetState(ModelExtractorStateEnum state)
		{
			switch (state)
			{
				case ModelExtractorStateEnum.ExtractorReady:
				case ModelExtractorStateEnum.ExtractorClosing:
					SetControls();
					break;
				case ModelExtractorStateEnum.ExtractorClosed:
				case ModelExtractorStateEnum.ExtractorInitialising:
					break;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Handles state changes in the extractor controller. </summary>
		///
		/// <remarks>   This can be called from other threads. </remarks>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        The ModelExtractorStateChangeArgs to process. </param>
		private void ControllerStateChanged(object sender, ModelExtractorStateChangeArgs e)
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
	}
}
