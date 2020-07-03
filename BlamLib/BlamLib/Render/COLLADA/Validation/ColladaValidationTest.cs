/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
