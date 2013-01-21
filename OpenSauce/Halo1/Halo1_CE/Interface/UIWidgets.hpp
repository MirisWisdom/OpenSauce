/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

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
		struct s_ui_widget_instance;

		void PlayAudioFeedbackSound(Enums::ui_audio_feedback_sound type);

		s_ui_widget_instance* LoadByNameOrTag(datum_index tag_index, cstring name = NULL, 
			s_ui_widget_instance* parent = NULL, 
			// don't set ctrl'er index unless you specify the last three params as well
			int16 controller_index = NONE, 
			datum_index arg_10 = datum_index::null, // DeLa tag_index
			datum_index arg_14 = datum_index::null, // DeLa tag_index
			int16 arg_18 = NONE); // some sort of index

		bool DisplayScriptedWidget(cstring name);
	};
};
#endif