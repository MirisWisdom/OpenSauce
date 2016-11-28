/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test that checks whether at least one element in a list exists
	/// </summary>
	public class ColladaOneRequired : ColladaValidationTest
	{
		private List<ColladaObject> _testFields;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="fields">A list of fields to run the test on</param>
		public ColladaOneRequired(Enums.ColladaElementType valid_parent, List<ColladaObject> fields)
			: base(valid_parent)
		{
			_testFields = fields;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			ColladaValidationException exception = null;

			// find out homw many fields in the list are not null
			int value_count = 0;
			foreach (var field in _testFields)
				value_count += ((field.GetValue() != null) ? 1 : 0);

			// if all the fields are null return an exception
			if (value_count == 0)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "OneRequired", "N//A", "a required element is missing"));
			return exception;
		}
	}
}
