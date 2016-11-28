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
using OpenSauceIDE.ModelExtractor.Extractors;

namespace OpenSauceIDE.ModelExtractor.UI
{
	public partial class JobListControl : UserControl
	{
		private class JobEntry
		{
			public string JobID { get; set; }
			public string JobName { get; set; }
			public ExtractionStateEnum JobState { get; set; }
			public string DisplayString { get; set; }
		}

		private IModelExtractorController mController;
		private BindingList<JobEntry> mJobItems = new BindingList<JobEntry>();

		public JobListControl()
		{
			InitializeComponent();

			mJobListBox.DataSource = mJobItems;
			mJobListBox.DisplayMember = "DisplayString";
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attaches the ui to the given controller. </summary>
		///
		/// <param name="controller">	The controller. </param>
		public void Attach(IModelExtractorController controller)
		{
			mController = controller;

			// Attach to controller state changes
			mController.StateChanged += ControllerStateChanged;
			SetState(ModelExtractorStateEnum.ExtractorClosed);

			// Attach to extraction manager events
			var extractionManager = mController.GetExtractionManager();

			extractionManager.JobAdded +=
				(sender, args) =>
				{
					if (InvokeRequired)
					{
						BeginInvoke(new Action(
							() => 
							{
								AddJob(args.JobID, args.JobName);
								SetJobState(args.JobID, args.JobState, "");
							})
						);
					}
					else
					{
						AddJob(args.JobID, args.JobName);
						SetJobState(args.JobID, args.JobState, "");
					}
				};

			extractionManager.JobRemoved +=
				(sender, args) =>
				{
					if (InvokeRequired)
					{
						BeginInvoke(new Action(
							() =>
							{
								RemoveJob(args.JobID);
							})
						);
					}
					else
					{
						RemoveJob(args.JobID);
					}
				};

			extractionManager.JobStateChanged +=
				(sender, args) =>
				{
					if (InvokeRequired)
					{
						BeginInvoke(new Action(
							() =>
							{
								SetJobState(args.JobID, args.JobState, args.ResultString);
							})
						);
					}
					else
					{
						SetJobState(args.JobID, args.JobState, args.ResultString);
					}
				};
		}

		#region Job Management
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a job to the ui. </summary>
		///
		/// <param name="jobID">  	Identifier for the job. </param>
		/// <param name="jobName">	Name of the job. </param>
		private void AddJob(string jobID, string jobName)
		{
			mJobItems.Add(new JobEntry()
			{
				JobID = jobID,
				JobName = jobName,
				JobState = ExtractionStateEnum.Queued
			});
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Removes the job from the ui. </summary>
		///
		/// <param name="jobID">	Identifier for the job. </param>
		private void RemoveJob(string jobID)
		{
			var item = mJobItems.Single((entry) => entry.JobID == jobID);

			mJobItems.Remove(item);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets a jobs state in the ui. </summary>
		///
		/// <param name="jobID">			Identifier for the job. </param>
		/// <param name="state">			The state. </param>
		/// <param name="resultMessage">	Message describing the job's result. </param>
		private void SetJobState(string jobID, ExtractionStateEnum state, string resultMessage)
		{
			// Get the job item
			var item = mJobItems.Single((entry) => entry.JobID == jobID);
			item.JobState = state;

			switch(state)
			{
				case ExtractionStateEnum.Queued:
					item.DisplayString = String.Format("Queued\t\t- {0}", item.JobName);
					break;
				case ExtractionStateEnum.InProgress:
					item.DisplayString = String.Format("In Progress\t- {0}", item.JobName);
					break;
				case ExtractionStateEnum.Succeeded:
					item.DisplayString = String.Format("Succeeded\t- {0}", item.JobName);
					break;
				case ExtractionStateEnum.Failed:
					item.DisplayString = String.Format("Failed\t\t- {0}", item.JobName);
					break;
			}

			// Sort the job list
			mJobItems.RaiseListChangedEvents = false;
			var sortedItems = mJobItems.ToList();
			sortedItems.Sort(
				(left, right) =>
				{
					if ((int)left.JobState < (int)right.JobState)
					{
						return -1;
					}
					if ((int)left.JobState > (int)right.JobState)
					{
						return 1;
					}
					return 0;
				}
			);

			mJobItems.Clear();
			sortedItems.ForEach((entry) => mJobItems.Add(entry));
			mJobItems.RaiseListChangedEvents = true;

			// Update the lists data bindings
			mJobItems.ResetBindings();
		}
		#endregion

		#region Button Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Clears the completed jobs in the extraction manager. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void ClearCompletedJobs(object sender, EventArgs e)
		{
			mController.GetExtractionManager().ClearCompletedJobs();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Triggers an extraction. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void AddJobButtonClick(object sender, EventArgs e)
		{
			mController.Extract();
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
					mAddJobButton.Enabled = true;
					break;
				case ModelExtractorStateEnum.ExtractorClosed:
				case ModelExtractorStateEnum.ExtractorClosing:
				case ModelExtractorStateEnum.ExtractorInitialising:
					mAddJobButton.Enabled = false;
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
