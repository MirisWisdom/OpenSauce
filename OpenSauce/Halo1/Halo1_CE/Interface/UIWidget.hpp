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
		void PlayAudioFeedbackSound(Enums::ui_audio_feedback_sound type);
	};
};
#endif