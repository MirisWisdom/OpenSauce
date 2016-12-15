/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.Deployment.WindowsInstaller;

namespace FilePatcher
{
	public class CustomActions
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Runs the FilePatcher custom action. </summary>
		///
		/// <param name="session">	The installer session. </param>
		///
		/// <returns>	An ActionResult. </returns>
		[CustomAction]
		public static ActionResult FilePatcherCA(Session session)
		{
			string actionData = session["CustomActionData"];

			bool errorOccurred = false;

			FilePatcherCustomAction.FilePatcher(actionData,
				(value) =>
				{
					session.Log(value);
				},
				(value, showError) =>
				{
					session.Log(value);

					if(showError)
					{
						var record = new Record(1);
						record.SetString(0, value);

						// Present an error message to the user
						session.Message(InstallMessage.Error | (InstallMessage)(MessageIcon.Error) | (InstallMessage)MessageButtons.OK, record);
					}

					errorOccurred = true;
				}
			);

			return errorOccurred ? ActionResult.Failure : ActionResult.Success;
		}
	}
}
