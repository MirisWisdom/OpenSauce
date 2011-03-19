/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C)  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

namespace BlamLib.Test
{
	partial class TestLibrary
	{
		public const string kTestResultsPath = @"C:\Mount\B\Kornner\Projects\test_results\BlamLib\";
	};

	partial class Halo1
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo1\";

		const string kTestResultsTagsPath = kTestResultsPath + @"tags\";

		const string kTestTagIndexTagsPath = @"C:\Program Files (x86)\Microsoft Games\Halo Custom Edition\";
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
}