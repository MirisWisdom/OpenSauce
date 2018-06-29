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

		public ProjectUtilitiesTests()
		{
			ProjectUtilities.InitialiseMSBuildLocations();
		}

		[Fact]
		public void DisableProjectInConfig_WithSolutionText_RemovesBuildEntriesForDebugConfigsInRelease()
		{
			var input = GetResourceAsString("BuildUtilities.Tests.VisualStudio.Resources.TestSolution.sln");
			var expected = GetResourceAsString("BuildUtilities.Tests.VisualStudio.Resources.SolutionWithoutDebugInRelease.sln");

			var result = ProjectUtilities.DisableProjectInConfig(input, "Release.*", Guid.Parse("{11081BA7-72F2-62FA-7BD7-F5070E6B529E}"));

			result.Should().Be(expected);
		}

		[Fact]
		public void SetRunCodeAnalysis_WithProject_SetsCodeAnalysisOption()
		{
			var project = new Project();

			ProjectUtilities.SetRunCodeAnalysis(project);

			project.GetPropertyValue("RunCodeAnalysis").Should().Be("true");
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
			var item = project.AddItem("CiInclude", @"some\Include.h").First();
			item.SetMetadataValue("Filter", mPathToRemove);

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			item.GetMetadata("Filter").Should().BeNull();
		}

		[Fact]
		public void TrimPathFromFilters_WithFilterMetadataStartingWithPath_RemovesThePathFromTheMetadata()
		{
			var project = new Project();
			var item = project.AddItem("CiInclude", @"some\Include.h").First();
			item.SetMetadataValue("Filter", Path.Combine(mPathToRemove, "subDirectory"));

			ProjectUtilities.TrimPathFromFilters(project, mPathToRemove);

			item.GetMetadataValue("Filter").Should().Be("subDirectory");
		}
	}
}