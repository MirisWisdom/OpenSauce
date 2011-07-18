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