#load "Common/Common.cake"
#load "Common/Build.cake"
#load "Common/Gyp.cake"
#reference "../../bin/BuildUtilities/Debug/BuildUtilities.dll"
#reference "../../bin/Cake.Gyp/Debug/Cake.Gyp.dll"

using System.Xml.Linq;
using System.Xml.XPath;
using System.Xml;
using System.IO;
using BuildUtilities.VisualStudio;

Task("InitialiseMSBuildLocation")
	.Does(() =>
	{
		ProjectUtilities.InitialiseMSBuildLocations();
	});

Task("CollectSourceFiles")
	.IsDependentOn("CloneGyp")
	.Does(()=>
	{
		var projectRoot = GetRootedPath(string.Empty);

		Information("Collecting Blam source files");
		GenerateSourceInclude(projectRoot,
			"source/blam",
			"build",
			BuildFilePatterns,
			GetRootedPath("build/Blam.generated.gypi"),
			"sourceFilesBlam");

		Information("Collecting Yelo source files");
		GenerateSourceInclude(projectRoot,
			"source/yelo",
			"build",
			BuildFilePatterns,
			GetRootedPath("build/Yelo.generated.gypi"),
			"sourceFilesYelo");

		Information("Collecting CheApe source files");
		GenerateSourceInclude(projectRoot,
			"source/cheape",
			"build",
			BuildFilePatterns,
			GetRootedPath("build/CheApe.generated.gypi"),
			"sourceFilesCheApe");

		Information("Collecting Halo source files");
		GenerateSourceInclude(projectRoot,
			"source/halo",
			"build",
			BuildFilePatterns,
			GetRootedPath("build/Halo.generated.gypi"),
			"sourceFilesHalo");
	});

Task("GenerateProjectOpenSauce")
	.IsDependentOn("CollectSourceFiles")
	.Does(()=>
	{
		Information("Building OpenSauce project");

		GenerateProject(
			GetRootedPath("external/gyp"),
			"OpenSauce.gyp",
			new GypSettings
			{
				WorkingDirectory = GetRootedPath("build"),
				OutputPlatform = GypOutputPlatform.VisualStudio2017,
				OutputDirectory = GetRootedPath("build/msvs2017_generated"),
			}
		);
	});

Task("SetProperties")
	.IsDependentOn("InitialiseMSBuildLocation")
	.Does(()=>
	{
		var buildDateValue = "$([System.DateTime]::Now.ToString(\"yyyy.MM.dd\"))";
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/blam.vcxproj"), "OpenSauceBuildDate", buildDateValue);
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/yelo.vcxproj"), "OpenSauceBuildDate", buildDateValue);
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/halo.vcxproj"), "OpenSauceBuildDate", buildDateValue);
	});

Task("GenerateProjects")
	.IsDependentOn("InitialiseMSBuildLocation")
	.IsDependentOn("GenerateProjectOpenSauce")
	.IsDependentOn("SetProperties")
	.Does(()=>
	{
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/blam.vcxproj"), "RunCodeAnalysis", "true");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/msvs2017_generated/blam.vcxproj.filters"), "../source/blam");
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/yelo.vcxproj"), "RunCodeAnalysis", "true");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/msvs2017_generated/yelo.vcxproj.filters"), "../source/yelo");
		ProjectUtilities.SetPropertyInFile(GetRootedPath("build/msvs2017_generated/halo.vcxproj"), "RunCodeAnalysis", "true");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/msvs2017_generated/halo.vcxproj.filters"), "../source/halo");

		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/7zip/7zip.vcxproj.filters"), "../external/7zip");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/TinyXml/TinyXml.vcxproj.filters"), "../external/TinyXml");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/GameSpyOpen/GameSpyOpen.vcxproj.filters"), "../external/GameSpyOpen");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/GWEN/GWEN.vcxproj.filters"), "../external/GWEN");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/Mongoose/Mongoose.vcxproj.filters"), "../external/Mongoose");
		ProjectUtilities.TrimPathFromFiltersInFile(GetRootedPath("build/Zlib/Zlib.vcxproj.filters"), "../external/Zlib");
	});

RunTarget("GenerateProjects");