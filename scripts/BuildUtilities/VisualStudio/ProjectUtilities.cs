using System;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using Microsoft.Build.Construction;
using Microsoft.Build.Evaluation;
using Microsoft.Build.Locator;

namespace BuildUtilities.VisualStudio
{
	public static class ProjectUtilities
	{
		private static bool sBuildLocationInitialised;

		public static void InitialiseMSBuildLocations()
		{
			if (sBuildLocationInitialised == false)
			{
				MSBuildLocator.RegisterDefaults();
				sBuildLocationInitialised = true;
			}
		}

		// NOTE: There is no obvious way to disable build configurations using Gyp these allow us to remove project entries from a solution file

		public static Guid GetProjectGuidByName(string solutionPath, string projectName)
		{
			ProjectInSolution project = SolutionFile.Parse(solutionPath)
				.ProjectsInOrder
				.FirstOrDefault(entry => entry.ProjectName == projectName);
			if (project == null)
			{
				throw new ArgumentException($"Project not found in solution: {projectName}", nameof(projectName));
			}

			return Guid.Parse(project.ProjectGuid);
		}

		public static void DisableProjectInConfigInSolutionFile(
			string solutionPath,
			string configurationRegex,
			Guid projectGuid)
		{
			string solutionString = File.ReadAllText(solutionPath);
			File.WriteAllText(solutionPath, DisableProjectInConfig(solutionString, configurationRegex, projectGuid));
		}

		public static string DisableProjectInConfig(string solutionString, string configurationRegex, Guid projectGuid)
		{
			string[] lines = solutionString.Replace("\r", string.Empty)
				.Replace("\n", Environment.NewLine)
				.Split(
					new[]
					{
						Environment.NewLine
					},
					StringSplitOptions.None);
			string guidString = projectGuid.ToString("B").ToUpper();
			return string.Join(
				Environment.NewLine,
				lines.Where(line => !Regex.IsMatch(line, $"\t\t{guidString}.{configurationRegex}.* = .*")));
		}

		public static void TrimPathFromFiltersInFile(string filtersFile, string pathToRemove)
		{
			var project = new Project(filtersFile);
			TrimPathFromFilters(project, pathToRemove);
			project.Save(filtersFile);
			ProjectCollection.GlobalProjectCollection.UnloadAllProjects();
		}

		public static void TrimPathFromFilters(Project project, string pathToRemove)
		{
			string formattedPath = pathToRemove.Replace(Path.AltDirectorySeparatorChar, Path.DirectorySeparatorChar);
			RemoveMatchingFilterItems(project, formattedPath);
			RemovePathFromFilterItems(project, formattedPath);
			RemovePathFromItemMetadata(project, formattedPath);
		}

		// NOTE: Gyp generates source paths relative to the supplied gyp definition which doesnt suit our needs
		// and results in broken filters. These functions remove bad filter entries and rebase source locations.

		private static void RemoveMatchingFilterItems(Project project, string path)
		{
			string[] directories = path.Split(Path.DirectorySeparatorChar);

			string currentPath = string.Empty;
			foreach (string directory in directories)
			{
				currentPath = Path.Combine(currentPath, directory);
				project.RemoveItems(project.GetItemsByEvaluatedInclude(currentPath));
			}
		}

		private static void RemovePathFromFilterItems(Project project, string path)
		{
			foreach (ProjectItem item in project.Items.Where(entry => entry.ItemType == "Filter"))
			{
				item.UnevaluatedInclude = item.UnevaluatedInclude == path
					? string.Empty
					: item.UnevaluatedInclude.Replace(path + Path.DirectorySeparatorChar, string.Empty);
			}
		}

		private static void RemovePathFromItemMetadata(Project project, string path)
		{
			foreach (ProjectItem item in project.Items)
			{
				ProjectMetadata metadata = item.GetMetadata("Filter");
				if (metadata == null)
				{
					continue;
				}

				if (metadata.UnevaluatedValue == path)
				{
					item.RemoveMetadata("Filter");
				}
				else
				{
					metadata.UnevaluatedValue = metadata.UnevaluatedValue == path
						? string.Empty
						: metadata.UnevaluatedValue.Replace(path + Path.DirectorySeparatorChar, string.Empty);
				}
			}
		}

		// NOTE: There is no obvious way to set properties using Gyp so this is used to set them manually

		public static void SetProperty(Project project, string property, string value)
		{
			if (project == null)
			{
				throw new ArgumentNullException(nameof(project));
			}

			if (string.IsNullOrWhiteSpace(property))
			{
				throw new ArgumentException("Value cannot be null or whitespace.", nameof(property));
			}

			if (value == null)
			{
				throw new ArgumentNullException(nameof(value));
			}

			project.SetProperty(property, value);
		}

		public static void SetPropertyInFile(string projectFile, string property, string value)
		{
			var project = new Project(projectFile);
			SetProperty(project, property, value);
			project.Save(projectFile);
			ProjectCollection.GlobalProjectCollection.UnloadAllProjects();
		}
	}
}
