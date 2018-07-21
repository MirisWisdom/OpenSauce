#load "../Common/Common.cake"
#addin nuget:?package=Cake.Git

using Microsoft.Win32;
using System.Security.Principal;

ChocolateyUpgradeSettings GetInstallSettings()
{
	return new ChocolateyUpgradeSettings
	{
		ExecutionTimeout = 7200
	};
}

Task("TestIsElevated")
	.WithCriteria(IsRunningOnWindows)
	.Does(() =>
	{
		var identity = WindowsIdentity.GetCurrent();
		var principal = new WindowsPrincipal(identity);
		if(!principal.IsInRole(WindowsBuiltInRole.Administrator))
		{
			throw new UnauthorizedAccessException("The script is not running with administrator permissions");
		}
	});

Task("InstallChocolatey")
	.IsDependentOn("TestIsElevated")
	.WithCriteria(!HasEnvironmentVariable("ChocolateyInstall") || !DirectoryExists(@"C:\ProgramData\chocolatey"))
	.Does(() =>
	{
		StartProcess("powershell", "-Command iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))");
		Environment.SetEnvironmentVariable("Path", @"C:\ProgramData\chocolatey", EnvironmentVariableTarget.Process);
	});

Task("UpgradeChocolatey")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("chocolatey", GetInstallSettings());
	});

Task("UpgradeTortoise")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("tortoisehg", GetInstallSettings());
	});

Task("UpgradePython")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("python2", GetInstallSettings());
	});

Task("UpgradeCppCheck")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("cppcheck", GetInstallSettings());
	});

Task("UpgradeOpenCppCoverage")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("opencppcoverage", GetInstallSettings());
	});

Task("UpgradeVisualStudio2017")
	.IsDependentOn("InstallChocolatey")
	.WithCriteria(Registry.GetValue(@"HKEY_CLASSES_ROOT\VisualStudio.DTE.15.0", null, null) == null)
	.Does(() =>
	{
		ChocolateyUpgrade("visualstudio2017community", GetInstallSettings());
	});

Task("UpgradeDotNET")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("dotnet4.6.1", GetInstallSettings());
	});

Task("UpgradeWindowsSDK")
	.IsDependentOn("InstallChocolatey")
	.IsDependentOn("UpgradeDotNET")
	.IsDependentOn("UpgradeVisualStudio2017")
	.Does(() =>
	{
		ChocolateyUpgrade("windows-sdk-10.1", GetInstallSettings());
	});

Task("UpgradeAudacity")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("audacity", GetInstallSettings());
	});

Task("UpgradeBlender")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("blender", GetInstallSettings());
	});

Task("UpgradeKrita")
	.IsDependentOn("InstallChocolatey")
	.Does(() =>
	{
		ChocolateyUpgrade("krita", GetInstallSettings());
	});

Task("Engineer")
	.IsDependentOn("UpgradeChocolatey")
	.IsDependentOn("UpgradeTortoise")
	.IsDependentOn("UpgradePython")
	.IsDependentOn("UpgradeWindowsSDK")
	.IsDependentOn("UpgradeCppCheck")
	.IsDependentOn("UpgradeOpenCppCoverage")
	.IsDependentOn("UpgradeVisualStudio2017");

Task("ContentCreator")
	.IsDependentOn("UpgradeChocolatey")
	.IsDependentOn("UpgradeTortoise")
	.IsDependentOn("UpgradeAudacity")
	.IsDependentOn("UpgradeBlender")
	.IsDependentOn("UpgradeKrita");

Task("BuildAgent")
	.IsDependentOn("UpgradeChocolatey")
	.IsDependentOn("UpgradeTortoise")
	.IsDependentOn("UpgradePython")
	.IsDependentOn("UpgradeWindowsSDK")
	.IsDependentOn("UpgradeCppCheck")
	.IsDependentOn("UpgradeOpenCppCoverage")
	.IsDependentOn("UpgradeVisualStudio2017");

RunTarget(Argument("DeveloperType", "Engineer"));