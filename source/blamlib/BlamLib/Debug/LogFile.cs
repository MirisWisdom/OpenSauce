/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;

namespace BlamLib.Debug
{
	public static class LogFile
	{
		#region LogFile
#if TRACE
		private static StreamWriter logFile;
#endif

		/// <summary>
		/// Opens the log file so we can log...stuff :-3
		/// </summary>
		[System.Diagnostics.Conditional("TRACE")]
		static void OpenLog()
		{
			logFile = new StreamWriter(new FileStream(
				Program.DebugFile, 
				FileMode.Append, FileAccess.Write, FileShare.ReadWrite));
		}

		static LogFile() { }

		/// <summary>
		/// Closes the log file
		/// </summary>
		[System.Diagnostics.Conditional("TRACE")]
		public static void CloseLog()
		{
			if(logFile != null)
			{
				logFile.Close();
				logFile = null;
			}
		}

		/// <summary>
		/// When true, Any 'Write' calls will perform 'WriteHeader'
		/// </summary>
		static bool NoErrors = true;
		#endregion

		/// <summary>
		/// Writes the header for a start of a log
		/// </summary>
		[System.Diagnostics.Conditional("TRACE")]
		private static void WriteHeader()
		{
			string app = string.Format("{0} {1} ----------------------------------------------", Program.Name, Program.Version);

			OpenLog();

			logFile.WriteLine();
			logFile.WriteLine();
			logFile.WriteLine("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				app);
			logFile.Flush();
		}

#if TRACE
		static string Format(string value)
		{
			// Fix tabbing
			value = value.Replace("\n", "\n\t\t\t\t\t  ");

			// Remove source code paths
			value = value.Replace(Program.SourcePath, "");
			value = value.Replace(Program.SourcePath.ToLower(), "");

			// Remove namespace
			value = value.Replace("BlamLib.", "");

			return value;
		}
#endif

		#region Write
		/// <summary>
		/// Writes a object to the log file
		/// </summary>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(object value) { Write(value.ToString()); }

		/// <summary>
		/// Writes a string to the log file
		/// </summary>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(string value)
		{
			// Write the log header if this is the first error being logged
			if (NoErrors)
			{
				WriteHeader();
				NoErrors = false;
			}

			value = Format(value);

			logFile.Write("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				value);
			logFile.Flush();
		}

		/// <summary>
		/// Writes a formatted string to the log file
		/// </summary>
		/// <param name="value">String with formatting</param>
		/// <param name="args">Format arguments</param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(string value, params object[] args) { Write(string.Format(value, args)); }

		/// <summary>
		/// Writes an array of strings to the log file
		/// </summary>
		/// <param name="values"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(string[] values)
		{
			System.Text.StringBuilder value = new System.Text.StringBuilder();
			foreach (string s in values)
				value.AppendFormat("{0}{1}", s, Program.NewLine);

			Write(value.ToString());
		}
		#endregion

		#region Write With Condition
		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(bool cond, object value) { if (cond) Write(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(bool cond, string value) { if (cond) Write(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log with formatting
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		/// <param name="args"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(bool cond, string value, params object[] args) { if (cond) Write(value, args); }

		/// <summary>
		/// If cond is true, writes 'values' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="values"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void Write(bool cond, string[] values) { if (cond) Write(values); }
		#endregion

		#region WriteLine
		/// <summary>
		/// Writes a object to the log file
		/// </summary>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(object value) { WriteLine(value.ToString()); }

		/// <summary>
		/// Writes a string to the log file
		/// </summary>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(string value)
		{
			// Write the log header if this is the first error being logged
			if (NoErrors)
			{
				WriteHeader();
				NoErrors = false;
			}

			value = Format(value);

			logFile.WriteLine("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				value);
			logFile.Flush();
		}

		/// <summary>
		/// Writes a formatted string to the log file
		/// </summary>
		/// <param name="value">String with formatting</param>
		/// <param name="args">Format arguments</param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(string value, params object[] args) { WriteLine(string.Format(value, args)); }

		/// <summary>
		/// Writes an array of strings to the log file
		/// </summary>
		/// <param name="values"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(string[] values)
		{
			System.Text.StringBuilder value = new System.Text.StringBuilder();
			foreach (string s in values)
				value.AppendFormat("{0}{1}", s, Program.NewLine);

			WriteLine(value.ToString());
		}
		#endregion

		#region WriteLine With Condition
		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(bool cond, object value) { if (cond) WriteLine(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(bool cond, string value) { if (cond) WriteLine(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log with formatting
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		/// <param name="args"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(bool cond, string value, params object[] args) { if (cond) WriteLine(value, args); }

		/// <summary>
		/// If cond is true, writes 'values' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="values"></param>
		[System.Diagnostics.Conditional("TRACE")]
		public static void WriteLine(bool cond, string[] values) { if (cond) WriteLine(values); }
		#endregion
	};
}