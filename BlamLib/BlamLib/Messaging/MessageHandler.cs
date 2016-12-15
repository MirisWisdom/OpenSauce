/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Messaging
{
	/// <summary>   A message handler for sending messages to attached handlers. </summary>
	public class MessageHandler
		: IMessageHandler
	{
		/// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
		public event EventHandler<MessageArgs> MessageSent;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Sends a message. </summary>
		///
		/// <param name="message">  The message. </param>
		public void SendMessage(string message)
		{
			var handler = MessageSent;
			if (handler != null)
			{
				handler(this, new MessageArgs(message));
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
