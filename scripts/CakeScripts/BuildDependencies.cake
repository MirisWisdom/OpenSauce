#load "Common/Common.cake"

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

Task("Build7zip")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "7zip");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildGameSpyOpen")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "GameSpyOpen");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildGWEN")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "GWEN");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildMongoose")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "Mongoose");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildTinyXml")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "TinyXml");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildZlib")
	.Does(() =>
	{
		var buildSettings = CreateCakeSettings();
		buildSettings.Arguments.Add("Library", "Zlib");
		CakeExecuteScript("Build/BuildLibrary.cake", buildSettings);
	});

Task("BuildDependencies")
	.IsDependentOn("TestScripts")
	.IsDependentOn("Build7zip")
	.IsDependentOn("BuildGameSpyOpen")
	.IsDependentOn("BuildGWEN")
	.IsDependentOn("BuildMongoose")
	.IsDependentOn("BuildTinyXml")
	.IsDependentOn("BuildZlib");

RunTarget("BuildDependencies");