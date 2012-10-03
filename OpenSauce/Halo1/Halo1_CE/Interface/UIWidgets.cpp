/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/UIWidgets.hpp"

#if !PLATFORM_IS_DEDI

#include "Memory/MemoryInterface.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace UIWidgets
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_UI_WIDGET
#include "Memory/_EngineLayout.inl"

		void PlayAudioFeedbackSound(Enums::ui_audio_feedback_sound type)
		{
			// TODO: dooooooooo me

			// TODO: harder.
		}

		static API_FUNC_NAKED s_ui_widget_instance* LoadByNameOrTag(cstring name, datum_index tag_index, 
			s_ui_widget_instance* parent, int16 controller_index, 
			datum_index arg_10,
			datum_index arg_14, 
			int32 /*int16*/ arg_18)
		{
			static uint32 CALL_ADDR = GET_FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG);

			API_FUNC_NAKED_START()
				push	arg_18
				push	arg_14
				push	arg_10
				push	controller_index
				push	parent
				push	tag_index
				push	name
				call	CALL_ADDR
			API_FUNC_NAKED_END_CDECL(7)
		}
		// Implementation has index second, but in OS we should mostly be calling with the tag index; no hardcoded strings!
		s_ui_widget_instance* LoadByNameOrTag(datum_index tag_index, cstring name, 
			s_ui_widget_instance* parent, 
			int16 controller_index, 
			datum_index arg_10,
			datum_index arg_14, 
			int16 arg_18)
		{
			ASSERT( tag_index != datum_index::null || name != NULL, "ui widget: tried to load without a name or tag index" );
			return LoadByNameOrTag(name, tag_index, parent, controller_index, arg_10, arg_14, arg_18);
		}

		static const TagGroups::s_project_yellow_scripted_ui_widget* FindWidget(cstring widget_name,
			const TAG_TBLOCK(& widgets, TagGroups::s_project_yellow_scripted_ui_widget))
		{
			for(int x = 0; x < widgets.Count; x++)
			{
				if(strcmp(widgets[x].name, widget_name) == 0)
				{
					const TagGroups::s_project_yellow_scripted_ui_widget* suw = &widgets[x];

					if(!suw->definition.tag_index.IsNull())
						return suw;
				}
			}

			return NULL;
		}
		bool DisplayScriptedWidget(cstring name)
		{
			const TagGroups::s_project_yellow_scripted_ui_widget* widget = NULL;

			if(!TagGroups::_global_yelo->IsNull())
				widget = FindWidget(name, TagGroups::_global_yelo->ui.scripted_widgets);
			// If it's not found in the scenario's yelo, search the yelo globals
			if(widget == NULL)
				widget = FindWidget(name, TagGroups::_global_yelo_globals->ui.scripted_widgets);
			
			if(widget != NULL)
			{
				return LoadByNameOrTag(widget->definition.tag_index, NULL, NULL, NONE)
					!= NULL;
			}

			return false;
		}
	};
};
#endif