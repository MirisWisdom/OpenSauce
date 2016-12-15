/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Messaging
{
	/// <summary>   Interface for a message source. </summary>
	public interface IMessageSource
	{
		event EventHandler<MessageArgs> MessageSent;
	}

	/// <summary>   Arguments for messages. </summary>
	public class MessageArgs : EventArgs
	{
		public string Message { get; private set; }

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="message">  The message. </param>
		public MessageArgs(string message)
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
