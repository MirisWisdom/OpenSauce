/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Reflection;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test to check that a list contains a required value
	/// </summary>
	/// <typeparam name="T1">The lists type</typeparam>
	/// <typeparam name="T2">The required value type</typeparam>
	public class ColladaListHasValue<T1, T2> : ColladaValidationTest
		where T1 : new()
	{
		private ColladaObjectElementList<T1> testField;
		private string propertyName;
		private T2 requiredValue;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="field">The field to run the test on</param>
		/// <param name="property_name">The proterty name to check the value of in each list element</param>
		/// <param name="required_value">The value that the list must contain at least one of</param>
		public ColladaListHasValue(Enums.ColladaElementType valid_parent,
			ColladaObjectElementList<T1> field, 
			string property_name,
			T2 required_value)
			: base(valid_parent)
		{
			testField = field;
			propertyName = property_name;
			requiredValue = required_value;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			// gets the property info of the specified property in the element type
			// if null, the implimentation of this test is wrong
			PropertyInfo property = typeof(T1).GetProperty(propertyName);
			if (property == null)
				return null;

			if (testField.Value == null)
				return null;

			// loop though the fields elements
			bool has_value = false;
			for (int i = 0; (i < testField.Value.Count) && !has_value; i++)
			{
				T2 element_value = (T2)property.GetValue(testField.Value[i], null);
				if (element_value == null)
					continue;

				has_value = element_value.Equals(requiredValue);
			}

			// if the required value was not found, return an exception
			ColladaValidationException exception = null;
			if (!has_value)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "ListHasValue", testField.GetTypeName(), "a list of elements is missing a necessary value"));
			return exception;
		}
	}
}
