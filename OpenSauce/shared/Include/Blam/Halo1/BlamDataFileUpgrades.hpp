/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace DataFiles
	{
		bool FindSet(cstring name, 
			const char (*&bitmaps)[MAX_PATH],
			const char (*&sounds)[MAX_PATH],
			const char (*&locale)[MAX_PATH]
			);

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