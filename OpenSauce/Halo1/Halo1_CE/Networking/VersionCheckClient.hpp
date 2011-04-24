/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#if PLATFORM_IS_USER && defined(YELO_VERSION_CHECK_ENABLE)
#include "Networking/VersionCheck.hpp"

namespace Yelo
{
	class TextBlock;

	namespace Networking { namespace VersionCheck
	{
		/*!
		 * \brief
		 * Handles checking for a new version of OS on the client setup.
		 * 
		 * This class will check for updates when the main menu is loaded
		 * and displays the result to the user.
		 */
		class c_version_check_manager_user : public c_version_check_manager_base
		{
			struct {
				/// Is the game currently on the main menu
				bool is_in_menu;
				PAD24;
			}m_states_user;
		public:
			void			Initialize();
			void			Dispose();

			void			Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void			OnLostDevice();
			void			OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
			void			Render();
			void			Release();

			void			InitializeForNewMap();
			void			Update(real delta_time);
		protected:
			void			UpdateState();
		};
		/*!
		 * \brief
		 * Manages how a new version is visually shown to the user.
		 * 
		 * The display manager handles how an available version update
		 * is displayed to the user. The version manager currently 
		 * draws a small piece of text in the lower left corner of
		 * the main menu displaying the current version, which is
		 * then replaced by a brighter piece of text showing the
		 * available version, which fades in and out.
		 */
		class c_version_display_manager
		{
			enum {
				/*!
				 * \brief
				 * The maximum length of the version strings.
				 */
				k_max_update_string_length = 31,
			};

			struct {
				wchar_t current_version[k_max_update_string_length+1];
				wchar_t available_version[k_max_update_string_length+1];
			}m_strings;

			struct {
				TextBlock* current_version;
				TextBlock* available_version;
			}m_textblocks;

			struct {
				/// Should the animation be played
				bool do_animation;
				/// Has the animation cycle reached 0
				bool is_cycle_complete;
				/// Toggles whether to increase or decrease current_position
				bool do_decrease;
				PAD8;
				/// The time it takes to get from 0 to 1
				real cycle_time;
				/// The current position in the animation cycle
				real current_position;

				/// The amount of time the animation should play for in total
				real display_time;
				/// The amount of time the animation has played for
				real current_time;
			}m_animation;

		public:
			/*!
			 * \brief
			 * Initialises the classes variables and allocates any extra memory (if needed).
			 */
			void		Initialize()
			{
				memset(this, 0, sizeof(*this));

				m_animation.cycle_time = 1.0f;
				m_animation.is_cycle_complete = true;
			}
			/*!
			 * \brief
			 * Deletes the memory allocated by Initialize (if any)
			 */
			void		Dispose()
			{
			}

			void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void		OnLostDevice();
			void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
			void		Render();
			void		Release();

			void		Update(real delta_time);

			void		SetCurrentVersionString(wcstring version_string);
			void		SetAvailableVersionString(wcstring version_string);

			void		StartUpdateDisplay(const real time);
			void		ResetDisplay();

			static c_version_display_manager g_instance;
		};
	}; };
};
#endif