/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.IO;

namespace BlamLib.Debug
{
	/// <summary>
	/// Class for doing special tracing
	/// </summary>
	public class Trace : IDisposable
	{
		static System.Collections.Generic.List<Trace> all_traces = new System.Collections.Generic.List<Trace>();
		public static void Close()
		{
			lock(all_traces)
				foreach (Trace t in all_traces)
					t.CloseLog();
		}

		#region LogFile
		private StreamWriter LogFile;

		/// <summary>
		/// Closes the log file
		/// </summary>
		public void CloseLog()
		{
			if(LogFile != null)
				LogFile.Close();
		}

		/// <summary>
		/// When true, Any 'Write' calls will perform 'WriteHeader'
		/// </summary>
		bool NoErrors = true;
		#endregion

		#region Purpose
		string purpose = "Unknown";
		/// <summary>
		/// The sole purpose of this trace file
		/// </summary>
		public string Purpose { get { return purpose; } }
		#endregion

		private string fileName;
		void OpenFile()
		{
			if (!Directory.Exists(Program.TracePath)) Directory.CreateDirectory(Program.TracePath);

			LogFile = new StreamWriter(new FileStream(Program.TracePath + fileName + ".log", FileMode.Append, FileAccess.Write, FileShare.Read));
		}

		/// <summary>
		/// Constructs a new Trace log file, that is created Program.TracePath
		/// </summary>
		/// <param name="FileName">Filename</param>
		/// <param name="purpose">The purpose for this tracing (use the class name or something)</param>
		public Trace(string FileName, string purpose)
		{
			this.fileName = FileName;

			this.purpose = purpose;

			lock(all_traces)
				all_traces.Add(this);
		}

		public void Dispose() { this.CloseLog(); all_traces.Remove(this); }

		/// <summary>
		/// Writes the header for a start of a log
		/// </summary>
		private void WriteHeader()
		{
			string app = Purpose + " trace file ----------------------------------------------";

			OpenFile();

			LogFile.WriteLine();
			LogFile.WriteLine();
			LogFile.WriteLine("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				app);
			LogFile.Flush();
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
		public void Write(object value) { Write(value.ToString()); }

		/// <summary>
		/// Writes a string to the log file
		/// </summary>
		/// <param name="value"></param>
		public void Write(string value)
		{
			// Write the log header if this is the first error being logged
			if (NoErrors)
			{
				WriteHeader();
				NoErrors = false;
			}

			value = Format(value);

			LogFile.Write("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				value);
			LogFile.Flush();
		}

		/// <summary>
		/// Writes a formatted string to the log file
		/// </summary>
		/// <param name="value">String with formatting</param>
		/// <param name="args">Format arguments</param>
		public void Write(string value, params object[] args) { Write(string.Format(value, args)); }

		/// <summary>
		/// Writes an array of strings to the log file
		/// </summary>
		/// <param name="values"></param>
		public void Write(string[] values)
		{
			string value = "";
			foreach (string s in values)
				value += (s + Program.NewLine);

			Write(value);
		}
		#endregion

		#region Write With Condition
		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		public void Write(bool cond, object value) { if (cond) Write(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		public void Write(bool cond, string value) { if (cond) Write(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log with formatting
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		/// <param name="args"></param>
		public void Write(bool cond, string value, params object[] args) { if (cond) Write(value, args); }

		/// <summary>
		/// If cond is true, writes 'values' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="values"></param>
		public void Write(bool cond, string[] values) { if (cond) Write(values); }
		#endregion

		#region WriteLine
		/// <summary>
		/// Writes a line terminator without a timestap
		/// </summary>
		public void WriteLine() { LogFile.WriteLine(); }

		/// <summary>
		/// Writes a object to the log file
		/// </summary>
		/// <param name="value"></param>
		public void WriteLine(object value) { WriteLine(value.ToString()); }

		/// <summary>
		/// Writes a string to the log file
		/// </summary>
		/// <param name="value"></param>
		public void WriteLine(string value)
		{
			// Write the log header if this is the first error being logged
			if (NoErrors)
			{
				WriteHeader();
				NoErrors = false;
			}

			value = Format(value);

			LogFile.WriteLine("{0} {1}  {2}",
				DateTime.Now.ToShortDateString(),
				DateTime.Now.ToLongTimeString(),
				value);
			LogFile.Flush();
		}

		/// <summary>
		/// Writes a formatted string to the log file
		/// </summary>
		/// <param name="value">String with formatting</param>
		/// <param name="args">Format arguments</param>
		public void WriteLine(string value, params object[] args) { WriteLine(string.Format(value, args)); }

		/// <summary>
		/// Writes an array of strings to the log file
		/// </summary>
		/// <param name="values"></param>
		public void WriteLine(string[] values)
		{
			string value = "";
			foreach (string s in values)
				value += (s + Program.NewLine);

			WriteLine(value);
		}
		#endregion

		#region WriteLine With Condition
		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		public void WriteLine(bool cond, object value) { if (cond) WriteLine(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		public void WriteLine(bool cond, string value) { if (cond) WriteLine(value); }

		/// <summary>
		/// If cond is true, writes 'value' to log with formatting
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="value"></param>
		/// <param name="args"></param>
		public void WriteLine(bool cond, string value, params object[] args) { if (cond) WriteLine(value, args); }

		/// <summary>
		/// If cond is true, writes 'values' to log
		/// </summary>
		/// <param name="cond"></param>
		/// <param name="values"></param>
		public void WriteLine(bool cond, string[] values) { if (cond) WriteLine(values); }
		#endregion
	};
}