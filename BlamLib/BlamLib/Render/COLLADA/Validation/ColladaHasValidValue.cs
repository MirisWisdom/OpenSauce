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

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test to check whether the specified element has a valid value
	/// </summary>
	/// <typeparam name="T"></typeparam>
	public class ColladaHasValidValue<T> : ColladaValidationTest
	{
		private ColladaObject testField;
		private List<T> validValues;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="field">The field to run the test on</param>
		/// <param name="valid_values">A list of values to check for</param>
		public ColladaHasValidValue(Enums.ColladaElementType valid_parent, ColladaObject field, List<T> valid_values)
			: base(valid_parent)
		{
			testField = field;
			validValues = valid_values;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			// get the value of the field
			T value = (T)testField.GetValue();

			// if the value is null, return null
			// there is a specific null value validation test for checking this
			if (value == null)
				return null;

			// if the valid value list does not have the current value return an exception
			ColladaValidationException exception = null;
			if (!validValues.Contains(value))
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "HasValidValue", testField.GetTypeName(), "an element has an invalid value"));
			return exception;
		}
	}
}
