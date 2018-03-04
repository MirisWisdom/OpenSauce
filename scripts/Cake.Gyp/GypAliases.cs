using System.Collections.Generic;
using Cake.Core;
using Cake.Core.Annotations;
using Cake.Core.IO;

namespace Cake.Gyp
{
	[CakeAliasCategory("Gyp")]
	public static class AddinAliases
	{
		[CakeMethodAlias]
		public static void GenerateProject(this ICakeContext context, DirectoryPath gypRoot, FilePath gypDefinition, GypSettings settings)
		{
			new GypRunner(context.FileSystem, context.Environment, context.ProcessRunner, context.Tools, gypRoot).Run(gypDefinition, settings);
		}

		[CakeMethodAlias]
		public static void GenerateSourceInclude(this ICakeContext context,
			DirectoryPath projectRoot,
			DirectoryPath fromDirectory,
			DirectoryPath relativeTo,
			IEnumerable<string> filePatterns,
			FilePath outputFile,
			string outputVariable)
		{
			new GypSourceIncludeBuilder(context.FileSystem, projectRoot).GenerateSourceInclude(fromDirectory, relativeTo, filePatterns, outputFile, outputVariable);
		}
	}
}