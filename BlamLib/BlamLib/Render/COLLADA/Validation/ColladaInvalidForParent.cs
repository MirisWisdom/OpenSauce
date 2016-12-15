/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test to see if any of a number of fields are present that are not valdi for the current parent.
	/// 
	/// The valid_parent value in the constructor should be set to the element type that the list of fields would be invalid for.
	/// </summary>
	public class ColladaInvalidForParent : ColladaValidationTest
	{
		List<ColladaObject> testFields;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="fields">A list of fields to check the presence of</param>
		public ColladaInvalidForParent(Enums.ColladaElementType valid_parent, List<ColladaObject> fields)
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

			// check if any of the invalid fields are not null
			bool invalid = false;
			for (int i = 0; (i < testFields.Count) && !invalid; i++)
				invalid = testFields[i].GetValue() != null;

			// in an invalid field was present, return an exception
			if (invalid)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "InvalidForParent", "N//A", "an element is present that is not valid for its parent"));
			return exception;
		}
	}
}
