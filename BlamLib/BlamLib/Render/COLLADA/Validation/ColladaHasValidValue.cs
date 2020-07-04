/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
