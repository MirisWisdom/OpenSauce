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

	namespace UIWidget
	{
		struct s_ui_widget_instance;

		void PlayAudioFeedbackSound(Enums::ui_audio_feedback_sound type);

		s_ui_widget_instance* LoadByNameOrTag(cstring name, datum_index tag_index, 
			s_ui_widget_instance* parent = NULL, int16 controller_index = 0, 
			datum_index arg_10 = datum_index::null, // DeLa tag_index. error message?
			datum_index arg_14 = datum_index::null, 
			int32 /*int16*/ arg_18 = NONE); // some sort of index

		bool DisplayScriptedWidget(int16 local_player_index, cstring name);
	};
};
#endif