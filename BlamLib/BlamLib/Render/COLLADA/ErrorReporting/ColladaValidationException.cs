/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA
{
	public class ColladaValidationException : ColladaException
	{
		private static string ValidationError = "COLLADA VALIDATION: a child element failed validation";

		private List<string> m_detail_strings;
		public List<string> ElementDetails
		{
			get { return m_detail_strings; }
			set { m_detail_strings = value; }
		}

		public ColladaValidationException() : base(ValidationError) { }
		public ColladaValidationException(Exception inner_exception) : base(ValidationError, inner_exception) { }
		public ColladaValidationException(string message) : base(message) { }
		public ColladaValidationException(string message, List<string> detail_strings) : base(message) { m_detail_strings = detail_strings; }
	}
}