/*
	Yelo: Open Sauce SDK
		Installer

	See license\OpenSauce\OpenSauce for specific license information
*/
using System;
using System.Collections.Generic;
using FilePatcher.Patcher;

namespace FilePatcher
{
	public class FilePatcherCustomAction
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	A patch instance container. </summary>
		public struct PatchInstance
		{
			public string FileID;
			public string FilePath;
			public string OutputPath;
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Patches a file. </summary>
		///
		/// <param name="actionData">	Information describing the action. </param>
		/// <param name="onLog">	 	The on log. </param>
		/// <param name="onError">   	The on error. </param>
		public static void FilePatcher(string actionData
			, Action<string> onLog
			, Action<string, bool> onError)
		{
			if((onLog == null) || (onError == null))
			{
				throw new ArgumentNullException("One or more required callbacks have not been supplied");
			}

			onLog("Begin FilePatcher");
			onLog(String.Format("Action data: {0}", actionData));

			// Check the custom action arguments have been set
			if (String.IsNullOrEmpty(actionData))
			{
				onError("FilePatcher: ERROR : CustomActionData not set", false);
				return;
			}

			// Split the patch arguments string, the count should be a multiple of 3
			var splitArguments = actionData.Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries);
			if ((splitArguments.Length == 0) || ((splitArguments.Length % 3) > 0))
			{
				onError("FilePatcher: ERROR : Invalid number of arguments passed to the FilePatcher", false);
				return;
			}

			// Populate the patch list
			var filesToPatch = new List<PatchInstance>();
			for (int i = 0; i < splitArguments.Length; i += 3)
			{
				filesToPatch.Add(new PatchInstance()
				{
					FileID = splitArguments[i],
					FilePath = splitArguments[i + 1],
					OutputPath = splitArguments[i + 2]
				});
			}

			// Create the patcher
			var filePatcher = new PatchApplier();
			filePatcher.ErrorOccurred +=
				(sender, args) =>
				{
					onError(args.Message, true);
				};

			// Patch each file
			foreach (var patchInstance in filesToPatch)
			{
				// Log the properties for debugging if necessary
				onLog("LOG: FILEPATCHERID : " + patchInstance.FileID);
				onLog("LOG: FILEPATCHERSRCFILE : " + patchInstance.FilePath);
				onLog("LOG: FILEPATCHEROUTPATH : " + patchInstance.OutputPath);

				// Patch the file
				if(!filePatcher.Patch(patchInstance.FileID
					, patchInstance.FilePath
					, patchInstance.OutputPath))
				{
					break;
				}
			}
		}
	}
}
