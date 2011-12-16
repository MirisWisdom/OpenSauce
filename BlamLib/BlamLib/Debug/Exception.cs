/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Debug
{
	/// <summary>
	/// Exception related utility class
	/// </summary>
	public static class Exceptions
	{
		#region Lib
		static void UnhandledException(object sender, UnhandledExceptionEventArgs e)
		{
			LogFile.WriteLine("Unhanded exception ({0}).{1}{2}{3}", sender, (e.IsTerminating ? " Terminating..." : ""), Program.NewLine, e.ExceptionObject);
		}

		static void ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
		{
			LogFile.WriteLine("A thread caused an unhanded exception ({0}).{1}{2}", sender, Program.NewLine, e.Exception);
		}

		static bool isInitialized = false;
		/// <summary>
		/// Initialize internal exception utilities
		/// </summary>
		public static void Initialize()
		{
			if (!isInitialized)
			{
				AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(UnhandledException);
				System.Windows.Forms.Application.ThreadException += new System.Threading.ThreadExceptionEventHandler(ThreadException);

				isInitialized = true;
			}
		}
		/// <summary>
		/// Dispose internal exception utilities
		/// </summary>
		public static void Dispose()
		{
			if (isInitialized)
			{
				AppDomain.CurrentDomain.UnhandledException -= new UnhandledExceptionEventHandler(UnhandledException);
				System.Windows.Forms.Application.ThreadException -= new System.Threading.ThreadExceptionEventHandler(ThreadException);

				isInitialized = false;
			}
		}
		#endregion

		/// <summary>
		/// Exception used in cases which shouldn't have been reached in the first place...
		/// </summary>
		public class UnreachableException : ExceptionLog
		{
			public UnreachableException()
			{
				System.Diagnostics.StackFrame st = new System.Diagnostics.StackFrame(1, true);
				LogFile.WriteLine("Unreachable case!{0}'{1}':{2} @ {3}", Program.NewLine,
					st.GetFileName(), st.GetMethod(), st.GetFileLineNumber());
			}
			public UnreachableException(object case_value)
			{
				System.Diagnostics.StackFrame st = new System.Diagnostics.StackFrame(1, true);
				LogFile.WriteLine("Unreachable case! '{4}'{0}'{1}':{2} @ {3}", Program.NewLine,
					st.GetFileName(), st.GetMethod(), st.GetFileLineNumber(), case_value);
			}
		}
	};

	/// <summary>
	/// Exception class that also logs the event to LogFile
	/// </summary>
	public class ExceptionLog : Exception
	{
		protected ExceptionLog() : base() {}
		
		public ExceptionLog(Exception inner, string msg) : base(msg, inner) {}

		/// <summary>
		/// Exception constructor
		/// </summary>
		/// <param name="obj">Object to log</param>
		public ExceptionLog(object obj) : base(obj.ToString())	{ LogFile.WriteLine("{0}{1}{2}", this.StackTrace, Program.NewLine, obj); }

		public ExceptionLog(object obj, Exception inner) : base(obj.ToString(), inner) { LogFile.WriteLine("{0}{1}{2}", this.StackTrace, Program.NewLine, obj); }

		/// <summary>
		/// Exception constructor
		/// </summary>
		/// <param name="format">Log line(s) formatting</param>
		/// <param name="args">formating parameters</param>
		public ExceptionLog(string format, params object[] args) : base(/*string.Format(format, args)*/)
		{
			LogFile.WriteLine(format, args);
		}

		public ExceptionLog(Exception inner, string format, params object[] args) : base(null, inner)
		{
			LogFile.Write(format, args);
			LogFile.Write("{1}{0}{1}", inner, Program.NewLine);
		}
	};
}