/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using BlamLib.Messaging;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>   Manager for extraction jobs. </summary>
	public class ExtractionManager
		: IExtractionManager
	{
		private const int kMaxCompletedJobs = 150;
		private List<IExtractionJob> mJobList = new List<IExtractionJob>();
		private List<IExtractionJob> mCompletedJobList = new List<IExtractionJob>();

		#region Job Added Event
		public event EventHandler<JobAddedEventArgs> JobAdded;

		private void OnJobAdded(string jobID, string jobName, ExtractionStateEnum jobState)
		{
			var handler = JobAdded;

			if (handler != null)
			{
				handler(this,
					new JobAddedEventArgs()
					{
						JobID = jobID,
						JobName = jobName,
						JobState = jobState
					}
				);
			}
		}
		#endregion

		#region Job Removed Event
		public event EventHandler<JobRemovedEventArgs> JobRemoved;

		private void OnJobRemoved(string jobID)
		{
			var handler = JobRemoved;

			if (handler != null)
			{
				handler(this,
					new JobRemovedEventArgs()
					{
						JobID = jobID
					}
				);
			}
		}
		#endregion

		#region Job State Changed Event
		public event EventHandler<JobStateChangeEventArgs> JobStateChanged;

		private void OnJobStateChanged(string jobID, ExtractionStateEnum jobState, string resultString)
		{
			var handler = JobStateChanged;

			if (handler != null)
			{
				handler(this,
					new JobStateChangeEventArgs()
					{
						JobID = jobID,
						JobState = jobState,
						ResultString = resultString
					}
				);
			}
		}
		#endregion

		#region Messaging
		private MessageHandler mMessageHandler = new MessageHandler();

		/// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
		public event EventHandler<MessageArgs> MessageSent
		{
			add { mMessageHandler.MessageSent += value; }
			remove { mMessageHandler.MessageSent -= value; }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Redirects a message to the managers message handler. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        The ModelExtractorMessageArgs to process. </param>
		private void MessageRedirect(object sender, MessageArgs e)
		{
			mMessageHandler.SendMessage(e.Message);
		}
		#endregion

		#region Job Handling
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Adds an extraction job to the manager. </summary>
		///
		/// <param name="job">  The job to add. </param>
		public void Add(IExtractionJob job)
		{
			// Check the job has not been run recently and is not queued before adding it
			lock (mCompletedJobList)
			{
				if (mCompletedJobList.Exists((extractionJob) => extractionJob.JobID == job.JobID))
				{
					mMessageHandler.SendMessage("A completed job already exists for {0}, clear the completed jobs before re-extracting", job.JobName);
					return;
				}
			}

			lock (mJobList)
			{
				if (mJobList.Exists((extractionJob) => extractionJob.JobID == job.JobID))
				{
					mMessageHandler.SendMessage("A job already exists for {0}", job.JobName);
					return;
				}

				job.PropertyChanged += JobPropertyChanged;

				mJobList.Add(job);
			}

			OnJobAdded(job.JobID, job.JobName, job.JobState);

			// Trigger the extraction thread so that it starts the job
			mRestartEvent.Set();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Moves a job to the completed list. </summary>
		///
		/// <param name="completedJob">	The completed job. </param>
		private void JobCompleted(IExtractionJob completedJob)
		{
			// Remove the job from the queued job list
			lock (mJobList)
			{
				mJobList.Remove(completedJob);
			}

			// Move the job to the completed list
			List<IExtractionJob> removedJobs = new List<IExtractionJob>();
			lock (mCompletedJobList)
			{
				mCompletedJobList.Add(completedJob);

				// Delete jobs if there are too many in the completed list
				while (mCompletedJobList.Count > kMaxCompletedJobs)
				{
					removedJobs.Add(mCompletedJobList[0]);
					mCompletedJobList.RemoveAt(0);
				}
			}

			foreach (var job in removedJobs)
			{
				job.PropertyChanged -= JobPropertyChanged;
				OnJobRemoved(job.JobID);
			}
		}

		/// <summary>	Clears the completed jobs list. </summary>
		public void ClearCompletedJobs()
		{
			List<IExtractionJob> removedJobs = new List<IExtractionJob>();
			lock (mCompletedJobList)
			{
				removedJobs.AddRange(mCompletedJobList);
				mCompletedJobList.Clear();
			}

			foreach (var job in removedJobs)
			{
				job.PropertyChanged -= JobPropertyChanged;
				OnJobRemoved(job.JobID);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Job property changed handler. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Property changed event information. </param>
		private void JobPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
		{
			IExtractionJob job = sender as IExtractionJob;

			if(e.PropertyName == "JobState")
			{
				OnJobStateChanged(job.JobID, job.JobState, job.JobResultMessage);
			}
		}
		#endregion

		#region Extraction Thread
		private Thread mExtractionThread;
		private volatile bool mEndThread = false;
		private ManualResetEvent mRestartEvent = new ManualResetEvent(false);

		/// <summary>   Starts the extraction thread. </summary>
		public void Start()
		{
			mExtractionThread = new Thread(ExtractionLoop);
			mExtractionThread.Start();
		}

		/// <summary>   Stops the extraction thread. </summary>
		public void Stop()
		{
			mEndThread = true;
			mRestartEvent.Set();
			mExtractionThread.Join();
			mExtractionThread = null;
		}

		/// <summary>   The manager's extraction loop. </summary>
		private void ExtractionLoop()
		{
			while(!mEndThread)
			{
				// Lock the jobs list and see if any jobs are available
				bool jobsAvailable = true;
				lock (mJobList)
				{
					if(mJobList.Count == 0)
					{
						jobsAvailable = false;

						// If no jobs are available, set the thread to do periodic updates
						mRestartEvent.Reset();
					}
				}

				if (jobsAvailable)
				{
					// Lock the jobs list then get the job
					IExtractionJob extractionJob;
					lock (mJobList)
					{
						extractionJob = mJobList[0];
					}

					mMessageHandler.SendMessage(String.Format("Job started: {0}", extractionJob.JobName));

					// Attach message handlers then run the job
					extractionJob.MessageSent += MessageRedirect;
					extractionJob.Execute();
					extractionJob.MessageSent -= MessageRedirect;
					
					JobCompleted(extractionJob);

					// Lock the jobs list then report the remaining count
					lock (mJobList)
					{
						mMessageHandler.SendMessage("Job complete...{0} jobs remaining", mJobList.Count);
					}
				}

				// Wait for jobs to be added
				mRestartEvent.WaitOne(TimeSpan.FromSeconds(1));
			}
		}
		#endregion
	}
}
