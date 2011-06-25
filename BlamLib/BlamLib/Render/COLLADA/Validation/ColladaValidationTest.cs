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

namespace BlamLib.Render.COLLADA.Validation
{
	public class ColladaValidationTest
	{
		protected static readonly string exceptionFormat = "VALIDATION ERROR : {0} : {1} : {2}";
		private Enums.ColladaElementType _validParentType;

		public ColladaValidationTest(Enums.ColladaElementType parent_type)
		{
			_validParentType = parent_type;
		}

		protected virtual ColladaValidationException ValidateImpl() { return null; }

		/// <summary>
		/// Runs the validation test
		/// </summary>
		/// <param name="parent_type">The parent element type, used to determine whether this test needs to be run</param>
		/// <returns></returns>
		public ColladaValidationException Validate(Enums.ColladaElementType parent_type)
		{
			bool valid_parent = false;
			// if the test should be run with any parent type, valid_parent will be true
			if (_validParentType == Enums.ColladaElementType.All)
				valid_parent = true;
			else
				valid_parent = (parent_type == _validParentType);

			// validate the field and return an exception if necessary
			ColladaValidationException exception = null;
			if (valid_parent)
				exception = ValidateImpl();
			return exception;
		}
	}
}
