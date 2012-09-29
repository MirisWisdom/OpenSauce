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

namespace FilePatcher
{
	public class CustomActions
	{
		private static Patcher g_file_patcher = null;

		public static void ApplyPatch(string file_id, string file_path, string output_path)
		{
			g_file_patcher = new Patcher(file_id);

			g_file_patcher.Patch(file_path, output_path);
		}

		[CustomAction]
		public static ActionResult PatchFile(Session session)
		{
			session.Log("Begin FilePatcher");

			// check the required properties have been set
			string file_to_patch = session["FILEPATCHERID"];
			string file_srcfile = session["FILEPATCHERSRCFILE"];
			string file_outpath = session["FILEPATCHEROUTPATH"];

			if ((file_to_patch == null) || (file_to_patch.Length == 0))
			{
				session.Log("FilePatcher: ERROR : FILEPATCHERID not set");
				return ActionResult.Failure;
			}
			if ((file_srcfile == null) || (file_srcfile.Length == 0))
			{
				session.Log("FilePatcher: ERROR : FILEPATCHERSRCFILE not set");
				return ActionResult.Failure;
			}
			if ((file_outpath == null) || (file_outpath.Length == 0))
			{
				session.Log("FilePatcher: ERROR : FILEPATCHEROUTPATH not set");
				return ActionResult.Failure;
			}

			// log the properties for debugging if necessary
			session.Log("LOG: FILEPATCHERID : " + file_to_patch);
			session.Log("LOG: FILEPATCHERSRCFILE : " + file_srcfile);
			session.Log("LOG: FILEPATCHEROUTPATH : " + file_outpath);

			// patch the file
			ApplyPatch(file_to_patch, file_srcfile, file_outpath);

			// show error messages
			if (g_file_patcher.ErrorMessages.Count != 0)
			{
				Record record = new Record(g_file_patcher.ErrorMessages.Count);

				// there should only ever be one error message since it returns immediately after
				// but just in case, log and display all in the list
				int index = 0;
				foreach (var message in g_file_patcher.ErrorMessages)
				{
					session.Log("FilePatcher: {0}", message);
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
