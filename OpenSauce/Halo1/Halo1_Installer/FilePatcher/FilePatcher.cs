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
		public struct PatchInstance
		{
			public string FileID;
			public string FilePath;
			public string OutputPath;
		};

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

			string patch_arguments = session["CustomActionData"];

			// check the custom action arguments have been set
			if ((patch_arguments == null) || (patch_arguments.Length == 0))
			{
				session.Log("FilePatcher: ERROR : CustomActionData not set");
				return ActionResult.Failure;
			}

			// split the patch arguments string, the count should be a multiple of 3
			string[] split_arguments = patch_arguments.Split(new char[] {';'}, StringSplitOptions.RemoveEmptyEntries);
			if((split_arguments.Length == 0) || ((split_arguments.Length % 3) > 0))
			{
				session.Log("Invalid number of arguments passed to the FilePatcher");
				return ActionResult.Failure;
			}

			// populate the patch list
			List<PatchInstance> files_to_patch = new List<PatchInstance>();
			for(int i = 0; i < split_arguments.Length; i+=3)
			{
				PatchInstance patch = new PatchInstance();

				patch.FileID = split_arguments[i];
				patch.FilePath = split_arguments[i + 1];
				patch.OutputPath = split_arguments[i + 2];

				files_to_patch.Add(patch);
			}

			// patch each file in order
			foreach(var patch_instance in files_to_patch)
			{
				// log the properties for debugging if necessary
				session.Log("LOG: FILEPATCHERID : " + patch_instance.FileID);
				session.Log("LOG: FILEPATCHERSRCFILE : " + patch_instance.FilePath);
				session.Log("LOG: FILEPATCHEROUTPATH : " + patch_instance.OutputPath);

				// patch the file
				ApplyPatch(patch_instance.FileID, patch_instance.FilePath, patch_instance.OutputPath);

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
			}

			return ActionResult.Success;
		}
	}
}
