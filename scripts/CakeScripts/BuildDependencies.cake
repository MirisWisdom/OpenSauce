#load "Common/Common.cake"
#addin nuget:?package=Cake.Git

var cleanDirectories = new []
{
	"bin/BuildUtilities/",
	"bin/BuildUtilities.Tests/",
	"bin/Cake.Gyp/",
	"bin/Cake.Gyp.Tests/",
};

Task("BuildScripts")
	.Does(() =>
	{
		foreach(var directory in cleanDirectories)
		{
			CleanDirectory(GetRootedPath(directory));
		}

		var solutionPath = GetRootedPath("scripts/Scripts.sln");
		NuGetRestore(solutionPath);
		MSBuild(solutionPath, settings => 
		{
			settings.SetConfiguration("Debug Windows");
		});
	});

Task("TestScripts")
	.IsDependentOn("BuildScripts")
	.Does(() =>
	{
		var xunitBuildUtilitiesSettings = CreateCakeSettings();
		xunitBuildUtilitiesSettings.Arguments.Add("AssemblyPattern", "bin/BuildUtilities.Tests/Debug/*.Tests.dll");
		CakeExecuteScript("Test/TestXUnit.cake", xunitBuildUtilitiesSettings);

		var xunitCakeSettings = CreateCakeSettings();
		xunitCakeSettings.Arguments.Add("AssemblyPattern", "bin/Cake.*/Debug/*.Tests.dll");
		CakeExecuteScript("Test/TestXUnit.cake", xunitCakeSettings);
	});

Task("BuildDependencies")
	.IsDependentOn("TestScripts");

RunTarget("BuildDependencies");