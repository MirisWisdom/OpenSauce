/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

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