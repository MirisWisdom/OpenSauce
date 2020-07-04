/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.Deployment.WindowsInstaller;

namespace CPUQuery
{
	public class CustomActions
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Runs the CPUQuery custom action. </summary>
		///
		/// <param name="session">	The installer session. </param>
		///
		/// <returns>	An ActionResult. </returns>
		[CustomAction]
		public static ActionResult CPUQueryCA(Session session)
		{
			string actionData = session["CPUQUERYDATA"];

			bool errorOccurred = false;

			CPUQueryCustomAction.CPUQuery(actionData,
				(property, value) =>
				{
					session[property] = value;
				},
				(value) =>
				{
					session.Log(value);
				},
				(value) =>
				{
					session.Log(value);
					errorOccurred = true;
				}
			);

			return errorOccurred ? ActionResult.Failure : ActionResult.Success;
		}
	}
}
