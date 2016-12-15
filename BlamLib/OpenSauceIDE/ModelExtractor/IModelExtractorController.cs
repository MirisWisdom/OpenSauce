/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using BlamLib.Messaging;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.ModelExtractor.Extractors;
using OpenSauceIDE.ModelExtractor.Settings;

namespace OpenSauceIDE.ModelExtractor
{
	/// <summary>   Enum for the current state of the model extractor controller. </summary>
	public enum ModelExtractorStateEnum
	{
		ExtractorInitialising,
		ExtractorReady,
		ExtractorClosing,
		ExtractorClosed,
	}

	/// <summary>   Arguments for the model extractor controller state change. </summary>
	public class ModelExtractorStateChangeArgs : EventArgs
	{
		public ModelExtractorStateEnum State { get; private set; }

		public ModelExtractorStateChangeArgs(ModelExtractorStateEnum state)
		{
			State = state;
		}
	}

	/// <summary>   Interface for the model extractor controller. </summary>
	public interface IModelExtractorController
		: IMessageSource
		, IDisposable
	{
		event EventHandler<ModelExtractorStateChangeArgs> StateChanged;

		IExtractorPathSettings GetExtractorSettings();
		IExtractorFactory GetExtractorFactory();
		IExtractionManager GetExtractionManager();

		void OpenExtractor(string extension);
		void CloseExtractor();
		void Extract();

		IEnumerable<System.Windows.Forms.Control> GetControls();
	}

	public interface IModelExtractorJobInterface
	{
		void AddJob(string modelFile, IExtractionData extractionData);
	}
}
