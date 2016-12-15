using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Deployment.WindowsInstaller;

namespace WebLauncher
{
	public class CustomActions
	{
		[CustomAction]
		public static ActionResult LaunchWebsite(Session session)
		{
			session.Log("Begin WebLauncher");

			string URL = session["WEBLAUNCHER_URL"];

			System.Diagnostics.Process.Start(URL);

			return ActionResult.Success;
		}
	}
}
