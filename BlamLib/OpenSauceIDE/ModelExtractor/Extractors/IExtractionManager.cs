/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Messaging;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	/// <summary>	Additional information for job added events. </summary>
	public class JobAddedEventArgs : EventArgs
	{
		public string JobID;
		public string JobName;
		public ExtractionStateEnum JobState;
	}

	/// <summary>	Additional information for job removed events. </summary>
	public class JobRemovedEventArgs : EventArgs
	{
		public string JobID;
	}

	/// <summary>	Additional information for job state change events. </summary>
	public class JobStateChangeEventArgs : EventArgs
	{
		public string JobID;
		public ExtractionStateEnum JobState;
		public string ResultString;
	}

	/// <summary>	Interface for an extraction manager. </summary>
	public interface IExtractionManager
		: IMessageSource
	{
		/// <summary>	Event queue for all listeners interested in JobAdded events. </summary>
		event EventHandler<JobAddedEventArgs> JobAdded;

		/// <summary>	Event queue for all listeners interested in JobRemoved events. </summary>
		event EventHandler<JobRemovedEventArgs> JobRemoved;

		/// <summary>	Event queue for all listeners interested in JobStateChanged events. </summary>
		event EventHandler<JobStateChangeEventArgs> JobStateChanged;

		/// <summary>	Starts the extraction manager thread. </summary>
		void Start();

		/// <summary>	Stops the extraction manager thread. </summary>
		void Stop();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a job to the extraction manager. </summary>
		///
		/// <param name="job">	The job to add. </param>
		void Add(IExtractionJob job);

		/// <summary>	Clears the completed jobs list. </summary>
		void ClearCompletedJobs();
	}
}
