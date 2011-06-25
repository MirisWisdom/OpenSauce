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
