/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

namespace BlamLib.Test
{
	partial class TestLibrary
	{
		public const string kTestResultsPath = @"C:\Mount\B\Kornner\Projects\test_results\BlamLib\";

		public const string kProgramFilesPath =
			@"C:\Program Files (x86)\"
			//@"C:\Program Files\
			;
	};

	partial class Halo1
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo1\";

		const string kTestResultsTagsPath = kTestResultsPath + @"tags\";

		const string kTestTagIndexTagsPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo Custom Edition\";
	};

	partial class Stubbs
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Stubbs\";

		const string kMapsDirectoryPc = @"C:\Mount\A\Bungie\GamesRelated\Stubbs\PC\Maps\";
		const string kMapsDirectoryXbox = @"";
	};

	#region Halo 2
	partial class Halo2
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo2\";
		const string kTestResultsPathXbox = kTestResultsPath + @"Xbox\";
		const string kTestResultsPathXboxAlpha = kTestResultsPath + @"Xbox\Alpha\";
		const string kTestResultsPathPc = kTestResultsPath + @"PC\";

		const string kMapsDirectoryXbox = @"C:\Mount\A\Bungie\Games\Halo2\Xbox\Retail\Maps\";
		const string kMapsDirectoryPc = @"C:\Mount\A\Bungie\Games\Halo2\PC\Maps\";
		const string kMapsDirectoryXboxAlpha = @"C:\Mount\A\Bungie\Games\Halo2\Xbox\Alpha\Maps\";

		const string kTestTagIndexTagsPath = TestLibrary.kProgramFilesPath + @"Microsoft Games\Halo 2 Map Editor\";
	};
	#endregion

	partial class Halo3
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo3\";

		const string kDirectoryXbox = @"C:\Mount\A\Bungie\Games\Halo3\Xbox\";
	};

	partial class HaloOdst
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"HaloOdst\";

		const string kMapsDirectoryXbox = @"C:\Mount\A\Bungie\Games\HaloOdst\Xbox\Maps\";
	};

	partial class HaloReach
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"HaloReach\";

		const string kDirectoryXbox = @"C:\Mount\A\Bungie\Games\HaloReach\Xbox\";
	};

	partial class Halo4
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo4\";

		const string kDirectoryXbox = @"C:\Mount\A\Bungie\Games\Halo4\Xbox\";
	};
}