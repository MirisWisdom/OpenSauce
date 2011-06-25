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
	/// Validation test that checks that an element list has a minimum number of elements
	/// </summary>
	/// <typeparam name="T">The element type of the list</typeparam>
	public class ColladaListMinCount<T> : ColladaValidationTest
		where T : new()
	{
		private ColladaObjectElementList<T> testField;
		private int minimumCount;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="field">The field to run the test on</param>
		/// <param name="minimum_count">The minimum number of elements the list must have</param>
		public ColladaListMinCount(Enums.ColladaElementType valid_parent, ColladaObjectElementList<T> field, int minimum_count)
			: base(valid_parent)
		{
			testField = field;
			minimumCount = minimum_count;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			// if the value is null, return null
			// there is a specific null value validation test for checking this
			if (testField.Value == null)
				return null;

			// if the list does no have the minimum number of elements, return an exception
			ColladaValidationException exception = null;
			if (testField.Value.Count < minimumCount)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "ListMinCount", testField.GetTypeName(), "an element list does not have the minimum number of elements"));
			return exception;
		}
	}
}
