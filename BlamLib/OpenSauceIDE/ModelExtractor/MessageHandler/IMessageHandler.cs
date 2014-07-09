using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.ModelExtractor.MessageHandler
{
    /// <summary>   Interface for a message source. </summary>
    public interface IMessageSource
    {
        event EventHandler<ModelExtractorMessageArgs> MessageSent;
    }

    /// <summary>   Arguments for model extractor messages. </summary>
    public class ModelExtractorMessageArgs : EventArgs
    {
        public string Message { get; private set; }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Constructor. </summary>
        ///
        /// <param name="message">  The message. </param>
        public ModelExtractorMessageArgs(string message)
        {
            Message = message;
        }
    }

    /// <summary>   Interface for a message handler. </summary>
    public interface IMessageHandler
        : IMessageSource
    {
        void SendMessage(string message);
        void SendMessage(string format, params object[] args);
    }
}
