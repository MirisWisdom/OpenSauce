using System;
using Cake.Core;
using Cake.Core.IO;
using Cake.Core.Tooling;

namespace Cake.Gyp
{
	public enum GypOutputPlatform
	{
		VisualStudio2013,
		VisualStudio2015,
		VisualStudio2017,
		Make,
		XCode,
		Ninja
	}

	public sealed class GypSettings : ToolSettings
	{
		public DirectoryPath OutputDirectory;
		public GypOutputPlatform OutputPlatform;

		public ProcessArgumentBuilder GetArguments()
		{
			var argumentBuilder = new ProcessArgumentBuilder();

			switch (OutputPlatform)
			{
				case GypOutputPlatform.VisualStudio2013:
					argumentBuilder.AppendSwitch("-f", "msvs");
					argumentBuilder.AppendSwitch("-G", "msvs_version=2013");
					break;
				case GypOutputPlatform.VisualStudio2015:
					argumentBuilder.AppendSwitch("-f", "msvs");
					argumentBuilder.AppendSwitch("-G", "msvs_version=2015");
					break;
				case GypOutputPlatform.VisualStudio2017:
					argumentBuilder.AppendSwitch("-f", "msvs");
					argumentBuilder.AppendSwitch("-G", "msvs_version=2017");
					break;
				case GypOutputPlatform.Make:
					argumentBuilder.AppendSwitch("-f", "make");
					break;
				case GypOutputPlatform.XCode:
					argumentBuilder.AppendSwitch("-f", "xcode");
					break;
				case GypOutputPlatform.Ninja:
					argumentBuilder.AppendSwitch("-f", "ninja");
					break;
				default:
					throw new ArgumentOutOfRangeException();
			}

			argumentBuilder.Append("--generator-output=\"{0}\"", OutputDirectory);

			return argumentBuilder;
		}
	}
}
