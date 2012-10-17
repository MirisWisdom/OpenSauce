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
		private static Validator g_validator = null;

		public static void ValidateInstallPath(string install_definition, string installation_path)
		{
			g_validator = new Validator(install_definition);

			g_validator.Validate(installation_path);
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
			session.Log("LOG: INSTALLVALIDATORDEF : " + definition_to_use);
			session.Log("LOG: INSTALLVALIDATORPATH : " + path_to_validate);

			// validate the installation path
			ValidateInstallPath(definition_to_use, path_to_validate);

			// show warning messages
			if (g_validator.WarningMessages.Count != 0)
			{
				Record message_record = new Record(1);
				MessageResult view_warnings_result;

				// ask the user if they want to see the warnings that were raised
				message_record.SetString(0, String.Format("{0} warnings were raised when validating your installation.\n\nDo you want to view them (Recommended)?",
					g_validator.WarningMessages.Count));
				view_warnings_result = session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.YesNo, message_record);
				if(MessageResult.Yes == view_warnings_result)
				{
					// display each warning in a seperate dialog
					foreach (var message in g_validator.WarningMessages)
					{
						session.Log("InstallValidator: {0}", message);
						message_record.SetString(0, message);
						session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.OK, message_record);
					}

					// see if the user still wants to continue the installation
					message_record.SetString(0, "Do you want to continue the installation?");
					view_warnings_result = session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.YesNo, message_record);

					// if the user wants to cancel the installation return user exit
					if (view_warnings_result == MessageResult.No)
						return ActionResult.UserExit;
				}
			}

			// show error messages
			if (g_validator.ErrorMessages.Count != 0)
			{
				Record record = new Record(g_validator.ErrorMessages.Count);
				
				// there should only ever be one error message since it returns immediately after finding one
				// but just in case, log and display all in the list
				int index = 0;
				foreach (var message in g_validator.ErrorMessages)
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
