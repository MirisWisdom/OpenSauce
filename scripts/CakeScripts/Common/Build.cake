#load "Common.cake"

public static readonly string[] BuildFilePatterns = new []
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

CakeSettings CreateBuildCakeSettings(string project, string configuration)
{
    var settings = CreateCakeSettings();
    settings.Arguments.Add("Solution", string.Format("build/{0}/{0}.sln", project));
    settings.Arguments.Add("Target", "Build");
    settings.Arguments.Add("Configuration", configuration);
    settings.Arguments.Add("Platform", "Win32");
    return settings;
}