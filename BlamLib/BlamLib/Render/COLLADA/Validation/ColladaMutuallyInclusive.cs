/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Render.COLLADA.Validation
{
	/// <summary>
	/// Validation test that checks whether a list of elements that must exist together, do exist together
	/// </summary>
	public class ColladaMutuallyInclusive : ColladaValidationTest
	{
		private List<ColladaObject> _testFields;

		/// <summary>
		/// Validation test constructor
		/// </summary>
		/// <param name="valid_parent">The parent collada element type to run this test for. Set to "All" to run regardless of parent type</param>
		/// <param name="fields">A list of fields to run the test on</param>
		public ColladaMutuallyInclusive(Enums.ColladaElementType valid_parent, List<ColladaObject> fields)
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

			// determine whether one or more fields exists without the others
			bool any_present = false;
			bool all_present = true;
			foreach (var field in _testFields)
			{
				all_present &= (field.GetValue() != null);
				any_present |= (field.GetValue() != null);
			}

			// if an element is missing, when at least one is present, return an exception
			if (any_present && !all_present)
				exception = new ColladaValidationException(
					String.Format(exceptionFormat, "MutuallyInclusive", "N//A", "one or more elements missing that are mutually inclusive"));
			return exception;
		}
	}
}
