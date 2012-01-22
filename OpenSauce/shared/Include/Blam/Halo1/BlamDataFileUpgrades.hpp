/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace DataFiles
	{
		//
		bool FindSet(cstring name, 
			const char (*&bitmaps)[MAX_PATH],
			const char (*&sounds)[MAX_PATH],
			const char (*&locale)[MAX_PATH]
			);
		// Determines if the mod-set files exists.
		// If one file doesn't exist, this returns false, and the path details in [details_buffer]
		// The data held in [details_buffer] when this returns true is undefined, be careful
		bool VerifySetFilesExist(cstring bitmaps, cstring sounds, cstring locale, char details_buffer[MAX_PATH]);

#if PLATFORM_IS_EDITOR
		void SaveSettings();

		// EEXIST - file set exists, thus wasn't re-added
		// ENOSPC - no more file set slots available
		errno_t AddFileSet(cstring name);
#endif

		void SharedInitialize();
		void SharedDispose();
	};
};