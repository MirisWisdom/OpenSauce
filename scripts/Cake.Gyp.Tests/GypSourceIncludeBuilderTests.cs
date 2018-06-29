using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Cake.Core;
using Cake.Core.IO;
using Cake.Testing;
using FluentAssertions;
using Newtonsoft.Json.Linq;
using Xunit;

namespace Cake.Gyp.Tests
{
	public class GypSourceIncludeBuilderTests
	{
		private readonly DirectoryPath mRootDirectory = DirectoryPath.FromString("/anyRootDirectory");
		private readonly DirectoryPath mSourceDirectory = DirectoryPath.FromString("anySourceDirectory");
		private readonly FilePath mOutputFile = FilePath.FromString("anyOutputFile.generated.gypi");
		private readonly FakeEnvironment mEnvironment;
		private readonly FakeFileSystem mFileSystem;

		public GypSourceIncludeBuilderTests()
		{
			mEnvironment = new FakeEnvironment(PlatformFamily.Unknown)
			{
				WorkingDirectory = DirectoryPath.FromString("/anyDirectory")
			};

			mFileSystem = new FakeFileSystem(mEnvironment);
			mFileSystem.CreateFile(mRootDirectory.Combine(mSourceDirectory).Combine(DirectoryPath.FromString("anySubDirectory")).CombineWithFilePath(FilePath.FromString("anyMatchingFile.anyExtension")));
			mFileSystem.CreateFile(mRootDirectory.Combine(mSourceDirectory).CombineWithFilePath(FilePath.FromString("anyMatchingFile.anyOtherExtension")));
			mFileSystem.CreateFile(mRootDirectory.Combine(mSourceDirectory).CombineWithFilePath(FilePath.FromString("anyNonMatchingFile.anyNonMatchingExtension")));
		}

		private static IEnumerable<string> GetIncludedFiles(IFileSystem fileSystem, FilePath outputFile)
		{
			var includeFile = fileSystem.GetFile(outputFile);
			using (var includeFileReader = new StreamReader(includeFile.Open(FileMode.Open, FileAccess.Read)))
			{
				dynamic includeFileObject = JObject.Parse(includeFileReader.ReadToEnd());
				JArray values = includeFileObject.variables.anyVariableName;
				return values.Select(entry => entry.Value<string>());
			}
		}

		[Theory]
		[InlineData(false, true)]
		[InlineData(true, false)]
		public void Ctor_WithInvalidParameters_Throws(bool nullFileSystem, bool nullRootDirectory)
		{
			Action constructAction = () => new GypSourceIncludeBuilder(
				nullFileSystem
					? null
					: mFileSystem,
				nullRootDirectory
					? null
					: DirectoryPath.FromString("anyDirectory"));

			constructAction.Should().Throw<ArgumentException>();
		}

		[Fact]
		public void BuildGypInclude_WithUnPopulatedDirectory_WritesEmptyGypInclude()
		{
			var fileSystem = new FakeFileSystem(mEnvironment);
			var includeBuilder = new GypSourceIncludeBuilder(fileSystem, mRootDirectory);

			includeBuilder.GenerateSourceInclude(
				mSourceDirectory,
				mSourceDirectory,
				new[] { "*.anyExtension", "*.anyOtherExtension" },
				mOutputFile,
				"anyVariableName");

			GetIncludedFiles(fileSystem, mOutputFile).Should().BeEmpty();
		}

		[Fact]
		public void BuildGypInclude_WithPopulatedDirectory_WritesSourceFilesToGypIncludeFile()
		{
			var includeBuilder = new GypSourceIncludeBuilder(mFileSystem, mRootDirectory);

			includeBuilder.GenerateSourceInclude(
				mSourceDirectory,
				mSourceDirectory,
				new[] { "*.anyExtension", "*.anyOtherExtension" },
				mOutputFile,
				"anyVariableName");

			GetIncludedFiles(mFileSystem, mOutputFile).Should().Equal("anySubDirectory/anyMatchingFile.anyExtension", "anyMatchingFile.anyOtherExtension");
		}

		[Fact]
		public void BuildGypInclude_WithRelativeDirectoryTraversal_WritesSourceFilesToGypIncludeFile()
		{
			var includeBuilder = new GypSourceIncludeBuilder(mFileSystem, mRootDirectory);

			includeBuilder.GenerateSourceInclude(
				mSourceDirectory,
				DirectoryPath.FromString("anyOtherDirectory"),
				new[] { "*.anyExtension", "*.anyOtherExtension" },
				mOutputFile,
				"anyVariableName");

			GetIncludedFiles(mFileSystem, mOutputFile).Should().Equal("../anySourceDirectory/anySubDirectory/anyMatchingFile.anyExtension", "../anySourceDirectory/anyMatchingFile.anyOtherExtension");
		}
	}
}