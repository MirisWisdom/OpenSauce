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
		public ColladaException() : base() { }
		public ColladaException(Exception inner_exception) : base("COLLADA: an exception occurred", inner_exception) { }
		public ColladaException(string exception_string) : base(exception_string) { }
		public ColladaException(string exception_string, Exception inner_exception) : base(exception_string, inner_exception) { }
	}
}