#addin nuget:?package=Cake.Git
#reference "../../bin/Cake.Gyp/Debug/Cake.Gyp.dll"
#load "Common/Common.cake"

Task("CloneGyp")
	.WithCriteria(() => !DirectoryExists(GetRootedPath("external/gyp")))
	.Does(()=>
	{
		EnsureDirectoryExists(GetRootedPath("external"));
		Information("Cloning Gyp to external/gyp");

		GitClone("https://chromium.googlesource.com/external/gyp", GetRootedPath("external/gyp"));
	});

Task("CollectSourceFiles")
	.IsDependentOn("CloneGyp")
	.Does(()=>
	{
		var filePatterns = new []
		{
			"*.cpp",
			"*.c",
			"*.h",
			"*.hpp",
			"*.inl",
			"*.txt",
			"*.def",
			"*.rc",
			"*.xml"
		};
		var projectRoot = GetRootedPath(string.Empty);

		Information("Collecting Blam source files");
		GenerateSourceInclude(projectRoot,
			"source/blam",
			"build",
			filePatterns,
			GetRootedPath("build/Blam.generated.gypi"),
			"blamSourceFiles");

		Information("Collecting Yelo source files");
		GenerateSourceInclude(projectRoot,
			"source/yelo",
			"build",
			filePatterns,
			GetRootedPath("build/Yelo.generated.gypi"),
			"yeloSourceFiles");

		Information("Collecting CheApe source files");
		GenerateSourceInclude(projectRoot,
			"source/cheape",
			"build",
			filePatterns,
			GetRootedPath("build/CheApe.generated.gypi"),
			"cheapeSourceFiles");

		Information("Collecting Halo source files");
		GenerateSourceInclude(projectRoot,
			"source/halo",
			"build",
			filePatterns,
			GetRootedPath("build/Halo.generated.gypi"),
			"haloSourceFiles");

		Information("Collecting GWEN source files");
		GenerateSourceInclude(projectRoot,
			"external/GWEN",
			"build",
			filePatterns,
			GetRootedPath("build/GWEN.generated.gypi"),
			"gwenSourceFiles");

		Information("Collecting GameSpyOpen source files");
		GenerateSourceInclude(projectRoot,
			"external/GameSpyOpen",
			"build",
			filePatterns,
			GetRootedPath("build/GameSpyOpen.generated.gypi"),
			"gameSpyOpenSourceFiles");

		Information("Collecting Mongoose source files");
		GenerateSourceInclude(projectRoot,
			"external/Mongoose",
			"build",
			filePatterns,
			GetRootedPath("build/Mongoose.generated.gypi"),
			"mongooseSourceFiles");

		Information("Collecting 7zip source files");
		GenerateSourceInclude(projectRoot,
			"external/7zip",
			"build",
			filePatterns,
			GetRootedPath("build/7zip.generated.gypi"),
			"sevenZipSourceFiles");

		Information("Collecting Zlib source files");
		GenerateSourceInclude(projectRoot,
			"external/Zlib",
			"build",
			filePatterns,
			GetRootedPath("build/Zlib.generated.gypi"),
			"zlibSourceFiles");

		Information("Collecting TinyXml source files");
		GenerateSourceInclude(projectRoot,
			"external/TinyXml",
			"build",
			filePatterns,
			GetRootedPath("build/TinyXml.generated.gypi"),
			"tinyXmlSourceFiles");
	});

Task("GenerateProjectsVS")
	.WithCriteria(IsRunningOnWindows)
	.IsDependentOn("CollectSourceFiles")
	.Does(() =>
	{
		CakeExecuteScript("GenerateProjectsVS.cake", CreateCakeSettings());
	});

Task("GenerateProjects")
	.IsDependentOn("GenerateProjectsVS");

RunTarget("GenerateProjects");