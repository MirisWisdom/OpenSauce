using Cake.Core;
using Cake.Core.IO;
using Cake.Core.Tooling;
using Cake.Testing;
using NSubstitute;

namespace Cake.Gyp.Tests
{
	public class GypRunnerBuilder
	{
		private readonly FakeEnvironment mEnvironment;
		private readonly FakeFileSystem mFileSystem;
		private readonly IToolLocator mToolLocator;
		private IProcessRunner mProcessRunner;
		private DirectoryPath mGypDirectory;

		public GypRunnerBuilder()
		{
			mEnvironment = new FakeEnvironment(PlatformFamily.Unknown)
			{
				WorkingDirectory = "/anyDirectory"
			};
			mFileSystem = new FakeFileSystem(mEnvironment);
			mProcessRunner = Substitute.For<IProcessRunner>();
			mToolLocator = Substitute.For<IToolLocator>();
		}

		public GypRunnerBuilder WithExistingFile(FilePath path)
		{
			mFileSystem.CreateFile(path);
			return this;
		}

		public GypRunnerBuilder WithExistingTool(string tool, FilePath path)
		{
			WithExistingFile(path);
			mToolLocator.Resolve(Arg.Is<string>(arg => arg == tool)).Returns(info => path);
			return this;
		}

		public GypRunnerBuilder WithProcessRunner(IProcessRunner runner)
		{
			mProcessRunner = runner;
			return this;
		}

		public GypRunnerBuilder WithGypRoot(DirectoryPath gypDirectory)
		{
			mGypDirectory = gypDirectory;
			return this;
		}

		public GypRunner Build()
		{
			return new GypRunner(mFileSystem, mEnvironment, mProcessRunner, mToolLocator, mGypDirectory);
		}
	}
}