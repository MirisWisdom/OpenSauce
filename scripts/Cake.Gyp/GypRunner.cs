using System;
using System.Collections.Generic;
using System.IO;
using Cake.Core;
using Cake.Core.IO;
using Cake.Core.Tooling;

namespace Cake.Gyp
{
	public sealed class GypRunner : Tool<GypSettings>
	{
		private readonly FilePath mGypFilePath;

		public GypRunner(IFileSystem fileSystem, ICakeEnvironment environment, IProcessRunner processRunner, IToolLocator tools, DirectoryPath gypRoot)
			: base(fileSystem, environment, processRunner, tools)
		{
			if (tools == null)
			{
				throw new ArgumentNullException(nameof(tools));
			}
			if (gypRoot == null)
			{
				throw new ArgumentNullException(nameof(gypRoot));
			}

			mGypFilePath = gypRoot.CombineWithFilePath(FilePath.FromString("gyp_main.py"));
			if (!fileSystem.Exist(mGypFilePath))
			{
				throw new FileNotFoundException($"Could not find gyp in directory: {gypRoot.FullPath}");
			}
		}

		protected override string GetToolName()
		{
			return "Gyp";
		}

		protected override IEnumerable<string> GetToolExecutableNames()
		{
			return new[]
			{ 
				"python.exe",
				"python"
			};
		}

		public void Run(FilePath gypDefinition, GypSettings settings)
		{
			if (settings == null)
			{
				throw new ArgumentNullException(nameof(settings));
			}

			Run(settings, BuildGypArguments(gypDefinition, settings));
		}

		private ProcessArgumentBuilder BuildGypArguments(FilePath gypDefinition, GypSettings settings)
		{
			var arguments = new ProcessArgumentBuilder();
			arguments.AppendQuoted(mGypFilePath.FullPath);
			arguments.AppendQuoted(gypDefinition.FullPath);
			arguments.Append("--depth=.");
			arguments.Append(settings.GetArguments().Render());
			return arguments;
		}
	}
}