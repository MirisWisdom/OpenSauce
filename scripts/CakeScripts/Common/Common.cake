public static string GetRootedPath(string projectRelativePath)
{
	var projectRoot = Environment.GetEnvironmentVariable("ProjectRoot");
	if(string.IsNullOrWhiteSpace(projectRoot))
	{
		throw new CakeException("The ProjectRoot environment variable has not been set");
	}

	var fullPath = System.IO.Path.GetFullPath(projectRoot);
	if(!System.IO.Directory.Exists(fullPath))
	{
		throw new CakeException("The ProjectRoot directory does not exist: " + fullPath);
	}

	// NOTE: Using \x005C as a workaround for a bug in Cake
	// https://github.com/cake-build/cake/issues/1167
	return PathCombine(fullPath, projectRelativePath).Replace("\x005C", "/");
}

public static FilePath GetRootedPath(FilePath projectRelativePath)
{
	return FilePath.FromString(GetRootedPath(projectRelativePath.ToString()));
}

public static DirectoryPath GetRootedPath(DirectoryPath projectRelativePath)
{
	return DirectoryPath.FromString(GetRootedPath(projectRelativePath.ToString()));
}

public static string PathCombine(params string[] parts)
{
	return System.IO.Path.Combine(parts);
}

public CakeSettings CreateCakeSettings()
{
	return new CakeSettings
	{
		ToolPath = GetRootedPath("scripts/tools/Cake/Cake.exe"),
		Arguments = new Dictionary<string, string>
		{
			{ "paths_tools", GetRootedPath("scripts/tools") }
		},
		EnvironmentVariables = new Dictionary<string, string> { { "ProjectRoot", GetRootedPath("") } },
		WorkingDirectory = GetRootedPath("scripts/CakeScripts")
	};
}
