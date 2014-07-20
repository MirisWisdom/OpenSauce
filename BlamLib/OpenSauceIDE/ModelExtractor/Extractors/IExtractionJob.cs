/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using BlamLib.Messaging;

namespace OpenSauceIDE.ModelExtractor.Extractors
{
	public enum ExtractionStateEnum
	{
		Queued,
		InProgress,
		Succeeded,
		Failed
	}

	/// <summary>   Interface for an extraction job. </summary>
	public interface IExtractionJob
		: IMessageSource
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the job. </summary>
		///
		/// <value>	The name of the job. </value>
		string JobName { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the state of the job. </summary>
		///
		/// <value>	The job state. </value>
		ExtractionStateEnum JobState { get; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a message describing the job result. </summary>
		///
		/// <value>	A message describing the job result. </value>
		string JobResultMessage { get; }

		/// <summary>	Executes the job. </summary>
		void Execute();
	}
}
