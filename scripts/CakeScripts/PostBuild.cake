#load "Common/Common.cake"

using Microsoft.Win32;

var configuration = Argument<string>("BuildConfiguration");
var hceDirectory = (string)Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Microsoft Games\\Halo CE", "EXE Path", null);

Task("ValidateHceDirectory")
    .Does(() =>
    {
        if(string.IsNullOrWhiteSpace(hceDirectory))
        {
            throw new Exception("Failed to find the Halo CE directory key in the registry");
        }
        if(!DirectoryExists(hceDirectory))
        {
            throw new Exception(string.Format("The Halo CE directory found in the registry does not exist: {0}", hceDirectory));
        }

        Information("Halo CE directory found at '{0}'", hceDirectory);
    });

Task("PostBuildClient")
    .IsDependentOn("ValidateHceDirectory")
    .WithCriteria(configuration.Contains("Client"))
    .Does(() =>
    {
        CopyFileToDirectory(GetRootedPath(string.Format(@"bin/halo/{0}/dinput8.dll", configuration)), hceDirectory);
    });

Task("PostBuildServer")
    .IsDependentOn("ValidateHceDirectory")
    .WithCriteria(configuration.Contains("Server"))
    .Does(() =>
    {
        CopyFileToDirectory(GetRootedPath(string.Format(@"bin/halo/{0}/OpenSauceDedi.dll", configuration)), hceDirectory);
    });

Task("PostBuild")
    .IsDependentOn("PostBuildClient")
    .IsDependentOn("PostBuildServer");

RunTarget("PostBuild");