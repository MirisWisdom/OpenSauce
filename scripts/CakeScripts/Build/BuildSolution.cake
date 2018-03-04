#load "../Common/Common.cake"

using System;

var solutionPath = Argument<string>("Solution");
var configuration = Argument<string>("Configuration");
var cleanDirectories = Argument<string>("CleanDirectories", string.Empty);
var platform = Argument<string>("Platform", "Win32");

Task("Clean")
	.Does(() =>
	{
		foreach(var directory in cleanDirectories.Split(new [] { ';' }, StringSplitOptions.RemoveEmptyEntries))
		{
			CleanDirectory(GetRootedPath(directory));
		}
	});

Task("RestoreNuGetPackages")
	.IsDependentOn("Clean")
	.Does(() =>
	{
		NuGetRestore(GetRootedPath(solutionPath));
	});

Task("Build")
	.IsDependentOn("RestoreNuGetPackages")
	.Does(() =>
	{
		PlatformTarget platformTarget;
		switch(platform)
		{
			case "Win32":
				platformTarget = PlatformTarget.Win32;
				break;
			case "x64":
				platformTarget = PlatformTarget.x64;
				break;
			default:
				throw new NotImplementedException();
		}

		var settings = new MSBuildSettings {
			Configuration = configuration,
			PlatformTarget = platformTarget
		};
		MSBuild(GetRootedPath(solutionPath), settings);
	});

RunTarget("Build");