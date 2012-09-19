/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <Common/Halo1/YeloSettingsDefinitions.hpp>
#include <Common/Halo1/YeloSharedSettings.hpp>

namespace Yelo
{
//////////////////////////////////////////////////////////////////////////
// Interface

	namespace Settings
	{
		extern cstring K_DEBUG_FILENAME;
		extern cstring K_EVENT_LOG_FILENAME;
		extern cstring K_CHAT_LOG_FILENAME;

		typedef void (API_FUNC* proc_settings_read)(TiXmlElement* element);
		typedef void (API_FUNC* proc_settings_save)(TiXmlElement* element);

		void Initialize();
		void Dispose();

		void LoadSettings();
		void SaveSettings();

		// Get the current time and format it into [time_str].
		// Not the most obvious place for this function to exist in, I know...
		void GetTimeStampString(_Out_ tag_string time_str);
		// Get the current time and format it into [time_str] using a file name friendly format.
		// The formatting output by this function is YYYY_MM_DD_hh_mm_ss.
		void GetTimeStampStringForFile(_Out_ tag_string time_str);
	};
};