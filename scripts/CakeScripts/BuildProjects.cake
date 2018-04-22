#load "Common/Common.cake"
#addin nuget:?package=Cake.Git

var cleanDirectories = new []
{
	"bin/7zip/",
	"bin/GameSpyOpen/",
	"bin/GWEN/",
	"bin/Mongoose/",
	"bin/TinyXml/",
	"bin/Zlib/"
};

CakeSettings CreateBuildCakeSettings(string project, string configuration)
{
    var settings = CreateCakeSettings();
    settings.Arguments.Add("Solution", string.Format("build/{0}/{0}.sln", project));
    settings.Arguments.Add("Target", "Build");
    settings.Arguments.Add("Configuration", configuration);
    settings.Arguments.Add("Platform", "Win32");
    return settings;
}

Task("Clean")
    .Does(() =>
    {
		foreach(var directory in cleanDirectories)
		{
			CleanDirectory(GetRootedPath(directory));
		}
    });

Task("Build7zipDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building 7zip Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("7zip", "Debug"));
	});

Task("Build7zipRelease")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building 7zip Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("7zip", "Release"));
	});

Task("BuildGameSpyOpenDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building GameSpyOpen Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("GameSpyOpen", "Debug"));
	});

Task("BuildGameSpyOpenRelease")
    .IsDependentOn("Clean")
	.Does(() =>
    {
		Information("Building GameSpyOpen Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("GameSpyOpen", "Release"));
	});

Task("BuildGWENDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building GWEN Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("GWEN", "Debug"));
	});

Task("BuildGWENRelease")
    .IsDependentOn("Clean")
	.Does(() =>
    {
		Information("Building GWEN Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("GWEN", "Release"));
	});

Task("BuildMongooseDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building Mongoose Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("Mongoose", "Debug"));
	});

Task("BuildMongooseRelease")
    .IsDependentOn("Clean")
	.Does(() =>
    {
		Information("Building Mongoose Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("Mongoose", "Release"));
	});

Task("BuildTinyXmlDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building TinyXml Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("TinyXml", "Debug"));
	});

Task("BuildTinyXmlRelease")
    .IsDependentOn("Clean")
	.Does(() =>
    {
		Information("Building TinyXml Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("TinyXml", "Release"));
	});

Task("BuildZlibDebug")
    .IsDependentOn("Clean")
	.Does(() =>
	{
		Information("Building Zlib Debug");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("Zlib", "Debug"));
	});

Task("BuildZlibRelease")
    .IsDependentOn("Clean")
	.Does(() =>
    {
		Information("Building Zlib Release");
		CakeExecuteScript("Build/BuildSolution.cake", CreateBuildCakeSettings("Zlib", "Release"));
	});


Task("BuildProjects")
	.IsDependentOn("Build7zipDebug")
	.IsDependentOn("Build7zipRelease")
	.IsDependentOn("BuildGameSpyOpenDebug")
	.IsDependentOn("BuildGameSpyOpenRelease")
	.IsDependentOn("BuildGWENDebug")
	.IsDependentOn("BuildGWENRelease")
	.IsDependentOn("BuildMongooseDebug")
	.IsDependentOn("BuildMongooseRelease")
	.IsDependentOn("BuildTinyXmlDebug")
	.IsDependentOn("BuildTinyXmlRelease")
	.IsDependentOn("BuildZlibDebug")
	.IsDependentOn("BuildZlibRelease");

RunTarget("BuildProjects");