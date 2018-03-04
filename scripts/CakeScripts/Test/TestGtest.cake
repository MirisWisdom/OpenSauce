#reference "../../../bin/Cake.GoogleTest/Debug/Cake.GoogleTest.dll"
#reference "../../../bin/Cake.OpenCppCoverage/Debug/Cake.OpenCppCoverage.dll"
#load "../Common/Common.cake"

using Cake.OpenCppCoverage;

var executable = Argument<string>("Executable");
var outputFile = Argument<string>("Output");
var withCoverage = HasArgument("WithCoverage");
var coverageOutput = Argument<string>("CoverageOutputDirectory", string.Empty);
var coverageSources = Argument<string>("CoverageSources", "*");
var coverageModules = Argument<string>("CoverageModules", "*");

Task("Test")
	.Does(() =>
	{
		bool result;
		if(withCoverage && IsRunningOnWindows())
		{
			var coverageOutputDirectory = DirectoryPath.FromString(coverageOutput);
			var processRunner = GetOpenCppCoverageRunner(new OpenCppCoverageSettings
			{
				Sources = coverageSources.Split(','),
				Modules = coverageModules.Split(','),
				ExportTypes = new [] { new Cake.OpenCppCoverage.Export.HtmlExportType(GetRootedPath(coverageOutputDirectory)) },
				ToolPath = Environment.ExpandEnvironmentVariables("%PROGRAMFILES%/OpenCppCoverage/OpenCppCoverage.exe")
			});

			result = GoogleTestRun(processRunner, GetRootedPath(executable), new GoogleTestSettings
			{
				Output = new XmlTestOutput(GetRootedPath(outputFile))
			});
		}
		else
		{
			result = GoogleTestRun(GetRootedPath(executable), new GoogleTestSettings
			{
				Output = new XmlTestOutput(GetRootedPath(outputFile))
			});
		}

		if(!result)
		{
			Error("A failure occurred when running tests");
		}

		if(BuildSystem.IsRunningOnTeamCity)
		{
			BuildSystem.TeamCity.ImportData("gtest", GetRootedPath(outputFile));
		}
	});

RunTarget("Test");