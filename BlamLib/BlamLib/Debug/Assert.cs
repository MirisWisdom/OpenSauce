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
	/// Debug logging
	/// </summary>
	public static class Assert
	{
		public delegate void AssertProc();

		/// <summary>
		/// Debugs if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> when <paramref name="cond"/> fails</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond)
		{
			if (!cond)
			{
				/*System.Diagnostics.Debugger.Break();*/
				System.Diagnostics.StackFrame st = new System.Diagnostics.StackFrame(1, true);
				throw new ExceptionLog("Assertion!{0}'{1}':{2} @ {3}", Program.NewLine,
					st.GetFileName(), st.GetMethod(), st.GetFileLineNumber());
			}
		}

		/// <summary>
		/// Debugs if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> when <paramref name="cond"/> fails</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond, string format, params object[] args) { if (!cond) { /*System.Diagnostics.Debugger.Break();*/ throw new Debug.ExceptionLog(format, args); } }

		/// <summary>
		/// Debugs if <paramref name="cond"/> is false, throwing 
		/// <paramref name="throw_this"/> as the exception
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <param name="throw_this">Exception to throw when evaluation fails</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> instead when <paramref name="throw_this"/> is null</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond, Exception throw_this, string format, params object[] args)
		{
			if (!cond)
			{
//				System.Diagnostics.Debugger.Break();
				if(throw_this != null)
				{
					LogFile.WriteLine(format, args);
					throw throw_this;
				}
				else throw new Debug.ExceptionLog(format, args);
			}
		}

		/// <summary>
		/// Debugs if <paramref name="obj"/> is null
		/// </summary>
		/// <param name="obj">Object to evaluate</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> when <paramref name="obj"/> is null</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(object obj, string format, params object[] args) { If(obj != null, format, args); }

		/// <summary>
		/// Debugs if <paramref name="obj"/> is null
		/// </summary>
		/// <param name="obj">Object to evaluate</param>
		/// <param name="throw_this">Exception to throw when evaluation fails</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> instead when <paramref name="throw_this"/> is null</remarks>
		public static void If(object obj, Exception throw_this, string format, params object[] args) { If(obj != null, throw_this, format, args); }

		/// <summary>
		/// Debugs if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <param name="proc">Code to run <b>before</b> the exception is thrown</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> when <paramref name="cond"/> fails</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond, AssertProc proc, string format, params object[] args)
		{
			if(!cond) proc();
			If(cond, format, args);
		}

		/// <summary>
		/// Debugs if <paramref name="obj"/> is null
		/// </summary>
		/// <param name="obj">Object to evaluate</param>
		/// <param name="proc">Code to run <b>before</b> the exception is thrown</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		/// <remarks>Throws <see cref="Debug.ExceptionLog"/> when <paramref name="cond"/> fails</remarks>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(object obj, AssertProc proc, string format, params object[] args)
		{
			if (obj == null)
			{
				proc();
				If(false, format, args);
			}
		}
	};

	/// <summary>
	/// Warning logging
	/// </summary>
	public static class Warn
	{
		public delegate void WarnProc();

		/// <summary>
		/// Warns if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond)
		{
			if (!cond)
			{
				System.Diagnostics.StackFrame st = new System.Diagnostics.StackFrame(1, true);
				Debug.LogFile.WriteLine("WARNING!{0}'{1}':{2} @ {3}", Program.NewLine,
					st.GetFileName(), st.GetMethod(), st.GetFileLineNumber());
			}
		}

		/// <summary>
		/// Warns if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond, string format, params object[] args) { if (!cond) Debug.LogFile.WriteLine(format, args); }

		/// <summary>
		/// Warns if <paramref name="obj"/> is null
		/// </summary>
		/// <param name="obj">Object to evaluate</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(object obj, string format, params object[] args) { If(obj != null, format, args); }

		/// <summary>
		/// Warns if <paramref name="cond"/> is false
		/// </summary>
		/// <param name="cond">Condition to evaluate</param>
		/// <param name="proc">Code to run if <paramref name="cond"/> is false</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(bool cond, WarnProc proc, string format, params object[] args)
		{
			if (!cond) proc();
			If(cond, format, args);
		}

		/// <summary>
		/// Warns if <paramref name="obj"/> is null
		/// </summary>
		/// <param name="obj">Object to evaluate</param>
		/// <param name="proc">Code to run if <paramref name="cond"/> is false</param>
		/// <param name="format">Format string</param>
		/// <param name="args">Format string inputs</param>
		[System.Diagnostics.Conditional("DEBUG")]
		public static void If(object obj, WarnProc proc, string format, params object[] args)
		{
			if (obj == null)
			{
				proc();
				If(false, format, args);
			}
		}
	};
}