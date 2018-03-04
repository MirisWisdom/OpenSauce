using Cake.Core.IO;
using FluentAssertions;
using Xunit;

namespace Cake.Gyp.Tests
{
	public class GypSettingsTests
	{
		private static readonly DirectoryPath sOutputDirectory = DirectoryPath.FromString("anyDirectory");

		public static object[] TestSettings => new object[]
		{
			new object[]
			{
				new GypSettings
				{
					OutputPlatform = GypOutputPlatform.Make,
					OutputDirectory = sOutputDirectory
				},
				"-f make --generator-output=\"anyDirectory\""
			},
			new object[]
			{
				new GypSettings
				{
					OutputPlatform = GypOutputPlatform.XCode,
					OutputDirectory = sOutputDirectory
				},
				"-f xcode --generator-output=\"anyDirectory\""
			},
			new object[]
			{
				new GypSettings
				{
					OutputPlatform = GypOutputPlatform.VisualStudio2013,
					OutputDirectory = sOutputDirectory
				},
				"-f msvs -G msvs_version=2013 --generator-output=\"anyDirectory\""
			},
			new object[]
			{
				new GypSettings
				{
					OutputPlatform = GypOutputPlatform.VisualStudio2015,
					OutputDirectory = sOutputDirectory
				},
				"-f msvs -G msvs_version=2015 --generator-output=\"anyDirectory\""
			},
			new object[]
			{
				new GypSettings
				{
					OutputPlatform = GypOutputPlatform.Ninja,
					OutputDirectory = sOutputDirectory
				},
				"-f ninja --generator-output=\"anyDirectory\""
			}
		};

		[Theory, MemberData(nameof(TestSettings))]
		public void GetArguments_WithParameterSet_ReturnsArguments(GypSettings settings, string expected)
		{
			var result = settings.GetArguments();

			result.Render().Should().Be(expected);
		}
	}
}