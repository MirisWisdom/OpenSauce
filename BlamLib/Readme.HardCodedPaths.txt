While most of the data in BlamLib is loaded via a resource file, some parts of the library itself use 
some hardcoded paths. Here you'll find information reguarding these paths and what they're for.

In a perfect world, I would have used app settings for this stuff but there were more important things in 
BlamLib that I wanted to improve upon before public consumption. You live, you learn. There's always round 2.

BlamLib
	Program.cs
		* kProjectsPath: The root Projects directory which hosts the BlamLib codebase
		* StartupPath: As the name suggests, this is the path used as the working directory when the assembly is executed
		* DebugFile: The debug file path & name (where generic debug messages will be spewed out)
		* TracePath: The path where trace files will be saved to
		* GamesPath: The path where external game engine definitions are located. Currently game definitions are loaded from the internal folder only.

BlamLib.Test
	Program.cs
		* kTestResultsPath: The path where test results will be saved to.
			NOTE: The various engine tests will create subfolders in this results path where they'll store their engine & platform specific results (ie, Halo2\Xbox)
	Halo1\Halo1.cs
		* kTestResultsTagsPath: Path where we'll save tags to
		* kTestTagIndexTagsPath: Path which contains the "tags" folder which we'll perform TagIndex tests with
	Halo2\Halo2.cs
		* kTestTagIndexTagsPath: Path which contains the "tags" folder which we'll perform TagIndex tests with