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
using System.Threading;
using BlamLib.Messaging;

namespace OpenSauceIDE.Controls
{
	public partial class MessageListControl : UserControl
	{
		#region Fields
		private IMessageSource mMessageSource;
		private BindingList<string> mMessages = new BindingList<string>();
		#endregion

		[Category("MessageList")]
		public int MaxMessageCount { get; set; }

		/// <summary>   Default constructor. </summary>
		public MessageListControl()
		{
			InitializeComponent();

			MaxMessageCount = 256;
			mMessageListBox.DataSource = mMessages;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Attaches to the given message source. </summary>
		///
		/// <param name="messageSource">    The message source. </param>
		public void Attach(IMessageSource messageSource)
		{
			mMessageSource = messageSource;

			mMessageSource.MessageSent += MessageSent;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Adds a message to the list. </summary>
		///
		/// <param name="message">  The message to add. </param>
		private void AddMessage(string message)
		{
			lock (mMessages)
			{
				// Remove messages from the end of the list if the max count is reached
				if ((mMessages.Count + 1) > MaxMessageCount)
				{
					mMessages.RemoveAt(mMessages.Count - 1);
				}

				// Add the message to the top of the list, and lock the selected to the top of the list if necessary
				var trackTopEntry = (mMessageListBox.SelectedIndex == 0) || (mMessageListBox.SelectedIndex == -1);

				mMessages.Insert(0, message);

				if (trackTopEntry)
				{
					mMessageListBox.SelectedIndex = 0;
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Adds a message to the list. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        The ModelExtractorMessageArgs to process. </param>
		private void MessageSent(object sender, MessageArgs e)
		{
			if (InvokeRequired)
			{
				BeginInvoke(new Action(() => AddMessage(e.Message)));
			}
			else
			{
				AddMessage(e.Message);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Clears the message list. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        Event information. </param>
		private void Clear(object sender, EventArgs e)
		{
			lock (mMessages)
			{
				mMessages.Clear();
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Copies the current messages to the clipboard. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        Event information. </param>
		private void Copy(object sender, EventArgs e)
		{
			lock (mMessages)
			{
				if (mMessages.Count > 0)
				{
					Clipboard.SetText(String.Join("\r\n", mMessages));
				}
			}
		}
	}
}
