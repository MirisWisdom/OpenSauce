/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using InstallValidator.Validator;
using Microsoft.Deployment.WindowsInstaller;

namespace InstallValidator
{
	public class InstallValidatorCustomAction
	{
		public enum MessageType
		{
			WarningOK,
			WarningYesNo,
			ErrorOK
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates an installation. </summary>
		///
		/// <exception cref="ArgumentNullException">
		/// 	Thrown when one or more required arguments are null.
		/// </exception>
		///
		/// <param name="validatorDefinition">	The validator definition. </param>
		/// <param name="validatorPath">	  	The path to validate. </param>
		/// <param name="onLog">			  	The logging callback. </param>
		/// <param name="onMessageUser">	  	The message user callback. </param>
		/// <param name="onError">			  	The error callback. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		public static bool InstallValidator(string validatorDefinition
			, string validatorPath
			, Action<string> onLog
			, Func<string, MessageType, bool> onMessageUser
			, Action<string, bool> onError)
		{
			if ((onLog == null) || (onMessageUser == null) || (onError == null))
			{
				throw new ArgumentNullException("A required callback delegate was not supplied");
			}

			onLog("Begin InstallValidator");

			// Check the custom action arguments have been set
			if (String.IsNullOrEmpty(validatorDefinition) || String.IsNullOrEmpty(validatorPath))
			{
				onError("InstallValidator: ERROR : Action data not set", false);
				return false;
			}

			// Log the properties for debugging if necessary
			onLog("LOG: INSTALLVALIDATORDEF : " + validatorDefinition);
			onLog("LOG: INSTALLVALIDATORPATH : " + validatorPath);

			// Validate the installation path
			var validator = new PathValidator();

			validator.Validate(validatorDefinition, validatorPath);

			// Show warning messages
			if (validator.WarningMessages.Count != 0)
			{
				// Ask the user if they want to see the warnings that were raised
				var displayMessages = onMessageUser(String.Format("{0} warnings were raised when validating your installation.\n\nDo you want to view them (Recommended)?", validator.WarningMessages.Count)
					, MessageType.WarningYesNo);

				if (displayMessages)
				{
					// Display each warning in a separate dialog
					foreach (var message in validator.WarningMessages)
					{
						onLog(String.Format("InstallValidator: {0}", message));
						onMessageUser(message, MessageType.WarningOK);
					}

					// See if the user still wants to continue the installation
					var continueInstallation = onMessageUser("Do you want to continue the installation?", MessageType.WarningYesNo);

					// If the user wants to cancel the installation return user exit
					if (!continueInstallation)
					{
						return false;
					}
				}
			}

			// Show error message
			if (validator.ErrorMessages.Count != 0)
			{
				onError(validator.ErrorMessages[0], true);
				return false;
			}

			return true;
		}
	}
}
