/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/interface/ui_widget.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum ui_audio_feedback_sound
		{
			_ui_audio_feedback_sound_cursor,
			_ui_audio_feedback_sound_forward,
			_ui_audio_feedback_sound_back,
			_ui_audio_feedback_sound_flag_failure,
		};
	};

	namespace UIWidgets
	{
		void PlayAudioFeedbackSound(Enums::ui_audio_feedback_sound type);

		s_ui_widget_instance* LoadByNameOrTag(datum_index ui_widget_definition_index, cstring name = nullptr,
			s_ui_widget_instance* parent = nullptr, 
			// don't set ctrl'er index unless you specify the last three params as well
			int16 controller_index = NONE, 
			datum_index topmost_widget_definition_index = datum_index::null,
			datum_index parent_widget_definition_index = datum_index::null,
			int16 child_index_from_parent = NONE);

		bool DisplayScriptedWidget(cstring name);
	};
};
#endif