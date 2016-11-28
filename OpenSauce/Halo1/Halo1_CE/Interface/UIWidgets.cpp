/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/UIWidgets.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Memory/MemoryInterface.hpp"

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

		// Implementation has index second, but in OS we should mostly be calling with the tag index; no hardcoded strings!
		s_ui_widget_instance* LoadByNameOrTag(datum_index ui_widget_definition_index, cstring name,
			s_ui_widget_instance* parent, 
			int16 controller_index, 
			datum_index topmost_widget_definition_index,
			datum_index parent_widget_definition_index,
			int16 child_index_from_parent)
		{
			YELO_ASSERT_DISPLAY( !ui_widget_definition_index.IsNull() || name != nullptr,
				"ui widget: tried to load without a name or tag index" );

			return blam::ui_widget_load_by_name_or_tag(name, ui_widget_definition_index, parent, controller_index,
				topmost_widget_definition_index, parent_widget_definition_index, child_index_from_parent);
		}

		static const TagGroups::s_project_yellow_scripted_ui_widget* FindWidget(cstring widget_name,
			const TAG_TBLOCK(& widgets, TagGroups::s_project_yellow_scripted_ui_widget))
		{
			for(const auto& widget : widgets)
			{
				if(strcmp(widget.name, widget_name) == 0 && !widget.definition.tag_index.IsNull())
					return &widget;
			}

			return nullptr;
		}
		bool DisplayScriptedWidget(cstring name)
		{
			const TagGroups::s_project_yellow_scripted_ui_widget* widget = nullptr;

			if (!Scenario::GetYelo()->IsNull())
				widget = FindWidget(name, Scenario::GetYelo()->ui.scripted_widgets);
			// If it's not found in the scenario's yelo, search the yelo globals
			if(widget == nullptr)
				widget = FindWidget(name, Scenario::GetYeloGlobals()->ui.scripted_widgets);
			
			if(widget != nullptr)
			{
				return LoadByNameOrTag(widget->definition.tag_index, nullptr, nullptr, NONE)
					!= nullptr;
			}

			return false;
		}
	};

	namespace blam
	{
		using namespace UIWidgets;

		API_FUNC_NAKED s_ui_widget_instance* PLATFORM_API ui_widget_load_by_name_or_tag(
			cstring name,
			datum_index ui_widget_definition_index,
			s_ui_widget_instance* parent,
			int16 controller_index,
			datum_index topmost_widget_definition_index,
			datum_index parent_widget_definition_index,
			int16 child_index_from_parent)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(UI_WIDGET_LOAD_BY_NAME_OR_TAG);

			__asm	jmp	FUNCTION
		}
	}
};
#endif