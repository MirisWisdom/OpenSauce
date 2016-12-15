/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test that checks whether elements exist together that should not be
	/// </summary>
	public class ColladaMutuallyExclusive : ColladaValidationTest
	{
		private List<ColladaObject> testFields;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="fields">A list of fields to run the test on</param>
		public ColladaMutuallyExclusive(Enums.ColladaElementType valid_parent, List<ColladaObject> fields)
			: base(valid_parent)
		{
			testFields = fields;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			ColladaValidationException exception = null;

			// get the number of elements in the list that are not null
			int value_count = 0;
			foreach (var field in testFields)
				value_count += ((field.GetValue() != null) ? 1 : 0);

			// if the value is more than 1 return an exception
			if (value_count > 1)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "MutuallyExclusive", "N//A", "an element list does not have the minimum number of elements"));
			return exception;
		}
	}
}
