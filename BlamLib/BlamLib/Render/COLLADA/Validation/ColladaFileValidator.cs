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
	public class ColladaFileValidator
	{
		List<string> localIDs = new List<string>();

		#region Events
		/// <summary>
		/// This event is fired when an error has occured, with an error string describing the error in detail.
		/// </summary>
		public event EventHandler<ColladaErrorEventArgs> ErrorOccured;

		protected void OnErrorOccured(string message)
		{
			if (ErrorOccured != null)
				ErrorOccured(this, new ColladaErrorEventArgs(message));
		}
		#endregion

		/// <summary>
		/// Validates a collada file
		/// </summary>
		/// <param name="collada_file">The file to validate</param>
		/// <returns>True if the file is valid</returns>
		public bool ValidateFile(ColladaFile collada_file)
		{
			// get the local IDs from the collada file
			localIDs.AddRange(collada_file.GetIDs());

			bool is_valid = false;
			try
			{
				collada_file.Validate();
				collada_file.ValidateLocalURIs(localIDs);

				is_valid = true;
			}
			catch (Exception exception)
			{
				// if the collada file is not valid, add a report detailing why it is not valid
				OnErrorOccured(ColladaExceptionStrings.ValidationFailed);

				// report an error for all inner exceptions
				for (var except = exception; except != null; except = except.InnerException)
				{
					OnErrorOccured(except.Message);

					// if the exception was a validation exception, report the element details
					var validation_exception = except as ColladaValidationException;
					if ((validation_exception != null) && (validation_exception.ElementDetails != null))
					{
						foreach (string detail in validation_exception.ElementDetails)
							OnErrorOccured(String.Format("COLLADA_DETAIL: {0}", detail));
					}
				}
			}

			return is_valid;
		}
	}
}
