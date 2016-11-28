/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Render.COLLADA
{
	public class ColladaErrorEventArgs : EventArgs
	{
		public string ErrorMessage { get; private set; }

		public ColladaErrorEventArgs(string message)
		{
			ErrorMessage = message;
		}
	};

	public class ColladaException : Exception
	{
		public ColladaException()
			: base()
		{ }

		public ColladaException(Exception inner_exception, string exception_string)
			: base(exception_string, inner_exception)
		{ }

		public ColladaException(Exception inner_exception, string exception_string, params object[] args)
			: this(inner_exception, String.Format(exception_string, args))
		{ }

		public ColladaException(Exception inner_exception)
			: this(inner_exception, "COLLADA: an exception occurred")
		{ }

		public ColladaException(string exception_string)
			: this(null, exception_string)
		{ }

		public ColladaException(string exception_string, params object[] args)
			: this(null, exception_string, args)
		{ }
	}
}