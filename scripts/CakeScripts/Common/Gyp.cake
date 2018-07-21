#load "Common.cake"
#addin nuget:?package=Cake.Git
#reference "../../bin/Cake.Gyp/Debug/Cake.Gyp.dll"

Task("CloneGyp")
	.WithCriteria(() => !DirectoryExists(GetRootedPath("external/gyp")))
	.Does(()=>
	{
		EnsureDirectoryExists(GetRootedPath("external"));
		Information("Cloning Gyp to external/gyp");

		GitClone("https://chromium.googlesource.com/external/gyp", GetRootedPath("external/gyp"));
	});