/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.Deployment.WindowsInstaller;

namespace InstallValidator
{
	public class CustomActions
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Runs the InstallValidator custom action. </summary>
		///
		/// <param name="session">	The installer session. </param>
		///
		/// <returns>	An ActionResult. </returns>
		[CustomAction]
		public static ActionResult InstallValidatorCA(Session session)
		{
			string validatorDefinition = session["INSTALLVALIDATORDEF"];
			string validatorPath = session["INSTALLVALIDATORPATH"];

			bool errorOccurred = false;

			bool continueInstallation = InstallValidatorCustomAction.InstallValidator(validatorDefinition,
				validatorPath,
				(value) =>
				{
					session.Log(value);
				},
				(value, messageType) =>
				{
					session.Log(value);

					MessageResult result = MessageResult.None;

					var record = new Record(1);
					record.SetString(0, value);

					switch(messageType)
					{
						// Present a warning message to the user
						case InstallValidatorCustomAction.MessageType.WarningOK:
							result = session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.OK, record);
							break;
						case InstallValidatorCustomAction.MessageType.WarningYesNo:
							result = session.Message(InstallMessage.Warning | (InstallMessage)(MessageIcon.Warning) | (InstallMessage)MessageButtons.YesNo, record);
							break;
						// Present an error message to the user
						case InstallValidatorCustomAction.MessageType.ErrorOK:
							result = session.Message(InstallMessage.Error | (InstallMessage)(MessageIcon.Error) | (InstallMessage)MessageButtons.OK, record);
							break;
					}

					return (result == MessageResult.Yes) || (result == MessageResult.OK);
				},
				(value, displayMessage) =>
				{
					session.Log(value);

					if (displayMessage)
					{
						// Present an error message to the user
						session.Message(InstallMessage.Error | (InstallMessage)(MessageIcon.Error) | (InstallMessage)MessageButtons.OK, new Record(value));
					}

					errorOccurred = true;
				}
			);

			return ((errorOccurred || !continueInstallation) ? ActionResult.Failure : ActionResult.Success);
		}
	}
}
