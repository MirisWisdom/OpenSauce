#tool "nuget:?package=xunit.runner.console"
#load "../Common/Common.cake"

var assemblyPattern = Argument<string>("AssemblyPattern");

Task("Test")
	.Does(() =>
	{
		XUnit2(GetRootedPath(assemblyPattern), new XUnit2Settings {
			NoAppDomain = true
		});
	});

RunTarget("Test");
