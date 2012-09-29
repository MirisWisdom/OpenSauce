/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Deployment.WindowsInstaller;

namespace InstallValidator
{
	public class CustomActions
	{
		private static Validator g_game_validator = null;

		public static void ValidateInstallPath(string game_version, string installation_path)
		{
			g_game_validator = new Validator(game_version);

			g_game_validator.Validate(installation_path);
		}

		[CustomAction]
		public static ActionResult ValidateInstall(Session session)
		{
			session.Log("Begin ValidateInstall");

			// check the required properties have been set
			string definition_to_use = session["INSTALLVALIDATORDEF"];
			string path_to_validate = session["INSTALLVALIDATORPATH"];

			if ((definition_to_use == null) || (definition_to_use.Length == 0))
			{
				session.Log("InstallValidator: ERROR : INSTALLVALIDATORDEF not set");
				return ActionResult.Failure;
			}
			if ((path_to_validate == null) || (path_to_validate.Length == 0))
			{
				session.Log("InstallValidator: ERROR : INSTALLVALIDATORPATH not set");
				return ActionResult.Failure;
			}

			// log the properties for debugging if necessary
			session.Log("LOG: INSTALLVALIDATORGAME : " + definition_to_use);
			session.Log("LOG: INSTALLVALIDATORPATH : " + path_to_validate);

			// validate the installation path
			ValidateInstallPath(definition_to_use, path_to_validate);

			// show warning messages
			if (g_game_validator.WarningMessages.Count != 0)
			{
				Record record = new Record(g_game_validator.WarningMessages.Count + 1);

				record.SetString(0, "The following warnings were reported when validating:");

				int index = 1;
				foreach (var message in g_game_validator.WarningMessages)
				{
					session.Log("InstallValidator: {0}", message);
					record.SetString(index, message);
					index++;
				}

				// present a warning message to the user
				MessageResult result = session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.OKCancel, record);

				// if the user wants to cancel the installation return user exit
				if(result == MessageResult.Cancel)
					return ActionResult.UserExit;
			}

			// show error messages
			if (g_game_validator.ErrorMessages.Count != 0)
			{
				Record record = new Record(g_game_validator.ErrorMessages.Count);
				
				// there should only ever be one error message since it returns immediately after
				// but just in case, log and display all in the list
				int index = 0;
				foreach (var message in g_game_validator.ErrorMessages)
				{
					session.Log("InstallValidator: {0}", message);
					record.SetString(index, message);
					index++;
				}

				// present an error message to the user
				session.Message(InstallMessage.Error | (InstallMessage)(MessageIcon.Error) | (InstallMessage)MessageButtons.OK, record);
				return ActionResult.Failure;
			}

			return ActionResult.Success;
		}
	}
}
