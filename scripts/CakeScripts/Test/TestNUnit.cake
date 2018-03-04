#tool "nuget:?package=NUnit.ConsoleRunner"
#load "../Common/Common.cake"

var assemblyPattern = Argument<string>("AssemblyPattern");

Task("Test")
	.Does(() =>
	{
		NUnit3(GetRootedPath(assemblyPattern));
	});

RunTarget("Test");
