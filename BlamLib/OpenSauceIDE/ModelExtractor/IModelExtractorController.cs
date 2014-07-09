using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenSauceIDE.ModelExtractor.Extractors;
using OpenSauceIDE.ModelExtractor.MessageHandler;

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

        IExtractorFactory GetExtractorFactory();

        void OpenExtractor(string extension);
        void CloseExtractor();
        void Extract(string tagsDirectory, string dataDirectory);

        System.Windows.Forms.Control GetCOLLADAControl();
        System.Windows.Forms.Control GetExtractorControl();
    }
}
