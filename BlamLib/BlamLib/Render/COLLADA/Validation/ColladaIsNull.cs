/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test to see if the specified fields value is null.
	/// </summary>
	public class ColladaIsNull : ColladaValidationTest
	{
		private ColladaObject testField;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="field">The field to run the test on</param>
		public ColladaIsNull(Enums.ColladaElementType valid_parent, ColladaObject field)
			: base(valid_parent)
		{
			testField = field;
		}

		/// <summary>
		/// Performs the validation test
		/// </summary>
		/// <returns></returns>
		protected override ColladaValidationException ValidateImpl()
		{
			// if the field is null, return an exception
			ColladaValidationException exception = null;
			if (testField.GetValue() == null)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "IsNull", testField.GetTypeName(), "a required element is null"));
			return exception;
		}
	}
}
