using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace OpenSauceIDE.Aether.Output
{
	public class OutputManager
	{
		public enum OutputTypeEnum
		{
			Normal,
			Warning,
			Error,
			Wrap
		}

		#region Events
		public class OutputManagerEventArgs : EventArgs
		{
			public string Line { get; private set; }

			public OutputManagerEventArgs(string line)
			{
				Line = line;
			}
		}

		public event EventHandler<OutputManagerEventArgs> LineAdded;
		public event EventHandler OutputCleared;

		private void OnLineAdded(string line)
		{
			var eventHandler = LineAdded;

			if (eventHandler != null)
			{
				eventHandler(this, new OutputManagerEventArgs(line));
			}
		}

		private void OnOutputCleared()
		{
			var eventHandler = OutputCleared;

			if (eventHandler != null)
			{
				eventHandler(this, EventArgs.Empty);
			}
		}
		#endregion

		private static int kMaxLineLength = 150;
		private Dictionary<OutputTypeEnum, string> mOutputTypeFormats = new Dictionary<OutputTypeEnum, string>()
		{
			{ OutputTypeEnum.Normal,  "{0}: MSG\t: {1}"},
			{ OutputTypeEnum.Warning, "{0}: WRN\t: {1}"},
			{ OutputTypeEnum.Error,   "{0}: ERR\t: {1}"},
			{ OutputTypeEnum.Wrap,    "{0}:    \t: {1}"},
		};

		private List<string> mOutputLines = new List<string>();

		private string GetTimeStamp()
		{
			return DateTime.Now.ToString("HH:mm:ss");
		}

		public void WriteLine(OutputTypeEnum lineType, string line)
		{
			string timeStamp = GetTimeStamp();

			List<string> wrappedLines = new List<string>();

			// Split on new lines if present
			wrappedLines.AddRange(line.Split(new string[] { "\r\n" }, StringSplitOptions.RemoveEmptyEntries));

			lock (new object())
			{
				int index = 0;
				do
				{
					string currentString = wrappedLines[index];

					currentString = currentString.Trim();

					if (kMaxLineLength <= currentString.Length)
					{
						// Find the last whitspace in the string, which we can cut at
						int cutPoint = currentString.LastIndexOfAny(new char[] { ' ', '\t' }, kMaxLineLength - 1, kMaxLineLength);
						if (cutPoint == -1)
						{
							// The string might have no whtespace, so just cut it at max characters
							cutPoint = kMaxLineLength - 1;
						}

						// Insert a new line into the list and cut the current string to length
						wrappedLines.Insert(index + 1, currentString.Substring(cutPoint));
						currentString = currentString.Substring(0, cutPoint + 1);
					}

					currentString = currentString.Trim();

					// Write the line to the output
					string outputString = String.Format(mOutputTypeFormats[lineType], timeStamp, currentString);
					mOutputLines.Add(outputString);
					OnLineAdded(outputString);

					// Revert line type to wrap for additional lines
					lineType = OutputTypeEnum.Wrap;
				}
				while (++index < wrappedLines.Count);
			}
		}

		public void Clear()
		{
			mOutputLines.Clear();

			OnOutputCleared();
		}

		public void CopyToClipboard()
		{
			StringBuilder stringBuilder = new StringBuilder();
			mOutputLines.ForEach(line => stringBuilder.AppendLine(line));
			System.Windows.Forms.Clipboard.SetText(stringBuilder.ToString());
		}
	}
}
