/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

namespace BlamLib.Test
{
	partial class TestLibrary
	{
		public const string kTestResultsPath = @"";

		public const string kProgramFilesPath =
			@"C:\Program Files (x86)\"
			//@"C:\Program Files\
			;
	};

	partial class Halo1
	{
		const string kTestInstallationRootPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo Custom Edition";
		const string kTestTagsDir = "tags";
		const string kTestDataDir = "data";

		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo1\";

		const string kTestResultsTagsPath = kTestResultsPath + kTestTagsDir;
		const string kTestResultsDataPath = kTestResultsPath + kTestDataDir;
	};

	partial class Stubbs
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Stubbs\";

		const string kMapsDirectoryPc = @"";
		const string kMapsDirectoryXbox = @"";
	};

	#region Halo 2
	partial class Halo2
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo2\";
		const string kTestResultsPathXbox = kTestResultsPath + @"Xbox\";
		const string kTestResultsPathXboxAlpha = kTestResultsPath + @"Xbox\Alpha\";
		const string kTestResultsPathPc = kTestResultsPath + @"PC\";

        const string kMapsDirectoryXbox = @"";
        const string kMapsDirectoryPc = @"";
		const string kMapsDirectoryXboxAlpha = @"";

		const string kTestInstallationRootPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo 2 Map Editor";
		const string kTestTagsDir = "tags";
		const string kTestDataDir = "data";
	};
	#endregion

	partial class Halo3
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo3\";

        const string kDirectoryXbox = @"";
	};

	partial class HaloOdst
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"HaloOdst\";

		const string kMapsDirectoryXbox = @"";
	};

	partial class HaloReach
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"HaloReach\";

		const string kDirectoryXbox = @"";
	};

	partial class Halo4
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo4\";

		const string kDirectoryXbox = @"";
	};
}