using System;
using System.IO;
using System.Linq;
using System.Reflection;
using BuildUtilities.VisualStudio;
using FluentAssertions;
using Microsoft.Build.Evaluation;
using Xunit;

namespace BuildUtilities.Tests.VisualStudio
{
	public class ProjectUtilitiesTests
	{
		public ProjectUtilitiesTests()
		{
			ProjectUtilities.InitialiseMSBuildLocations();
		}

		private const string mPathToRemove = @"any\Path";

		private static string GetResourceAsString(string resource)
		{
			using (var reader = new StreamReader(Assembly.GetExecutingAssembly().GetManifestResourceStream(resource)))
			{
				return reader.ReadToEnd()
					.Replace("\r", string.Empty)
					.Replace("\n", Environment.NewLine);
			}
		}

		[Theory]
		[InlineData(null)]
		[InlineData("")]
		[InlineData("\t")]
		public void SetProperty_NullOrWhitespaceName_Throws(string value)
		{
			Action setProperty = () => ProjectUtilities.SetProperty(new Project(), value, "anyValue");

			setProperty.Should().Throw<ArgumentException>();
		}

		[Fact]
		public void DisableProjectInConfig_WithSolutionText_RemovesBuildEntriesForDebugConfigsInRelease()
		{
			string input = GetResourceAsString("BuildUtilities.Tests.VisualStudio.Resources.TestSolution.sln");
			string expected = GetResourceAsString("BuildUtilities.Tests.VisualStudio.Resources.SolutionWithoutDebugInRelease.sln");

			string result = ProjectUtilities.DisableProjectInConfig(
				input,
				"Release.*",
				Guid.Parse("{11081BA7-72F2-62FA-7BD7-F5070E6B529E}"));

			result.Should().Be(expected);
		}

		[Fact]
		public void SetProperty_AnyPropertyAndNullValue_Throws()
		{
			var project = new Project();

			Action setProperty = () => ProjectUtilities.SetProperty(project, "anyProperty", null);

			setProperty.Should().Throw<ArgumentNullException>();
		}

		[Fact]
		public void SetProperty_AnyPropertyAndValue_SetsProperty()
		{
			var project = new Project();

			ProjectUtilities.SetProperty(project, "anyProperty", "anyValue");

			project.GetPropertyValue("anyProperty").Should().Be("anyValue");
		}

		[Fact]
		public void SetProperty_MultipleCalls_ChangesValue()
		{
			var project = new Project();

			ProjectUtilities.SetProperty(project, "anyProperty", "anyValue");
			ProjectUtilities.SetProperty(project, "anyProperty", "anyOtherValue");

			project.GetPropertyValue("anyProperty").Should().Be("anyOtherValue");
		}

		[Fact]
		public void SetProperty_NullProject_Throws()
		{
			Action setProperty = () => ProjectUtilities.SetProperty(null, "anyProperty", "anyValue");

			setProperty.Should().Throw<ArgumentNullException>();
		}

		[Fact]
		public void TrimPathFromFilters_WithFilterItemMatchingPath_RemovesTheFilterItem()
		{
			var project = new Project();
			project.AddItem("Filter", mPathToRemove);

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			project.Items.Should().BeEmpty();
		}

		[Fact]
		public void TrimPathFromFilters_WithFilterItemStartingWithPath_RemovesThePathFromTheFilterItem()
		{
			var project = new Project();
			project.AddItem("Filter", Path.Combine(mPathToRemove, "subDirectory"));

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			project.Items.First().EvaluatedInclude.Should().Be("subDirectory");
		}

		[Fact]
		public void TrimPathFromFilters_WithFilterMetadataMatchingPath_RemovesTheMetadata()
		{
			var project = new Project();
			ProjectItem item = project.AddItem("CiInclude", @"some\Include.h").First();
			item.SetMetadataValue("Filter", mPathToRemove);

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			item.GetMetadata("Filter").Should().BeNull();
		}

		[Fact]
		public void TrimPathFromFilters_WithFilterMetadataStartingWithPath_RemovesThePathFromTheMetadata()
		{
			var project = new Project();
			ProjectItem item = project.AddItem("CiInclude", @"some\Include.h").First();
			item.SetMetadataValue("Filter", Path.Combine(mPathToRemove, "subDirectory"));

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			item.GetMetadataValue("Filter").Should().Be("subDirectory");
		}
	}
}
