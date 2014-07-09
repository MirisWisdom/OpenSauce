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
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo1\";

        const string kTestResultsTagsPath = kTestResultsPath + @"tags\";
        const string kTestResultsDataPath = kTestResultsPath + @"data\";

        const string kTestTagIndexTagsPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo Custom Edition\tags";
        const string kTestTagIndexDataPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo Custom Edition\data";
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

		const string kTestTagIndexTagsPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo 2 Map Editor\tags";
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