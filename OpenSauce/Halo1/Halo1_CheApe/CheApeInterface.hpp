/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

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
#pragma once

// Name of the memory map file with definitions for CheApe to load
#define CHEAPE_CACHE_FILE_NAME "CheApe.map"

namespace Yelo
{
	namespace CheApe
	{
		// Changes the protection of the exe segment in the memory of the executing application so we can write without a problem
		void UnProtectMemoryRegion();

		// Initialize everything we use for the cache file to be read into memory
		void PhysicalMemoryMapInitialize();

		// Load the cache file into memory so we can begin to hack up the executing application
		void LoadCacheFile();

		// Copy the original tag group pointers to our custom list, so we can still use them in the executing application
		void SetupTagGroupPointers();

		// Update the count checks to use the new tag groups count
		void SetupTagGroupCounts();

		// Update the tag group address refs to use the new tag group address
		void UpdateTagGroupReferences();

		void Dispose();

		enum {
			k_error_none,
			k_error_UnProtectMemoryRegion,
			k_error_PhysicalMemoryMapInitialize,
			k_error_LoadCacheFile,
			k_error_SetupTagGroupPointers,
			k_error_SetupTagGroupCounts,
			k_error_UpdateTagGroupReferences,

			k_error_other,
			k_error,
		};
		extern long_enum _InitError;
	};

	void Initialize();
	void Dispose();
};