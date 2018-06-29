using System;
using System.IO;
using Cake.Core;
using Cake.Core.IO;
using Cake.Core.Tooling;
using Cake.Testing;
using FluentAssertions;
using NSubstitute;
using Xunit;

namespace Cake.Gyp.Tests
{
	public class GypRunnerTests
	{
		[Theory]
		[InlineData(false, false, false, false, true)]
		[InlineData(false, false, false, true, false)]
		[InlineData(false, false, true, false, false)]
		[InlineData(false, true, false, false, false)]
		[InlineData(true, false, false, false, false)]
		public void Ctor_WithNullParameters_Throws(bool nullFilesystem, bool nullEnvironment, bool nullProcessRunner, bool nullTools, bool nullGypRoot)
		{
			var fakeEnvironment = new FakeEnvironment(PlatformFamily.Unknown)
			{
				WorkingDirectory = "/anyDirectory"
			};

			Action constructAction = () =>
			{
				new GypRunner(
					nullFilesystem
						? null
						: new FakeFileSystem(fakeEnvironment),
					nullEnvironment
						? null
						: fakeEnvironment,
					nullProcessRunner
						? null
						: Substitute.For<IProcessRunner>(),
					nullTools
						? null
						: Substitute.For<IToolLocator>(),
					nullGypRoot
						? null
						: DirectoryPath.FromString("anyRoot"));
			};

			constructAction.Should().Throw<ArgumentNullException>();
		}

		[Fact]
		public void Ctor_WithIncorrectGypRoot_Throws()
		{
			Action constructAction = () => new GypRunnerBuilder()
				.WithGypRoot("nonExistingDirectory")
				.Build();

			constructAction.Should().Throw<FileNotFoundException>();
		}

		[Fact]
		public void Run_WithNullSettings_Throws()
		{
			var runner = new GypRunnerBuilder()
				.WithExistingFile(FilePath.FromString("gypDirectory/gyp_main.py"))
				.WithGypRoot(DirectoryPath.FromString("gypDirectory"))
				.Build();

			Action runAction = () => runner.Run(FilePath.FromString("any Directory/anyFilePath.gyp"), null);

			runAction.Should().Throw<ArgumentNullException>();
		}

		[Fact]
		public void Run_WhenToolDoesNotExist_Throws()
		{
			var gypDefinition = FilePath.FromString("any Directory/anyFilePath.gyp");
			var runner = new GypRunnerBuilder()
				.WithExistingFile(FilePath.FromString("gypDirectory/gyp_main.py"))
				.WithGypRoot(DirectoryPath.FromString("gypDirectory"))
				.WithExistingFile(gypDefinition)
				.Build();

			Action runAction = () => runner.Run(gypDefinition, new GypSettings());

			runAction.Should().Throw<CakeException>();
		}

		[Fact]
		public void Run_WithDefinitionAndSettings_LaunchesGyp()
		{
			var processRunner = Substitute.For<IProcessRunner>();
			var gypDefinition = FilePath.FromString("any Directory/anyFilePath.gyp");
			var runner = new GypRunnerBuilder()
				.WithExistingTool("python.exe", "python.exe")
				.WithExistingFile(FilePath.FromString("gypDirectory/gyp_main.py"))
				.WithGypRoot(DirectoryPath.FromString("gypDirectory"))
				.WithExistingFile(gypDefinition)
				.WithProcessRunner(processRunner)
				.Build();

			string arguments = null;
			processRunner.Start(Arg.Is<FilePath>(value => value.FullPath == "python.exe"), Arg.Any<ProcessSettings>())
			             .Returns(new FakeProcess())
			             .AndDoes(info => arguments = info.Arg<ProcessSettings>().Arguments.Render());
			runner.Run(gypDefinition, new GypSettings
			{
				OutputPlatform = GypOutputPlatform.VisualStudio2015,
				OutputDirectory = DirectoryPath.FromString("anyOutputDirectory")
			});

			arguments.Should().NotBeNull();
			arguments.Should().Be("\"gypDirectory/gyp_main.py\" \"any Directory/anyFilePath.gyp\" --depth=. -f msvs -G msvs_version=2015 --generator-output=\"anyOutputDirectory\"");
		}
	}
}