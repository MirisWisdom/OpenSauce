using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OpenSauceIDE.ModelExtractor.MessageHandler
{
    /// <summary>   A message handler for sending messages to attached handlers. </summary>
    public class MessageHandler
        : IMessageHandler
    {
        /// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
        public event EventHandler<ModelExtractorMessageArgs> MessageSent;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Sends a message. </summary>
        ///
        /// <param name="message">  The message. </param>
        public void SendMessage(string message)
        {
            var handler = MessageSent;
            if (handler != null)
            {
                handler(this, new ModelExtractorMessageArgs(message));
            }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Sends a formatted message. </summary>
        ///
        /// <param name="format">   Describes the format to use. </param>
        /// <param name="args">
        ///     A variable-length parameters list containing the format arguments.
        /// </param>
        public void SendMessage(string format, params object[] args)
        {
            SendMessage(String.Format(format, args));
        }
    }
}
