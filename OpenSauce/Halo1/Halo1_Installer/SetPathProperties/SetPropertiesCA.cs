/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using Microsoft.Deployment.WindowsInstaller;

namespace SetProperties
{
	public class CustomActions
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Runs the SetProperties custom action. </summary>
		///
		/// <param name="session">	The installer session. </param>
		///
		/// <returns>	An ActionResult. </returns>
		[CustomAction]
		public static ActionResult SetPropertiesCA(Session session)
		{
			string definition = session["SETPROPERTIESDEF"];

			var errorOccurred = false;

			var result = SetPropertiesCustomAction.SetProperties(definition,
				(value) =>
				{
					session.Log(value);
				},
				(property, value) =>
				{
					session[property] = value;
				},
				(property) =>
				{
					return session[property] as string;
				},
				(value) =>
				{
					errorOccurred = true;

					session.Log(value);
				}
			);

			return (!errorOccurred && result) ? ActionResult.Success : ActionResult.Failure;
		}
	}
}