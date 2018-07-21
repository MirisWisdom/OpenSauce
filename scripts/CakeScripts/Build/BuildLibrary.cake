#load "../Common/Build.cake"
#load "../Common/Gyp.cake"
#reference "../../../bin/Cake.Gyp/Debug/Cake.Gyp.dll"

var libraryName = Argument<string>("Library");

Task("CollectSourceFiles")
	.IsDependentOn("CloneGyp")
	.Does(()=>
	{
		Information($"Collecting {libraryName} source files");
		
		GenerateSourceInclude(
			GetRootedPath(string.Empty),
			$"external/{libraryName}",
			"build",
			BuildFilePatterns,
			GetRootedPath($"build/{libraryName}.generated.gypi"),
			$"sourceFiles{libraryName}");
	});

Task("GenerateProject")
	.IsDependentOn("CollectSourceFiles")
	.Does(()=>
	{
		Information($"Building {libraryName} project");

		GenerateProject(
			GetRootedPath("external/gyp"),
			$"{libraryName}.gyp",
			new GypSettings
			{
				WorkingDirectory = GetRootedPath("build"),
				OutputPlatform = GypOutputPlatform.VisualStudio2017,
				OutputDirectory = GetRootedPath($"build/{libraryName}"),
			}
		);
	});

Task("BuildDebug")
    .IsDependentOn("GenerateProject")
	.Does(() =>
	{
		Information($"Building {libraryName} Debug");
		
        CleanDirectory(GetRootedPath($"bin/{libraryName}/Debug"));
		CakeExecuteScript("../Build/BuildSolution.cake", CreateBuildCakeSettings(libraryName, "Debug"));
	});

Task("BuildRelease")
    .IsDependentOn("GenerateProject")
	.Does(() =>
	{
		Information($"Building {libraryName} Release");

        CleanDirectory(GetRootedPath($"bin/{libraryName}/Release"));
		CakeExecuteScript("../Build/BuildSolution.cake", CreateBuildCakeSettings(libraryName, "Release"));
	});

Task("Build")
	.IsDependentOn("BuildDebug")
	.IsDependentOn("BuildRelease");

RunTarget("Build");